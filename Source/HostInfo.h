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

#ifdef TITLER_LIVE_OVRSTREAM
#define NEWBLUE_SKU_NAME	"OvrStream"
#define GROUP_NAME		     "NewBlue " NEWBLUE_SKU_NAME
#else
#define NEWBLUE_SKU_NAME	"TitlerLive"
#define GROUP_NAME		     ""
#endif

#define HOST_NAME			"OBSStudio"