/*
 * Multiple screens.
 */
#ifndef SCREEN_H
#define SCREEN_H

typedef enum eScreens {
	TITLE = 0,
	GAMEPLAY
} Screens;

/*
 * Get current screen.
 */
Screens getScreen();

/*
 * Switch to specific screen.
 */
void setScreen(Screens s);

#endif // !SCREEN_H
