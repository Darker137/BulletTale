#ifndef WAVES_H
#define WAVES_H

#include "Attacks.h"



class WaveBase
{
protected:
	vector<Attack*> attacks; // list of attacks in the wave
	bool isActive; // is the wave active
public:
	WaveBase(); // constructor
	void AddAttack(Attack* attack); // add an attack to the wave
	void Update(float* deltatime, Square* PlayerHitbox, Square* borderHitbox); // update the wave
	void Draw(); // draw the wave
	bool CheckForWaveEnd(); // set the wave as active or inactive
};

class WaveManager 
{
protected:
	int waveNumber; // current wave number
	vector<WaveBase*> waves; // list of waves
public:
	WaveManager(const Vector2* screenScale); // constructor
	void Update(float* deltatime, Square* PlayerHitbox, Square* borderHitbox); // update the wave manager
	void Draw(); // draw the wave manager
	int* GetWaveNumber(); // get the current wave number
};




#endif // WAVES_H