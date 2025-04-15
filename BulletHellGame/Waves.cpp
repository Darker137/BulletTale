#include "Waves.h" // Include the header file for the Waves class

// Wave class implementation

WaveBase::WaveBase() // Constructor for the Wave class
{
	isActive = true; // Initialize the wave as active
}

void WaveBase::AddAttack(Attack* attack) // Add an attack to the wave
{
	attacks.push_back(attack); // Add the attack to the vector
}

void WaveBase::Update(float* deltatime, Player* player, Square* borderHitbox) // Update the wave
{
	for (size_t i = 0; i < attacks.size(); i++) { // Loop through each attack in the wave
		attacks[i]->Update(deltatime, player, borderHitbox); // Update the attack
	}
}

void WaveBase::Draw() // Draw the wave
{
	for (size_t i = 0; i < attacks.size(); i++) { // Loop through each attack in the wave
		attacks[i]->Draw(); // Draw the attack
	}
}

bool WaveBase::CheckForWaveEnd() // Check if the wave is active
{
	for (size_t i = 0; i < attacks.size(); i++) { // Loop through each attack in the wave
		if (*attacks[i]->GetIsActive()) { // Check if the attack is active
			return true; // Return true if any attack is active
		}
	}
	return false; // Return false if no attacks are active
}

// WaveManager class implementation

WaveManager::WaveManager(Square* border) // Constructor for the WaveManager class
{
	waveNumber = 1; // Initialize the wave number

	spawnArea.position = { border->position.x + 16, border->position.y + 16 }; // Set the spawn area position
	spawnArea.size = { border->size.x - 32, border->size.y - 32 }; // Set the spawn area size

/*
	// Initialize the waves vector with a new wave
	WaveBase* wave1 = new WaveBase(); // Create a new wave
	wave1->AddAttack(new BulletWheel(&waveNumber, { screenScale->x * 0.5f, screenScale->y * 0.25f })); // Add a BulletWheel attack to the wave
	waves.push_back(wave1); // Add the wave to the vector

	WaveBase* wave2 = new WaveBase(); // Create a new wave
	wave2->AddAttack(new BulletWheel(&waveNumber, { screenScale->x * 0.25f, screenScale->y * 0.5f })); // Add a BulletWheel attack to the wave
	wave2->AddAttack(new BulletWheel(&waveNumber, { screenScale->x * 0.75f, screenScale->y * 0.5f })); // Add a BulletWheel attack to the wave
	waves.push_back(wave2); // Add the wave to the vector

	WaveBase* wave3 = new WaveBase(); // Create a new wave
	wave3->AddAttack(new BulletWheel(&waveNumber, { screenScale->x * 0.25f, screenScale->y * 0.25f })); // Add a BulletWheel attack to the wave
	wave3->AddAttack(new BulletWheel(&waveNumber, { screenScale->x * 0.75f, screenScale->y * 0.25f })); // Add a BulletWheel attack to the wave
	wave3->AddAttack(new BulletWheel(&waveNumber, { screenScale->x * 0.25f, screenScale->y * 0.75f })); // Add a BulletWheel attack to the wave
	wave3->AddAttack(new BulletWheel(&waveNumber, { screenScale->x * 0.75f, screenScale->y * 0.75f })); // Add a BulletWheel attack to the wave
	waves.push_back(wave3); // Add the wave to the vector
*/
}

void WaveManager::Update(float* deltatime, Player* player, Square* borderHitbox) // Update the wave manager
{
	if (waves.size() > 0) { // Check if there are no waves left
		waves[0]->Update(deltatime, player, borderHitbox); // Update the front wave
		if (waves[0]->CheckForWaveEnd()) { // Check if the wave is still active
			waves[0]->Draw(); // Draw the wave
		}
		else { // If the wave is not active
			delete waves[0]; // Delete the wave
			waves.erase(waves.begin()); // Remove the wave from the vector
			waveNumber++; // Increment the wave number
		}

	}
	else { // If there are no waves left
		NewWave(); // Create a new wave
	}

}

void WaveManager::Draw() // Draw the wave manager
{
	if (waves.size() > 0)
	{
		waves[0]->Draw(); // Draw the front wave
	}
}

int* WaveManager::GetWaveNumber() // Get the current wave number
{
	return &waveNumber; // Return the wave number
}

void WaveManager::NewWave() {
	int attackCount = waveNumber / 5 + 1; // Calculate the number of attacks in the wave
	if (rand() % 10 == 0) {
		attackCount *= 2; // Double the attack count with a 10% chance
	}
	if (rand() % 2 == 0) {
		attackCount++; // Increase the attack count with a 50% chance
	}
	WaveBase* newWave = new WaveBase(); // Create a new wave
	for (int i = 0; i < attackCount; i++) { // Loop through the number of attacks
		newWave->AddAttack(new BulletWheel(&waveNumber, { spawnArea.position.x + (rand() % (int)spawnArea.size.x), spawnArea.position.y + (rand() % (int)spawnArea.size.y) })); // Add a BulletWheel attack to the wave
	}
	waves.push_back(newWave); // Add the wave to the vector
}