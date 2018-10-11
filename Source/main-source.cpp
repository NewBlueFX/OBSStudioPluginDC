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


#include <obs-module.h>
#include "SourcePlugin.h"
#include "HostInfo.h"

// At current time OBS Studio plug-in is used for OvrStream and TitlerLive products
const char * g_szSkuPath = NEWBLUE_SKU_NAME;

#ifdef TITLER_LIVE_OVRSTREAM
#	define NEWBLUE_SOURCE_ID_PREFIX	"ovrstream"
#else
#	define NEWBLUE_SOURCE_ID_PREFIX	"titlerlive"
#endif

#	define NEWBLUE_PLUGIN_NAME	NEWBLUE_SKU_NAME
#	define NEWBLUE_SOURCE_ID	NEWBLUE_SOURCE_ID_PREFIX"_dc_source"

namespace
{

const char *source_get_name(void *)
{
	return NEWBLUE_PLUGIN_NAME;
}

void *source_create(obs_data_t *settings, obs_source_t *source)
{
	SourcePlugin * pSource = new SourcePlugin(true, GROUP_NAME, HOST_NAME, NEWBLUE_SKU_NAME, settings, source);

	if(!pSource->Init())
	{
		delete pSource;
		pSource = nullptr;
	}

	assert(pSource);
	return pSource;
}

void source_destroy(void *data)
{
	SourcePlugin * pSource = static_cast<SourcePlugin *>(data);
	assert(pSource);
	if(pSource)
		delete pSource;
}

uint32_t source_get_width(void * data)
{
	uint32_t result = 0;
	SourcePlugin * pSource = static_cast<SourcePlugin *>(data);
	assert(pSource);

	if(pSource)
		result = pSource->GetWidth();

    return result;
}

uint32_t source_get_height(void *data)
{
	uint32_t result = 0;
	SourcePlugin * pSource = static_cast<SourcePlugin *>(data);
	assert(pSource);

	if (pSource)
		result = pSource->GetHeight();

    return result;
}

void source_activate(void *data)
{
	SourcePlugin * pSource = static_cast<SourcePlugin *>(data);
	assert(pSource);

	if(pSource)
	{
		pSource->Start();
	}
}

void source_deactivate(void *data)
{
	SourcePlugin * pSource = static_cast<SourcePlugin *>(data);
	assert(pSource);

	if(pSource)
	{
		pSource->Stop();
	}
}

}

obs_source_info create_titler_live_source_info() 
{
	struct obs_source_info source_info = { 0 };

	source_info.id = NEWBLUE_SOURCE_ID;
	source_info.type = OBS_SOURCE_TYPE_INPUT;
	source_info.output_flags = OBS_SOURCE_ASYNC_VIDEO | OBS_SOURCE_AUDIO | OBS_SOURCE_DO_NOT_DUPLICATE;

	source_info.get_name = source_get_name;
	source_info.create = source_create;
	source_info.destroy = source_destroy;
	source_info.get_width = source_get_width;
	source_info.get_height = source_get_height;
	source_info.activate = source_activate;
	source_info.deactivate = source_deactivate;

	return source_info;
}

void unload_module_titler_live_source()
{
	SourcePlugin::UnloadResources();
}
