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
#ifndef __FINNYSETTINGS_H__
#define __FINNYSETTINGS_H__

#include <string>
#include <list>
#include <fstream>
#include "mp3settings.h"
#include <stdio.h>
#include <stdlib.h>

using namespace std;

//Helper function
void GetToken(string& input,string& token,string& leftover);


class Preset
{
public:
	Preset();
	Preset(bool am, float f);
	~Preset();
	
	enum Band
	{
		FM = 0,
		AM = 1
	};
	
	Preset::Band GetBand();
	void SetBand( Preset::Band bnd);
	float GetFreq();
	void SetFreq(float freq);
	string& GetDescription();
	void SetDescription( string& desc);
	
	void Write( ofstream& outfile);
	void WritePreset( ofstream& outfile);
	static void Write( ofstream& outfile, bool AM, float freq, string& desc);
	bool Read( ifstream& infile);
	static void Read( ifstream& infile, bool& AM, float& freq, string& desc);
	
protected:
	Band m_Band;
	float m_Frequency;
	string m_Description;
};

struct FinnySettings
{
	Preset StartFreq;
	bool UpdateStartFreqOnClose;
	float StartVolume;
	bool UpdateStartVolumeOnClose;
	string RecordingPath;
	MP3Settings MP3;
	bool AutogenerateRecordingNames;
	string VisualizationName;
	bool PokeScreensaver;
	bool LoggingEnabled;
	bool UseXvimagesink;
	
	FinnySettings()
		:StartFreq(false,99.5f)
		,UpdateStartFreqOnClose(true)
		,StartVolume(0.0f)
		,UpdateStartVolumeOnClose(false)
		,RecordingPath("./")
		,AutogenerateRecordingNames(true)
		,VisualizationName("monoscope")
		,PokeScreensaver(true)
		,UseXvimagesink(true)
	{
		char * pHome;
		pHome = getenv ("HOME");
		if (pHome!=NULL)
		{
			RecordingPath = string(pHome);
			RecordingPath+"/";
		}
	};
	//General helpers
	static void LoadSetting( FinnySettings& settings, ifstream& infile);
	static void WriteSettings( FinnySettings& settings, ofstream& outfile);
	static void ReadBool(ifstream& infile, bool& value);
	static void WriteBool(ofstream& outfile, bool value);
	static void ReadFloat(ifstream& infile, float& value);
	static void WriteFloat(ofstream& outfile, float value);
};

#endif // __FINNYSETTINGS_H__
