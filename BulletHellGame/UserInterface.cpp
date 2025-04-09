#include "UserInterface.h"

BorderBox::BorderBox(const Vector2* screenScale )
{
	// Set the default position, size, and color of the border box
	outerBorder.position = { 32 , 64 };
	outerBorder.size = { screenScale->x - 64, screenScale->y - 128 };
	color = WHITE;
	thickness = 8.0f;
	innerBorder.position = { outerBorder.position.x + thickness, outerBorder.position.y + thickness};
	innerBorder.size = { outerBorder.size.x - (thickness * 2), outerBorder.size.y - (thickness * 2) };
}

void BorderBox::Draw()
{
	// Draw the border box using the specified position, size, color, and thickness
	DrawRectangleLinesEx({ outerBorder.position.x, outerBorder.position.y, outerBorder.size.x, outerBorder.size.y }, thickness, color);
}

Square* BorderBox::ReturnInnerBorder() {
	//returns the inner border square
	return &innerBorder;
}

WaveCounter::WaveCounter(const Vector2* screenScale)
{
	// Set the initial wave count, font size, position, and color
	waveCount = 1;
	fontSize = 48;
	position = { screenScale->x / 2, 32 };
	color = WHITE;

	drawingPosition = {}; SetDrawingPosition(); // initialize the drawing position
}

void WaveCounter::SetDrawingPosition()
{
	// Set the drawing position based on the wave count and font size
	drawingPosition.x = position.x - MeasureText(TextFormat("%i", waveCount), fontSize) / 2;
	drawingPosition.y = position.y - (fontSize / 2);
}

void WaveCounter::Draw()
{
	// Draw the wave count text at the calculated position
	DrawText(TextFormat("%i", waveCount), drawingPosition.x, drawingPosition.y, fontSize, color);
}

HealthBar::HealthBar(const Vector2* screenScale)
{
	// Set the initial health bar dimensions and missing health
	healthBar.position = { 32 , screenScale->y - 48 };
	healthBar.size = { screenScale->x - 64, 32 };
	missingHealth = 0.0f;
	color = GREEN; // Set the color of the health bar
	healthPercent; // Initialize the health percentage
}

void HealthBar::Draw(int* maxHealth, int* health)
{
	// Calculate the missing health based on the current and maximum health
	missingHealth = ((float)(*maxHealth - *health) / (float)*maxHealth) * healthBar.size.x;
	healthPercent = ((float)*health / (float)*maxHealth) * 100.0f;
	// Draw the health bar using the specified position and size

	if (healthPercent > 50) {
		color = GREEN; // Set color to green if health is above 50%
	}
	else if (healthPercent > 25) {
		color = YELLOW; // Set color to yellow if health is between 25% and 50%
	}
	else {
		color = RED; // Set color to red if health is below 25%
	}

	DrawRectangle(healthBar.position.x , healthBar.position.y, healthBar.size.x - missingHealth, healthBar.size.y, color);
	DrawRectangleLinesEx({ healthBar.position.x, healthBar.position.y, healthBar.size.x, healthBar.size.y }, 2.0f, WHITE);
}