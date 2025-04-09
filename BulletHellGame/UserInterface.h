#ifndef UNSERINTERFACE_H
#define UNSERINTERFACE_H

#include "GeneralFunctions.h"

class BorderBox {
protected:
	Square outerBorder; // outer dimensions of border
	Color color; // color of border
	float thickness; // thickness of border
	Square innerBorder; // inner dimensions of border

public:
	BorderBox(const Vector2* screenScale);
	void Draw();
	Square* ReturnInnerBorder(); 
};

class WaveCounter {
protected:
	int waveCount; // current wave count
	int fontSize; // font size
	Vector2 position; // position of the text
	Color color; // color of the text

	Vector2 drawingPosition; // position of the text to be drawn
public:
	WaveCounter(const Vector2* screenScale);
	void SetDrawingPosition();
	void Draw();
};

class HealthBar {
protected:
	Square healthBar; // health bar dimensions
	float missingHealth; // amount of health missing
	Color color; // color of the health bar
	float healthPercent; // percentage of health remaining
public:
	HealthBar(const Vector2* screenScale);
	void Draw(int* maxHealth, int* health);
};

#endif// UserInterface.h