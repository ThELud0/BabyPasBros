#include "Wall.hpp"
using namespace std::literals;

/// <summary>
/// Initialise le mur à une position (x,y) avec une taille (width,height).
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="height"></param>
/// <param name="width"></param>
Wall::Wall(float x, float y, int height, int width, const bool &vertical) : UnmovingEntity{ x, y, height, width, vertical } {}

/// <summary>
/// Initialise le mur à une position (x,y) avec une taille (width,height),
/// ces paramètres étant récupérés à partir d'un node xml.
/// </summary>
/// <param name="node"></param>
Wall::Wall(const pugi::xml_node& node) : UnmovingEntity{ node } {}



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

