// BulletHellGame.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Player.h"
#include "UserInterface.h"
#include "Waves.h"

int main()
{
	// Initialization
	// window initialization
	// Set the screen size and initialize the window
    const Vector2 screenScale = { 1280, 720 };
	InitWindow(screenScale.x, screenScale.y, "Bullet Hell Game");
	SetTargetFPS(60);
	//game initialization
	Player* player = new Player(&screenScale);
	BorderBox* borderBox = new BorderBox(&screenScale);
	WaveCounter* waveCounter = new WaveCounter(&screenScale);
	HealthBar* healthBar = new HealthBar(&screenScale);


	WaveManager* waveManager = new WaveManager(&screenScale); // Initialize the wave manager
	// Initialize the wave counter and health bar

	float deltatime = 0.0f;

	// end Initialization

	// Main game loop
	while (!WindowShouldClose())
	{
		//updates
		deltatime = GetFrameTime();

		player->Movement(&deltatime, borderBox->ReturnInnerBorder()); //updates the player position
		waveManager->Update(&deltatime, player->ReturnHitbox(), borderBox->ReturnInnerBorder()); //updates the wave manager

		BeginDrawing();

		ClearBackground(BLACK);
		// DrawBackingGridCheckers(); //draws the backing grid for testing
		borderBox->Draw(); //draws the border box
		player->Draw(); //draws the player
		waveCounter->Draw(); //draws the wave counter
		healthBar->Draw(player->ReturnMaxHealth(), player->ReturnHealth()); //draws the health bar
		waveManager->Draw(); //draws the wave manager

		EndDrawing();
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
