#include "Entity.hpp"
#pragma once

class UnmovingEntity : public Entity {
public:
	explicit				UnmovingEntity(float x, float y, int height, int width, const bool& vertical);
	explicit				UnmovingEntity(const pugi::xml_node& node);
	virtual void			setTexture(std::map<std::string, const sf::Texture, std::less<>>& textures) = 0;
	virtual void			setSoundBuffer(std::map<std::string, const sf::SoundBuffer, std::less<>>& soundBuffers) = 0;
	void					drawCurrent(sf::RenderWindow& window) const;
	sf::Vector2f			getPos() override;
	sf::Vector2f			getSiz() override;
	virtual void			collide(sf::Vector2f wallPos, sf::Vector2f wallSize, const sf::Time& elapsedTime, bool physical) = 0;
	virtual void			update(const sf::Time& elapsedTime, sf::View& view, std::map<std::string, const sf::Texture, std::less<>>& textures) = 0;
	virtual void			handlePlayerInput(const sf::Keyboard::Key& key, const bool& isPressed) = 0;
	bool					isVertical() const;

	sf::RectangleShape	wShape;
	const bool			vertical;
};
