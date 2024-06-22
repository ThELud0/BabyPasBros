#include "UnmovingEntity.hpp"
using namespace std::literals;

/// <summary>
/// Initialise le mur à une position (x,y) avec une taille (width,height).
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="height"></param>
/// <param name="width"></param>
UnmovingEntity::UnmovingEntity(float x, float y, int height, int width, const bool& vertical) : Entity{ x, y, height, width }, vertical(vertical) {
	wShape.setPosition(x, y);
	wShape.setSize(sf::Vector2f(static_cast<float>(width), static_cast<float>(height)));
}

/// <summary>
/// Initialise le mur à une position (x,y) avec une taille (width,height),
/// ces paramètres étant récupérés à partir d'un node xml.
/// </summary>
/// <param name="node"></param>
UnmovingEntity::UnmovingEntity(const pugi::xml_node& node) : Entity{ node },
vertical(node.attribute("vertical").as_bool()) {
	wShape.setPosition(x, y);
	wShape.setSize(sf::Vector2f(static_cast<float>(width), static_cast<float>(height)));
}


void UnmovingEntity::drawCurrent(sf::RenderWindow& window) const {
	window.draw(wShape);
}

/// <summary>
/// retourne la position (x,y) du sprite du mur
/// </summary>
/// <returns></returns>
sf::Vector2f UnmovingEntity::getPos() {
	return wShape.getPosition();
}

/// <summary>
/// retourne la taille (width,height) du sprite du mur
/// </summary>
/// <returns></returns>
sf::Vector2f UnmovingEntity::getSiz() {
	return wShape.getSize();
}

/// <summary>
/// retourne true si le mur est verticale, false sinon
/// </summary>
/// <returns></returns>
bool UnmovingEntity::isVertical() const {
	return vertical;
}


