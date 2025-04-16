#ifndef ATTACKS_H
#define ATTACKS_H

#include "GeneralFunctions.h"
#include "Player.h"

class Bullet // Bullet class
{
protected:
	Vector2 position; // position of the bullet
	Square hitbox; // hitbox of the bullet
	Vector2 velocity; // velocity of the bullet
	bool healer; // is the bullet a healer
public:
	Bullet(Vector2 position, Vector2 velocity); // constructor
	void Update(float* deltatime); // update the bullet
	void Draw(); // draw the bullet
	bool PlayerCollision(Player* player); // check for collision with player hitbox
	bool BorderCollision(Square* borderBox); // check for collision with border box
	bool OtherCollision(Rectangle* otherBox); // check for collision with other box
	bool* ReturnHealer(); // return if the bullet is a healer

};

class Attack
{
protected:
	int damage; // damage of the attack
	float baseSpeed; // speed of the attack
	float uniqueSpeed; // current speed of the attack
	bool isActive; // is the attack active
public:
	Attack(int* wave); // constructor
	virtual void Update(float* deltatime, Player* player, Square* borderHitbox) = 0; // update the attack
	virtual void Draw() = 0; // draw the attack
	virtual void PlayerCollision(Player* player) = 0; // check for collision with player hitbox
	virtual void CheckForAttackEnd() = 0; // check for attack end
	bool* GetIsActive(); // get the active status of the attack
};

class BulletAttack : public Attack
{
protected:
	Vector2 sourcePosition;
	Vector2 sourceSize;
	Rectangle sourceHitbox;
	Vector2 sourceOrigin;
	float sourceRotation;

	vector<Bullet> bullets; // vector of bullets
	int bulletCount; // number of bullets shot in total
	int bulletCap; // maximum number of bullets
	float bulletInterval; // interval between bullets
	float bulletTimer; // timer for bullet interval

	float warmupInterval; // interval for warmup
	float warmupTimer; // timer for warmup
public:
	BulletAttack(int* wave); // constructor
	void Draw() override; // draw the attack
	void PlayerCollision(Player* player) override; // check for collision with player hitbox
	void BorderCollision(Square* borderHitbox); // check for collision with border box
	void CheckForAttackEnd() override; // check for attack end
	void SetOrigin(); // set the origin of the source
	bool Warmup(float* deltatime); // warmup the attack and return if the attack is ready

	virtual void NewBullet() = 0; // create a new bullet
};


class BulletWheel : public BulletAttack
{
protected:

	bool clockWise; // is the attack clockwise

public:
	BulletWheel(int* wave, Vector2 position); // constructor
	void Update(float* deltatime, Player* player, Square* borderHitbox) override; // update the attack
	void NewBullet() override; // create a new bullet
};

class BulletRing : public BulletAttack
{
protected:
	bool outward; // is the attack outward
	bool alternation; // is the attack alternating
public:
	BulletRing(int* wave, Vector2 position); // constructor
	void Update(float* deltatime, Player* player, Square* borderHitbox) override; // update the attack
	void NewBullet() override; // create a new bullet
	void SourceCollision(); // check for bullet collision with the source
};

#endif