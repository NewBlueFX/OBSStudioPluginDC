/*	DirectConnectionLibLink.h
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

#include "DirectConnectionInterface.h"
#include "DirectConnectionCrossProcessObjects.h"
#include <string>
#include <mutex>
#include <list>
#include <memory>


namespace DCTL
{

class DirectConnectionInstanceCreator;

// Class used for managing connection with the Standalone (server).
// This class is thread-safe.
class DirectConnectionLibLink
{
	using InstanceCreatorPtr = std::shared_ptr<DirectConnectionInstanceCreator>;

public:

	// Constructor takes name of host (used to identify connections).
	// Takes SKU name to construct file path for loading of Standalone.
	explicit DirectConnectionLibLink(const wchar_t* pwszHostName, const wchar_t* pwszSkuName, bool bLaunchAppOnDemand, 
		bool bEnableMultipleAppInstances);
	~DirectConnectionLibLink();

	// Clear all currently active connections.
	void Clear();

	// Return array of ports with channels which are active right now.
	std::list<PortAndChannels> GetAvailableSources();

	// Create IDirectConnection instance for given port and channel.
	// Returns addrefed instance.
	IDirectConnection* CreateInstance(Port port, Channel channel);

	// Uses the list of instance creators to end external ntx process
	DCResult CloseNTXProcesses();

protected:

	// Store our host's and SKU's name
	const std::wstring m_hostName;
	const std::wstring m_wideSkuName;
	const std::string m_skuName;

	// Array of creators which will create IDirectConnection instances.
	// These creators manage connections.
	std::list<InstanceCreatorPtr>	m_instanceCreators;

	// For managing multi-threading.
	mutable std::recursive_mutex m_instanceCreatorsLock;

	std::string m_sHostPID;
	bool m_bLaunchAppOnDemand = true;
	// if m_bEnableMultipleAppInstances == false will be used only one app instance
	// for example NTX can start multiple app(NTX) instances for host
	// but OBS should use only one generic instance
	bool m_bEnableMultipleAppInstances = false;    
};

}	// end namespace DCTL
