#include "Door.hpp"

using namespace std::literals;

Door::Door(float x, float y, int height, int width, const bool& vertical) : Entity{ x, y, height, width }, vertical(vertical) {
	wShape.setPosition(x, y);
	wShape.setSize(sf::Vector2f(static_cast<float>(width), static_cast<float>(height)));
	doorText.setSize(sf::Vector2f(static_cast<float>(width), static_cast<float>(width) * 751.f / 81.f));
	doorText.setPosition(x, y + static_cast<float>(height) + static_cast<float>(width) * 751.f / 81.f / 2);
	
}

Door::Door(const pugi::xml_node& node) : Entity{ node },
vertical(node.attribute("vertical").as_bool()) {
	wShape.setPosition(x, y);
	wShape.setSize(sf::Vector2f(static_cast<float>(width), static_cast<float>(height)));
	doorText.setSize(sf::Vector2f(static_cast<float>(width*1.25), static_cast<float>(width*1.25) * 81.f / 751.f));
	doorText.setPosition(x + (static_cast<float>(width) - static_cast<float>(width * 1.25))/2, y + static_cast<float>(height)/ 2.f - static_cast<float>(width) * 81.f / 751.f );
}

void Door::drawCurrent(sf::RenderWindow& window) const {
	window.draw(wShape);
	if ((isNear) && (physical)) {
		window.draw(doorText);
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
		else if (pair.first == "openDoorText") {
			doorText.setTexture(&pair.second);
		}
	}

	
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
		&&(yPlayer + static_cast<float>(heiPlayer)*1.5  >= yWall)) {
		isNear = true;
	}

	//joueur en dessous de la porte
	if ((yPlayer + static_cast<float>(heiPlayer) > yWall + heiWall) && (xPlayer + widPlayer > xWall) 
		&& (xPlayer < xWall + widWall)&& (yPlayer - static_cast<float>(heiPlayer) * 0.25 <= yWall + heiWall)) {
		isNear = true;
	}

	//joueur à gauche de la porte
	if ((xPlayer < xWall) && (yPlayer <= yWall + heiWall) 
		&& (yPlayer + heiPlayer >= yWall) && (xPlayer + widPlayer*1.5 >= xWall)) {
		isNear = true;
	}
	//joueur à droite du mur
	if ((xPlayer + widPlayer > xWall + widWall) && (yPlayer <= yWall + heiWall) 
		&& (yPlayer + heiPlayer >= yWall) && (xPlayer - widPlayer*0.5 <= xWall + widWall)) {
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