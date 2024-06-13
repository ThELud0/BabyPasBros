#include "Group.hpp"
using namespace std::literals;


/// <summary>
/// lecture d'un node xml repr�sentant un niveau dont le nom est donn� dans un label, qui sera utilis� pour nommer la fen�tre 
/// de jeu du-dit niveau
/// les �l�ments du niveau sont initialis�s dans un groupe constitu� d'un joueur MainCharacter
/// et (pour l'instant) d'une liste de murs Wall.
/// </summary>
/// <param name="node"> le node contenant les informations du niveau � stocker dans le groupe</param>
Group::Group(const pugi::xml_node& node) : Entity{ 0,0,0,0 }, windowName(node.attribute("label").as_string()) {
	for (auto const& child : node.children()) {
		if (child.name() == "Player"sv) {
			mainCharacter = std::make_unique<Player>(child);
		}
		
		else if (child.name() == "Wall"sv) {
			auto s = std::make_unique<Wall>(child);
			children.push_back(std::move(s));
		}

		else if (child.name() == "Door"sv) {
			auto s = std::make_unique<Door>(child);
			children.push_back(std::move(s));
		}
		
	}
}


/// <summary>
/// Les objets du groupe prennent chacun leur texture parmi celles disponibles, 
/// qui sont initialis�es et stock�es dans une map nomm�e textures.
/// La texture choisie pour chaque objet est d�finie selon une r�gle dans la fonction setTexture de leur classe.
/// </summary>
/// <param name="textures"></param>
void Group::setTexture(std::map<std::string, const sf::Texture, std::less<>>& textures) {
	mainCharacter->setTexture(textures);
	for (auto& entity : children) {
		entity->setTexture(textures);
	}
	
}

/// <summary>
/// Appelle la fonction update du mainCharacter et des �l�ments du groupe.
/// </summary>
/// <param name="elapsedTime"></param>
/// <param name="view"></param>
/// <param name="textures"></param>
void Group::update(const sf::Time& elapsedTime, sf::View& view, std::map<std::string, const sf::Texture, std::less<>>& textures) {
	mainCharacter->update(elapsedTime, view, textures);
	for (auto& entity : children) {
		entity->update(elapsedTime, view, textures);
	}
	
}

void Group::collide(sf::Vector2f mcPos, sf::Vector2f mcSize, const sf::Time& elapsedTime, bool physical) {
	for (auto& entity : children) {
		mainCharacter->collide(entity->getPos(), entity->getSiz(), elapsedTime, entity->getPhysicalState());
		entity->collide(mcPos, mcSize, elapsedTime, physical);
	}
}

/// <summary>
/// Dessine tous les �l�ments du groupe.
/// </summary>
/// <param name="window"></param>
void Group::drawCurrent(sf::RenderWindow& window) const {
	
	for (auto const& entity : children) {
		entity->drawCurrent(window);
	}
	mainCharacter->drawCurrent(window);
}

/// <summary>
/// Retourne le nom du niveau que les �l�ments du groupe constituent.
/// </summary>
/// <returns></returns>
std::string Group::returnName() const {
	return windowName;
}

sf::Vector2f Group::getPos() {
	return mainCharacter -> getPos();
}

sf::Vector2f Group::getSiz() {
	return mainCharacter->getSiz();
}

/// <summary>
/// Transmet les �v�nement aux �l�ments individuels du groupe pour traitement.
/// </summary>
/// <param name="key"></param>
/// <param name="isPressed"></param>
void Group::handlePlayerInput(const sf::Keyboard::Key& key, const bool& isPressed) {
	mainCharacter->handlePlayerInput(key, isPressed);
	for (auto const& entity : children) {
		entity->handlePlayerInput(key,isPressed);
	}
}

void Group::dragTowards(sf::Vector2f targetPos) {
	mainCharacter->dragTowards(targetPos);
}