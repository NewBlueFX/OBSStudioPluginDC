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
OBS_DECLARE_MODULE()

extern obs_source_info create_titler_live_source_info();
extern void unload_module_titler_live_source();

bool obs_module_load()
{
	obs_source_info source_info = create_titler_live_source_info();
	obs_register_source(&source_info);

	return true;
}

void obs_module_unload()
{
	unload_module_titler_live_source();
}