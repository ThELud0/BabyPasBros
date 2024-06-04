#include "Entity.hpp"
#include "Player.hpp"
#include "Wall.hpp"
#pragma once

class Door : public Entity {
public:
	explicit		Door(float x, float y, int height, int width, const bool& vertical);
	explicit		Door(const pugi::xml_node& node);
	void			setTexture(std::map<std::string, const sf::Texture>& textures) override;
	void			drawCurrent(sf::RenderWindow& window) const override;
	sf::Vector2f	getPos() override;
	sf::Vector2f	getSiz() override;
	void			collide(sf::Vector2f mcPos, sf::Vector2f mcSize, const sf::Time& elapsedTime, bool physical) override;
	void			update(const sf::Time& elapsedTime, sf::View& view, std::map<std::string, const sf::Texture>& textures) override;
	void			handlePlayerInput(const sf::Keyboard::Key& key, const bool& isPressed) override;

	bool			isVertical() const;
private:
	sf::RectangleShape	wShape;
	const bool			vertical;
	bool				isNear{ false };
	sf::RectangleShape	doorText;

};
