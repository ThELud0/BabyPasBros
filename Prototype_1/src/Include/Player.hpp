#include "SFML/Graphics.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>



class Player {
public:
    Player(float radius, sf::Color color, float x, float y);
    void					drawCurrent(sf::RenderWindow& window) const;
    void					handlePlayerInput(const sf::Keyboard::Key& key, const bool& isPressed);
    void					update(const sf::Time& elapsedTime, sf::View& view);
    sf::CircleShape         getShape();

private:
    float                   PlayerSpeed = 100.f;
    sf::Vector2f            mSpeed;
    sf::CircleShape			mShape;
    bool					mIsMovingUp{ false };
    bool					mIsMovingDown{ false };
    bool					mIsMovingRight{ false };
    bool					mIsMovingLeft{ false };
};