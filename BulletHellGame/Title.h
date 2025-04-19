#ifndef TITLE_H
#define TITLE_H

#include "GeneralFunctions.h"

class MenuButton {
protected:
	string text;
	Square hitbox;
	GameState changeState;
	bool isHover; // Flag to check if the button is hovered
public:
	MenuButton(string buttonText, Vector2 centre, float height, GameState state); // Constructor for the button
	void Draw(); // Draw the button
	void IsPressed(Vector2* mousePosition, GameState* gameState); // Check if the mouse is over the button
};


class Title {
protected:
	Texture2D titleTexture; // Texture for the title screen
	float scale; // Scale of the title screen
	Square titleSquare; // Square for the title screen

	vector<MenuButton> buttons; // Vector of buttons for the title screen
public:
	Title(const Vector2* screenScale); // Constructor for the title screen
	void Draw(); // Draw the title screen
	void Update(GameState* gameState); // Update the title screen
};



#endif