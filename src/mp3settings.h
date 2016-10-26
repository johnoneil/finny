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
#ifndef __MP3SETTINGS_H__
#define __MP3SETTINGS_H__

#include <string>
#include <fstream>

using namespace std;

struct MP3Settings
{
	enum Mode
	{
		Stereo = 0,
		Joint_Stereo = 1,
		Dual_Channel = 2,
		Mono = 3,
		Auto = 4
	};
	
	std::string filename;
	int bitrate; //(8 to 320)
	Mode mode;
	
	MP3Settings()
		:filename("recording.mp3")
		,bitrate(128)
		,mode(MP3Settings::Auto)
	{
		
	};
	//Some helpers
	static string mode2string(MP3Settings::Mode md);
	static MP3Settings::Mode string2mode( const string& md);
	
	//File IO
	void Read(ifstream& infile);
	void Write(ofstream& outfile);
};

#endif // __MP3SETTINGS_H__
