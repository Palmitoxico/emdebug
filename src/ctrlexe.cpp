/**
 * @file ctrlexe.cpp
 * @brief Library for control child processes
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

#include "ctrlexe.h"
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sysexits.h>
#include <unistd.h>
#include <sys/types.h>
#include <iostream>
#include <sys/select.h>
#include <chrono>

ctrlexe::ctrlexe()
{
	proc_pid = -1;
	read_timeout = -1;
}

ctrlexe::~ctrlexe()
{
	KillProcess();
}

void ctrlexe::StartProcess(const char *Cmd, char const * const argv[])
{
	int child_err[2];
	int count, err;
	if (IsRunning())
	{
		return;
	}
	pipe(proc_in);
	pipe(proc_out);
	pipe(child_err);
	fcntl(child_err[1], F_SETFD, fcntl(child_err[1], F_GETFD) | FD_CLOEXEC);

	proc_pid = fork();
	if (proc_pid == 0) //child
	{
		int nargs;
		
		const char **_argv;
		if (argv == NULL)
		{
			_argv = new const char*[2];
			_argv[0] = Cmd;
			_argv[1] = NULL;
		}
		else
		{
			for (nargs = 1; argv[nargs - 1] != NULL; nargs++);
			_argv = new const char*[nargs + 1];
			_argv[0] = Cmd;
			_argv[nargs] = NULL;
			for (nargs = 1; argv[nargs] != NULL; nargs++)
			{
				_argv[nargs] = argv[nargs];
			}
		}
		close(child_err[0]);

		/*Associates stdin to proc_in and stdout to proc_out*/
		dup2(proc_in[0], STDIN_FILENO);
		dup2(proc_out[1], STDOUT_FILENO);

		/*Child closes the input side of proc_out and the output side of proc_in*/
		close(proc_out[0]);
		close(proc_in[1]);

		/*Pass the control to the process*/
		if (execvp(_argv[0], (char**)argv) == -1)
		{
			write(child_err[1], &errno, sizeof(int));
			_exit(0);
		}
	}

	else if (proc_pid == -1)
	{
		throw std::runtime_error("Unable to fork!");
	}
	else
	{
		close(child_err[1]);
		while ((count = read(child_err[0], &err, sizeof(errno))) == -1)
		{
			if (errno != EAGAIN && errno != EINTR) break;
		}

		if (count)
		{
			throw std::runtime_error(strerror(err));
			proc_pid = -1;
		}
		close(child_err[0]);
		close(proc_in[0]);
		close(proc_out[1]);
	}
}

void ctrlexe::ReadLine(std::string &Str, const char delimiter)
{
	fd_set set;
	struct timeval timeout;
	int rv;
	char buf[2];
	FD_ZERO(&set);
	FD_SET(proc_out[0], &set);
	Str = "";
	auto start = std::chrono::steady_clock::now();
	
	if (proc_pid > 0)
	{
		if (read_timeout >= 0)
		{
			while(1)
			{
				timeout.tv_sec = read_timeout/2000;
				timeout.tv_usec = (read_timeout % 2000)*1000;
				rv = select(proc_out[0] + 1, &set, NULL, NULL, &timeout);
				if (rv == -1)
				{
					throw std::runtime_error("Erro select!");
				}
				auto duration = std::chrono::duration_cast<std::chrono::milliseconds>
					(std::chrono::steady_clock::now() - start);
				if ((duration.count() >= read_timeout) && (read_timeout >= 0))
				{
					std::cout << duration.count() << std::endl;
					throw std::runtime_error("Timeout!");
					return;
				}
				if (rv != 0)
				{
					if (read(proc_out[0], buf, 1) == 0)
					{
						throw std::runtime_error("Read = 0!");
					}
					else
					{
						buf[1] = 0;
						if (buf[0] != delimiter)
						{
							Str += buf;
						}
						else
						{
							return;
						}
					}
				}
			}
		}
		else
		{
			while(1)
			{
				if (read(proc_out[0], buf, 1) == 0)
				{
					throw std::runtime_error("Read = 0!");
				}
				else
				{
					buf[1] = 0;
					if (buf[0] != delimiter)
					{
						Str += buf;
					}
					else
					{
						return;
					}
				}
			}
		}
	}	
	else
	{
		throw std::runtime_error("Pid = -1");
	}
}

int ctrlexe::ReadBytes(char *buffer, int bsize)
{
	struct timeval timeout;
	fd_set set;
	FD_ZERO(&set);
	FD_SET(proc_out[0], &set);

	if (read_timeout >= 0)
	{
		timeout.tv_sec = read_timeout/1000;
		timeout.tv_usec = (read_timeout % 1000)*1000;
		if (select(proc_out[0] + 1, &set, NULL, NULL, &timeout) == 0)
		{
			throw std::runtime_error("Timeout!");
		}
	}
	return read(proc_out[0], buffer, bsize);
}

void ctrlexe::SetReadTimeout(int timeout)
{
	read_timeout = timeout;
}

void ctrlexe::KillProcess()
{
	if (proc_pid != -1)
	{
		kill(proc_pid, SIGTERM);
		proc_pid = -1;
	}
}

void ctrlexe::WriteString(const char *Str)
{
	if (write(proc_in[1], Str, sizeof(Str)) < 0)
	{
		throw std::runtime_error("Write!");
	}
}

void ctrlexe::WriteString(const std::string &Str)
{
	if (write(proc_in[1], Str.c_str(), Str.size()) < 0)
	{
		throw std::runtime_error("Write!");
	}
}

void ctrlexe::WriteBytes(const char *buffer, int nbytes)
{
	if (write(proc_in[1], buffer, nbytes) < 0)
	{
		throw std::runtime_error("Write!");
	}
}

bool ctrlexe::IsRunning()
{
	if (proc_pid != -1)
	{
		if (kill(proc_pid, 0) < 0)
		{
			proc_pid = -1;
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		return false;
	}
}
