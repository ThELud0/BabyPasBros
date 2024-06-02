#include "Wall.hpp"
using namespace std::literals;

Wall::Wall(float x, float y, int height, int width, const sf::Texture& initTexture, const bool &vertical) : Entity{ x, y, height, width }, vertical(vertical) {
	wShape.setPosition(x, y);
	wShape.setSize(sf::Vector2f(width, height));
	//wShape.setTexture(&initTexture);
	//wShape.setOutlineThickness(2);
}

Wall::Wall(const pugi::xml_node& node) : Entity{ node },
										vertical(node.attribute("vertical").as_bool()) {
	wShape.setPosition(x, y);
	wShape.setSize(sf::Vector2f(static_cast<float>(width), static_cast<float>(height)));
}

void Wall::drawCurrent(sf::RenderWindow& window) const {
	window.draw(wShape);
}

void Wall::setTexture(std::map<std::string, const sf::Texture>& textures) {
	for (auto& pair : textures) {
		if ((pair.first == "groundCloud")&&(!vertical)) {
			wShape.setTexture(&pair.second);
		}
		else if ((pair.first == "flippedCloud")&&(vertical)) {
			wShape.setTexture(&pair.second);
		}
	}
}

sf::Vector2f Wall::getPos() {
	return wShape.getPosition();
}


sf::Vector2f Wall::getSiz() {
	return wShape.getSize();
}

bool Wall::isVertical() {
	return vertical;
}