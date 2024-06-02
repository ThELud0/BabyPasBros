#include "Entity.hpp"
#pragma once

class Wall : public Entity {
public:
	explicit Wall(float x, float y, int height, int width, const sf::Texture& initTexture, const bool &vertical);
	explicit Wall(const pugi::xml_node& node);
	void setTexture(std::map<std::string, const sf::Texture>& textures) override;
	void drawCurrent(sf::RenderWindow& window) const override;
	sf::Vector2f getPos() override;
	sf::Vector2f getSiz() override;

	bool isVertical();
private:
	sf::RectangleShape wShape;
	const bool		   vertical;
};