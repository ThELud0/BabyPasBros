#include "Entity.hpp"
#pragma once

class Wall : public Entity {
public:
	explicit				Wall(float x, float y, int height, int width, const bool &vertical);
	explicit				Wall(const pugi::xml_node& node);
	void					setTexture(std::map<std::string, const sf::Texture, std::less<>>& textures) override;
	void					setSoundBuffer(std::map<std::string, const sf::SoundBuffer, std::less<>>& soundBuffers) override;
	void					drawCurrent(sf::RenderWindow& window) const override;
	sf::Vector2f			getPos() override;
	sf::Vector2f			getSiz() override;
	void					collide(sf::Vector2f wallPos, sf::Vector2f wallSize, const sf::Time& elapsedTime, bool physical) override;
	void					update(const sf::Time& elapsedTime, sf::View& view, std::map<std::string, const sf::Texture, std::less<>>& textures) override;
	void					handlePlayerInput(const sf::Keyboard::Key& key, const bool& isPressed) override;
	bool					isVertical() const;

private:
	sf::RectangleShape	wShape;
	const bool			vertical;
};