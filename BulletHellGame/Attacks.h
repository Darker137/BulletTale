#ifndef ATTACKS_H
#define ATTACKS_H

#include "GeneralFunctions.h"

class Bullet // Bullet class
{
protected:
	Vector2 position; // position of the bullet
	Square hitbox; // hitbox of the bullet
	Vector2 velocity; // velocity of the bullet
public:
	Bullet(Vector2 position, Vector2 velocity); // constructor
	void Update(float* deltatime); // update the bullet
	void Draw(); // draw the bullet
	bool PlayerCollision(Square* playerHitbox); // check for collision with player hitbox
	bool BorderCollision(Square* borderBox); // check for collision with border box
};

class Attack
{
protected:
	int damage; // damage of the attack
	float baseSpeed; // speed of the attack
	float uniqueSpeed; // current speed of the attack
	bool isActive; // is the attack active
public:
	Attack(int* wave, float uniqueSpeed); // constructor
	virtual void Update(float* deltatime, Square* playerHitbox, Square* borderHitbox) = 0; // update the attack
	virtual void Draw() = 0; // draw the attack
	virtual void Collision(Square* playerHitbox, Square* borderHitbox) = 0; // check for collision with player hitbox
	virtual void CheckForAttackEnd() = 0; // check for attack end
	bool* GetIsActive(); // get the active status of the attack
};

class BulletWheel : public Attack
{
protected:
	Vector2 wheelPosition;
	Vector2 wheelSize;
	Rectangle wheelHitbox;
	Vector2 wheelOrigin;
	float wheelRotation;

	vector<Bullet> bullets; // vector of bullets
	int bulletCount; // number of bullets shot in total
	int bulletCap; // maximum number of bullets
	float bulletInterval; // interval between bullets
	float bulletTimer; // timer for bullet interval

	float warmupInterval; // interval for warmup
	float warmupTimer; // timer for warmup

	float uniqueSpeed; // unique speed of the attack

public:
	BulletWheel(int* wave, Vector2 position); // constructor
	void Update(float* deltatime, Square* playerHitbox, Square* borderHitbox) override; // update the attack
	void NewBullet(); // create a new bullet
	void Draw() override; // draw the attack
	void Collision(Square* playerHitbox, Square* borderHitbox) override; // check for collision with player hitbox
	void CheckForAttackEnd() override; // check for attack end

	void SetOrigin(); // set the origin of the wheel
};

#endif