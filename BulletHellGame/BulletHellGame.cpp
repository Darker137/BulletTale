// BulletHellGame.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Player.h"
#include "UserInterface.h"
#include "Waves.h"
#include "Title.h"

int main()
{
	// Initialization
	// window initialization
	// Set the screen size and initialize the window
    const Vector2 screenScale = { 1280, 720 };
	InitWindow(screenScale.x, screenScale.y, "Bullet Hell Game");
	SetTargetFPS(60);
	//game initialization
	GameState gameState = PLAYING; // Set the initial game state

	float deltatime = 0.0f;
	int waveReached = 0; // Initialize the wave reached variable

	// end Initialization

	// Main game loop
	while (!WindowShouldClose() && gameState != EXIT)
	{
		switch (gameState) // Check the game state

		{
		case MAINMENU:
		{
			Title* title = new Title(&screenScale); // Create a new title object
			while (!WindowShouldClose() && gameState == MAINMENU) // Loop until the game is closed or the state changes
			{
				title->Update(&gameState); // Update the title screen

				BeginDrawing();

				ClearBackground(BLACK); // Clear the background
				title->Draw(); // Draw the title screen

				EndDrawing();
			}
			break;
		}
		case PLAYING: // If the game is in the playing state
		{
			Player* player = new Player(&screenScale);
			BorderBox* borderBox = new BorderBox(&screenScale);
			WaveCounter* waveCounter = new WaveCounter(&screenScale);
			HealthBar* healthBar = new HealthBar(&screenScale);
			WaveManager* waveManager = new WaveManager(borderBox->ReturnInnerBorder()); // Initialize the wave manager

			while (!WindowShouldClose() && gameState == PLAYING) // Loop until the game is closed or the state changes
			{
				// Update
				deltatime = GetFrameTime();

				player->Update(&deltatime, borderBox->ReturnInnerBorder(), &gameState); //updates the player position
				waveManager->Update(&deltatime, player, borderBox->ReturnInnerBorder()); //updates the wave manager

				BeginDrawing();

				ClearBackground(BLACK);
				//DrawBackingGridCheckers(); //draws the backing grid for testing
				player->Draw(); //draws the player
				waveManager->Draw(); //draws the wave manager
				borderBox->Draw(); //draws the border box
				waveCounter->Draw(waveManager->GetWaveNumber()); //draws the wave counter
				healthBar->Draw(player->ReturnMaxHealth(), player->ReturnHealth()); //draws the health bar

				EndDrawing();
			}
			waveReached = *waveManager->GetWaveNumber(); // Get the wave reached
			// Clean up
			delete player; // Delete the player object
			delete borderBox; // Delete the border box object
			delete waveCounter; // Delete the wave counter object
			delete healthBar; // Delete the health bar object
			delete waveManager; // Delete the wave manager object
			break;
		}
		case GAMEOVER: // If the game is over
		{
			while (!WindowShouldClose() && gameState == GAMEOVER) // Loop until the game is closed or the state changes
			{
				if (IsKeyPressed(KEY_R)) {
					gameState = MAINMENU; // Restart the game
				}
				else if (IsKeyPressed(KEY_ESCAPE)) {
					gameState = EXIT; // Exit the game
				}

				BeginDrawing();
				ClearBackground(BLACK); // Clear the background
				// Display game over screen
				DrawText("Game Over", screenScale.x / 2 - MeasureText("Game Over", 20) / 2, screenScale.y / 2 - 10, 20, WHITE);
				DrawText(TextFormat("You reached wave %i", waveReached), screenScale.x / 2 - MeasureText(TextFormat("You reached wave %i", waveReached), 20) / 2, screenScale.y / 2 + 10, 20, WHITE);
				DrawText("Press R to restart or ESC to exit", screenScale.x / 2 - MeasureText("Press R to restart or ESC to exit", 20) / 2, screenScale.y / 2 + 30, 20, WHITE);

				EndDrawing();
			}
			break;
		}
		}
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
