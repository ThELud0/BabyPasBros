#include "Pacifier.hpp"

using namespace std::literals;

/// <summary>
/// Initialise la tétine à une position (x,y) avec une taille (width,height).
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="height"></param>
/// <param name="width"></param>
Pacifier::Pacifier(float x, float y, int height, int width) : Entity{ x, y, height, width } {
	wShape.setPosition(x, y);
	wShape.setSize(sf::Vector2f(static_cast<float>(width), static_cast<float>(height)));
	pacifierText.setSize(sf::Vector2f(static_cast<float>(width * 1.5), static_cast<float>(width * 1.5) * 53.f / 912.f));
	pacifierText.setPosition(x - 0.25*width, y + static_cast<float>(height) + static_cast<float>(width) * 53.f / 912.f / 2);

}

/// <summary>
/// Initialise la tétine à une position (x,y) avec une taille (width,height),
/// ces paramètres étant récupérés à partir d'un node xml.
/// </summary>
/// <param name="node"></param>
Pacifier::Pacifier(const pugi::xml_node& node) : Entity{ node } {
	physical = false;
	wShape.setPosition(x, y);
	wShape.setSize(sf::Vector2f(static_cast<float>(width), static_cast<float>(height)));
	pacifierText.setSize(sf::Vector2f(static_cast<float>(width * 1.5), static_cast<float>(width * 1.5) * 53.f / 912.f));
	pacifierText.setPosition(x + (static_cast<float>(width) - static_cast<float>(width * 1.5)) / 2, y);
}

/// <summary>
/// dessine la tétine
/// </summary>
/// <param name="window"></param>
void Pacifier::drawCurrent(sf::RenderWindow& window) const {
	window.draw(wShape);
	//si le joueur est proche de la tétine, le texte pour paasser au niveau suivant s'affiche
	if (isNear) {
		window.draw(pacifierText);
	}

}

/// <summary>
/// Permet de donner la texture initiale à la tétine lors de son apparition.
/// </summary>
/// <param name="textures"></param>
void Pacifier::setTexture(std::map<std::string, const sf::Texture, std::less<>>& textures) {
	for (auto const& [keyName, value] : textures) {
		if (keyName == "babyPacifier") {
			wShape.setTexture(&value);
		}
		else if (keyName == "pacifierText") {
			pacifierText.setTexture(&value);
		}
	}
}

/// <summary>
/// Permet de charger les sons initiaux de la tétine lors de son apparition.
/// </summary>
/// <param name="soundBuffers"></param>
void Pacifier::setSoundBuffer(std::map<std::string, const sf::SoundBuffer, std::less<>>& soundBuffers) {
	for (auto const& [key, value] : soundBuffers) {
		if (key == "babydie"sv) {
			mPacifierSound.setBuffer(value);
		}
	}
	mPacifierSound.setVolume(50);
}

/// <summary>
/// retourne la position (x,y) du sprite de la tétine
/// </summary>
/// <returns></returns>
sf::Vector2f Pacifier::getPos() {
	return wShape.getPosition();
}

/// <summary>
/// retourne la taille (width,height) du sprite de la tétine
/// </summary>
/// <returns></returns>
sf::Vector2f Pacifier::getSiz() {
	return wShape.getSize();
}

/// <summary>
/// retourne un booléen valant true si le joueur peut passer au niveau suivant
/// </summary>
/// <returns></returns>
bool Pacifier::passLevel() const {
	return nextLevel;
}

/// <summary>
/// Player::collide s'occupe déjà de la vraie collision, Pacifier::collide() sert donc à voir si le joueur est au niveau  
/// de la tétine pour intéragir avec.
/// </summary>
/// <param name="mcPos"></param>
/// <param name="mcSize"></param>
/// <param name="elapsedTime"></param>
/// <param name="physical"></param>
void Pacifier::collide(sf::Vector2f mcPos, sf::Vector2f mcSize, const sf::Time& elapsedTime, bool physical) {
	//on récupère la position actuelle et taille du joueur 
	float xPlayer = mcPos.x;
	float yPlayer = mcPos.y;
	float widPlayer = mcSize.x;
	float heiPlayer = mcSize.y;

	float xPacifier = x;
	float yPacifier = y;
	auto widPacifier = static_cast<float>(width);
	auto heiPacifier = static_cast<float>(height);

	//joueur intersecte la tétine
	if ((yPlayer < yPacifier + heiPacifier) && (xPlayer + widPlayer > xPacifier) && (xPlayer < xPacifier + widPacifier)
		&& (yPlayer + heiPlayer  > yPacifier)) {
		isNear = true;
	}
}

/// <summary>
/// Met à jour la tétine selon les informations disponibles
/// </summary>
/// <param name="elapsedTime"></param>
/// <param name="view"></param>
/// <param name="babyTextures"></param>
void Pacifier::update(const sf::Time& elapsedTime, sf::View& view, std::map<std::string, const sf::Texture, std::less<>>& textures) {
	//on reset isNear à false, update() la mettra à true s'il le faut
	isNear = false;
}

/// <summary>
/// Gère les événements que doit traiter la tétine
/// </summary>
/// <param name="key"></param>
/// <param name="isPressed"></param>
void Pacifier::handlePlayerInput(const sf::Keyboard::Key& key, const bool& isPressed) {
	//si le joueur est proche et qu'il a cliqué sur la touche indiquée, on passe au niveau suivant
	if ((key == sf::Keyboard::E) && isNear && isPressed) {
		nextLevel = true;
		mPacifierSound.play();
	}
}