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
#include "mainwindowimpl.h"
#include "settingsdialog.h"

#include <QStandardItem>
#include <QList>
#include <QModelIndex>
#include <QFileDialog>
#include <QApplication>
#include <QX11Info>
#include  <alsa/asoundlib.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include <X11/Xlib.h>
#include "logging.h"


MainWindowImpl::MainWindowImpl( QWidget * parent, Qt::WFlags f) 
	: QMainWindow(parent, f)
	,m_pRadioshark(NULL)
	,pConnection(NULL)
	,pProxy(NULL)
{
	setupUi(this);

	//We need DBus to poke screensaver
	this->SetupDBus();
	
	Presets->setModel(&m_Presets);
	
	this->LoadSettings();
	m_AudioInterface.SetVisualizationName(m_Settings.VisualizationName);
	ScreensaverPoke( m_Settings.PokeScreensaver);
	
	if( this->CheckRadioshark() == true)
	{
		
	}else{
		//Couldn't connect to radioshark
		//We'll start a timer to check for a recently attached one
		//at 1Hz.
		connect(&m_CheckRadiosharkTimer,SIGNAL(timeout()),this,
									SLOT(OnCheckRadiosharkTimer()));
		m_CheckRadiosharkTimer.start(1000);
	}

}
void MainWindowImpl::closeEvent(QCloseEvent *event)
 {
 	this->SaveSettings();
 	
 	this->CleanupDBus();
 	
	if(m_pRadioshark)
	{
		m_pRadioshark->Close();
		delete m_pRadioshark;
		m_pRadioshark = NULL;
	}
	
	m_AudioInterface.stop();
	m_AudioInterface.Close();
	
	QMainWindow::closeEvent(event);

}
bool  MainWindowImpl::CheckRadioshark(void)
{
	string dev;
	if( FindRadioshark(dev) == false )
	{
		Logger::Write("ERROR:Could not find radioSHARK device. I'll keep looking...");
		ERRORS->setText("NO RADIOSHARK");
		return false;
	}
	//Found the device. If necessary initialize HID
	if(!m_pRadioshark)
	{
		Logger::Write("Created radioshark HID device.");
		m_pRadioshark = new Radioshark::Radioshark2Interface();
	}
	if( m_pRadioshark )
	{
		if( m_pRadioshark->Open() == false)
		{
			Logger::Write("ERROR: Problem opening HID device.");
			ERRORS->setText("HID PROBLEM");
			return false;
		}
		this->UpdateFrequencyDisplay();
	}else{
		Logger::Write("ERROR: The hid device pointer is undefined.");
		ERRORS->setText("HID ABSENT");
		return false;
	}
	
	//Set the start frequency
	if(m_Settings.StartFreq.GetBand() == Preset::AM)
	{
		m_pRadioshark->SetAMFreq(m_Settings.StartFreq.GetFreq());
	}else{
		m_pRadioshark->SetFMFreq(m_Settings.StartFreq.GetFreq());
	}
	
	//Open the actual audio capture.
	Visualization->setAttribute(Qt::WA_PaintOnScreen);
	Visualization->setAttribute(Qt::WA_OpaquePaintEvent);
	Visualization->setAttribute(Qt::WA_NoSystemBackground);
	if( m_AudioInterface.Open(dev,string("PULSE"),Visualization->winId(),
													m_Settings.UseXvimagesink) )
	{
		//Set the start volume
		OnVolume((int)m_Settings.StartVolume);
		Volume->setValue((int)m_Settings.StartVolume);
	}else{
		//Uh-oh.
		ERRORS->setText("GSTREAMER PROBLEM");
		return false;
	}
	//One last thing to do. Try to start the device.
	if( !m_AudioInterface.run() )
	{
		Logger::Write("ERROR: Can't run the audio interface.");
		return false;
	}
	ERRORS->setText("");
	this->UpdateFrequencyDisplay();
	//Perhaps LAME is unavilaible, so disable recording button.
	if(!m_AudioInterface.CanRecord())
	{
		Record->setDisabled(true);
	}
	return true;
}
void MainWindowImpl::OnCheckRadiosharkTimer(void)
{
	if( this->CheckRadioshark() == true )
	{
		//Finished setup, let's stop the timer
		this->m_CheckRadiosharkTimer.stop();
	}
}

bool MainWindowImpl::FindRadioshark(string& device)
{
	int card = -1;
	int dev = -1;
	
	snd_ctl_t *handle = NULL;
	snd_ctl_card_info_t *info = NULL;
	snd_pcm_info_t *pcminfo = NULL;
	snd_pcm_stream_t stream = SND_PCM_STREAM_CAPTURE;
	char card_id[32];
	char *name = NULL;

	snd_ctl_card_info_alloca (&info);
	snd_pcm_info_alloca (&pcminfo);

	if (snd_card_next (&card) < 0 || card < 0)
	{
		return false;
	}
	while(card >= 0)
	{
		snprintf (card_id, 32, "hw:%d", card);
		if (snd_ctl_open (&handle, card_id, 0) == 0)
		{
			snd_ctl_card_info (handle, info);
			while (1)
			{
				snd_ctl_pcm_next_device (handle, &dev);
				if (dev < 0){
					break;
				}
				snd_pcm_info_set_device (pcminfo, dev);
				snd_pcm_info_set_subdevice (pcminfo, 0);
				snd_pcm_info_set_stream (pcminfo, stream);
	
				if (snd_ctl_pcm_info (handle, pcminfo) >= 0)
				{
					snd_card_get_name (card, &name);
					if(strncmp(name,"radioSHARK",32) == 0)
					{void PokeScreensaver(void);
						snd_ctl_close(handle);
						free (name);
						snprintf (card_id, 32, "hw:%d,%d", card,dev);
						device = string(card_id);
						return true;
					}
					free (name);
				}
			}
			snd_ctl_close(handle);
		}
		snd_card_next (&card);
	}
	return false;//didn't find the proper Radioshark card.
}

void MainWindowImpl::UpdateFrequencyDisplay(void)
{
	if(!m_pRadioshark)
	{
		return;
	}
	QString blurb;
	if(m_pRadioshark->GetBand() == Radioshark::FM)
	{
		blurb.sprintf("%3.1f FM",m_pRadioshark->GetFMFreq());
	}else{
		blurb.sprintf("%3.0f AM",m_pRadioshark->GetAMFreq());
	}
	Frequency->setText(blurb);
}
//
void MainWindowImpl::OnFreqUp(void)
{
	if(!m_pRadioshark)
	{
		return;
	}
	if( m_pRadioshark->GetBand() == Radioshark::FM )
	{
		m_pRadioshark->FMFreqUp();
	}else{
		m_pRadioshark->AMFreqUp();
	}
	this->UpdateFrequencyDisplay();

}
void MainWindowImpl::OnFreqDown(void)
{
	if(!m_pRadioshark)
	{void PokeScreensaver(void);
		return;
	}
	if(  m_pRadioshark->GetBand() == Radioshark::FM )
	{
		m_pRadioshark->FMFreqDown();
		
	}else{
		m_pRadioshark->AMFreqDown();
	}
	this->UpdateFrequencyDisplay();
}

void  MainWindowImpl::OnVolume(int volume )
{
	m_AudioInterface.UpdateVolume(volume);
}
void  MainWindowImpl::OnMute( bool muted )
{
	m_AudioInterface.Mute(muted);
}
void MainWindowImpl::OnRecord( bool start )
{
	//if we're starting a new recording we form the new
	//recording fielname with station, time and data, (.mp3)
	if(start == true && m_pRadioshark )
	{
		MP3Settings settings;
		if(m_Settings.AutogenerateRecordingNames)
		{
			string station_date_time;
			m_pRadioshark->ToStationTimeDateString(station_date_time);
			settings.filename = m_Settings.RecordingPath + "/"
									+ station_date_time +".mp3";
		}else{
			QString home = m_Settings.RecordingPath.c_str();
			home+="/recording.mp3";
			QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
									 home,
									tr("MP3 Files (*.mp3)"));
			if(fileName.isNull())
			{
				return;
			}
		}
		settings.bitrate = m_Settings.MP3.bitrate;
		settings.mode = m_Settings.MP3.mode;
		//TODO: CHECK THE FORMEDNESS OF THE RECORDING NAME AND PATH
		//TODO: CHECK PERMISSIONS OF THE RECORDING PATH
		//TODO: CHECK EXISTENCE OF THE RECORDING PATH
		
		m_AudioInterface.Record(true,&settings);
		
	}else{
		m_AudioInterface.Record(false);
	}
	
}
void MainWindowImpl::OnBandChange(void)
{
	if(!m_pRadioshark)
	{
		return;
	}
	if(m_pRadioshark->GetBand() == Radioshark::AM)
	{
		m_pRadioshark->SetFM();
	}else{
		m_pRadioshark->SetAM();
	}
	this->UpdateFrequencyDisplay();void PokeScreensaver(void);
}

void MainWindowImpl::OnSavePreset(void)
{
	if(!m_pRadioshark)
	{
		return;
	}
	//Save the current freq as a preset
	float freq = 0.0f;
	bool fmam = false;
	if(  m_pRadioshark->GetBand() == Radioshark::FM )
	{
		freq = m_pRadioshark->GetFMFreq();
		fmam = false;
	}else{
		freq = m_pRadioshark->GetAMFreq();
		fmam = true;
	}

	int row = m_Presets.rowCount();	
	QStandardItem* newitem = new QStandardItem();
	newitem->setData((double)freq,Qt::UserRole + 1);
	newitem->setData(fmam,Qt::UserRole + 2);
	QString band;
	if(!fmam)
	{
		band.sprintf("FM %3.1f",freq);
	}else{
		band.sprintf("AM %4.0f",freq);
	}
	newitem->setText(band);
	m_Presets.insertRow(row,newitem);

}
void MainWindowImpl::OnPreset(const QModelIndex &index)
{
	//Get the item
	QStandardItem* pitem = m_Presets.itemFromIndex(index);
	if(!pitem || !m_pRadioshark)
	{
		return;
	}
	QVariant vfreq = pitem->data(Qt::UserRole+1);
	float freq = (float)vfreq.toDouble();
	QVariant vam = pitem->data(Qt::UserRole+2);
	bool am = vam.toBool();
	if(am)
	{
		m_pRadioshark->SetAMFreq(freq);
		
	}else{
		m_pRadioshark->SetFMFreq(freq);
	}
	this->UpdateFrequencyDisplay();
}
void MainWindowImpl::OnSettings(void)
{
	SettingsDialog settings;
	settings.Set(m_Settings);
	if( settings.exec() == QDialog::Accepted )
	{
		//update our settings from the dialog.
		settings.Get(m_Settings);
		
		//We may have changed the screensaver supression
		ScreensaverPoke( m_Settings.PokeScreensaver);
		
		//We may have changed visualization
		m_AudioInterface.SetVisualization(m_Settings.VisualizationName,
											Visualization->winId(),true,
											m_Settings.UseXvimagesink );
	}
}

void MainWindowImpl::LoadSettings(void)
{
	//Find our settings file
	char * pHome;
	string settings_filenameandpath;
	pHome = getenv ("HOME");
	if (pHome!=NULL)
	{
		settings_filenameandpath = string(pHome);
	}
	settings_filenameandpath+="/.finny.settings";
	ifstream infile(settings_filenameandpath.c_str());
	if(!infile.is_open())
	{
		return;
	}
	while( !infile.eof() )
	{
		string tag;
		infile>>tag;
		if(tag == "PRESET:")
		{
			bool am;
			float freq;
			string desc;
			Preset::Read(infile,am,freq,desc);
			QStandardItem* newitem = new QStandardItem();
			newitem->setData((double)freq,Qt::UserRole + 1);void PokeScreensaver(void);
			newitem->setData(am,Qt::UserRole + 2);
			newitem->setText(QString(desc.c_str()));
			m_Presets.insertRow(m_Presets.rowCount(),newitem);
		}else if(tag == "SETTING:"){
			FinnySettings::LoadSetting(m_Settings,infile);
		}
	}
}
void MainWindowImpl::SaveSettings(void)
{
	//Find our settings file
	char * pHome;
	string settings_filenameandpath;
	pHome = getenv ("HOME");
	if (pHome!=NULL)
	{
		settings_filenameandpath = string(pHome);
	}
	settings_filenameandpath+="/.finny.settings";
	ofstream outfile(settings_filenameandpath.c_str());
	if(!outfile.is_open())
	{
		return;
	}
	//Save presets
	int n_rows = m_Presets.rowCount();
	for(int i = 0; i < n_rows; i++)
	{
		QStandardItem* item = m_Presets.item(i);
		QVariant vfreq = item->data(Qt::UserRole+1);
		QVariant vam = item->data(Qt::UserRole+2);
		float freq = (float)vfreq.toDouble();
		bool am = vam.toBool();
		QString desc = item->text();
		string std_desc(desc.toAscii());
		Preset::Write(outfile,am,freq,std_desc);
	}
	//And write the rest of our settings
	if( m_Settings.UpdateStartFreqOnClose && m_pRadioshark )
	{
		Radioshark::Band band = m_pRadioshark->GetBand();
		if( band == Radioshark::AM)
		{
			m_Settings.StartFreq = Preset(Preset::AM,m_pRadioshark->GetAMFreq());
		}else{
			m_Settings.StartFreq = Preset(Preset::FM,m_pRadioshark->GetFMFreq());
		}
		
	}
	if( m_Settings.UpdateStartVolumeOnClose)
	{
		float volume = (float) m_AudioInterface.GetVolume();
		m_Settings.StartVolume = volume;
	}
	FinnySettings::WriteSettings(m_Settings,outfile);
	
	outfile.close();
}
void MainWindowImpl::OnFastUp(void)
{
	if(!m_pRadioshark)
	{
		return;
	}
	if( m_pRadioshark->GetBand() == Radioshark::FM )
	{
		m_pRadioshark->FMFreqUp(1.0f);
	}else{
		m_pRadioshark->AMFreqUp(10.0f);
	}
	this->UpdateFrequencyDisplay();
}
void MainWindowImpl::OnFastDown(void)
{
	if(!m_pRadioshark)
	{
		return;
	}
	if(  m_pRadioshark->GetBand() == Radioshark::FM )
	{
		m_pRadioshark->FMFreqDown(1.0f);
		
	}else{
		m_pRadioshark->AMFreqDown(10.0f);
	}
	this->UpdateFrequencyDisplay();
}
void MainWindowImpl::OnRemovePreset(void)
{
	//Find which preset is selected
	QModelIndexList indices = Presets->selectionModel()->selectedRows();
	QModelIndexList::iterator i;
	for(i = indices.begin();i!=indices.end();++i)
	{
		QStandardItem* item = m_Presets.itemFromIndex(*i);
		m_Presets.removeRow(item->row(),QModelIndex());
	}
}
void  MainWindowImpl::GetAudioFormat(void)
{
	AudioFormat format;
	m_AudioInterface.GetAudioFormat(format);
}
void MainWindowImpl::SetupDBus(void)
{
	GError *pError = NULL;
	pConnection = dbus_g_bus_get( DBUS_BUS_SESSION, &pError );
	if(pConnection == NULL)
	{
		g_error_free( pError );
		return;
	}
	pProxy = dbus_g_proxy_new_for_name (pConnection,
										"org.gnome.ScreenSaver",
										"/org/gnome/ScreenSaver",
										"org.gnome.ScreenSaver");
}
void MainWindowImpl::CleanupDBus(void)
{
	if( pConnection)
	{
		dbus_g_connection_unref(pConnection);
		pConnection = NULL;
	}
}
void MainWindowImpl::PokeScreensaver(void)
{
	XResetScreenSaver (QX11Info::display ());
	if( pProxy )
	{
		dbus_g_proxy_call_no_reply (pProxy, "SimulateUserActivity",G_TYPE_INVALID);
	}
}

void MainWindowImpl::ScreensaverPoke(bool on)
{
	if( on )
	{
		connect( &m_ScreensaverPokeTimer, SIGNAL(timeout()),
								this, SLOT(PokeScreensaver()) );
		m_ScreensaverPokeTimer.start(30000);
	}else{
		m_ScreensaverPokeTimer.stop();
	}
}