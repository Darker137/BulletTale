#include "Attacks.h"

//bullet class implementation

/// Constructor for the Bullet class
Bullet::Bullet(Vector2 position, Vector2 velocity) : position(position), velocity(velocity) // Initialize position
{
	// Set the hitbox size and position
	hitbox.size = { 16.0f, 16.0f };
	hitbox.position = { position.x - (hitbox.size.x /2), position.y - (hitbox.size.y / 2) };
}

/// Update the bullet position based on the delta time
void Bullet::Update(float* deltatime)
{
	// Update the bullet position based on the velocity and delta time
	position.x += velocity.x * (*deltatime);
	position.y += velocity.y * (*deltatime);
	// Update the hitbox position
	hitbox.position = { position.x - (hitbox.size.x / 2), position.y - (hitbox.size.y / 2) };
}

/// Draw the bullet at its current position
void Bullet::Draw()
{
	// Draw the bullet as a rectangle at its current position
	DrawRectangle(hitbox.position.x, hitbox.position.y, hitbox.size.x, hitbox.size.y, WHITE);
}

/// Check for collision with the player hitbox
bool Bullet::PlayerCollision(Square* playerHitbox)
{
	// Check for collision with the player hitbox
	return (hitbox.position.x < playerHitbox->position.x + playerHitbox->size.x &&
		hitbox.position.x + hitbox.size.x > playerHitbox->position.x &&
		hitbox.position.y < playerHitbox->position.y + playerHitbox->size.y &&
		hitbox.position.y + hitbox.size.y > playerHitbox->position.y);
}

bool Bullet::BorderCollision(Square* borderBox)
{
	// check for collision with the border of play area

	return (hitbox.position.x < borderBox->position.x ||
		hitbox.position.x + hitbox.size.x > borderBox->position.x + borderBox->size.x ||
		hitbox.position.y < borderBox->position.y ||
		hitbox.position.y + hitbox.size.y > borderBox->position.y + borderBox->size.y);
}



// Attack class implementation

/// Constructor for the Attack class
Attack::Attack(int* wave, float uniqueSpeed) : uniqueSpeed(uniqueSpeed) // Initialize unique speed
{
	// Initialize the attack with the given wave
	damage = 0; // Set default damage
	baseSpeed = 100.0f * pow(1.05f, *wave - 1);
	isActive = true; // Set the attack as active
}

bool* Attack::GetIsActive()
{
	// Return the active status of the attack
	return &isActive;
}

BulletWheel::BulletWheel(int* wave, Vector2 position) : Attack(wave, uniqueSpeed)
{
	// Initialize the BulletWheel attack with the given wave and unique sp
	damage = 10; // Set default damage for BulletWheel
	uniqueSpeed = 1 * baseSpeed; // Set unique speed based on base speed

	wheelPosition = position; // Set the wheel position
	wheelSize = { 32.0f, 32.0f }; // Set the wheel size
	wheelHitbox.width = 0.0f; // Set the wheel hitbox size
	wheelHitbox.height = 0.0f; // Set the wheel hitbox size
	wheelHitbox.x = wheelPosition.x; // Set the wheel hitbox position
	wheelHitbox.y = wheelPosition.y; // Set the wheel hitbox position
	wheelOrigin = { 0,0 }; // Set the wheel origin
	wheelRotation = 0.0f; // Set the wheel rotation

	bulletCap = 20 * pow(1.05f, *wave - 1); // Set the maximum number of bullets
	bulletInterval = 0.2 - (pow(1.1, (*wave - 1)) / 1000); // Set the interval between bullets
	bulletCount = 0.0f; // Initialize the bullet count
	bulletTimer = 0.0f; // Initialize the bullet timer

	warmupInterval = 1.0f; // Set the warmup interval
	warmupTimer = 0.0f; // Initialize the warmup timer
}

void BulletWheel::Update(float* deltatime, Square* PlayerHitbox, Square* borderHitbox) {
	if (warmupTimer < warmupInterval) { // Check if the warmup timer is less than the warmup interval
			warmupTimer += *deltatime; // Update the warmup timer
			wheelHitbox.width += wheelSize.x * (*deltatime); // Increase the wheel hitbox width
			wheelHitbox.height += wheelSize.y * (*deltatime); // Increase the wheel hitbox height
			if (wheelHitbox.width >= wheelSize.x) { // Check if the wheel hitbox width exceeds the maximum
				wheelHitbox.width = wheelSize.x; // Set the wheel hitbox width to the maximum
			}
			if (wheelHitbox.height >= wheelSize.y) { // Check if the wheel hitbox height exceeds the maximum
				wheelHitbox.height = wheelSize.y; // Set the wheel hitbox height to the maximum
			}
			SetOrigin(); // Set the origin of the wheel
		}
	else { // If the warmup is complete
		if (bulletCount < bulletCap) { // Check if the bullet count is less than the maximum
			if (bulletTimer >= bulletInterval) {
				NewBullet(); // Create a new bullet
				bulletTimer = 0.0f; // Reset the bullet timer
				bulletCount++; // Increment the bullet count
			}
			else {
				bulletTimer += *deltatime; // Update the bullet timer
			}
		}
	}

	// Update the wheel rotation
	wheelRotation += 360.0f * (*deltatime) * 0.5f; // Rotate the wheel
	if (wheelRotation >= 360.0f) { // Check if the wheel rotation exceeds 360 degrees
		wheelRotation -= 360.0f; // Reset the wheel rotation
	}
	// Update the position of each bullet
	for (size_t i = 0; i < bullets.size(); i++) {
		bullets[i].Update(deltatime); // Update each bullet
	}
	// Check for collision with the player hitbox and border hitbox
	Collision(PlayerHitbox, borderHitbox); // Check for collision with player and border
	CheckForAttackEnd(); // Check if the attack is still active
}

void BulletWheel::NewBullet() {
	// Create a new bullet and add it to the vector
	Vector2 bulletVelocity = { cosf(wheelRotation * DEG2RAD) * uniqueSpeed, sinf(wheelRotation * DEG2RAD) * uniqueSpeed }; // Calculate the bullet velocity based on the wheel rotation
	bullets.push_back(Bullet(wheelPosition, bulletVelocity)); // Add the new bullet to the vector
}

void BulletWheel::Draw() {
	// Draw the wheel
	DrawRectanglePro(wheelHitbox, wheelOrigin, wheelRotation, WHITE);
	// Draw each bullet
	for (size_t i = 0; i < bullets.size(); i++) {
		bullets[i].Draw(); // Draw each bullet
	}
}

void BulletWheel::Collision(Square* playerHitbox, Square* borderHitbox) {
	// Check for collision with the player hitbox
	for (size_t i = 0; i < bullets.size(); i++) {
		if (bullets[i].PlayerCollision(playerHitbox)) {
			// Handle collision with player
			bullets.erase(bullets.begin() + i); // Remove the bullet
			i--; // Decrement the index
		}
	}
	// Check for collision with the border hitbox
	for (size_t i = 0; i < bullets.size(); i++) {
		if (bullets[i].BorderCollision(borderHitbox)) {
			// Handle collision with border
			bullets.erase(bullets.begin() + i); // Remove the bullet
			i--; // Decrement the index
		}
	}
}

void BulletWheel::CheckForAttackEnd() {
	// Check if the attack is still active
	if (isActive) {
		// Check if the bullet count is less than the maximum
		if (bulletCount >= bulletCap && bullets.size() <= 0) {
			isActive = false; // Keep the attack active
		}
		else {
			isActive = true; // End the attack
		}
	}
}

void BulletWheel::SetOrigin() {
	// Set the origin of the wheel
	wheelOrigin.x = wheelHitbox.width / 2;
	wheelOrigin.y = wheelHitbox.height / 2;
}