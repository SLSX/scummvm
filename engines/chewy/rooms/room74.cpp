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

#include "chewy/defines.h"
#include "chewy/events.h"
#include "chewy/globals.h"
#include "chewy/rooms/room74.h"
#include "chewy/rooms/room75.h"
#include "chewy/sound.h"

namespace Chewy {
namespace Rooms {

void Room74::entry(int16 eib_nr) {
	g_engine->_sound->playSound(0, 0);
	g_engine->_sound->playSound(0);
	_G(spieler).ScrollxStep = 2;
	_G(spieler).ZoomXy[P_HOWARD][0] = 70;
	_G(spieler).ZoomXy[P_HOWARD][1] = 100;
	_G(spieler).ZoomXy[P_NICHELLE][0] = 70;
	_G(spieler).ZoomXy[P_NICHELLE][1] = 100;
	_G(spieler_mi)[P_HOWARD].Mode = true;
	_G(spieler_mi)[P_NICHELLE].Mode = true;
	_G(zoom_horizont) = 110;
	_G(flags).ZoomMov = true;
	_G(zoom_mov_fak) = 3;
	_G(SetUpScreenFunc) = setup_func;

	if (_G(spieler).flags29_1)
		_G(det)->startDetail(0, 255, false);

	if (_G(flags).LoadGame)
		return;

	switch (eib_nr) {
	case 105:
		leftEntry();
		break;
	case 108:
		_G(spieler).scrollx = 188;
		Room75::proc1();
		break;
	case 109:
		rightEntry();
		break;
	default:
		break;
	}
}

void Room74::xit(int16 eib_nr) {
	_G(spieler).ScrollxStep = 1;

	if (_G(spieler)._personRoomNr[P_HOWARD] != 74)
		return;

	switch (eib_nr) {
	case 112:
	case 113:
		_G(spieler)._personRoomNr[P_HOWARD] = _G(spieler)._personRoomNr[P_NICHELLE] = 72;
		break;

	case 114:
		_G(spieler)._personRoomNr[P_HOWARD] = _G(spieler)._personRoomNr[P_NICHELLE] = 71;
		break;

	default:
		break;
	}
}

int Room74::proc1() {
	int retVal = 0;
	hideCur();

	if (isCurInventory(MACHETE_INV)) {
		retVal = 1;
		_G(spieler).R74CutRubberPlant = true;
		autoMove(4, P_CHEWY);
		_G(spieler)._personHide[P_CHEWY] = true;
		startSetAILWait(1, 1, ANI_FRONT);
		setPersonPos(272, 116, P_CHEWY, P_RIGHT);
		_G(spieler)._personHide[P_CHEWY] = false;
		_G(det)->startDetail(0, 255, false);
		_G(spieler).flags29_1 = true;
		_G(atds)->set_ats_str(435, 1, ATS_DATA);

	} else if (!_G(spieler).inv_cur && _G(spieler).R74CutRubberPlant) {
		_G(atds)->setControlBit(435, ATS_ACTIVE_BIT, ATS_DATA);
		autoMove(5, P_CHEWY);
		start_spz_wait(13, 1, false, P_CHEWY);
		new_invent_2_cur(RUBBER_INV);
	}

	showCur();
	return retVal;
}

} // namespace Rooms
} // namespace Chewy
