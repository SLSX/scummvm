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

#ifndef CHEWY_ROOMS_ROOM28_H
#define CHEWY_ROOMS_ROOM28_H

#include "chewy/room.h"

namespace Chewy {
namespace Rooms {

class Room28 {
private:
	static void haendler();
	static void setup_func();
	static void get_pump();

public:
	static void entry(int16 eib_nr);
	static void xit(int16 eib_nr);
	static void gedAction(int index);

	static void use_surimy();
	static void set_pump();
	static int16 use_breifkasten();
};

} // namespace Rooms
} // namespace Chewy

#endif
