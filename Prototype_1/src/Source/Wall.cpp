#include "Wall.hpp"
using namespace std::literals;

Wall::Wall(float x, float y, int height, int width, const bool &vertical) : Entity{ x, y, height, width }, vertical(vertical) {
	wShape.setPosition(x, y);
	wShape.setSize(sf::Vector2f(width, height));

}

Wall::Wall(const pugi::xml_node& node) : Entity{ node },
										vertical(node.attribute("vertical").as_bool()) {
	wShape.setPosition(x, y);
	wShape.setSize(sf::Vector2f(static_cast<float>(width), static_cast<float>(height)));
}

void Wall::drawCurrent(sf::RenderWindow& window) const {
	window.draw(wShape);
}

void Wall::setTexture(std::map<std::string, const sf::Texture, std::less<>>& textures) {
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

bool Wall::isVertical() const{
	return vertical;
}


void Wall::collide(sf::Vector2f wallPos, sf::Vector2f wallSize, const sf::Time& elapsedTime, bool physical) {//No interactions
}

void Wall::update(const sf::Time& elapsedTime, sf::View& view, std::map<std::string, const sf::Texture, std::less<>>& textures) {//No interactions
}

void Wall::handlePlayerInput(const sf::Keyboard::Key & key, const bool& isPressed) {//No interactions 
}