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

#include <string>

struct obs_data;
struct obs_source;
typedef struct obs_data    obs_data_t;
typedef struct obs_source  obs_source_t;

namespace Common
{
	class AsyncReceiver;
}

// This is source plugin for OBS Studio application, which work in asynchronous mode with host.
class SourcePlugin
{
	SourcePlugin(const SourcePlugin &);
	SourcePlugin& operator=(const SourcePlugin &);

public:

	SourcePlugin(const bool bUseBGRA, const std::string& groupName, const std::string& hostName,
		const std::string& skuName, obs_data_t* settings, obs_source_t* source);
	~SourcePlugin();

	bool Init();
	void Deinit();

	void Start();
	void Stop();

	unsigned int GetWidth() const;
	unsigned int GetHeight() const;

	static void UnloadResources();

private:

	static void VideoDataReady(void* context, const unsigned char* pbData, unsigned int xres, unsigned int yres, unsigned int rowBytes, __int64 timeCode);
	static void AudioDataReady(void* context, const float* pbData, unsigned int channelStrideInBytes, unsigned int channels, unsigned int frames, unsigned int samplesPerSecond, __int64 timeCode);

	void ProcessVideoData(const unsigned char* pbData, unsigned int xres, unsigned int yres, unsigned int rowBytes, __int64 timeCode);
	void ProcessAudioData(const float* pbData, unsigned int channelStrideInBytes, unsigned int channels, unsigned int frames, unsigned int samplesPerSecond, __int64 timeCode);

private:

	obs_source_t * m_pSource;
	const std::string m_groupName;
	const std::string m_hostName;
	const std::string m_skuName;
	Common::AsyncReceiver * m_pReceiver;
	const bool m_bBGRA;
};
