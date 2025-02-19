/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "common/scummsys.h"

#ifdef __amigaos4__

#include "backends/platform/sdl/amigaos/amigaos.h"
#include "backends/fs/amigaos/amigaos-fs-factory.h"
#include "backends/dialogs/amigaos/amigaos-dialogs.h"

void OSystem_AmigaOS::init() {
	// Initialze File System Factory
	_fsFactory = new AmigaOSFilesystemFactory();

	// Invoke parent implementation of this method
	OSystem_SDL::init();

#if defined(USE_SYSDIALOGS)
	_dialogManager = new AmigaOSDialogManager();
#endif
}

bool OSystem_AmigaOS::hasFeature(Feature f) {
#if defined(USE_SYSDIALOGS)
	if (f == kFeatureSystemBrowserDialog)
		return true;
#endif

	return OSystem_SDL::hasFeature(f);
}
#endif
