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

#ifndef CHEWY_IO_GAME_H
#define CHEWY_IO_GAME_H

#include "chewy/mcga.h"
#include "chewy/cursor.h"
#include "chewy/ngstypes.h"

namespace Chewy {

#define IOG_END 1
#define USER_NAME 36

class IOGame {
private:
	char _fileFind[20][USER_NAME + 4];

private:
	int16 get_savegame_files();

public:
	IOGame() {}

	char *io_init();
	void save_entry(int16 nr);
};

} // namespace Chewy

#endif
