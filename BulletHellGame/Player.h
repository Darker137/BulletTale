#ifndef PLAYER_H
#define PLAYER_H

#include "GeneralFunctions.h"

class Player {
protected:
	// Player texture and position
	Texture2D texture;
	Vector2 centerPosition;
	float scale;
	Square drawingSquare;

	// Player hitbox
	Square hitboxSquare;

	// Player movement
	Vector2 velocity;
	float speed;

	//player health
	int maxHealth;
	int health;

	bool isInvincible; // Flag for invincibility
	float invincibilityDuration; // Duration of invincibility in seconds
	float invincibilityTimer; // Timer for invincibility
	
	bool flash = false; // Flag for flash effect
	float flashDuration = 0.05f; // Duration of the flash effect in seconds
	float flashTimer = 0.0f; // Timer for the flash effect
	
public:
	Player(const Vector2* screenScale);
	void SetDrawingPosition();
	void SetHitboxPosition();
	void Draw();
	void BorderCollision(Square* borderBox);
	void Movement(float* deltatime, Square* borderBox);
	void HealPlayer(int healAmount);
	void DamagePlayer(int damageAmount);
	void Update(float* deltatime, Square* borderBox, GameState* gameState);

	// Getters
	int* ReturnHealth();
	int* ReturnMaxHealth();
	Square* ReturnHitbox();
};


#endif // PLAYER_H