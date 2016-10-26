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
#ifndef __LOGGING_H__
#define __LOGGING_H__

#include <string>
#include <fstream>

using namespace std;

class Logger
{
	protected:
	Logger();
	~Logger();
	static Logger m_Instance;
	ofstream m_Outfile;
	bool m_bEnabled;
	
	public:
	static void Write(const char* msg);
	static void Enable(void);
	static void Disable(void);
	static bool IsEnabled(void);
};

#endif // __LOGGING_H__
