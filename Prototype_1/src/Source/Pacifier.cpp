#include "Pacifier.hpp"

using namespace std::literals;

/// <summary>
/// Initialise la t�tine � une position (x,y) avec une taille (width,height).
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
/// Initialise la t�tine � une position (x,y) avec une taille (width,height),
/// ces param�tres �tant r�cup�r�s � partir d'un node xml.
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
/// dessine la t�tine
/// </summary>
/// <param name="window"></param>
void Pacifier::drawCurrent(sf::RenderWindow& window) const {
	window.draw(wShape);
	//si le joueur est proche de la t�tine, le texte pour paasser au niveau suivant s'affiche
	if (isNear) {
		window.draw(pacifierText);
	}

}

/// <summary>
/// Permet de donner la texture initiale � la t�tine lors de son apparition.
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
/// Permet de charger les sons initiaux de la t�tine lors de son apparition.
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
/// retourne la position (x,y) du sprite de la t�tine
/// </summary>
/// <returns></returns>
sf::Vector2f Pacifier::getPos() {
	return wShape.getPosition();
}

/// <summary>
/// retourne la taille (width,height) du sprite de la t�tine
/// </summary>
/// <returns></returns>
sf::Vector2f Pacifier::getSiz() {
	return wShape.getSize();
}

/// <summary>
/// retourne un bool�en valant true si le joueur peut passer au niveau suivant
/// </summary>
/// <returns></returns>
bool Pacifier::passLevel() const {
	return nextLevel;
}

/// <summary>
/// Player::collide s'occupe d�j� de la vraie collision, Pacifier::collide() sert donc � voir si le joueur est au niveau  
/// de la t�tine pour int�ragir avec.
/// </summary>
/// <param name="mcPos"></param>
/// <param name="mcSize"></param>
/// <param name="elapsedTime"></param>
/// <param name="physical"></param>
void Pacifier::collide(sf::Vector2f mcPos, sf::Vector2f mcSize, const sf::Time& elapsedTime, bool physical) {
	//on r�cup�re la position actuelle et taille du joueur 
	float xPlayer = mcPos.x;
	float yPlayer = mcPos.y;
	float widPlayer = mcSize.x;
	float heiPlayer = mcSize.y;

	float xPacifier = x;
	float yPacifier = y;
	auto widPacifier = static_cast<float>(width);
	auto heiPacifier = static_cast<float>(height);

	//joueur intersecte la t�tine
	if ((yPlayer < yPacifier + heiPacifier) && (xPlayer + widPlayer > xPacifier) && (xPlayer < xPacifier + widPacifier)
		&& (yPlayer + heiPlayer  > yPacifier)) {
		isNear = true;
	}
}

/// <summary>
/// Met � jour la t�tine selon les informations disponibles
/// </summary>
/// <param name="elapsedTime"></param>
/// <param name="view"></param>
/// <param name="babyTextures"></param>
void Pacifier::update(const sf::Time& elapsedTime, sf::View& view, std::map<std::string, const sf::Texture, std::less<>>& textures) {
	//on reset isNear � false, update() la mettra � true s'il le faut
	isNear = false;
}

/// <summary>
/// G�re les �v�nements que doit traiter la t�tine
/// </summary>
/// <param name="key"></param>
/// <param name="isPressed"></param>
void Pacifier::handlePlayerInput(const sf::Keyboard::Key& key, const bool& isPressed) {
	//si le joueur est proche et qu'il a cliqu� sur la touche indiqu�e, on passe au niveau suivant
	if ((key == sf::Keyboard::E) && isNear && isPressed) {
		nextLevel = true;
		mPacifierSound.play();
	}
}