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

#include <obs.h>
#include "SourcePlugin.h"
#include "AsyncReceiver.h""
#include <assert.h>


SourcePlugin::SourcePlugin(const bool bUseBGRA, const std::string& groupName, const std::string& hostName,
	const std::string& skuName, obs_data_t* /*settings*/, obs_source_t* source)
	: m_groupName(groupName)
	, m_pReceiver(nullptr)
	, m_pSource(source)
	, m_bBGRA(bUseBGRA)
	, m_hostName(hostName)
	, m_skuName(skuName)
{
}

SourcePlugin::~SourcePlugin()
{
	Deinit();
}

bool SourcePlugin::Init()
{
	Deinit();

	bool result = false;
	obs_video_info ovi = {};
	obs_audio_info oai = {};

	// Gets the current video settings, returns false if no video
	const bool videoSettingsValid = obs_get_video_info(&ovi);
	// Gets the current audio settings, returns false if no audio
	const bool audioSettingsValid = obs_get_audio_info(&oai);

	if (videoSettingsValid || audioSettingsValid)
	{
		m_pReceiver = new Common::AsyncReceiver(m_groupName, m_hostName, m_skuName);

		// Take video settings
		int width = 0;
		int heigth = 0;
		double frameRate = 0;

		if (videoSettingsValid && ovi.base_width > 0 && ovi.base_height > 0 && ovi.fps_num > 0 && ovi.fps_den > 0)
		{
			width = ovi.base_width;
			heigth = ovi.base_height;
			frameRate = ovi.fps_num / static_cast<double>(ovi.fps_den);
		}

		// Take audio settings
		int sampleRate = 0;
		int numChannels = 0;

		if (audioSettingsValid && oai.samples_per_sec > 0 && oai.speakers > SPEAKERS_UNKNOWN)
		{
			sampleRate = oai.samples_per_sec;
			numChannels = (oai.speakers == SPEAKERS_MONO) ? 1 : 2;
		}

		result = m_pReceiver->Init(this, SourcePlugin::VideoDataReady, SourcePlugin::AudioDataReady, m_bBGRA,
			width, heigth, frameRate, sampleRate, numChannels);
		assert(result);
		if (!result)
		{
			Deinit();
		}
	}

	return result;
}

void SourcePlugin::Deinit()
{
	if(m_pReceiver)
	{
		m_pReceiver->Deinit();
		delete m_pReceiver;
		m_pReceiver = nullptr;
	}
}

void SourcePlugin::Start()
{
	if(m_pReceiver)
	{
		m_pReceiver->Start();
	}
}

void SourcePlugin::Stop()
{
	if(m_pReceiver)
	{
		m_pReceiver->Stop();
	}
}

unsigned int SourcePlugin::GetWidth() const
{
	assert(m_pReceiver);
	if(m_pReceiver)
	{
		return m_pReceiver->GetActualWidth();
	}

	return 1920;
}

unsigned int SourcePlugin::GetHeight() const
{
	assert(m_pReceiver);
	if(m_pReceiver)
	{
		return m_pReceiver->GetActualHeight();
	}

	return 1080;
}

void SourcePlugin::UnloadResources()
{
	Common::AsyncReceiver::UnloadResources();
}

void SourcePlugin::VideoDataReady(void* context, const unsigned char* pbData, unsigned int xres, unsigned int yres, unsigned int rowBytes, __int64 timeCode)
{
	assert(context);
	if(!context)
	{
		return;
	}

	SourcePlugin * pSource = static_cast<SourcePlugin *>(context);
	pSource->ProcessVideoData(pbData, xres, yres, rowBytes, timeCode);
}

void SourcePlugin::AudioDataReady(void* context, const float* pbData, unsigned int channelStrideInBytes, unsigned int channels, unsigned int frames, unsigned int samplesPerSecond, __int64 timeCode)
{
	assert(context);
	if (!context)
	{
		return;
	}

	SourcePlugin * pSource = static_cast<SourcePlugin *>(context);

	pSource->ProcessAudioData(pbData, channelStrideInBytes, channels, frames, samplesPerSecond, timeCode);
}

void SourcePlugin::ProcessVideoData(const unsigned char * pbData, unsigned int xres, unsigned int yres, unsigned int rowBytes, __int64 timeCode)
{
	assert(pbData && xres && yres && rowBytes);
	if(!pbData || !xres || !yres || !rowBytes)
	{
		return;
	}

	obs_source_frame obs_frame = {0};

	obs_frame.timestamp = timeCode;
	obs_frame.width = xres;
	obs_frame.height = yres;
	obs_frame.format = m_bBGRA ? VIDEO_FORMAT_BGRA : VIDEO_FORMAT_UYVY;
	obs_frame.data[0] = const_cast<uint8_t *>(pbData);
	obs_frame.linesize[0] = rowBytes;
	obs_frame.full_range = true;

	obs_source_output_video(m_pSource, &obs_frame);
}

void SourcePlugin::ProcessAudioData(const float* pbData, unsigned int channelStrideInBytes, unsigned int channels, unsigned int frames, unsigned int samplesPerSecond, __int64 timeCode)
{
	assert(pbData && channelStrideInBytes && channels && frames && samplesPerSecond);
	if (!pbData || !channelStrideInBytes || !channels || !frames || !samplesPerSecond)
	{
		return;
	}

	obs_source_audio obs_audio = { 0 };

	obs_audio.speakers = static_cast<speaker_layout>(channels);
	obs_audio.frames = frames;
	obs_audio.samples_per_sec = samplesPerSecond;
	obs_audio.format = AUDIO_FORMAT_FLOAT_PLANAR;
	obs_audio.timestamp = timeCode;

	for (unsigned int i = 0; i < channels; ++i)
	{
		obs_audio.data[i] = reinterpret_cast<const unsigned char*>(pbData) + i * channelStrideInBytes;
	}

	obs_source_output_audio(m_pSource, &obs_audio);
}
