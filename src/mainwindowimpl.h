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

#ifndef MAINWINDOWIMPL_H
#define MAINWINDOWIMPL_H
//
#include <QMainWindow>
#include "ui_mainwindow.h"

#include <stdio.h>
#include <string.h>

#include "AudioInterface.h"
#include "radioshark.h"
#include "finnysettings.h"

#include <QStandardItemModel>
#include <QTimer>
#include <QGraphicsScene>

//GLib DBus support to poke the screensaver.
#include <dbus/dbus-glib.h>

class MainWindowImpl : public QMainWindow, public Ui::MainWindow
{
Q_OBJECT
public:
	MainWindowImpl( QWidget * parent = 0, Qt::WFlags f = 0 );
	void closeEvent(QCloseEvent *event);

	
protected:

	AudioInterface m_AudioInterface;
	Radioshark::Radioshark2Interface* m_pRadioshark;
	
	//Presets for now
	QStandardItemModel m_Presets;
	
	void LoadSettings(void);
	void SaveSettings(void);
	
	void GetAudioFormat(void);
	void UpdateFrequencyDisplay(void);

	//Look for the Radioshark! (audio capture device)
	bool FindRadioshark(string& device);
	bool CheckRadioshark(void);
	QTimer m_CheckRadiosharkTimer;
	
	//Save off/load settings
	FinnySettings m_Settings;
	
	//Screensaver supression
	QTimer m_ScreensaverPokeTimer;
	DBusGConnection *pConnection;
	DBusGProxy *pProxy;
	void SetupDBus(void);
	void CleanupDBus(void);
	void ScreensaverPoke(bool on);
	
private slots:
	void OnFreqUp(void);
	void OnFreqDown(void);
	void OnVolume(int volume );
	void OnMute( bool muted );
	void OnRecord( bool start );
	void OnBandChange(void);
	void OnSavePreset(void);
	void OnPreset(const QModelIndex &index);
	void OnSettings(void);
	void OnFastUp(void);
	void OnFastDown(void);
	void OnRemovePreset(void);
	void OnCheckRadiosharkTimer(void);
	void PokeScreensaver(void);
};
#endif




