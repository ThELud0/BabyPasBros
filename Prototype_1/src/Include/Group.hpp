#include "Entity.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Wall.hpp"
#include "Door.hpp"
#include "Pacifier.hpp"
#pragma once

class Group : public Entity {
public:
	explicit		Group(const pugi::xml_node& node);

	void			setTexture(std::map<std::string, const sf::Texture, std::less<>>& textures) override;
	void            setSoundBuffer(std::map<std::string, const sf::SoundBuffer, std::less<>>& soundBuffers) override;
	void			drawCurrent(sf::RenderWindow& window) const override;
	void			update(const sf::Time& elapsedTime, sf::View& view, std::map<std::string, const sf::Texture, std::less<>>& textures) override;
	void			handlePlayerInput(const sf::Keyboard::Key& key, const bool& isPressed) override ;
	void			collide(sf::Vector2f mcPos, sf::Vector2f mcSize, const sf::Time& elapsedTime, bool physical) override;
	void			dragTowards(sf::Vector2f targetPos);
	bool			nextLevel() const;
	sf::Vector2f	getPos() override;
	sf::Vector2f	getSiz() override;
	std::string		returnName() const;
private:
	std::string const						windowName;
	std::vector<std::unique_ptr<Entity>>	children;
	std::unique_ptr<Player>					mainCharacter;
	std::unique_ptr<Pacifier>				pacifier;
};