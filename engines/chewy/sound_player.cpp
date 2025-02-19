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

#include "audio/audiostream.h"
#include "audio/decoders/raw.h"
#include "chewy/chewy.h"
#include "chewy/sound_player.h"
#include "chewy/globals.h"
#include "chewy/ngshext.h"
#include "chewy/sound.h"

namespace Chewy {

#define MAX_VOICES 8
#define NORMAL_PLAYBACK 0
#define PATTERN_PLAYBACK 1
#define SEQUENCE_PLAYBACK 2
#define FADE_OUT 1
#define FADE_IN 2

static int16 FreqTable[] = {
	856, 808, 762, 720, 678, 640, 604, 570, 538, 508, 480, 453,
	428, 404, 381, 360, 339, 320, 302, 285, 269, 254, 240, 226,
	214, 202, 190, 180, 170, 160, 151, 143, 135, 127, 120, 113,
	107, 101, 95, 90, 85, 80, 76, 71, 67, 64, 60, 57,

	862, 814, 768, 725, 684, 646, 610, 575, 543, 513, 484, 457,
	431, 407, 384, 363, 342, 323, 305, 288, 272, 256, 242, 228,
	216, 203, 192, 181, 171, 161, 152, 144, 136, 128, 121, 114,
	94, 89, 83, 78, 73, 68, 64, 59, 55, 53, 48, 45,

	868, 820, 774, 730, 689, 651, 614, 580, 547, 516, 487, 460,
	434, 410, 387, 365, 345, 325, 307, 290, 274, 258, 244, 230,
	217, 205, 193, 183, 172, 163, 154, 145, 137, 129, 122, 115,
	94, 89, 83, 78, 73, 68, 64, 59, 55, 53, 48, 45,

	875, 826, 779, 736, 694, 655, 619, 584, 551, 520, 491, 463,
	437, 413, 390, 368, 347, 328, 309, 292, 276, 260, 245, 232,
	219, 206, 195, 184, 174, 164, 155, 146, 138, 130, 123, 116,
	94, 89, 83, 78, 73, 68, 64, 59, 55, 53, 48, 45,

	881, 832, 785, 741, 699, 660, 623, 588, 555, 524, 494, 467,
	441, 416, 392, 370, 350, 330, 312, 294, 278, 262, 247, 233,
	220, 208, 196, 185, 175, 165, 156, 147, 139, 131, 123, 117,
	94, 89, 83, 78, 73, 68, 64, 59, 55, 53, 48, 45,

	887, 838, 791, 746, 704, 665, 628, 592, 559, 528, 498, 470,
	444, 419, 395, 373, 352, 332, 314, 296, 280, 264, 249, 235,
	222, 209, 198, 187, 176, 166, 157, 148, 140, 132, 125, 118,
	94, 89, 83, 78, 73, 68, 64, 59, 55, 53, 48, 45,

	894, 844, 796, 752, 709, 670, 632, 597, 563, 532, 502, 474,
	447, 422, 398, 376, 355, 335, 316, 298, 282, 266, 251, 237,
	223, 211, 199, 188, 177, 167, 158, 149, 141, 133, 125, 118,
	94, 89, 83, 78, 73, 68, 64, 59, 55, 53, 48, 45,

	900, 850, 802, 757, 715, 675, 636, 601, 567, 535, 505, 477,
	450, 425, 401, 379, 357, 337, 318, 300, 284, 268, 253, 238,
	225, 212, 200, 189, 179, 169, 159, 150, 142, 134, 126, 119,
	94, 89, 83, 78, 73, 68, 64, 59, 55, 53, 48, 45,

	907, 856, 808, 762, 720, 678, 640, 604, 570, 538, 508, 480,
	453, 428, 404, 381, 360, 339, 320, 302, 285, 269, 254, 240,
	226, 214, 202, 190, 180, 170, 160, 151, 143, 135, 127, 120,
	94, 89, 83, 78, 73, 68, 64, 59, 55, 53, 48, 45,

	814, 768, 725, 684, 646, 610, 575, 543, 513, 484, 457, 431,
	407, 384, 363, 342, 323, 305, 288, 272, 256, 242, 228, 216,
	204, 192, 181, 171, 161, 152, 144, 136, 128, 121, 114, 108,
	94, 89, 83, 78, 73, 68, 64, 59, 55, 53, 48, 45,

	820, 774, 730, 689, 651, 614, 580, 547, 516, 487, 460, 434,
	410, 387, 365, 345, 325, 307, 290, 274, 258, 244, 230, 217,
	205, 193, 183, 172, 163, 154, 145, 137, 129, 122, 115, 109,
	94, 89, 83, 78, 73, 68, 64, 59, 55, 53, 48, 45,

	826, 779, 736, 694, 655, 619, 584, 551, 520, 491, 463, 437,
	413, 390, 368, 347, 328, 309, 292, 276, 260, 245, 232, 219,
	206, 195, 184, 174, 164, 155, 146, 138, 130, 123, 116, 109,
	94, 89, 83, 78, 73, 68, 64, 59, 55, 53, 48, 45,

	832, 785, 741, 699, 660, 623, 588, 555, 524, 495, 467, 441,
	416, 392, 370, 350, 330, 312, 294, 278, 262, 247, 233, 220,
	208, 196, 185, 175, 165, 156, 147, 139, 131, 124, 117, 110,
	94, 89, 83, 78, 73, 68, 64, 59, 55, 53, 48, 45,

	838, 791, 746, 704, 665, 628, 592, 559, 528, 498, 470, 444,
	419, 395, 373, 352, 332, 314, 296, 280, 264, 249, 235, 222,
	209, 198, 187, 176, 166, 157, 148, 140, 132, 125, 118, 111,
	94, 89, 83, 78, 73, 68, 64, 59, 55, 53, 48, 45,

	844, 796, 752, 709, 670, 632, 597, 563, 532, 502, 474, 447,
	422, 398, 376, 355, 335, 316, 298, 282, 266, 251, 237, 224,
	211, 199, 188, 177, 167, 158, 149, 141, 133, 125, 118, 112,
	94, 89, 83, 78, 73, 68, 64, 59, 55, 53, 48, 45,

	850, 802, 757, 715, 674, 637, 601, 567, 535, 505, 477, 450,
	425, 401, 379, 357, 337, 318, 300, 284, 268, 253, 239, 225,
	213, 201, 189, 179, 169, 159, 150, 142, 134, 126, 119, 113,
	100, 95, 89, 84, 79, 74, 70, 65, 61, 59, 54, 51,
};
static uint16 RealFreq[800] = {0};

void *dig; // FIXME: DIG_DRIVER
int smp[MAX_VOICES]; // FIXME: HSAMPLE
int TimerHandle; // FIXME: HTIMER
int16 TimerEnabled = false;
int16 RealVoices;

int16 LoopEnable = OFF;
int16 PlaybackMode = NORMAL_PLAYBACK;
int16 StartPos = 0;
int16 EndPos = 0;
int16 MusicFade = OFF;
uint16 FadeStart = 0;
uint16 FadeCounter = 0;
uint16 FadeVol = 0;
uint16 GlobalFrequency = 22050;

int16 MusicStatus = OFF;
int16 InInterrupt = 0;
int16 PatternCount = 6;
int16 CurrentTempo = 6;
int16 FineSpeedV;
int16 FineSpeedN;
int16 FineSpeedS;
TmfInst Instrument[4] = {{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }};
uint16 InsFreq[4] = {0};
char *Sample[4] = {0};

TmfHeader *ActiveSong;
int16 SeqPointer;
int16 PatPointer;
int16 PatLine;
char *Patterns[128];
char *CurrentLine;
int16 StereoPos[8] = {63};

SoundPlayer::SoundPlayer() {
	MusicFade = false;
}

SoundPlayer::~SoundPlayer() {
	warning("STUB: ailclass::~ailclass()");
}

void SoundPlayer::initMixMode() {
	warning("STUB: ailclass::initMixMode()");

	initNoteTable(GlobalFrequency);
#if 0
	TimerHandle = AIL_register_timer(mod_irq);
	if (TimerHandle != -1) {
		AIL_set_timer_frequency(TimerHandle, 50);
		AIL_start_timer(TimerHandle);
		TimerEnabled = true;
	} else
#endif

	TimerEnabled = false;
	StereoPos[0] = 7;
	StereoPos[1] = 120;
	StereoPos[2] = 30;
	StereoPos[3] = 90;
}

void SoundPlayer::playMod(TmfHeader *th) {
	char *tmp;
	int16 i;
	ActiveSong = th;
	tmp = (char *)ActiveSong;
	tmp += sizeof(TmfHeader);
	for (i = 0; i < 128; i++) {
		Patterns[i] = tmp;
		tmp += 1024;
	}
	SeqPointer = 0;
	PatLine = 0;
	PatPointer = ActiveSong->_sequence[SeqPointer];
	CurrentLine = Patterns[PatPointer];
	CurrentTempo = 6;
	PatternCount = 0;
	FineSpeedV = 1;
	FineSpeedN = 0;
	FineSpeedS = 0;

	MusicStatus = ON;
}

void SoundPlayer::stopMod() {
	warning("STUB: ailclass::stopMod()");

#if 0
	int16 i;
	if (SoundEnable) {
		for (i = 0; i < 4; i++) {
			AIL_end_sample(smp[i]);
			Instrument[i].replen = 0;
		}
		MusicStatus = OFF;
	}
#endif
}

void SoundPlayer::continueMusic() {
	if (!strncmp(ActiveSong->id, "TMF", 3))
		MusicStatus = ON;
}

void SoundPlayer::playSequence(int16 startPos, int16 endPos) {
	if (!strncmp(ActiveSong->id, "TMF", 3)) {
		StartPos = startPos;
		EndPos = endPos;
		SeqPointer = StartPos;
		PatLine = 0;
		PatPointer = ActiveSong->_sequence[SeqPointer];
		CurrentLine = Patterns[PatPointer];
		MusicStatus = ON;
	}
}

void SoundPlayer::playPattern(int16 pattern) {
	if (!strncmp(ActiveSong->id, "TMF", 3)) {
		StartPos = 0;
		EndPos = 0;
		SeqPointer = 0;
		PatLine = 0;
		PatPointer = pattern;
		CurrentLine = Patterns[PatPointer];
		MusicStatus = ON;
	}
}

void SoundPlayer::setLoopMode(int16 mode) {
	mode &= 1;
	LoopEnable = mode;
}

void SoundPlayer::fadeOut(uint16 delay) {
	FadeStart = delay;
	FadeCounter = 0;
	MusicFade = FADE_OUT;
	FadeVol = _G(spieler).MusicVol;
}

void SoundPlayer::getMusicInfo(musik_info *mi) {
	mi->musik_playing = MusicStatus;
	mi->play_mode = 0;
	mi->pattern_line = PatLine;
	mi->sequence_pos = SeqPointer;
	mi->cur_pattnr = PatPointer;
}

int16 SoundPlayer::musicPlaying() {
	return 0;	// Temporary hack until the music code is finished
	//return MusicStatus;
}

void SoundPlayer::initNoteTable(uint16 sfreq) {
	float TimerFreq;
	TimerFreq = 7093789.2f;
	for (int j = 0; j < 16; j++) {
		for (int i = 0; i < 48; i++)
			RealFreq[(j * 48) + i] = (uint16)(TimerFreq / (float)(FreqTable[(j * 48) + i] << 1));

	}
}

void mod_irq() {
	if (!InInterrupt) {
		++InInterrupt;
		if (MusicStatus == ON) {
			if (PatternCount <= 0) {
				PatternCount = CurrentTempo;
				DecodePatternLine();
			}
			if (MusicFade == FADE_IN) {
				if (!FadeCounter) {
					FadeCounter = FadeStart;
					if ((_G(spieler).MusicVol / 2 + 2) <= FadeVol)
						_G(spieler).MusicVol += 2;
					else {
						MusicFade = OFF;
						_G(spieler).MusicVol = FadeVol;
					}
				} else
					--FadeCounter;
			} else if (MusicFade == FADE_OUT) {
				if (!FadeCounter) {
					FadeCounter = FadeStart;
					if (_G(spieler).MusicVol - 2 > 0)
						_G(spieler).MusicVol -= 2;
					else {
						MusicFade = OFF;
						MusicStatus = OFF;
						_G(spieler).MusicVol = FadeVol;
					}
				} else
					--FadeCounter;
			}
		}
		PatternCount -= FineSpeedV;
		FineSpeedS += FineSpeedN;
		while (FineSpeedS > 1000) {
			FineSpeedS -= 1000;
			--PatternCount;
		}
	}
	--InInterrupt;
}

void DecodePatternLine() {
	DecodeChannel(0);
	DecodeChannel(1);
	DecodeChannel(2);
	DecodeChannel(3);
	++PatLine;
	if (PatLine > 63) {
		PatLine = 0;
		++SeqPointer;
		switch (PlaybackMode) {
		case NORMAL_PLAYBACK:
			if (SeqPointer < ActiveSong->lied_len) {
				PatPointer = ActiveSong->_sequence[SeqPointer];
				CurrentLine = Patterns[PatPointer];
			} else {
				if (LoopEnable == OFF)
					MusicStatus = OFF;
				else {
					PatLine = 0;
					SeqPointer = 0;
					PatPointer = ActiveSong->_sequence[SeqPointer];
					CurrentLine = Patterns[PatPointer];
				}
			}
			break;

		case SEQUENCE_PLAYBACK:
			if (SeqPointer != EndPos) {
				PatPointer = ActiveSong->_sequence[SeqPointer];
				CurrentLine = Patterns[PatPointer];
			} else {
				if (LoopEnable == OFF)
					MusicStatus = OFF;
				else {
					PatLine = 0;
					SeqPointer = StartPos;
					PatPointer = ActiveSong->_sequence[SeqPointer];
					CurrentLine = Patterns[PatPointer];
				}
			}
			break;

		case PATTERN_PLAYBACK:
			if (LoopEnable == OFF)
				MusicStatus = OFF;
			else {
				PatLine = 0;
				SeqPointer = 0;
				PatPointer = StartPos;
				CurrentLine = Patterns[PatPointer];
			}
			break;
		}
	}
}

void DecodeChannel(int16 ch) {
	warning("STUB: ailclass::decode_channel()");

#if 0
	byte note, effect, inst, effpar;
	tmf_inst *ti;
	int16 tmp1;
	dword tmp2;
	ch &= 3;
	ti = &Instrument[ch];
	note = *CurrentLine;
	++CurrentLine;
	inst = *CurrentLine;
	++CurrentLine;
	effect = *CurrentLine;
	++CurrentLine;
	effpar = *CurrentLine;
	++CurrentLine;
	if (note <= 48) {
		switch (effect) {
		case 0x9:
			break;

		case 0xa:
			if (effpar & 0xf0) {
				effpar >>= 4;
				ti->insvol += effpar;
				if ((byte)ti->insvol > (byte)MusicMasterVol)
					ti->insvol = (byte)MusicMasterVol;
			} else {
				effpar &= 0x0f;
				if (ti->insvol > effpar)
					ti->insvol -= effpar;
				else
					ti->insvol = 0;
			}
			AIL_set_sample_volume(smp[ch], ti->insvol);
			break;

		case 0xb:
			PatLine = 63;
			SeqPointer = effpar;
			break;

		case 0xc:
			ti->insvol = effpar;
			if (ti->insvol > (byte)MusicMasterVol)
				ti->insvol = (byte)MusicMasterVol;
			AIL_set_sample_volume(smp[ch], ti->insvol);
			break;

		case 0xd:
			PatLine = 63;
			break;

		case 0xe:
			break;

		case 0xf:
			if (effpar >= 0x20) {
				FineSpeedV = effpar / 125;
				FineSpeedN = ((effpar % 125) * 1000) / 125;
			} else {
				CurrentTempo = (int16) effpar;
				FineSpeedV = 1;
				FineSpeedN = 0;
				FineSpeedS = 0;
			}
			break;
		}
		if (note < 48) {
			InsFreq[ch] = RealFreq[note];
			if (inst > 0) {
				--inst;
				memcpy(ti, &ActiveSong->instrument[inst], sizeof(tmf_inst));
				Sample[ch] = ActiveSong->ipos[inst];
			}

			if (ti->insvol > (byte)MusicMasterVol)
				ti->insvol = (byte)MusicMasterVol;
			AIL_end_sample(smp[ch]);
			while (AIL_sample_status(smp[ch]) == SMP_PLAYING);
			AIL_init_sample(smp[ch]);
			AIL_set_sample_type(smp[ch], DIG_F_MONO_8, 0);
			AIL_set_sample_address(smp[ch], Sample[ch], ti->laenge);
			AIL_set_sample_playback_rate(smp[ch], InsFreq[ch]);
			AIL_set_sample_volume(smp[ch], ti->insvol);
			AIL_set_sample_pan(smp[ch], StereoPos[ch]);
			AIL_start_sample(smp[ch]);
		}
	}
#endif
}

} // namespace Chewy
