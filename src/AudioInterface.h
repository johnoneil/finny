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
#ifndef __AUDIOINTERFACE_H__
#define __AUDIOINTERFACE_H__

/*GSTREAMER INCLUDE*/
#include <gst/gst.h>
#include <gst/interfaces/mixer.h>
#include <gst/app/gstappsink.h>

#include <iostream>
#include <string>

#include "mp3settings.h"

using namespace std;

struct AudioFormat
{
	bool is_signed;
	int width;
	int depth;
	int sampling_rate;
	int endianness;
	int channels;
	//string type;
};

class AudioInterface
{
public:
	
	AudioInterface();
	~AudioInterface();
	
protected:
	//Gstreamer main pipeline
	GstElement *m_pPipeline;
		GstElement* m_pElementIn;
		GstElement* m_pElementOut;
		GstElement *m_pMixer;
		GstElement *m_pTeeOne;
		GstElement *m_pQueue1;
	
	//MP3 recording bin (branch off Tee above when on)
	bool ConstructMP3RecorderBin( void );
	GstElement *m_pMP3Recorder;
		GstElement *m_pEncoder;
		GstElement *m_pFile;
		GstElement *m_pQueue2;
		
	//Appsink, to allow appliction to get audio visualization data
	bool ConstructVisualizationBin( unsigned int xwindow_id,bool use_xvimagesink = false );
	GstElement *m_pVisualizationBin;
		GstElement *m_pQueue3;
		GstElement *m_pColorspace;
		GstElement *m_pVisualization;
		GstElement* m_pAppSink;
		
	//Bus
	GstBus *m_pBus;
	static gboolean BusCallback(GstBus *bus,
									GstMessage *message,
									gpointer data);

	bool m_bRunning;
	string m_VisualizationName;

public:
	bool Open(const string& capture_dev,
				const string& output_dev,
				unsigned int xwindow_id = 0,
				bool use_xvimagesink = true);
	bool run();
	void stop();
	void Close(void);
	void UpdateVolume( int vol );
	int GetVolume(void);
	void Mute(bool muted);
	void Record(bool start,MP3Settings* settings = NULL );
	float GetAudioLevel(void);
	bool GetAudioFormat( AudioFormat& format);
	void SetVisualizationName(const string& name);
	
	bool SetVisualization( const string& viz_name, 
							unsigned int xwindow_id,
							bool stop_pipeline = false ,
							bool use_xvimagesink = true);
	bool CanRecord(void);
};

#endif // __AUDIOINTERFACE_H__
