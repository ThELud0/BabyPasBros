#include "Door.hpp"
using namespace std::literals;

/// <summary>
/// Initialise la porte à une position (x,y) avec une taille (width,height).
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="height"></param>
/// <param name="width"></param>
Door::Door(float x, float y, int height, int width, const bool& vertical) : InteractibleUnmoving{ x, y, height, width, vertical} {
}

/// <summary>
/// Initialise la porte à une position (x,y) avec une taille (width,height),
/// ces paramètres étant récupérés à partir d'un node xml.
/// </summary>
/// <param name="node"></param>
Door::Door(const pugi::xml_node& node) : InteractibleUnmoving{ node } {}


/// <summary>
/// Permet de donner la texture initiale à la porte lors de son apparition.
/// </summary>
/// <param name="textures"></param>
void Door::setTexture(std::map<std::string, const sf::Texture, std::less<>>& textures) {
	for (auto const& [keyName,value] : textures) {
		if ((keyName == "closedDoor") && vertical) {
			wShape.setTexture(&value);
		}
		else if ((keyName == "flippedClosedDoor") && (!vertical)) {
			wShape.setTexture(&value);
		}
		else if (keyName == "openDoorText") {
			mText.setTexture(&value);
		}
	}
}

/// <summary>
/// Permet de charger les sons initiaux de la porte lors de son apparition.
/// </summary>
/// <param name="soundBuffers"></param>
void Door::setSoundBuffer(std::map<std::string, const sf::SoundBuffer, std::less<>>& soundBuffers) {
	for (auto const& [key, value] : soundBuffers) {
		if (key == "door"sv) {
			mSound.setBuffer(value);
		}
	}
	mSound.setVolume(50);
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

/// <summary>
/// Met à jour la porte selon les informations disponibles
/// </summary>
/// <param name="elapsedTime"></param>
/// <param name="view"></param>
/// <param name="babyTextures"></param>
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

/// <summary>
/// Gère les événements que doit traiter la porte
/// </summary>
/// <param name="key"></param>
/// <param name="isPressed"></param>
void Door::handlePlayerInput(const sf::Keyboard::Key& key, const bool& isPressed) {
	//si le joueur est proche et qu'il a cliqué sur la touche indiquée, la porte s'ouvre
	if ((key == sf::Keyboard::E)&&physical&&isNear&&isPressed){
		physical = false;
		mSound.play();
	}
}