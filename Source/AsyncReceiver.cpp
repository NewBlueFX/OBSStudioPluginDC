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



#include "initguid.h"
#include "AsyncReceiver.h"
#include <sstream>
#include <set>
#include <iostream>
#include <algorithm>

#include "DirectConnectionLibLink.h"
#include "DirectConnectionNTXInterfaces.h"


namespace Common
{

namespace
{

// Convert seconds to nanoseconds
inline __int64 Sec2NSec(double seconds)
{
	return static_cast<__int64>(1000000000.0 * seconds + 0.5);
}

class ElapsedTime
{
public:
	ElapsedTime();
	void   restart();
	double elapsedTime() const;

private:
	std::chrono::steady_clock::time_point m_startPoint;
};

ElapsedTime::ElapsedTime()
{
	restart();
}

void ElapsedTime::restart()
{
	m_startPoint = std::chrono::steady_clock::now();
}

double ElapsedTime::elapsedTime() const
{
	std::chrono::steady_clock::time_point endPoint = std::chrono::steady_clock::now();
	double dSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(endPoint - m_startPoint).count() / 1000.0;
	return dSeconds;
}

}

std::recursive_mutex AsyncReceiver::s_libLinkLock;
DCTL::DirectConnectionLibLink* AsyncReceiver::s_pLibLink = nullptr;

AsyncReceiver::AsyncReceiver(const std::string& groupName, const std::string& hostName, const std::string& skuName)
	: m_GroupName(groupName)
	, m_bProcessing(false)
	, m_dDesirableFrameRate(30)
	, m_iDesirableWidth(1920)
	, m_iDesirableHeight(1080)
	, m_dActualFrameRate(30)
	, m_iActualWidth(1920)
	, m_iActualHeight(1080)
	, m_bInterlaced(false)
	, m_bBGRA(false)
	, m_iAudioSampleRate(48000)
	, m_iAudioNumChannels(2)
	, m_pVideoDataSend(nullptr)
	, m_pAudioDataSend(nullptr)
	, m_pContext(nullptr)
	, m_hostName(hostName)
	, m_skuName(skuName)
{
}

AsyncReceiver::~AsyncReceiver()
{
	Deinit();
}

bool AsyncReceiver::Init(void * context, VideoDataReady_t videoProc, AudioDataReady_t audioProc,
	bool bBGRA /* else UYVY */, int width, int height, double frameRate, int audioSampleRate, int audioNumChannels)
{
	if (width < 0 || height < 0 || frameRate <= 0 || audioSampleRate < 0 || audioNumChannels < 0)
	{
		return false;
	}

	assert(videoProc && audioProc);
	if (!videoProc || !audioProc)
	{
		return false;
	}

	Deinit();
	std::lock_guard<std::recursive_mutex> lock(m_csReceiverGuard);

	m_pVideoDataSend = videoProc;
	m_pAudioDataSend = audioProc;
	m_pContext = context;
	// There is no necessary to lock m_csSettingsGuard here, because the thread is not started yet.
	m_bBGRA = bBGRA;
	m_iDesirableWidth = width;
	m_iDesirableHeight = height;
	m_dDesirableFrameRate = frameRate;
	m_bInterlaced = false;
	m_iAudioSampleRate = audioSampleRate;
	m_iAudioNumChannels = audioNumChannels;

	return true;
}

void AsyncReceiver::Deinit()
{
	Stop();

	std::lock_guard<std::recursive_mutex> lock(m_csReceiverGuard);
	m_pVideoDataSend = nullptr;
	m_pAudioDataSend = nullptr;
	m_pContext = nullptr;
}

void AsyncReceiver::Start()
{
	std::lock_guard<std::recursive_mutex> lock(m_csReceiverGuard);
	assert(m_bProcessing == false);
	if(m_bProcessing)
	{
		return;
	}

	m_bProcessing = true;
	m_senderThread = std::thread([this]{
		SendData();
	});
}

void AsyncReceiver::Stop()
{
	std::lock_guard<std::recursive_mutex> lock(m_csReceiverGuard);
	if(!m_bProcessing)
	{
		return;
	}

	m_bProcessing = false;
	if (m_senderThread.joinable())
	{
		try 
		{
			m_senderThread.join();
		}
		catch (const std::exception& exc)
		{
			std::cout << "Failed to join thread " << exc.what() << std::endl;
		}
	}
}

unsigned int AsyncReceiver::GetActualWidth() const
{
	std::lock_guard<std::recursive_mutex> lock(m_csSettingsGuard);
	return m_iActualWidth;
}

unsigned int AsyncReceiver::GetActualHeight() const
{
	std::lock_guard<std::recursive_mutex> lock(m_csSettingsGuard);
	return m_iActualHeight;
}

double AsyncReceiver::GetActualFrameRate() const
{
	std::lock_guard<std::recursive_mutex> lock(m_csSettingsGuard);
	return m_dActualFrameRate;
}

void AsyncReceiver::UnloadResources()
{
	std::lock_guard<std::recursive_mutex> lock(s_libLinkLock);
	if (s_pLibLink)
	{
		delete s_pLibLink;
		s_pLibLink = nullptr;
	}
}

std::string AsyncReceiver::MakeConnectionMetadata(const MediaSettings* pSettings) const
{
	// Receiver Name and Resolution Information
	std::ostringstream stream;

	// open metadata Root node
	stream << "<metadata>";

	if (pSettings)
	{
		// add media_settings node
		stream << "<media_settings width=\"" << pSettings->iWidth << "\" "
			<< "height=\"" << pSettings->iHeight << "\" "
			<< "framerate=\"" << pSettings->dFrameRate << "\" "
			<< "interlaced=\"" << (pSettings->bInterlaced ? "true" : "false")  << "\" "
			<< "audio_sample_rate=\"" << pSettings->iAudioSampleRate << "\" "
			<< "audio_channels=\"" << pSettings->iAudioChannels << "\" "
			<< "/>";
	}

	{
		// add receiver_info node with host name
		stream << "<receiver_info name=\"" << m_hostName << "\" />";
	}

	// close metadata Root node
	stream << "</metadata>";

	return stream.str();
}

void AsyncReceiver::SendData()
{
	// We do the copy of video and audio settings. It is done for future when these settings will be changed dynamically.
	// And we use this copy, when settings in the class will be changed, we will compare it and will reinitialize 'instance'.
	MediaSettings currentSettings;	// Current video and audio settings

	// Main reference of DirectConnection.
	DCTL::IDirectConnection* pInstance = nullptr;

	// Buffer for video frame. To prevent reallocations will hold it during whole source lifetime
	std::vector<BYTE> videoFrameBuffer;
	// The video frame, it wraps the memory of videoFrameBuffer.
	DCTL::VideoFrameParams videoFrame;

	// Buffer for audio frame. To prevent reallocations will hold it during whole source lifetime
	std::vector<float> audioFrameBuffer;
	// The audio frame, it wraps the memory of audioFrameBuffer.
	DCTL::AudioFrameParams audioFrame;

	// We request this time from DC instance through InitializeDirectConnection and then we use it for GetVideoFrame/GetAudioFrame,
	// We are incrementing it using current frame rate.
	double dLiveTime = 0.0;

	// This time counter is used to measure the rendering time and sleep the remaining time before m_nextTimeToSleep.
	ElapsedTime time;
	double dNextTimeToSleep = 0.0;	// It is incrementing by 1/m_dFrameRate.

	bool initialSettingsSent = false;

	while(m_bProcessing)
	{
		unsigned int uiSleep = 50;	// in milliseconds

		if (!pInstance)
		{
			uiSleep = 500;

			if (!s_pLibLink)
			{
				std::lock_guard<std::recursive_mutex> lock(s_libLinkLock);
				if (!s_pLibLink)
				{
					const std::wstring unicodeHostName(m_hostName.begin(), m_hostName.end());
					const std::wstring unicodeSkuName(m_skuName.begin(), m_skuName.end());

					s_pLibLink = new DCTL::DirectConnectionLibLink(unicodeHostName.data(), unicodeSkuName.data(), true, false);
				}
			}

			// Attempt to create connection.
			if (s_pLibLink)
			{
				// Take all available sources
				const std::list<DCTL::PortAndChannels>& portsAndChannels = s_pLibLink->GetAvailableSources();
				// Check that at least one port exists
				if (!portsAndChannels.empty())
				{
					const DCTL::PortAndChannels& portData = portsAndChannels.front();
					// Check that at least one channel exists
					if (!portData.second.empty())
					{
						// Create the DirectConnection Instance and initialize.
						pInstance = s_pLibLink->CreateInstance(portData.first, portData.second.front());

						if (pInstance)
						{
							dLiveTime = 0.0;	// Reset this value, just in case.

							// Initialize DirectConnection instance on the side of standalone.
							DCTL::DCResult res = pInstance->InitializeDirectConnection(
								m_iDesirableWidth, m_iDesirableHeight, m_dDesirableFrameRate, 1.0, m_bInterlaced,
								m_iAudioSampleRate, m_iAudioNumChannels, &dLiveTime);
							assert(res == DCTL::DCResult::DCERR_OK);
							if (res != DCTL::DCResult::DCERR_OK)
							{
								pInstance->Release();
								pInstance = nullptr;
							}
							else
							{
								dLiveTime += 1.0 / m_dDesirableFrameRate;	// We need to render next frame.
								dNextTimeToSleep = 1.0 / m_dDesirableFrameRate;	// New instance created, reset this time.
								initialSettingsSent = false;

								{
									std::lock_guard<std::recursive_mutex> lock(s_libLinkLock);
									double dPixelAspectRatio;
									bool bInterlaced;
									pInstance->GetVideoSettings(&m_iActualWidth, &m_iActualHeight, &m_dActualFrameRate, &dPixelAspectRatio, &bInterlaced);
								}
								time.restart();
							}
						}
					}
				}
			}
		}

		if (pInstance && m_bProcessing)
		{
			// Compare all video settings
			if (m_iActualWidth != currentSettings.iWidth || m_iActualHeight != currentSettings.iHeight || m_dActualFrameRate != currentSettings.dFrameRate ||
				// now compare all audio settings
				m_iAudioSampleRate != currentSettings.iAudioSampleRate || m_iAudioNumChannels != currentSettings.iAudioChannels)
			{
				{
					std::lock_guard<std::recursive_mutex> lock(m_csSettingsGuard);
					assert(m_iActualWidth > 0 && m_iActualHeight > 0 && m_dActualFrameRate > 0);
					// Copy video settings
					currentSettings.dFrameRate = m_dActualFrameRate;
					currentSettings.iWidth = m_iActualWidth;
					currentSettings.iHeight = m_iActualHeight;
					currentSettings.bInterlaced = m_bInterlaced;
					assert(m_iAudioSampleRate > 0 && m_iAudioNumChannels > 0);
					// Copy audio settings
					currentSettings.iAudioSampleRate = m_iAudioSampleRate;
					currentSettings.iAudioChannels = m_iAudioNumChannels;

					// If metadata was sent, need to send it again.
					initialSettingsSent = false;
					// We copied settings and can free the lock.
				}

				// Reallocate video frame
				videoFrameBuffer.resize(currentSettings.iHeight * currentSettings.iWidth * 4);	// 4 - byte depth for BGRA8, 4 bytes per pixel.

				videoFrame = DCTL::VideoFrameParams(videoFrameBuffer.data(), currentSettings.iWidth, currentSettings.iHeight, currentSettings.iWidth * 4, 4, DCTL::VPF_BGRA8);
				videoFrame.m_bFlipped = true;

				// Reallocate audio frame
				audioFrameBuffer.resize(currentSettings.iAudioSampleRate * currentSettings.iAudioChannels);

				audioFrame = DCTL::AudioFrameParams(audioFrameBuffer.data(), currentSettings.iAudioSampleRate,
					currentSettings.iAudioChannels, currentSettings.iAudioSampleRate);
			}

			if (!initialSettingsSent)
			{
				const std::string & str = MakeConnectionMetadata(&currentSettings);
				initialSettingsSent = pInstance->SendMetadataFrame(str.c_str()) == DCTL::DCResult::DCERR_OK;
			}

			// We do not allow to change original dctlFrame;
			DCTL::VideoFrameParams tempVideoFrame = videoFrame;
			DCTL::DCResult res = pInstance->GetVideoFrame(&tempVideoFrame, dLiveTime);

			if (res == DCTL::DCResult::DCERR_OK || res == DCTL::DCResult::DCERR_OK_VideoSettingsNotMatch)
			{
				const __int64 currentTimestamp = Sec2NSec(dLiveTime);

				if (m_pVideoDataSend && m_bProcessing)
				{
					m_pVideoDataSend(
						m_pContext,
						tempVideoFrame.m_pBuffer,
						tempVideoFrame.m_nWidth,
						tempVideoFrame.m_nHeight,
						tempVideoFrame.m_nSpan,
						currentTimestamp);
				}

				DCTL::AudioFrameParams tempAudioFrame = audioFrame;

				if (m_bProcessing &&
					pInstance->GetAudioFrame(&tempAudioFrame, dLiveTime) == DCTL::DCResult::DCERR_OK &&
					// At this moment OvrStream does not support the audio.
					// When it will be supported OvrStream will change m_nSampleCount, therefore
					// we just check this value that it was changed or not to be sure that we got valid audio samples.
					audioFrame.m_nSampleCount != tempAudioFrame.m_nSampleCount)
				{
					if (m_pAudioDataSend && m_bProcessing)
					{
						m_pAudioDataSend(m_pContext,
							tempAudioFrame.m_pBuffer,
							tempAudioFrame.m_nSampleCount * sizeof(float),
							tempAudioFrame.m_nChannels,
							tempAudioFrame.m_nSampleCount,
							tempAudioFrame.m_nSampleRate,
							currentTimestamp);
					}
				}

				if (res == DCTL::DCResult::DCERR_OK_VideoSettingsNotMatch)
				{
					std::lock_guard<std::recursive_mutex> lock(s_libLinkLock);
					double dPixelAspectRatio;
					bool bInterlaced;
					pInstance->GetVideoSettings(&m_iActualWidth, &m_iActualHeight, &m_dActualFrameRate, &dPixelAspectRatio, &bInterlaced);
				}

				const double dTimeStep = 1.0 / currentSettings.dFrameRate;
				double dElapsedTime = time.elapsedTime();

				while (m_bProcessing && dElapsedTime < dNextTimeToSleep)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
					dElapsedTime = time.elapsedTime();
				}

				// Step to next time for rendering.
				dLiveTime += dTimeStep;

				// Convert the elapsed time to frames to have the aligned value in frames.
				// The timer can return the value which is much more than current dNextTimeToSleep in the following cases:
				//	1. The PC is resuming from a low-power state.
				//	2. The big delay was occured between two calls of GetVideoFrame(), more than 1/fps.
				// Therefore recalculate dNextTimeToSleep to align with the timer.
				const __int64 iElapsedFrames = static_cast<__int64>(0.5 + dElapsedTime / dTimeStep);

				dNextTimeToSleep = iElapsedFrames * dTimeStep;
				// Step to next time for sleep.
				dNextTimeToSleep += dTimeStep;

				uiSleep = 0;	// We handle the time to sleep ourselves.
			}
			else if (res == DCTL::DCResult::DCERR_ServerShutdown)
			{
				// Remove connection and restart the initialization again.
				if (pInstance)
				{
					pInstance->Release();
					pInstance = nullptr;
					uiSleep = 0;
				}
			}
		}

		if (uiSleep > 0)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(uiSleep));
		}
	}

	// Clear DirectConnection instance
	if (pInstance)
	{
		pInstance->Release();
		pInstance = nullptr;
	}
	m_bProcessing = false;
}

}   // end namespace Common
