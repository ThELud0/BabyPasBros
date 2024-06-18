#include "Wall.hpp"
using namespace std::literals;

/// <summary>
/// Initialise le mur � une position (x,y) avec une taille (width,height).
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="height"></param>
/// <param name="width"></param>
Wall::Wall(float x, float y, int height, int width, const bool &vertical) : Entity{ x, y, height, width }, vertical(vertical) {
	wShape.setPosition(x, y);
	wShape.setSize(sf::Vector2f(static_cast<float>(width), static_cast<float>(height)));
}

/// <summary>
/// Initialise le mur � une position (x,y) avec une taille (width,height),
/// ces param�tres �tant r�cup�r�s � partir d'un node xml.
/// </summary>
/// <param name="node"></param>
Wall::Wall(const pugi::xml_node& node) : Entity{ node },
										vertical(node.attribute("vertical").as_bool()) {
	wShape.setPosition(x, y);
	wShape.setSize(sf::Vector2f(static_cast<float>(width), static_cast<float>(height)));
}


void Wall::drawCurrent(sf::RenderWindow& window) const {
	window.draw(wShape);
}

/// <summary>
/// Permet de donner la texture initiale du mur lors de son apparition.
/// </summary>
/// <param name="textures"></param>
void Wall::setTexture(std::map<std::string, const sf::Texture, std::less<>>& textures) {
	for (auto const& [keyName,value] : textures) {
		if ((keyName == "groundCloud")&&(!vertical)) {
			wShape.setTexture(&value);
		}
		else if ((keyName == "flippedCloud")&&vertical) {
			wShape.setTexture(&value);
		}
	}
}

/// <summary>
/// Permet de charger les sons initiaux du mur lors de son apparition (aucuns).
/// </summary>
/// <param name="soundBuffers"></param>
void Wall::setSoundBuffer(std::map<std::string, const sf::SoundBuffer, std::less<>>& soundBuffers) {
}

/// <summary>
/// retourne la position (x,y) du sprite du mur
/// </summary>
/// <returns></returns>
sf::Vector2f Wall::getPos() {
	return wShape.getPosition();
}

/// <summary>
/// retourne la taille (width,height) du sprite du mur
/// </summary>
/// <returns></returns>
sf::Vector2f Wall::getSiz() {
	return wShape.getSize();
}

/// <summary>
/// retourne true si le mur est verticale, false sinon
/// </summary>
/// <returns></returns>
bool Wall::isVertical() const{
	return vertical;
}

/// <summary>
/// permet de g�rer les collisions avec d'autres Entity
/// </summary>
/// <param name="wallPos"></param>
/// <param name="wallSize"></param>
/// <param name="elapsedTime"></param>
void Wall::collide(sf::Vector2f wallPos, sf::Vector2f wallSize, const sf::Time& elapsedTime, bool physical) {
	//Pas d'interactions
}

/// <summary>
/// Met � jour le mur selon les informations disponibles
/// </summary>
/// <param name="elapsedTime"></param>
/// <param name="view"></param>
/// <param name="babyTextures"></param>
void Wall::update(const sf::Time& elapsedTime, sf::View& view, std::map<std::string, const sf::Texture, std::less<>>& textures) {
	//Pas de changements
}

/// <summary>
/// G�re les �v�nements que doit traiter le mur
/// </summary>
/// <param name="key"></param>
/// <param name="isPressed"></param>
void Wall::handlePlayerInput(const sf::Keyboard::Key & key, const bool& isPressed) {
	// Pas d'�v�nements
}