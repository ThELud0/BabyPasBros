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
	//si la porte est fermée et le joueur proche de la porte, le texte pour l'ouvrir s'affiche
	if (isNear && physical) {
		window.draw(doorText);
	}

}

void Door::setTexture(std::map<std::string, const sf::Texture, std::less<>>& textures) {
	for (auto const& [keyName,value] : textures) {
		if ((keyName == "closedDoor") && vertical) {
			wShape.setTexture(&value);
		}
		else if ((keyName == "flippedClosedDoor") && (!vertical)) {
			wShape.setTexture(&value);
		}
		else if (keyName == "openDoorText") {
			doorText.setTexture(&value);
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
	float widPlayer = mcSize.x;
	float heiPlayer = mcSize.y;

	float xWall = x;
	float yWall = y;
	auto widWall = static_cast<float>(width);
	auto heiWall = static_cast<float>(height);

	//joueur au dessus de la porte
	if ((yPlayer < yWall) && (xPlayer + widPlayer > xWall) && (xPlayer < xWall + widWall)
		&&(yPlayer + heiPlayer*1.5  >= yWall)) {
		isNear = true;
	}

	//joueur en dessous de la porte
	if ((yPlayer + heiPlayer > yWall + heiWall) && (xPlayer + widPlayer > xWall) 
		&& (xPlayer < xWall + widWall)&& (yPlayer - heiPlayer * 0.25 <= yWall + heiWall)) {
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

void Door::update(const sf::Time& elapsedTime, sf::View& view, std::map<std::string, const sf::Texture, std::less<>>& textures) {

	for (auto const&  [keyName,value] : textures) {
		//si la porte est physique = non franchissable = fermée on montre la porte fermée
		if (physical) {
			if ((keyName == "closedDoor") && vertical) {
				wShape.setTexture(&value);
			}
			else if ((keyName == "flippedClosedDoor") && (!vertical)) {
				wShape.setTexture(&value);
			}
		}
		//sinon on montre la porte ouverte!
		else if (!physical) {
			if ((keyName == "openedDoor") && vertical) {
				wShape.setTexture(&value,true);
				wShape.setSize(sf::Vector2f(static_cast<float>(width) * static_cast<float>(1.5), static_cast<float>(height)));				
			}
			else if ((keyName == "flippedOpenedDoor") && (!vertical)) {
				wShape.setTexture(&value);
			}
		}
	}

	//on reset isNear à false, update() la mettra à true s'il le faut
	isNear = false;
}

void Door::handlePlayerInput(const sf::Keyboard::Key& key, const bool& isPressed) {
	//si le joueur est proche et qu'il a cliqué sur la touche indiquée, la porte s'ouvre
	if ((key == sf::Keyboard::E)&&physical&&isNear&&isPressed){
		physical = false;
	}
}