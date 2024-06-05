#include "Entity.hpp"
#include "Wall.hpp"
#pragma once

class Player : public Entity {
public:
    explicit Player(float x, float y, int height, int width);
    explicit Player(const pugi::xml_node& node);
    void                    setTexture(std::map<std::string, const sf::Texture> &textures) override;
    void					drawCurrent(sf::RenderWindow& window) const override;
    void					handlePlayerInput(const sf::Keyboard::Key& key, const bool& isPressed) override;
    void					update(const sf::Time& elapsedTime, sf::View& view, std::map<std::string, const sf::Texture>& babyTextures) override;
    sf::Sprite              getSelf();
    void                    collide(sf::Vector2f wallPos, sf::Vector2f wallSize, const sf::Time& elapsedTime, bool physical) override;
    sf::Vector2f            getPos() override;
    sf::Vector2f            getSiz() override;
    void                    dragTowards(sf::Vector2f targetPos);

private:
    float                   PlayerSpeed = 200.f;
    float                   acceleration = 0.f;
    float                   maxGravity = 1000.f;
    sf::Sprite			    mChar;
    sf::Vector2f            dragMovement{ 0.f, 0.f };
    bool					mIsMovingUp{ false };
    bool					mIsMovingDown{ false };
    bool					mIsMovingRight{ false };
    bool					mIsMovingLeft{ false };
    bool                    collideUp{ false };
    bool                    collideDown{ false };
    bool                    collideRight{ false };
    bool                    collideLeft{ false };
    bool                    dragging{ false };
};