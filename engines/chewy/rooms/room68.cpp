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
#include "chewy/room.h"
#include "chewy/rooms/room68.h"
#include "chewy/sound.h"

namespace Chewy {
namespace Rooms {

static const AniBlock ABLOCK38[2] = {
	{ 5, 1, ANI_FRONT, ANI_WAIT, 0 },
	{ 6, 1, ANI_FRONT, ANI_WAIT, 0 },
};

static const AniBlock ABLOCK39[3] = {
	{ 15, 1, ANI_FRONT, ANI_WAIT, 0 },
	{ 17, 1, ANI_FRONT, ANI_WAIT, 0 },
	{ 15, 1, ANI_BACK, ANI_WAIT, 0 },
};


void Room68::entry() {
	_G(spieler).ScrollxStep = 2;
	_G(spieler).DiaAMov = 1;
	_G(SetUpScreenFunc) = setup_func;
	_G(r68HohesC) = -1;
	_G(spieler_mi)[P_HOWARD].Mode = true;
	_G(spieler_mi)[P_NICHELLE].Mode = true;
	
	if (_G(spieler).R68Papagei) {
		_G(det)->showStaticSpr(12);
		_G(det)->startDetail(21, 255, ANI_FRONT);
	}

	if (!_G(spieler).R68DivaWeg) {
		_G(timer_nr)[0] = _G(room)->set_timer(255, 10);
		_G(det)->set_static_ani(18, -1);
	} else
		_G(det)->hideStaticSpr(3);
	
	if (!_G(flags).LoadGame) {
		hideCur();
		if (_G(spieler)._personRoomNr[P_HOWARD] == 68) {
			setPersonPos(524, 51, P_HOWARD, P_LEFT);
			setPersonPos(550, 54, P_NICHELLE, P_LEFT);
		}
		autoMove(7, P_CHEWY);
		showCur();
	}
}

void Room68::xit() {
	_G(spieler).ScrollxStep = 1;
	if (_G(spieler)._personRoomNr[P_HOWARD] == 68) {
		_G(spieler)._personRoomNr[P_HOWARD] = 66;
		_G(spieler)._personRoomNr[P_NICHELLE] = 66;
	}
}

bool Room68::timer(int16 t_nr, int16 ani_nr) {
	if (t_nr == _G(timer_nr)[0])
		calc_diva();
	else
		return true;

	return false;
}

void Room68::setup_func() {
	switch (_G(r68HohesC)) {
	case 0:
		if (_G(det)->get_ani_status(_G(r68HohesC)) == false) {
			_G(r68HohesC) = 1;
			_G(det)->startDetail(_G(r68HohesC), 1, ANI_FRONT);
		}
		break;

	case 1:
		if (_G(det)->get_ani_status(_G(r68HohesC)) == false) {
			_G(r68HohesC) = 2;
			if (_G(spieler).SpeechSwitch) {
				g_engine->_sound->playSound(2, 0);
				g_engine->_sound->playSound(_G(r68HohesC));
				_G(det)->startDetail(_G(r68HohesC), 255, ANI_FRONT);

				// TODO
				//while (_G(sndPlayer)->getSampleStatus(1) == 4)
				//	setupScreen(DO_SETUP);
				_G(det)->stop_detail(_G(r68HohesC));
			} else {
				_G(det)->startDetail(_G(r68HohesC), 3, ANI_FRONT);
			}
			
			
			_G(det)->startDetail(_G(r68HohesC), 3, ANI_FRONT);
		}
		break;

	case 2:
		if (_G(det)->get_ani_status(_G(r68HohesC)) == false) {
			_G(r68HohesC) = -1;
			_G(det)->stop_detail(18);
			_G(det)->showStaticSpr(3);
			_G(uhr)->resetTimer(_G(timer_nr)[0], 0);
		}
		break;

	default:
		break;
	}
	calc_person_look();
	int16 ho_y = 75;
	int16 ni_y = 75;
	int16 ch_x = _G(spieler_vector)[P_CHEWY].Xypos[0];
	int16 ho_x, ni_x;

	if (ch_x < 130) {
		ho_x = 223;
		ni_x = 260;
	} else {
		ho_x = 320;
		ni_x = 350;
	}
	goAutoXy(ho_x, ho_y, P_HOWARD, ANI_GO);
	goAutoXy(ni_x, ni_y, P_NICHELLE, ANI_GO);
}

void Room68::look_kaktus() {
	hideCur();
	autoMove(6, P_CHEWY);
	startAadWait(383);
	showCur();
}

void Room68::talk_indigo() {
	talk_indigo(-1);
}

void Room68::talk_indigo(int16 aad_nr) {
	hideCur();
	autoMove(3, P_CHEWY);
	_G(room)->set_timer_status(8, TIMER_STOP);
	_G(det)->del_static_ani(8);
	if (aad_nr == -1) {
		_G(det)->set_static_ani(9, -1);
		startAadWait(384 + (int16)_G(spieler).R68IndigoDia);
		_G(spieler).R68IndigoDia ^= 1;
		_G(det)->del_static_ani(9);
	} else {
		_G(det)->set_static_ani(12, -1);
		startAadWait(aad_nr);
		_G(det)->del_static_ani(12);
	}
	_G(room)->set_timer_status(8, TIMER_START);
	_G(det)->set_static_ani(8, -1);
	showCur();
}

int16 Room68::use_indigo() {
	int16 action_flag = false;
	hideCur();
	if (isCurInventory(CLINT_500_INV)) {
		action_flag = true;
		if (_G(spieler).R68Lied) {
			hideCur();
			autoMove(3, P_CHEWY);
			auto_scroll(78, 0);
			delInventory(_G(spieler).AkInvent);
			talk_indigo(394);
			_G(cur_hide_flag) = false;
			hideCur();
			_G(room)->set_timer_status(8, TIMER_STOP);
			_G(det)->del_static_ani(8);
			_G(det)->stop_detail(8);
			startSetAILWait(13, 3, ANI_FRONT);
			startSetAILWait(25, 1, ANI_FRONT);
			_G(det)->set_static_ani(12, -1);
			talk_indigo(398);
			_G(cur_hide_flag) = false;
			hideCur();
			_G(room)->set_timer_status(8, TIMER_STOP);
			_G(det)->del_static_ani(8);
			_G(det)->stop_detail(8);
			startSetAILWait(26, 1, ANI_FRONT);
			_G(room)->set_timer_status(8, TIMER_START);
			_G(det)->set_static_ani(8, -1);
			new_invent_2_cur(KARTE_INV);
			_G(spieler).R68KarteDa = true;
		} else {
			talk_indigo(397);
		}
	} else if (isCurInventory(CLINT_1500_INV) || isCurInventory(CLINT_3000_INV)) {
		action_flag = true;
		startAadWait(393);
	}
	showCur();
	return action_flag;
}

void Room68::talk_keeper() {
	hideCur();
	autoMove(2, P_CHEWY);
	_G(room)->set_timer_status(20, TIMER_STOP);
	_G(det)->del_static_ani(20);
	startSetAILWait(15, 1, ANI_FRONT);
	_G(det)->set_static_ani(16, -1);
	showCur();
	int16 x = _G(spieler_vector)[P_CHEWY].Xypos[0] - _G(spieler).scrollx + _G(spieler_mi)[P_CHEWY].HotX;;
	int16 y = _G(spieler_vector)[P_CHEWY].Xypos[1] - _G(spieler).scrolly;
	_G(atds)->set_split_win(3, x, y);
	startAdsWait(20);
	_G(cur_hide_flag) = false;
	hideCur();
	_G(det)->del_static_ani(16);
	startSetAILWait(15, 1, ANI_BACK);
	_G(room)->set_timer_status(20, TIMER_START);
	_G(det)->set_static_ani(20, -1);
	showCur();
}

int16 Room68::use_papagei() {
	int16 action_flag = false;
	if (isCurInventory(PAPAGEI_INV)) {
		hideCur();
		action_flag = true;
		_G(spieler).R68Papagei = true;
		delInventory(_G(spieler).AkInvent);
		autoMove(5, P_CHEWY);
		start_spz_wait(CH_LGET_O, 1, false, P_CHEWY);
		_G(det)->showStaticSpr(12);
		_G(det)->startDetail(21, 255, ANI_FRONT);
		_G(atds)->delControlBit(408, ATS_ACTIVE_BIT, ATS_DATA);
		_G(atds)->set_ats_str(407, 1, ATS_DATA);
		showCur();
	}
	return action_flag;
}

void Room68::calc_diva() {
	if (!_G(spieler).R68DivaWeg) {
		if (!_G(spieler).R68Papagei) {
			if (_G(r68HohesC) == -1) {
				_G(uhr)->resetTimer(_G(timer_nr)[0], 0);
				_G(r68HohesC) = 0;
				_G(det)->hideStaticSpr(3);
				_G(det)->startDetail(_G(r68HohesC), 1, ANI_BACK);
				_G(det)->startDetail(18, 255, ANI_FRONT);
			}
		} else if (!_G(spieler).R68Gutschein && !is_chewy_busy()) {
			hideCur();
			_G(spieler).R68Gutschein = true;
			autoMove(4, P_CHEWY);
			startAadWait(386);
			start_spz_wait(CH_LGET_O, 1, false, P_CHEWY);
			new_invent_2_cur(BAR_GUT_INV);
			_G(uhr)->resetTimer(_G(timer_nr)[0], 0);
			showCur();
		}
	}
}

int16 Room68::use_keeper() {
	int16 action_flag = false;
	if (isCurInventory(BAR_GUT_INV)) {
		hideCur();
		delInventory(_G(spieler).AkInvent);
		action_flag = true;
		autoMove(2, P_CHEWY);
		start_spz_wait(CH_LGET_O, 1, false, P_CHEWY);
		_G(room)->set_timer_status(20, TIMER_STOP);
		_G(det)->del_static_ani(20);
		startAniBlock(3, ABLOCK39);
		_G(room)->set_timer_status(20, TIMER_START);
		_G(det)->set_static_ani(20, -1);
		new_invent_2_cur(B_MARY_INV);
		showCur();
	}
	return action_flag;
}

int16 Room68::use_diva() {
	int16 action_flag;
	hideCur();
	if (isCurInventory(B_MARY_INV)) {
		delInventory(_G(spieler).AkInvent);
		action_flag = 1;
		autoMove(4, P_CHEWY);
		_G(uhr)->resetTimer(_G(timer_nr)[0], 0);
		_G(det)->hideStaticSpr(3);
		startSetAILWait(4, 1, ANI_FRONT);
		_G(spieler).R68Gutschein = false;
		_G(det)->showStaticSpr(3);
	} else if (isCurInventory(B_MARY2_INV)) {
		delInventory(_G(spieler).AkInvent);
		action_flag = 1;
		autoMove(4, P_CHEWY);
		_G(det)->hideStaticSpr(3);
		_G(spieler).R68DivaWeg = true;
		startAniBlock(2, ABLOCK38);
		flic_cut(FCUT_083);
		_G(det)->del_static_ani(18);
		register_cutscene(22);
		_G(det)->del_static_ani(18);
		startSetAILWait(7, 1, ANI_FRONT);
		_G(atds)->setControlBit(407, ATS_ACTIVE_BIT, ATS_DATA);
		_G(atds)->setControlBit(412, ATS_ACTIVE_BIT, ATS_DATA);
		setPersonSpr(P_RIGHT, P_CHEWY);
		startAadWait(402);
	} else
		action_flag = use_papagei();
	showCur();
	return action_flag;
}

void Room68::kostuem_aad(int16 aad_nr) {
	hideCur();
	if (_G(spieler).DiaAMov != -1) {
		autoMove(_G(spieler).DiaAMov, P_CHEWY);
	}

	startAadWait(aad_nr);

	if (!_G(spieler).R68DivaWeg)
		startAadWait(388);
	else if (!_G(spieler).R67LiedOk)
		startAadWait(389);
	else {
		if (_G(spieler).DisplayText == 0)
			_G(sndPlayer)->fadeOut(5);
		
		_G(SetUpScreenFunc) = nullptr;
		delInventory(_G(spieler).AkInvent);
		goAutoXy(150, -13, P_NICHELLE, ANI_WAIT);
		_G(spieler)._personHide[P_NICHELLE] = true;
		goAutoXy(161, 59, P_HOWARD, ANI_GO);
		autoMove(4, P_CHEWY);
		startAadWait(390);
		startSetAILWait(22, 1, ANI_FRONT);
		_G(spieler)._personHide[P_HOWARD] = true;
		_G(det)->startDetail(27, 255, ANI_FRONT);

		if (_G(spieler).DisplayText)
			startSetAILWait(23, 3, ANI_FRONT);
		else {
			_G(det)->startDetail(23, 255, ANI_FRONT);
			g_engine->_sound->playSound(109, 1, false);
			waitShowScreen(2);

			g_engine->_sound->waitForSpeechToFinish();

			_G(det)->stop_detail(23);
		}

		if (_G(spieler).DisplayText) {
			g_engine->_sound->playSound(108, 1, false);
		}
		
		_G(det)->startDetail(24, 255, ANI_FRONT);
		setPersonPos(26, 40, P_NICHELLE, P_RIGHT);
		if (_G(spieler).DisplayText) {
			startAadWait(391);
		} else {
			waitShowScreen(100);
			startAadWait(602);
			waitShowScreen(100);
		}

		_G(room)->set_timer_status(8, TIMER_STOP);
		_G(det)->del_static_ani(8);
		startSetAILWait(10, 1, ANI_FRONT);
		_G(det)->startDetail(11, 255, ANI_FRONT);
		startAadWait(396);
		_G(det)->stop_detail(11);
		_G(det)->set_static_ani(12, -1);
		startAadWait(611);
		_G(det)->del_static_ani(12);
		_G(det)->startDetail(14, 255, ANI_FRONT);
		startAadWait(395);
		_G(det)->stop_detail(14);
		_G(room)->set_timer_status(8, TIMER_START);
		_G(det)->set_static_ani(8, -1);
		startAadWait(392);

		g_engine->_sound->waitForSpeechToFinish();
		
		_G(spieler)._personHide[P_HOWARD] = false;
		_G(det)->stop_detail(27);
		_G(det)->stop_detail(24);
		_G(det)->showStaticSpr(13);
		_G(spieler).R68Lied = true;
		autoMove(1, P_CHEWY);
		auto_scroll(216, 0);
		_G(det)->hideStaticSpr(13);
		_G(spieler)._personHide[P_NICHELLE] = false;
		setPersonPos(150, -13, P_NICHELLE, P_RIGHT);

		if (_G(spieler).DisplayText) {
			_G(currentSong) = -1;
			load_room_music(_G(spieler)._personRoomNr[0]);
		}
	}
	showCur();
}

void Room68::talk_papagei() {
	hideCur();
	autoMove(5, P_CHEWY);
	showCur();

	_G(atds)->set_split_win(2, 60, 80);
	startAdsWait(18);
}

} // namespace Rooms
} // namespace Chewy
