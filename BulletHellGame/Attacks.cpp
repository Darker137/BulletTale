#include "Attacks.h"

//bullet class implementation

/// Constructor for the Bullet class
Bullet::Bullet(Vector2 position, Vector2 velocity) : position(position), velocity(velocity) // Initialize position
{
	// Set the hitbox size and position
	hitbox.size = { 16.0f, 16.0f };
	hitbox.position = { position.x - (hitbox.size.x /2), position.y - (hitbox.size.y / 2) };
	if (rand() % 29 == 0)
	{
		healer = true; // Set the bullet as a healer
	}
	else
	{
		healer = false; // Set the bullet as not a healer
	}
}

/// Update the bullet position based on the delta time
void Bullet::Update(float* deltatime)
{
	// Update the bullet velocity by a small factor
	velocity.x *= 1.01f;
	velocity.y *= 1.01f;
	// Update the bullet position based on the velocity and delta time
	position.x += velocity.x * (*deltatime / 2);
	position.y += velocity.y * (*deltatime / 2);
	// Update the hitbox position
	hitbox.position = { position.x - (hitbox.size.x / 2), position.y - (hitbox.size.y / 2) };
}

/// Draw the bullet at its current position
void Bullet::Draw()
{
	// Draw the bullet as a rectangle at its current position
	if (healer)
	{
		DrawRectangle(hitbox.position.x, hitbox.position.y, hitbox.size.x, hitbox.size.y, GREEN);
	}
	else
	{
		DrawRectangle(hitbox.position.x, hitbox.position.y, hitbox.size.x, hitbox.size.y, WHITE);
	}
}

/// Check for collision with the player hitbox
bool Bullet::PlayerCollision(Player* player)
{
	// Check for collision with the player hitbox
	return (hitbox.position.x < player->ReturnHitbox()->position.x + player->ReturnHitbox()->size.x &&
		hitbox.position.x + hitbox.size.x > player->ReturnHitbox()->position.x &&
		hitbox.position.y < player->ReturnHitbox()->position.y + player->ReturnHitbox()->size.y &&
		hitbox.position.y + hitbox.size.y > player->ReturnHitbox()->position.y);
}

bool Bullet::BorderCollision(Square* borderBox)
{
	// check for collision with the border of play area

	return (hitbox.position.x < borderBox->position.x ||
		hitbox.position.x + hitbox.size.x > borderBox->position.x + borderBox->size.x ||
		hitbox.position.y < borderBox->position.y ||
		hitbox.position.y + hitbox.size.y > borderBox->position.y + borderBox->size.y);
}

bool* Bullet::ReturnHealer()
{
	// Return if the bullet is a healer
	return &healer;
}

bool Bullet::OtherCollision(Rectangle* otherBox)
{
	// Check for collision with another box
	return (hitbox.position.x < otherBox->x + otherBox->width &&
		hitbox.position.x + hitbox.size.x > otherBox->x &&
		hitbox.position.y < otherBox->y + otherBox->height &&
		hitbox.position.y + hitbox.size.y > otherBox->y);
}
// Attack class implementation

/// Constructor for the Attack class
Attack::Attack(int* wave) : uniqueSpeed(0) // Initialize unique speed
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

// BulletAttack class implementation

BulletAttack::BulletAttack(int* wave) : Attack(wave)
{
	sourcePosition = { 0.0f,0.0f }; // Initialize the source position
	sourceSize = { 0.0f,0.0f }; // Initialize the source size
	sourceHitbox = { 0.0f,0.0f,0.0f,0.0f }; // Initialize the source hitbox
	sourceOrigin = { 0.0f,0.0f }; // Initialize the source origin
	sourceRotation = 0;	// Initialize the source rotation

	bullets; // vector of bullets EMPTY
	bulletCount = 0; // number of bullets shot in total
	bulletCap = 0; // maximum number of bullets
	bulletInterval = 0; // interval between bullets
	bulletTimer = 0; // timer for bullet interval

	warmupInterval = 0; // interval for warmup
	warmupTimer = 0; // timer for warmup
}

void BulletAttack::Draw() {
	// Draw the source
	DrawRectanglePro(sourceHitbox, sourceOrigin, sourceRotation, WHITE);
	// Draw each bullet
	for (size_t i = 0; i < bullets.size(); i++) {
		bullets[i].Draw(); // Draw each bullet
	}
}

void BulletAttack::PlayerCollision(Player* player) {

	for (int i = bullets.size() - 1; i >= 0; i--) {
		if (bullets[i].PlayerCollision(player))
		{
			if (*bullets[i].ReturnHealer()) { // Check if the bullet is a healer
				player->HealPlayer(10); // Heal the player
			}
			else {
				player->DamagePlayer(damage); // Damage the player
			}
			bullets.erase(bullets.begin() + i); // Remove the bullet
		}
	}
}

void BulletAttack::BorderCollision(Square* borderHitbox) {
	for (int i = bullets.size() - 1; i >= 0; i--) {
		if (bullets[i].BorderCollision(borderHitbox)) {
			bullets.erase(bullets.begin() + i); // Remove the bullet
		}
	}
}

void BulletAttack::CheckForAttackEnd() {
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

void BulletAttack::SetOrigin() {
	// Set the origin of the source
	sourceOrigin.x = sourceHitbox.width / 2;
	sourceOrigin.y = sourceHitbox.height / 2;
}

// Warmup the attack and return if the attack is ready
bool BulletAttack::Warmup(float* deltatime) {
	// Warmup the attack and return if the attack is ready
	if (warmupTimer < warmupInterval) { // Check if the warmup timer is less than the warmup interval
		warmupTimer += *deltatime; // Update the warmup timer
		sourceHitbox.width += sourceSize.x * (*deltatime); // Increase the wheel hitbox width
		sourceHitbox.height += sourceSize.y * (*deltatime); // Increase the wheel hitbox height
		if (sourceHitbox.width >= sourceSize.x) { // Check if the wheel hitbox width exceeds the maximum
			sourceHitbox.width = sourceSize.x; // Set the wheel hitbox width to the maximum
		}
		if (sourceHitbox.height >= sourceSize.y) { // Check if the wheel hitbox height exceeds the maximum
			sourceHitbox.height = sourceSize.y; // Set the wheel hitbox height to the maximum
		}
		SetOrigin(); // Set the origin of the wheel
		return false; // Return false to indicate that the attack is not ready
	}
	else {
		return true;
	}
}


// BulletWheel class implementation
BulletWheel::BulletWheel(int* wave, Vector2 position) : BulletAttack(wave) // Initialize the BulletWheel attack
{
	// Initialize the BulletWheel attack with the given wave and unique sp
	damage = 10; // Set default damage for BulletWheel
	uniqueSpeed = 1 * baseSpeed; // Set unique speed based on base speed

	sourcePosition = position; // Set the wheel position
	sourceSize = { 32.0f, 32.0f }; // Set the wheel size
	sourceHitbox.width = 0.0f; // Set the wheel hitbox size
	sourceHitbox.height = 0.0f; // Set the wheel hitbox size
	sourceHitbox.x = sourcePosition.x; // Set the wheel hitbox position
	sourceHitbox.y = sourcePosition.y; // Set the wheel hitbox position
	sourceOrigin = { 0,0 }; // Set the wheel origin
	sourceRotation = 0.0f; // Set the wheel rotation

	bulletCap = 20 * pow(1.05f, *wave - 1); // Set the maximum number of bullets
	bulletInterval = 0.1 - (pow(1.1, (*wave - 1)) / 1000); // Set the interval between bullets
	bulletCount = 0.0f; // Initialize the bullet count
	bulletTimer = 0.0f; // Initialize the bullet timer

	warmupInterval = 1.0f; // Set the warmup interval
	warmupTimer = 0.0f; // Initialize the warmup timer

	if (rand() % 2 == 0) // Randomly set the attack direction
	{
		clockWise = true; // Set the attack as clockwise
	}
	else
	{
		clockWise = false; // Set the attack as counter-clockwise
	}
}

void BulletWheel::Update(float* deltatime, Player* player, Square* borderHitbox) {
	if (Warmup(deltatime)) { // Check if the attack is ready 
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
	if (clockWise) { // Check if the attack is clockwise
		sourceRotation -= 360.0f * (*deltatime) * 0.8f; // Rotate the wheel
		if (sourceRotation <= 0.0f) { // Check if the wheel rotation is less than or equal to 0 degrees
			sourceRotation += 360.0f; // Reset the wheel rotation
		}
	}
	else {
		sourceRotation += 360.0f * (*deltatime) * 0.8f; // Rotate the wheel
		if (sourceRotation >= 360.0f) { // Check if the wheel rotation is greater than or equal to 360 degrees
			sourceRotation -= 360.0f; // Reset the wheel rotation
		}
	}
	// Update the position of each bullet
	for (size_t i = 0; i < bullets.size(); i++) {
		bullets[i].Update(deltatime); // Update each bullet
	}
	// Check for collision with the player hitbox and border hitbox
	PlayerCollision(player); // Check for collision with player and border
	BorderCollision(borderHitbox); // Check for collision with border
	CheckForAttackEnd(); // Check if the attack is still active
}

void BulletWheel::NewBullet() {
	// Create a new bullet and add it to the vector
	Vector2 bulletVelocity = { cosf(sourceRotation * DEG2RAD) * uniqueSpeed, sinf(sourceRotation * DEG2RAD) * uniqueSpeed }; // Calculate the bullet velocity based on the wheel rotation
	bullets.push_back(Bullet(sourcePosition, bulletVelocity)); // Add the new bullet to the vector
}


// BulletRing class implementation

BulletRing::BulletRing(int* wave, Vector2 position) : BulletAttack(wave) // Initialize the BulletRing attack
{
	// Initialize the BulletWheel attack with the given wave and unique sp
	damage = 10; // Set default damage for BulletWheel
	uniqueSpeed = 1 * baseSpeed; // Set unique speed based on base speed

	sourcePosition = position; // Set the wheel position
	sourceSize = { 32.0f, 32.0f }; // Set the wheel size
	sourceHitbox.width = 0.0f; // Set the wheel hitbox size
	sourceHitbox.height = 0.0f; // Set the wheel hitbox size
	sourceHitbox.x = sourcePosition.x; // Set the wheel hitbox position
	sourceHitbox.y = sourcePosition.y; // Set the wheel hitbox position
	sourceOrigin = { 0,0 }; // Set the wheel origin
	sourceRotation = 0.0f; // Set the wheel rotation

	bulletCap = ((*wave / 10) + 1) * 2;
	bulletInterval = 0.5 - (pow(1.1, (*wave - 1)) / 1000); // Set the interval between bullets
	bulletCount = 0.0f; // Initialize the bullet count
	bulletTimer = 0.0f; // Initialize the bullet timer

	warmupInterval = 1.0f; // Set the warmup interval
	warmupTimer = 0.0f; // Initialize the warmup timer
	alternation = false; // Set the attack as not alternated

	if (rand() % 2 == 0) // Randomly set the attack direction
	{
		outward = true; // Set the attack as clockwise
	}
	else
	{
		outward = false; // Set the attack as counter-clockwise
	}
}

void BulletRing::Update(float* deltatime, Player* player, Square* borderHitbox)
{
	if (Warmup(deltatime)) { // Check if the attack is ready 
		if (bulletCount < bulletCap) { // Check if the bullet count is less than the maximum
			if (bulletTimer >= bulletInterval) {
				NewBullet(); // Create new bullets
				bulletTimer = 0.0f; // Reset the bullet timer
				bulletCount += 1; // Increment the bullet count
				sourceRotation = 45.0f * bulletCount; // Set the wheel rotation
				alternation = !alternation; // Alternate the attack direction
			}
			else {
				bulletTimer += *deltatime; // Update the bullet timer
			}
		}
	}
	// Update the position of each bullet
	for (size_t i = 0; i < bullets.size(); i++) {
		bullets[i].Update(deltatime); // Update each bullet
	}
	PlayerCollision(player); // Check for collision with player and border
	if (outward) {
		BorderCollision(borderHitbox); // Check for collision with border
	}
	else {
		SourceCollision(); // Check for collision with source
	}
	CheckForAttackEnd(); // Check if the attack is still active
}

void BulletRing::NewBullet() {

	// Create a new bullet and add it to the vector
	for (int i = 0; i < 360; i += 30)
	{
		float offset = 0;
		if (alternation)
		{
			offset = 15;
		}
		else
		{
			offset = 0;
		}
		Vector2 bulletVelocity = { cosf((i + offset) * DEG2RAD) * uniqueSpeed, sinf((i + offset) * DEG2RAD) * uniqueSpeed }; // Calculate the bullet velocity based on the wheel rotation
		if (outward)
		{
			bullets.push_back(Bullet(sourcePosition, bulletVelocity)); // Add the new bullet to the vector
		}
		else
		{
			bullets.push_back(Bullet({sourcePosition.x + cosf((i + offset) * DEG2RAD) * 1000, sourcePosition.y + sinf((i + offset) * DEG2RAD) * 1000}, {-bulletVelocity.x, -bulletVelocity.y})); // Add the new bullet to the vector
		}
	}
}

void BulletRing::SourceCollision()
{
	// Check for collision with the border of play area
	for (int i = bullets.size() - 1; i >= 0; i--) {
		if (bullets[i].OtherCollision(&sourceHitbox)) {
			bullets.erase(bullets.begin() + i); // Remove the bullet
		}
	}
}