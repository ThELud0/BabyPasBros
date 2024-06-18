#ifndef SIMPLE_GAME_ROUND_TARGET_H
#define SIMPLE_GAME_ROUND_TARGET_H

#include "SFML/Graphics.hpp"

enum class RoundTargetStatus { Alive, Dying, Dead };

class RoundTarget {
public:
    RoundTarget(float radius, sf::Color color, float x, float y, float xspeed, float yspeed);
    void					drawCurrent(sf::RenderWindow &window) const;
    void					handlePlayerInput(const sf::Keyboard::Key &key, const bool &isPressed);
    void					update(const sf::Time &elapsedTime, const sf::View &view);
    sf::CircleShape         getShape() const;
    bool                    isHitByMouse(const sf::Vector2f &mousePosition) const;
    enum RoundTargetStatus  getStatus() const;
    void                    setStatus(RoundTargetStatus stat);
    void                    isDying();

private:
    enum RoundTargetStatus  status;
    sf::Vector2f            mSpeed;
    sf::CircleShape			mShape;
    float                   ogRadius;
    int                     deathCountdown=60;
};

#endif SIMPLE_GAME_ROUND_TARGET_H