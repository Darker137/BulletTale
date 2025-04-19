#ifndef GENERALFUNCTIONS_H 
#define GENERALFUNCTIONS_H

#include <iostream>
#include <vector>
#include "raylib.h"

using namespace std;

struct Square {
	Vector2 position;
	Vector2 size;
};

enum GameState {
	MAINMENU,
	PLAYING,
	GAMEOVER,
	EXIT,
};

void DrawBackingGridCheckers(); //draws the backing grid for testing	

float Vector2Length(Vector2 vector);

Vector2 Vector2Normalize(Vector2 vector);

#endif // GENERALFUNCTIONS_H