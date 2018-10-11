//	DirectConnectionTypes.h
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

#include "Platform.h"
#include <vector>

// The size of DCTLInstanceID type should be the same for all architectures (x86, x86_64).

#ifdef _WIN32

// Take types from generated file by IDL
#include "DirectConnectionTLTypes.h"

typedef DWORD DCTLProcessID;

#elif __APPLE__

typedef pid_t DCTLProcessID;
typedef uint64_t DCTLClientID;
typedef uint64_t DCTLInstanceID;

enum DCTLSharedMemoryType
{
    DCTLSM_RENDER_VIDEO_FRAME = 0,
    DCTLSM_RENDER_AUDIO_FRAME = 1,
	DCTLSM_RENDER_HOST_FRAME1 = 2,
	DCTLSM_RENDER_HOST_FRAME2 = 3,
};

#else
#   error Unsupported platform
#endif

// Declare own types
namespace DCTL
{

	typedef int Port;
	typedef int Channel;
	typedef std::vector<Channel> ChannelList;
	typedef std::pair<Port, ChannelList> PortAndChannels;

}	// end namespace DCTL
