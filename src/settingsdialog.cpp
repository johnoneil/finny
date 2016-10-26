/*************************************************************************
	Source code for Finny... A linux frontend for the Griffin Radioshark2
	Copyright (C) 2009  John O'Neil
	Email: oneil.code@gmail.com

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.

 *************************************************************************/
#include "settingsdialog.h"
#include <QFileDialog>
#include "logging.h"
//
SettingsDialog::SettingsDialog( QWidget * parent, Qt::WFlags f) 
	: QDialog(parent, f)
{
	setupUi(this);
}

//Set with the current application settings:
void SettingsDialog::Set( FinnySettings& settings)
{
	if(settings.StartFreq.GetBand() == Preset::AM )
	{
		StartBand->setCurrentIndex(0);
		OnBandChange(0);
	}else{
		StartBand->setCurrentIndex(1);
		OnBandChange(1);
	}
	StartFreq->setValue(settings.StartFreq.GetFreq());
	FixedVolume->setValue(settings.StartVolume);
	
	if(settings.UpdateStartFreqOnClose)
	{
		StartAtLastFrequency->setChecked(true);
		StartFrequencyPanel->setDisabled(true);
	}else{
		StartAtFixedFrequency->setChecked(true);
	}
	if(settings.UpdateStartVolumeOnClose)
	{
		StartAtLastVolume->setChecked(true);
		StartVolumePanel->setDisabled(true);
	}else{
		StartAtVolume->setChecked(true);
	}
	RecordingPath->setText( settings.RecordingPath.c_str() );
	Bitrate->setValue(settings.MP3.bitrate);
	MP3Format->setCurrentIndex((int)settings.MP3.mode);
	if( settings.AutogenerateRecordingNames )
	{
		AutoGenerateMP3Filenames->setChecked(true);
	}else{
		BrowseMP3Filenames->setChecked(true);
	}
	int index = VisualizationName->findText(settings.VisualizationName.c_str());
	if(index >= 0)
	{
		VisualizationName->setCurrentIndex(index);
	}
	DisableScreensaver->setChecked(settings.PokeScreensaver);
	Logging->setChecked(Logger::IsEnabled());
	if(settings.UseXvimagesink)
	{
		xvimagesink->setChecked(true);
	}else{
		ximagesink->setChecked(true);
	}
}
//Get the current settings
void SettingsDialog::Get( FinnySettings& settings)
{
	if( StartAtLastFrequency->isChecked())
	{
		settings.UpdateStartFreqOnClose = true;
	}else{
		settings.UpdateStartFreqOnClose = false;
		if(StartBand->currentIndex() == 0)
		{
			settings.StartFreq.SetBand(Preset::AM);
		}else{
			settings.StartFreq.SetBand(Preset::FM);
		}
		settings.StartFreq.SetFreq((float)StartFreq->value());
	}
	if(StartAtLastVolume->isChecked())
	{
		settings.UpdateStartVolumeOnClose = true;
	}else{
		settings.UpdateStartVolumeOnClose = false;
		settings.StartVolume = (float)FixedVolume->value();
	}
	settings.RecordingPath = string(RecordingPath->text().toAscii());
	settings.MP3.mode = (MP3Settings::Mode) MP3Format->currentIndex();
	settings.MP3.bitrate = Bitrate->value();
	settings.AutogenerateRecordingNames = AutoGenerateMP3Filenames->isChecked();
	settings.VisualizationName = string(VisualizationName->itemText(
										VisualizationName->currentIndex())
										.toAscii());
	settings.PokeScreensaver = DisableScreensaver->isChecked();
	if( Logging->isChecked()){
		Logger::Enable();
	}else{
		Logger::Disable();
	}
	settings.UseXvimagesink = xvimagesink->isChecked();
}
void SettingsDialog::OnBandChange(int band)
{
	if(band == 0)
	{
		StartFreq->setMinimum(520.0);
		StartFreq->setMaximum(1610.0);
		StartFreq->setSingleStep(1.0);
		StartFreq->setValue(630.0);
	}else{
		StartFreq->setMinimum(87.5);
		StartFreq->setMaximum(108.0);
		StartFreq->setSingleStep(0.1);
		StartFreq->setValue(99.5);
	}
}
void SettingsDialog::OnRecordingPathBrowse()
{
	QString newpath= QFileDialog::getExistingDirectory(this,
		tr("MP3 Recording Directory"), RecordingPath->text());
	if(!newpath.isNull())
	{
		RecordingPath->setText(newpath);
	}
}
void SettingsDialog::OnResetDefaultSettings(void)
{
	FinnySettings reset_settings;
	this->Set(reset_settings);
}
//

