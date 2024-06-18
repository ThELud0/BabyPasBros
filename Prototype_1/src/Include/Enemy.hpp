#include "Entity.hpp"
//#include "Player.hpp"
//#include "Wall.hpp"
#pragma once

class Enemy : public Entity {
public:
	explicit		Enemy(float x, float y, int height, int width);
	explicit		Enemy(const pugi::xml_node& node);
	//void			setTexture(std::map<std::string, const sf::Texture, std::less<>>& textures) override;
	//void            setSoundBuffer(std::map<std::string, const sf::SoundBuffer, std::less<>>& soundBuffers) override;
	//void			drawCurrent(sf::RenderWindow& window) const override;
	//sf::Vector2f	getPos() override;
	//sf::Vector2f	getSiz() override;
	//void			collide(sf::Vector2f mcPos, sf::Vector2f mcSize, const sf::Time& elapsedTime, bool physical) override;
	//void			update(const sf::Time& elapsedTime, sf::View& view, std::map<std::string, const sf::Texture, std::less<>>& textures) override;
	//void			handlePlayerInput(const sf::Keyboard::Key& key, const bool& isPressed) override;
//
//private:
//	sf::RectangleShape	wShape;
//	sf::Sound			mEnemySound;
//	bool				isNear{ false };

};
