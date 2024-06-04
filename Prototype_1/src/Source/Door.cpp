#include "Door.hpp"

using namespace std::literals;

Door::Door(float x, float y, int height, int width, const bool& vertical) : Entity{ x, y, height, width }, vertical(vertical) {
	wShape.setPosition(x, y);
	wShape.setSize(sf::Vector2f(width, height));

}

Door::Door(const pugi::xml_node& node) : Entity{ node },
vertical(node.attribute("vertical").as_bool()) {
	wShape.setPosition(x, y);
	wShape.setSize(sf::Vector2f(static_cast<float>(width), static_cast<float>(height)));
}

void Door::drawCurrent(sf::RenderWindow& window) const {
	window.draw(wShape);

	if ((isNear) && (physical)) {
		std::cout << "writing\n"; 
		window.draw(dText);
	}

}

void Door::setTexture(std::map<std::string, const sf::Texture>& textures) {
	for (auto& pair : textures) {
		if ((pair.first == "closedDoor") && (vertical)) {
			wShape.setTexture(&pair.second);
		}
		else if ((pair.first == "flippedClosedDoor") && (!vertical)) {
			wShape.setTexture(&pair.second);
		}
	}
	dText.setCharacterSize(60);
	dText.setString("Click \"E\"\n to open\n");
	dText.setOrigin(dText.getGlobalBounds().getSize().x / 2, dText.getGlobalBounds().getSize().y / 2);
	dText.setPosition(x + width / 2, y + height / 2);

}

sf::Vector2f Door::getPos() {
	return wShape.getPosition();
}


sf::Vector2f Door::getSiz() {
	return wShape.getSize();
}

bool Door::isVertical() const {
	return vertical;
}

/// <summary>
/// Player::collide s'occupe déjà de la vraie collision, Door::collide() sert donc à voir si le joueur est proche 
/// de la porte pour intéragir avec.
/// </summary>
/// <param name="mcPos"></param>
/// <param name="mcSize"></param>
/// <param name="elapsedTime"></param>
/// <param name="physical"></param>
void Door::collide(sf::Vector2f mcPos, sf::Vector2f mcSize, const sf::Time& elapsedTime, bool physical) {
	//on récupère la position actuelle et taille du joueur 
	float xPlayer = mcPos.x;
	float yPlayer = mcPos.y;
	int widPlayer = mcSize.x;
	int heiPlayer = mcSize.y;

	float xWall = x;
	float yWall = y;
	int widWall = width;
	int heiWall = height;

	//joueur au dessus de la porte
	if ((yPlayer < yWall) && (xPlayer + widPlayer > xWall) && (xPlayer < xWall + widWall)
		&&(yPlayer + static_cast<float>(heiPlayer)  >= yWall)) {
		isNear = true;
	}

	//joueur en dessous de la porte
	if ((yPlayer + static_cast<float>(heiPlayer) > yWall + heiWall) && (xPlayer + widPlayer > xWall) 
		&& (xPlayer < xWall + widWall)&& (yPlayer <= yWall + heiWall)) {
		isNear = true;
	}

	//joueur à gauche de la porte
	if ((xPlayer < xWall) && (yPlayer < yWall + heiWall) 
		&& (yPlayer + height > yWall) && (xPlayer + width >= xWall)) {
		isNear = true;
	}
	//joueur à droite du mur
	if ((xPlayer + width > xWall + widWall) && (yPlayer < yWall + heiWall) 
		&& (yPlayer + height > yWall) && (xPlayer <= xWall + widWall)) {
		isNear = true;
	}
	

}

void Door::update(const sf::Time& elapsedTime, sf::View& view, std::map<std::string, const sf::Texture>& textures) {

	for (auto& pair : textures) {
		if (physical) {
			if ((pair.first == "closedDoor") && (vertical)) {
				wShape.setTexture(&pair.second);
			}
			else if ((pair.first == "flippedClosedDoor") && (!vertical)) {
				wShape.setTexture(&pair.second);
			}
		}
		else if (!physical) {
			if ((pair.first == "openedDoor") && (vertical)) {
				wShape.setTexture(&pair.second);
			}
			else if ((pair.first == "flippedOpenedDoor") && (!vertical)) {
				wShape.setTexture(&pair.second);
			}
		}
	}
	isNear = false;
}

void Door::handlePlayerInput(const sf::Keyboard::Key& key, const bool& isPressed) {
	if ((key == sf::Keyboard::E)&&(physical)&&(isNear)&&(isPressed)){
		physical = false;
	}
}