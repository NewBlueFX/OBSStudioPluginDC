//	DirectConnectionCrossProcessObjects.h
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

#include "SharedMemory.h"
#include "DirectConnectionTypes.h"
#include <list>


namespace DCTL
{

// If current module is client (some plugin): use the function StandaloneApplication::IsLaunched() to know
// if Standalone app is launched at current time.
// If current module is server (Standalone app): the instance of this class will be exist during lifetime of app and
// therefore client can know if app is launched.
class StandaloneApplication
{
    StandaloneApplication(const StandaloneApplication &str);	// not implemented
    StandaloneApplication& operator=(const StandaloneApplication &str);	// not implemented

public:

	// pszSkuName should contain only letters and numbers, e.g. "OvrStream", "TitlerLive", "NTX".
	StandaloneApplication(const char* pszSkuName, const char* appName, bool* pbResult);
	~StandaloneApplication();

public:
	// Return 'true' when Standalone application is launched at current time.
	static bool IsLaunched(const char* pszSkuName, const char* appName);

	// Try to launch standalone app.
	static bool LaunchStandaloneApplication(const char* pszSkuName, const char* appName);

private:

	CSharedMemory	m_systemObject;
};

// When current module is client (some plugin), just use ServerPortAndChannels::GetAvailableSources() to take
// all available sources on this PC at current time.
// When current module is server (Standalone app), it will create the instance of this class when "Direct Connection"
// output is selected for Outputs 
class ServerPortAndChannels
{
    ServerPortAndChannels(const ServerPortAndChannels &str);	// not implemented
    ServerPortAndChannels& operator=(const ServerPortAndChannels &str);	// not implemented

public:

	ServerPortAndChannels(const char* pszSkuName, const char* pHostPID);
	~ServerPortAndChannels();

	// Set this port and channels to system object.
	// In this function will be initialized global system object.
	bool SetPortAndChannels(const PortAndChannels& portAndChannels);

public:
	// Return array of ports with channels which are active right now.
	static std::list<PortAndChannels> GetAvailableSources(const char* pszSkuName, const char* pHostPID);

private:

	bool m_portAndChannelsValid = false;
	PortAndChannels m_currentPortAndChannels;
	CSharedMemory m_systemObject;
	const std::string m_skuName;
	std::string m_sHostPID;
};


}	// end namespace DCTL
