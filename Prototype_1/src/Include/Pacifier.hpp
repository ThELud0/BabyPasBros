#include "InteractiveUnmoving.hpp"

#pragma once

class Pacifier : public InteractiveUnmoving {
public:
	explicit		Pacifier(float x, float y, int height, int width, bool& vertical);
	explicit		Pacifier(const pugi::xml_node& node);
	void			setTexture(std::map<std::string, const sf::Texture, std::less<>>& textures) override;
	void            setSoundBuffer(std::map<std::string, const sf::SoundBuffer, std::less<>>& soundBuffers) override;

	void			collide(sf::Vector2f mcPos, sf::Vector2f mcSize, const sf::Time& elapsedTime, bool physical) override;
	void			update(const sf::Time& elapsedTime, sf::View& view, std::map<std::string, const sf::Texture, std::less<>>& textures) override;
	void			handlePlayerInput(const sf::Keyboard::Key& key, const bool& isPressed) override;

	bool			passLevel() const;

private:
	bool				nextLevel{ false };
};