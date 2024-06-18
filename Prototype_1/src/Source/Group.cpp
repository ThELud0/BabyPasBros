#include "Group.hpp"
using namespace std::literals;

/// <summary>
/// Lecture d'un node xml représentant un niveau dont le nom est donné dans un label, qui sera utilisé pour nommer la fenêtre 
/// de jeu du-dit niveau
/// les éléments du niveau sont initialisés dans un groupe constitué d'un joueur MainCharacter
/// et (pour l'instant) d'une liste de murs Wall et de portes Door.
/// </summary>
/// <param name="node"> le node contenant les informations du niveau à stocker dans le groupe</param>
Group::Group(const pugi::xml_node& node) : Entity{ 0,0,0,0 }, windowName(node.attribute("label").as_string()) {
	for (auto const& child : node.children()) {
		if (child.name() == "Player"sv) {
			mainCharacter = std::make_unique<Player>(child);
		} else if (child.name() == "Wall"sv) {
			auto s = std::make_unique<Wall>(child);
			children.push_back(std::move(s));
		} else if (child.name() == "Door"sv) {
			auto s = std::make_unique<Door>(child);
			children.push_back(std::move(s));
		} else if (child.name() == "Pacifier"sv) {
			pacifier = std::make_unique<Pacifier>(child);
		}
	}
}

/// <summary>
/// Les objets du groupe prennent chacun leur texture parmi celles disponibles, 
/// qui sont initialisées et stockées dans une map nommée textures.
/// La texture choisie pour chaque objet est définie selon une règle dans la fonction setTexture de leur classe.
/// </summary>
/// <param name="textures"></param>
void Group::setTexture(std::map<std::string, const sf::Texture, std::less<>>& textures) {
	mainCharacter->setTexture(textures);
	pacifier->setTexture(textures);
	for (auto const& entity : children) {
		entity->setTexture(textures);
	}	
}

/// <summary>
/// Les objets du groupe prennent chacun leur soundBuffer parmi ceux disponibles, 
/// stockées dans la table soundBuffers.
/// </summary>
/// <param name="soundBuffers"></param>
void Group::setSoundBuffer(std::map<std::string, const sf::SoundBuffer, std::less<>>& soundBuffers) {
	mainCharacter->setSoundBuffer(soundBuffers);
	pacifier->setSoundBuffer(soundBuffers);

	for (auto const& entity : children) {
		entity->setSoundBuffer(soundBuffers);
	}
}

/// <summary>
/// Appelle la fonction update du mainCharacter et des éléments du groupe.
/// </summary>
/// <param name="elapsedTime"></param>
/// <param name="view"></param>
/// <param name="textures"></param>
void Group::update(const sf::Time& elapsedTime, sf::View& view, std::map<std::string, const sf::Texture, std::less<>>& textures) {
	mainCharacter->update(elapsedTime, view, textures);
	pacifier->update(elapsedTime, view, textures);
	for (auto const& entity : children) {
		entity->update(elapsedTime, view, textures);
	}
}

/// <summary>
/// Appelle la fonction collide du mainCharacter et des éléments du groupe.
/// </summary>
/// <param name="mcPos"></param>
/// <param name="mcSize"></param>
/// <param name="elapsedTime"></param>
/// <param name="physical"></param>
void Group::collide(sf::Vector2f mcPos, sf::Vector2f mcSize, const sf::Time& elapsedTime, bool physical) {
	for (auto const& entity : children) {
		mainCharacter->collide(entity->getPos(), entity->getSiz(), elapsedTime, entity->getPhysicalState());
		pacifier->collide(mcPos, mcSize, elapsedTime, physical);
		entity->collide(mcPos, mcSize, elapsedTime, physical);
	}
}

/// <summary>
/// Dessine tous les éléments du groupe.
/// </summary>
/// <param name="window"></param>
void Group::drawCurrent(sf::RenderWindow& window) const {
	for (auto const& entity : children) {
		entity->drawCurrent(window);
	}
	pacifier->drawCurrent(window);
	mainCharacter->drawCurrent(window);
}

/// <summary>
/// Retourne le nom du niveau que les éléments du groupe constituent.
/// </summary>
/// <returns></returns>
std::string Group::returnName() const {
	return windowName;
}

/// <summary>
/// retourne la position (x,y) du joueur contenu dans le groupe
/// </summary>
/// <returns></returns>
sf::Vector2f Group::getPos() {
	return mainCharacter -> getPos();
}

/// <summary>
/// retourne la taille (width,height) du joueur contenu dans le groupe
/// </summary>
/// <returns></returns>
sf::Vector2f Group::getSiz() {
	return mainCharacter->getSiz();
}

/// <summary>
/// retourne un booléen valant true si le joueur peut passer au niveau suivant, false sinon
/// </summary>
/// <returns></returns>
bool Group::nextLevel() const {
	return pacifier->passLevel();
}

/// <summary>
/// Transmet les événement aux éléments individuels du groupe pour traitement.
/// </summary>
/// <param name="key"></param>
/// <param name="isPressed"></param>
void Group::handlePlayerInput(const sf::Keyboard::Key& key, const bool& isPressed) {
	mainCharacter->handlePlayerInput(key, isPressed);
	pacifier->handlePlayerInput(key, isPressed);
	for (auto const& entity : children) {
		entity->handlePlayerInput(key,isPressed);
	}
}

/// <summary>
/// Harponne le joueur vers le RoundTarget cliqué
/// </summary>
/// <param name="targetPos"></param>
void Group::dragTowards(sf::Vector2f targetPos) {
	mainCharacter->dragTowards(targetPos);
}