#include "GeneralFunctions.h"

void DrawBackingGridCheckers() { //draws the backing grid for testing
	for (int i = 0; i < 1280; i += 32)
	{
		for (int j = 0; j < 720; j += 32)
		{
			if ((i / 32 + j / 32) % 2 == 0)
			{
				DrawRectangle(i, j, 32, 32, RAYWHITE);
			}
			else
			{
				DrawRectangle(i, j, 32, 32, GRAY);
			}
		}
	}
}

float Vector2Length(Vector2 vector) {
	// Calculate the length of the vector using the Pythagorean theorem
	return sqrtf((vector.x * vector.x) + (vector.y * vector.y));
}

Vector2 Vector2Normalize(Vector2 vector) {
	// Normalize the vector by dividing each component by its length
	float length = Vector2Length(vector);
	if (length == 0) return { 0, 0 };
	return { vector.x / length, vector.y / length };
}