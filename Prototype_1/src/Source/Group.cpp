#include "Group.hpp"
using namespace std::literals;

Group::Group(const pugi::xml_node& node) : windowName(node.attribute("label").as_string()){
	for (auto const& child : node.children()) {
		if (child.name() == "Player"sv) {
			mainCharacter = std::make_unique<Player>(child);
		}
		
		else if (child.name() == "Wall"sv) {
			auto s = std::make_unique<Wall>(child);
			children.push_back(std::move(s));
		}
		
	}
}

void Group::setTexture(std::map<std::string, const sf::Texture>& textures) {
	mainCharacter->setTexture(textures);
	for (auto& entity : children) {
		entity->setTexture(textures);
	}
}

void Group::update(const sf::Time& elapsedTime, sf::View& view, std::map<std::string, const sf::Texture>& textures) {
	mainCharacter->update(elapsedTime, view, textures);

	for (auto& entity : children) {
		mainCharacter->collide(entity->getPos(), entity->getSiz(), elapsedTime);
	}
}

void Group::drawCurrent(sf::RenderWindow& window) const {
	mainCharacter->drawCurrent(window);
	for (auto const& entity : children) {
		entity->drawCurrent(window);
	}
}

std::string Group::returnName() const {
	return windowName;
}

void Group::handlePlayerInput(const sf::Keyboard::Key& key, const bool& isPressed) {
	mainCharacter->handlePlayerInput(key, isPressed);
}