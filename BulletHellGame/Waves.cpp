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

	playArea.position = { border->position.x , border->position.y }; // Set the spawn area position
	playArea.size = { border->size.x , border->size.y }; // Set the spawn area size

	float xPos[2];
	xPos[0] = playArea.position.x + (playArea.size.y * 0.2f); // Calculate the x position for the spawn area
	xPos[1] = playArea.position.x + playArea.size.x - (playArea.size.y * 0.2f); // Calculate the x position for the spawn area

	for (int i = 0; i < 2; i++) { // Loop through the x positions
		for (int j = 0; j < 4; j++) { // Loop through the y positions
			bulletAttackPositions.push_back({ xPos[i], playArea.position.y + (playArea.size.y * 0.2f * (j+1)) }); // Add the spawn position to the vector
		}
	}

	laserSpawnArea.position = { playArea.position.x + (playArea.size.x * 0.2f) , playArea.position.y + (playArea.size.y * 0.2f)};
	laserSpawnArea.size = { playArea.size.x - (playArea.size.x * 0.4f) , playArea.size.y - (playArea.size.y * 0.4f)};
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
	int bulletAttackCount = waveNumber / 5 + 1; // Calculate the number of attacks in the wave	
	if (bulletAttackCount > 4) { // Limit the maximum number of attacks to 5
		bulletAttackCount = 4; // Set the attack count to 5
	}
	if (rand() % 2 == 0) { // Randomly increase the attack count
		bulletAttackCount++; // Increase the attack count with a 50% chance
	}
	WaveBase* newWave = new WaveBase(); // Create a new wave

	vector<int> usedSpawns; // Vector to keep track of used spawn positions

	for (int i = 0; i < bulletAttackCount; i++) { // Loop through the number of attacks
		int spawnIndex = rand() % bulletAttackPositions.size(); // Randomly select a spawn position
		while (find(usedSpawns.begin(), usedSpawns.end(), spawnIndex) != usedSpawns.end()) {
			// Check if the spawn position has already been used
			spawnIndex = rand() % bulletAttackPositions.size(); // Select a new spawn position
		}
		usedSpawns.push_back(spawnIndex); // Add the spawn position to the used list
		if (rand() % 2 == 0) { // Randomly choose between BulletWheel and BulletRing
			newWave->AddAttack(new BulletWheel(&waveNumber, bulletAttackPositions[spawnIndex])); // Add a BulletWheel attack to the wave
		}
		else {
			// Add a BulletRing attack to the wave
			newWave->AddAttack(new BulletRing(&waveNumber, bulletAttackPositions[spawnIndex])); // Add a BulletWheel attack to the wave
		}
	}

	int laserAttackCount = waveNumber / 5 + 1;

	for (int i = 0; i < laserAttackCount; i++) {
		newWave->AddAttack(new Laser(&waveNumber, &playArea, &laserSpawnArea));
	}
	waves.push_back(newWave); // Add the wave to the vector
}