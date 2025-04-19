#include "Title.h"

MenuButton::MenuButton(string buttonText, Vector2 centre, float height, GameState state)
{
	// Set the button text and hitbox dimensions
	text = buttonText; // Set the button text
	hitbox.size = { float(MeasureText(text.c_str(), height)), height }; // Set the button size based on the text and height
	hitbox.position = { centre.x - (hitbox.size.x / 2), centre.y - (hitbox.size.y / 2) }; // Center the button text
	changeState = state; // Set the state to change to when the button is pressed
	isHover = false;
}

void MenuButton::Draw()
{
	// Draw the button text at the calculated position
	if (isHover) // Check if the button is hovered
	{
		DrawText(text.c_str(), hitbox.position.x, hitbox.position.y, hitbox.size.y, WHITE);
	}
	else
	{
		DrawText(text.c_str(), hitbox.position.x, hitbox.position.y, hitbox.size.y, BLACK);
	}
}

void MenuButton::IsPressed(Vector2* mousePosition, GameState* gameState)
{
	if (mousePosition->x > hitbox.position.x && mousePosition->x < hitbox.position.x + hitbox.size.x &&
		mousePosition->y > hitbox.position.y && mousePosition->y < hitbox.position.y + hitbox.size.y)
	{
		isHover = true; // Set the button as hovered
		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) // Check if the left mouse button is pressed
		{
			*gameState = changeState; // Change the game state if the button is pressed
		}
	}
	else {
		isHover = false; // Reset the hover state
	}
}

Title::Title(const Vector2* screenScale)
{
	// Load the title texture
	titleTexture = LoadTexture("Resources/TitleIcon.png");
	// Set the initial position and scale
	scale = 4.0f;
	titleSquare.size = { (float)titleTexture.width * scale, (float)titleTexture.height * scale };
	titleSquare.position = { screenScale->x / 2 - (titleSquare.size.x / 2), screenScale->y / 2 - (titleSquare.size.y / 2) };

	int buttonHeight = 100;
	Vector2 Center = { screenScale->x / 2, screenScale->y / 2 };
	// Create the button and add it to the vector
	buttons.push_back(MenuButton("Play", { Center.x, Center.y - 60}, buttonHeight, PLAYING)); // Create the "Play" button
	buttons.push_back(MenuButton("Exit", { Center.x, Center.y + 60}, buttonHeight, EXIT)); // Create the "Exit" button
}

void Title::Draw()
{
	// Draw the title texture at the calculated position
	DrawTextureEx(titleTexture, titleSquare.position, 0.0f, scale, WHITE);

	for (MenuButton button : buttons) // Loop through each button in the vector
	{
		button.Draw(); // Draw the button
	}
}

void Title::Update(GameState* gameState)
{
	Vector2 mousePosition = GetMousePosition(); // Get the current mouse position
	for (MenuButton& button : buttons) // Loop through each button in the vector
	{
		button.IsPressed(&mousePosition, gameState); // Check if the button is pressed
	}
}