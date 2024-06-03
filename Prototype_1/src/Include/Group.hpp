#include "Entity.hpp"
#include "Player.hpp"
#include "Wall.hpp"
#pragma once

class Group {
public:
	explicit Group(const pugi::xml_node& node);

	void setTexture(std::map<std::string, const sf::Texture>& textures);
	void drawCurrent(sf::RenderWindow& window) const;
	void update(const sf::Time& elapsedTime, sf::View& view, std::map<std::string, const sf::Texture>& textures);
	std::string	returnName() const;
	void handlePlayerInput(const sf::Keyboard::Key& key, const bool& isPressed);
	sf::Vector2f getMCPos();

private:
	std::string const windowName;
	std::vector<std::unique_ptr<Entity>> children;
	std::unique_ptr<Player> mainCharacter;

};