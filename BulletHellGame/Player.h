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
	
public:
	Player(const Vector2* screenScale);
	void SetDrawingPosition();
	void SetHitboxPosition();
	void Draw();
	void BorderCollision(Square* borderBox);
	void Movement(float* deltatime, Square* borderBox);
	void HealPlayer(int healAmount);
	void DamagePlayer(int damageAmount);

	// Getters
	int* ReturnHealth();
	int* ReturnMaxHealth();
	Square* ReturnHitbox();
};


#endif // PLAYER_H