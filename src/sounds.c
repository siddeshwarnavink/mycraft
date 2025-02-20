#include <stdio.h>

#include "sounds.h"

Sound sounds[10] = {};

void loadSounds() {
	char buf[24];
	for (int i = 0; i < 10; ++i) {
		sprintf(buf, "resources/sounds/%d.ogg", i);
		sounds[i] = LoadSound(buf);
	}
	SetSoundVolume(sounds[S_BACKGROUND1], 0.25);
	SetSoundVolume(sounds[S_BREAK1], 0.75);
	SetSoundVolume(sounds[S_BREAK2], 0.75);
	SetSoundVolume(sounds[S_PUNCH], 0.75);
}

void unloadSounds() {
	for (int i = 0; i < 7; ++i) {
		UnloadSound(sounds[i]);
	}
}

void playSound(enum SoundFile file) {
	PlaySound(sounds[file]);
}
