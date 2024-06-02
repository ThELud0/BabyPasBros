#include "SFML/Graphics.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "pugixml.hpp"
#pragma once

class Entity {
public:
	explicit Entity(float x, float y, int hei, int wid) ;
	explicit Entity(const pugi::xml_node& node);
	virtual void setTexture(std::map<std::string, const sf::Texture> &textures) = 0;
	virtual void drawCurrent(sf::RenderWindow& window) const = 0;
	virtual sf::Vector2f getPos() = 0 ;
	virtual sf::Vector2f getSiz() = 0 ;

	//Spawn coordinates of the entity
	float x;
	float y;
	int height;
	int width;

};