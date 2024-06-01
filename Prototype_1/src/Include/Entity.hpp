#include "SFML/Graphics.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

class Entity {
public:
	Entity(float x, float y) : xSpawn(x), ySpawn(y) {};

private:
	//Spawn coordinates of the entity
	float xSpawn;
	float ySpawn;

};