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

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H
//
#include <QDialog>
#include "ui_Settings.h"
#include "finnysettings.h"
//
class SettingsDialog : public QDialog, public Ui::Dialog
{
Q_OBJECT
public:
	SettingsDialog( QWidget * parent = 0, Qt::WFlags f = 0 );
	
	//Set with the current application settings:
	void Set( FinnySettings& settings);
	//Get the current settings
	void Get( FinnySettings& settings);
private slots:

public slots:
	void OnBandChange(int band);
	void OnRecordingPathBrowse();
	void OnResetDefaultSettings(void);
};
#endif





