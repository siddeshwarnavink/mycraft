/*
 * Manages all in-game sound fx and music.
 */
#ifndef SOUNDS_H
#define SOUNDS_H

#include "raylib.h"

enum SoundFile {
	S_BACKGROUND1,
	S_JUMP1,
	S_JUMP2,
	S_HURT1,
	S_HURT2,
	S_HURT3,
	S_HURT4,
};
extern Sound sounds[7];

/*
 * Loads all the sound into memory.
 */
void loadSounds();

/*
 * Unloads all the sound from memory.
 */
void unloadSounds();

/*
 * Play a specific sound.
 */
void playSound(enum SoundFile file);

#endif // !SOUNDS_H
