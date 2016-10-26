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
#include "logging.h"
#include <stdio.h>
#include <stdlib.h>
Logger Logger::m_Instance;

Logger::Logger()
	:m_bEnabled(false)
{
	
}
Logger::~Logger()
{
	m_Outfile.close();
}
void Logger::Write(const char* msg){
	if(m_Instance.m_bEnabled)
	{
		m_Instance.m_Outfile<<msg<<endl;
	}
}
void Logger::Enable(void)
{
	if(!m_Instance.m_Outfile.is_open())
	{
		//Open our logfile in the user's home
		string dir;
		char* home_dir = getenv("HOME");
		if(home_dir!=NULL)
		{
			dir = string(home_dir);
		}
		dir+="/finny.log";
		m_Instance.m_Outfile.open(dir.c_str());
	}
	m_Instance.m_bEnabled = true;
}
void Logger::Disable(void)
{
	m_Instance.m_bEnabled = false;
}
bool Logger::IsEnabled(void)
{
	return m_Instance.m_bEnabled;
}
