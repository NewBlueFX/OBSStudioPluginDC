/*
This file is part of OvrStream_OBS.

OvrStream_OBS is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OvrStream_OBS is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with OvrStream_OBS.  If not, see <https://www.gnu.org/licenses/>.
*/


#pragma once

#include <vector>
#include <string>
#include <thread>
#include <mutex>

namespace DCTL
{
    class DirectConnectionLibLink;
}

namespace Common
{

class AsyncReceiver
{
	AsyncReceiver(const AsyncReceiver &) = delete;
	AsyncReceiver& operator=(const AsyncReceiver &) = delete;

public:

	// Functions which owner will use to receive audio/video data from NDI or DC.
	typedef void(*VideoDataReady_t)(void* context, const unsigned char* pbData, unsigned int xres, unsigned int yres, unsigned int rowBytes, __int64 timeCode);
	typedef void(*AudioDataReady_t)(void* context, const float* pbData, unsigned int channelStrideInBytes, unsigned int channels, unsigned int frames, unsigned int samplesPerSecond, __int64 timeCode);

	// c-r fills required names for host and sku, group name is used by NDI only now.
	AsyncReceiver(const std::string& groupName, const std::string& hostName, const std::string& skuName);
	~AsyncReceiver();

	bool Init(void* context, VideoDataReady_t videoProc, AudioDataReady_t audioProc,
		bool bBGRA /* else UYVY */, int width, int height, double frameRate, int audioSampleRate, int audioNumChannels);
	void Deinit();

	void Start();
	void Stop();

	unsigned int GetActualWidth() const;
	unsigned int GetActualHeight() const;
	double GetActualFrameRate() const;

	static void UnloadResources();

private:

	struct MediaSettings
	{
		int iWidth = 0;
		int iHeight = 0;
		double dFrameRate = 0;
		bool bInterlaced = false;
		int iAudioSampleRate = 0;
		int iAudioChannels = 0;
	};

	// If pSettings is valid, the node for media settings will be added to metadata.
	std::string MakeConnectionMetadata(const MediaSettings* pSettings) const;

	void SendData();

private:
	std::recursive_mutex	m_csReceiverGuard;      // Lock for Init/Deinit/Start/Stop.

	volatile bool			m_bProcessing;
	std::thread				m_senderThread;

	mutable std::recursive_mutex	m_csSettingsGuard;      // For video and audio settings

	double					m_dDesirableFrameRate;	// Video/audio framerate requested by OBS
	int						m_iDesirableWidth;		// Video frame width requested by OBS
	int						m_iDesirableHeight;		// Video frame height requested by OBS

	double					m_dActualFrameRate;		// Video/audio framerate actually set by OVRStream
	int						m_iActualWidth;			// Video frame width actually set by OVRStream
	int						m_iActualHeight;		// Video frame height actually set by OVRStream


	bool					m_bInterlaced;          // Video frame interlaced
	int						m_iAudioSampleRate;     // Audio sample rate
	int						m_iAudioNumChannels;    // The number of channels for audio

	bool					m_bBGRA;
	VideoDataReady_t		m_pVideoDataSend;
	AudioDataReady_t		m_pAudioDataSend;
	void*					m_pContext;

	const std::string		m_GroupName;
	// Current host name
	const std::string		m_hostName;
	const std::string		m_skuName;

	static std::recursive_mutex			    s_libLinkLock;
	static DCTL::DirectConnectionLibLink*	s_pLibLink;
};

}
