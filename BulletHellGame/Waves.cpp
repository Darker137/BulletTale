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

void WaveBase::Update(float* deltatime, Square* PlayerHitbox, Square* borderHitbox) // Update the wave
{
	for (size_t i = 0; i < attacks.size(); i++) { // Loop through each attack in the wave
		attacks[i]->Update(deltatime, PlayerHitbox, borderHitbox); // Update the attack
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

WaveManager::WaveManager(const Vector2* screenScale) // Constructor for the WaveManager class
{
	waveNumber = 1; // Initialize the wave number

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
}

void WaveManager::Update(float* deltatime, Square* PlayerHitbox, Square* borderHitbox) // Update the wave manager
{
	if (waves.size() > 0) { // Check if there are no waves left
		waves[0]->Update(deltatime, PlayerHitbox, borderHitbox); // Update the front wave
		if (waves[0]->CheckForWaveEnd()) { // Check if the wave is still active
			waves[0]->Draw(); // Draw the wave
		}
		else { // If the wave is not active
			delete waves[0]; // Delete the wave
			waves.erase(waves.begin()); // Remove the wave from the vector
		}
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