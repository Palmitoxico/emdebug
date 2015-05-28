/*
Copyright (c) 2015, Augusto Fraga Giachero
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL AUGUSTO FRAGA GIACHERO BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "simple_exception.h"

SimpleException::SimpleException(const char *_StrMessage,  const char *_StrDbgMessage, int _ExceptionType)
{
	StrMessage = new char[strlen(_StrMessage) + 1];
	StrDbgMessage = new char[strlen(_StrDbgMessage) + 1];

	strcpy(StrMessage, _StrMessage);
	strcpy(StrDbgMessage, _StrDbgMessage);
}

SimpleException::~SimpleException()
{
	delete [] StrDbgMessage;
	delete [] StrMessage;
}

const char *SimpleException::Message()
{
	return StrMessage;
}

const char *SimpleException::DebugMessage()
{
	return StrDbgMessage;
}

int SimpleException::Type()
{
	return ExceptionType;
}