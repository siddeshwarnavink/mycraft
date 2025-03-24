#include "screen.h"

static Screens currentScreen = TITLE;

Screens getScreen() {
	return currentScreen;
}

void setScreen(Screens s) {
	currentScreen = s;
}
