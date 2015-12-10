/**
 * @file ctrlexe.h
 * @brief Header for ctrlexe library
 */

/*
 * This program source code file is part of EmDebug, a free front-end GUI for GDB + OpenOCD.
 *
 * Copyright (C) 2014-2015 Augusto Fraga Giachero
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License V2
 * as published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */
#include <string>
#include <exception>
#include "simple_exception.h"

#ifndef CTRLEXE_HEADER
#define CTRLEXE_HEADER

class ctrlexe
{
public:
	ctrlexe();
	~ctrlexe();
	void StartProcess(const char *Cmd, char const * const argv[]);
	void KillProcess();
	void WriteBytes(const char *buffer, int nbytes);
	void WriteString(const char *Str);
	void WriteString(const std::string &Str);
	int ReadBytes(char *buffer, int bsize);
	void ReadLine(std::string &Str, char delimiter);
	void SetReadTimeout(int r_timeout);
	bool IsRunning();
private:
	int proc_out[2], proc_in[2], proc_pid, read_timeout;

};
#endif
