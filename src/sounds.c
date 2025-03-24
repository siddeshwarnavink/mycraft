#include <stdio.h>

#include "sounds.h"

Sound sounds[10] = {};

void initSounds() {
    InitAudioDevice();

	char buf[24];
	for (int i = 0; i < 11; ++i) {
		sprintf(buf, "resources/sounds/%d.ogg", i);
		sounds[i] = LoadSound(buf);
	}
	SetSoundVolume(sounds[S_BACKGROUND1], 0.25);
	SetSoundVolume(sounds[S_BREAK1], 0.75);
	SetSoundVolume(sounds[S_BREAK2], 0.75);
	SetSoundVolume(sounds[S_PUNCH], 0.75);
}

void freeSounds() {
	for (int i = 0; i < 7; ++i) {
		UnloadSound(sounds[i]);
	}
    CloseAudioDevice();
}

void playSound(enum SoundFile file) {
	PlaySound(sounds[file]);
}
