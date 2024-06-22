#include "InteractiveUnmoving.hpp"
using namespace std::literals;

/// <summary>
/// Initialise la porte à une position (x,y) avec une taille (width,height).
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="height"></param>
/// <param name="width"></param>
InteractiveUnmoving::InteractiveUnmoving(float x, float y, int height, int width, const bool& vertical) : UnmovingEntity{ x, y, height, width, vertical} {
	mText.setSize(sf::Vector2f(static_cast<float>(width * 1.5), static_cast<float>(height) / 6.5));
	mText.setPosition(x + (static_cast<float>(width) - static_cast<float>(width * 1.5)) / 2, y + static_cast<float>(height) / 3.f);
}

/// <summary>
/// Initialise la porte à une position (x,y) avec une taille (width,height),
/// ces paramètres étant récupérés à partir d'un node xml.
/// </summary>
/// <param name="node"></param>
InteractiveUnmoving::InteractiveUnmoving(const pugi::xml_node& node) : UnmovingEntity{ node } {
	mText.setSize(sf::Vector2f(static_cast<float>(width * 1.5), static_cast<float>(height) / 6.5));
	mText.setPosition(x + (static_cast<float>(width) - static_cast<float>(width * 1.5)) / 2, y + static_cast<float>(height) / 3.f);
}

/// <summary>
/// dessine la porte
/// </summary>
/// <param name="window"></param>
void InteractiveUnmoving::drawCurrent(sf::RenderWindow& window) const {
	window.draw(wShape);
	//si la porte est fermée et le joueur proche de la porte, le texte pour l'ouvrir s'affiche
	if (isNear) {
		window.draw(mText);
	}
}
