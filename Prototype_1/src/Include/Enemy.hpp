#include "Entity.hpp"
#include "Wall.hpp"
#pragma once

class Enemy : public Entity {
    public:
    explicit Enemy(float x, float y, int height, int width);
    explicit Enemy(const pugi::xml_node& node);
    void                    setTexture(std::map<std::string, const sf::Texture, std::less<>>& textures) override;
    void                    setSoundBuffer(std::map<std::string, const sf::SoundBuffer, std::less<>>& soundBuffers) override;
    void					drawCurrent(sf::RenderWindow& window) const override;
    void					handlePlayerInput(const sf::Keyboard::Key& key, const bool& isPressed) override;
    void					update(const sf::Time& elapsedTime, sf::View& view, std::map<std::string, const sf::Texture, std::less<>>& textures) override;
    sf::Sprite              getSelf() const;
    void                    collide(sf::Vector2f wallPos, sf::Vector2f wallSize, const sf::Time& elapsedTime, bool physical) override;
    sf::Vector2f            getPos() override;
    sf::Vector2f            getSiz() override;
};