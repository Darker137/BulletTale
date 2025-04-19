#include "Player.h"


Player::Player(const Vector2* screenScale)
{
	// Load the player texture
	texture = LoadTexture("Resources/PlayerIcon.png");

	// Set the initial position and scale
	centerPosition = { screenScale->x / 2 , screenScale->y / 2 };
	scale = 2.0f;
	drawingSquare.size = { (float)texture.width * scale, (float)texture.height * scale };
	drawingSquare.position = { 0,0 }; SetDrawingPosition();

	// Set the hitbox size and position
	hitboxSquare.size = { drawingSquare.size.x - (4 * scale), drawingSquare.size.y - (4 * scale) };
	hitboxSquare.position = { 0,0 }; SetHitboxPosition();

	velocity = { 0, 0 };
	speed = 300.0f;

	maxHealth = 100;
	health = maxHealth;

	isInvincible = false;
	invincibilityDuration = 0.5f; // Duration of invincibility in seconds
	invincibilityTimer = 0.0f; // Timer for invincibility
}

void Player::SetDrawingPosition()
{
	// Set the draw position based on the player's position and size
	drawingSquare.position.x = centerPosition.x - (drawingSquare.size.x / 2);
	drawingSquare.position.y = centerPosition.y - (drawingSquare.size.y / 2);
}

void Player::SetHitboxPosition()
{
	// Set the hitbox position based on the player's position and size
	hitboxSquare.position.x = centerPosition.x - (hitboxSquare.size.x / 2);
	hitboxSquare.position.y = centerPosition.y - (hitboxSquare.size.y / 2);
}

void Player::Draw()
{
	if (isInvincible) {
		if (flash) {
			// Draw the hitbox with a red color if the player is invincible
			DrawTextureEx(texture, drawingSquare.position, 0.0f, scale, WHITE);
		}
	}
	else {
		// Draw the player texture at the calculated edge position
		DrawTextureEx(texture, drawingSquare.position, 0.0f, scale, WHITE);
	}
}

void Player::BorderCollision(Square* borderBox)
{
	// Check for collision with the border box
	if (centerPosition.x - (drawingSquare.size.x / 2) < borderBox->position.x) {
		centerPosition.x = borderBox->position.x + (drawingSquare.size.x / 2);
	}
	else if (centerPosition.x + (drawingSquare.size.x / 2) > borderBox->position.x + borderBox->size.x) {
		centerPosition.x = borderBox->position.x + borderBox->size.x - (drawingSquare.size.x / 2);
	}
	if (centerPosition.y - (drawingSquare.size.y / 2) < borderBox->position.y) {
		centerPosition.y = borderBox->position.y + (drawingSquare.size.y / 2);
	}
	else if (centerPosition.y + (drawingSquare.size.y / 2) > borderBox->position.y + borderBox->size.y) {
		centerPosition.y = borderBox->position.y + borderBox->size.y - (drawingSquare.size.y / 2);
	}
}

void Player::Movement(float* deltatime, Square* borderBox) 
{
	// Reset the velocity vector
	velocity.x = 0;
	velocity.y = 0;
	// get player input
	if (IsKeyDown(KEY_UP)) velocity.y -= speed;
	if (IsKeyDown(KEY_DOWN)) velocity.y += speed;
	if (IsKeyDown(KEY_LEFT)) velocity.x -= speed;
	if (IsKeyDown(KEY_RIGHT)) velocity.x += speed;
	// Normalize the velocity vector if its length exceeds maxSpeed
	if (Vector2Length(velocity) > speed) {
		velocity = Vector2Normalize(velocity);
		velocity.x *= speed;
		velocity.y *= speed;
	}
	// Update the player's position based on the velocity and delta time
	centerPosition.x += velocity.x * (*deltatime);
	centerPosition.y += velocity.y * (*deltatime);
	// Set the edge position based on the new center position
	BorderCollision(borderBox);
	SetDrawingPosition();
	SetHitboxPosition();
}

void Player::HealPlayer(int healAmount)
{
	// Heal the player by the specified amount
	health += healAmount;
	if (health > maxHealth) {
		health = maxHealth; // Cap the health at maxHealth
	}
}

void Player::DamagePlayer(int damageAmount)
{	
	if (!isInvincible) {
		// Damage the player by the specified amount
		health -= damageAmount;
		if (health < 0) {
			health = 0; // Cap the health at 0
		}
	}
	cout << health << endl; // Print the current health
	isInvincible = true; // Set the player as invincible
	flash = false; // Reset the flash effect
}

// Getters

int* Player::ReturnHealth() {
	// Return the player's health
	return &health;
}

int* Player::ReturnMaxHealth() {
	// Return the player's max health
	return &maxHealth;
}

Square* Player::ReturnHitbox() {
	// Return the player's hitbox
	return &hitboxSquare;
}

// Update the player's position based on the delta time

void Player::Update(float* deltatime, Square* borderBox, GameState* gameState)
{
	if (health <= 0) {
		*gameState = GAMEOVER; // Change the game state to GAMEOVER if health is 0
	}
	if (isInvincible) {
		invincibilityTimer += *deltatime; // Update the invincibility timer
		if (invincibilityTimer >= invincibilityDuration) {
			isInvincible = false; // Reset the invincibility status
			invincibilityTimer = 0.0f; // Reset the invincibility timer
		}
		else {
			flashTimer += *deltatime; // Update the flash timer
			if (flashTimer >= flashDuration) {
				flash = !flash; // Toggle the flash effect
				flashTimer = 0.0f; // Reset the flash timer
			}
		}
	}
	// Update the player's position and hitbox
	Movement(deltatime, borderBox);
}