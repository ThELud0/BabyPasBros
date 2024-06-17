#include "Entity.hpp"
#include "Wall.hpp"
#pragma once

class Player : public Entity {
public:
    explicit Player(float x, float y, int height, int width);
    explicit Player(const pugi::xml_node& node);
    void                    setTexture(std::map<std::string, const sf::Texture, std::less<>>& textures) override;
    void                    setSoundBuffer(std::map<std::string, const sf::SoundBuffer, std::less<>>& soundBuffers) override;
    void					drawCurrent(sf::RenderWindow& window) const override;
    void					handlePlayerInput(const sf::Keyboard::Key& key, const bool& isPressed) override;
    void					update(const sf::Time& elapsedTime, sf::View& view, std::map<std::string, const sf::Texture, std::less<>>& babyTextures) override;
    sf::Sprite              getSelf() const;
    void                    collide(sf::Vector2f wallPos, sf::Vector2f wallSize, const sf::Time& elapsedTime, bool physical) override;
    sf::Vector2f            getPos() override;
    sf::Vector2f            getSiz() override;
    void                    dragTowards(sf::Vector2f targetPos);

private:
    float                   PlayerSpeed = 200.f;
    float                   acceleration = 0.f;
    float                   maxGravity;
    sf::Sprite			    mChar;
    sf::Sound               mJumpSound;
    sf::Sound               mWalkSound1;
    sf::Sound               mWalkSound2;
    sf::Sound               mWalkSound3;
    sf::Sound               mHitSound;
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
    void                    playWalkSound();
    sf::Clock               clock;
};