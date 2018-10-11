/*	PlatformWin.h
/*
MIT License

Copyright (c) 2016,2018 NewBlue, Inc. <https://github.com/NewBlueFX>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once


#ifndef WINVER				// Allow use of features specific to Windows Vista or later.
#define WINVER 0x0600		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows Vista or later.                   
#define _WIN32_WINNT 0x0600	// Change this to the appropriate value to target other versions of Windows.
#endif						

//#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
//#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
//#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 7.0 or later.
#define _WIN32_IE 0x0700	// Change this to the appropriate value to target other versions of IE.
#endif

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#define NOMINMAX

#include <tchar.h>
#include <basetyps.h>
#include <windows.h>
#include <unknwn.h>
#include <process.h>

#pragma warning(disable: 4996) 


#define NBAbs(N)		( (N) < 0    ? (-(N)) : (N))



#define GetAValue(rgb)      (LOBYTE((rgb)>>24))


// useful define
#ifndef CheckPointer
	#define CheckPointer(p,ret)		{if((p)==NULL) return (ret);}
#endif



#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <iosfwd>

struct Dummy {
    static void Assert(bool condition, const char* expression, const char* file, long line) 
    {
        if (!condition) {
            char* logstr= new char[1024];
            sprintf_s(logstr, 1024, "%s \nin file %s\nat line %d\n\n\nPlease send text (or screenshot) of this error to developers.", expression, file, line);
            MessageBoxA(0, logstr, "ASSERT", 0);
            delete []logstr;
            exit(0);
        }
    }
};

#if defined(USE_ASSERT)
#define NBAssert(x) Dummy::Assert((x) != 0, #x, __FILE__, __LINE__) 
#elif !defined(NDEBUG)
#define NBAssert(x)		assert(x)
#else
#define NBAssert(x)		void(0)
#endif

#define NBForceAssert(x) Dummy::Assert((x) != 0, #x, __FILE__, __LINE__) 

struct NBAssertStrHelper
{
	static void ShowErrorBox(const char * msg, ...)
	{
		char text[1024];
		va_list arg_list;
		va_start(arg_list, msg);
		  vsprintf(text, msg, arg_list);
		va_end(arg_list);
		MessageBoxA(NULL, text, "Error", MB_OK | MB_ICONERROR);
	}
};

#if !defined(NDEBUG) || defined(USE_ASSERT)
#define NBAssertStr(x, msg, ...)	\
	if (!(x)) \
	{	\
		NBAssertStrHelper::ShowErrorBox(msg, __VA_ARGS__); \
		assert(x); \
	}

#else
#define NBAssertStr(x, msg, ...)	void(0)

#endif