#include "SFML/Graphics.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "pugixml.hpp"
#pragma once

class Entity {
public:
	explicit				Entity(float x, float y, int hei, int wid) ;
	explicit				Entity(const pugi::xml_node& node);
	virtual void			setTexture(std::map<std::string, const sf::Texture> &textures) = 0;
	virtual void			drawCurrent(sf::RenderWindow& window) const = 0;
	virtual sf::Vector2f	getPos() = 0 ;
	virtual sf::Vector2f	getSiz() = 0 ;
	virtual void			update(const sf::Time& elapsedTime, sf::View& view, std::map<std::string, const sf::Texture>& textures) = 0;
	virtual void			collide(sf::Vector2f wallPos, sf::Vector2f wallSize, const sf::Time& elapsedTime, bool physical) = 0;
	virtual void			handlePlayerInput(const sf::Keyboard::Key& key, const bool& isPressed) = 0;
	bool					getPhysicalState() { return physical; };

	//Spawn coordinates of the entity
	float					x;
	float					y;
	int						height;
	int						width;
	bool					physical{ true };

};