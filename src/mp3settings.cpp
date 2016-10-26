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
#include "mp3settings.h"
string MP3Settings::mode2string(MP3Settings::Mode md)
{
	switch(md)
	{
		case Stereo:
			return string("Stereo");
			break;
		case Joint_Stereo:
			return string("Joint_Stereo");
			break;
		case Dual_Channel:
			return string("Dual_Channel");
			break;
		case Mono:
			return string("Mono");
			break;
		case Auto:
			return string("Auto");
			break;
	}
	return string("Auto");
}
MP3Settings::Mode MP3Settings::string2mode( const string& md)
{
	if( md == "Stereo")
	{
		return MP3Settings::Stereo;
	}else if(md == "Joint_Stereo")
	{
		return MP3Settings::Joint_Stereo;
	}else if( md == "Dual_Channel" )
	{
		return MP3Settings::Dual_Channel;
	}else if( md == "Mono")
	{
		return MP3Settings::Mono;
	}else if( md == "Auto")
	{
		return MP3Settings::Auto;
	}else{
		return MP3Settings::Auto;
	}
}
void MP3Settings::Read(ifstream& infile)
{
	string md;
	infile>>bitrate;
	infile>>md;//mode;
	mode = MP3Settings::string2mode(md);
	
}
void MP3Settings::Write(ofstream& outfile)
{
	string md = MP3Settings::mode2string(mode);
	outfile<<"SETTING: MP3 "<<bitrate<<" "<<md<<endl;
}
