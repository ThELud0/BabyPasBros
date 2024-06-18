#include "RoundTarget.hpp"
#include <iostream>

/// <summary>
/// Initialise la cible de rayon radius et de couleur color à la position (x,y) avec une vitesse (xspeed, yspeed)
/// </summary>
/// <param name="radius"></param>
/// <param name="color"></param>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="xspeed"></param>
/// <param name="yspeed"></param>
RoundTarget::RoundTarget(float radius, sf::Color color, float x, float y, float xspeed, float yspeed)
{
    mShape.setRadius(radius);
    ogRadius = radius;
    mShape.setFillColor(color);
    mShape.setPosition(x,y);
    mSpeed.x = xspeed;
    mSpeed.y = yspeed;
    status = RoundTargetStatus::Alive;
}

/// <summary>
/// dessine la cible
/// </summary>
/// <param name="window"></param>
void RoundTarget::drawCurrent(sf::RenderWindow &window) const {
    window.draw(mShape);
}

/// <summary>
/// retourne le sprite qui représente la cible
/// </summary>
/// <returns></returns>
sf::CircleShape RoundTarget::getShape() const {
    return mShape;
}

/// <summary>
/// retourne le status de la cible
/// </summary>
/// <returns></returns>
RoundTargetStatus RoundTarget::getStatus() const {
    return status;
}

/// <summary>
/// met à jour le status de la cible
/// </summary>
void RoundTarget::setStatus(RoundTargetStatus stat) {
    status = stat;
}

/// <summary>
/// Met à jour la cible selon les informations disponibles
/// </summary>
/// <param name="elapsedTime"></param>
/// <param name="view"></param>
void RoundTarget::update(const sf::Time &elapsedTime, const sf::View &view) {
    if ((mShape.getPosition().x + mSpeed.x * elapsedTime.asSeconds() <= view.getCenter().x - view.getSize().x/2) || (mShape.getPosition().x + mSpeed.x * elapsedTime.asSeconds() >= view.getCenter().x + view.getSize().x / 2 - 2 * mShape.getRadius())) {
        mSpeed.x = -mSpeed.x;
        if (mShape.getPosition().x - view.getCenter().x > 0) {
            mShape.setPosition(view.getCenter().x + view.getSize().x / 2 - 2 * mShape.getRadius(), mShape.getPosition().y);
        }
        else if (mShape.getPosition().x - view.getCenter().x < 0) {
            mShape.setPosition(view.getCenter().x - view.getSize().x / 2, mShape.getPosition().y);
        }
    }

    if ((mShape.getPosition().y + mSpeed.y * elapsedTime.asSeconds() <= view.getCenter().y - view.getSize().y / 2) || (mShape.getPosition().y + mSpeed.y * elapsedTime.asSeconds() >= view.getCenter().y + view.getSize().y / 2 - 2 * mShape.getRadius())) {
        mSpeed.y = -mSpeed.y;
        if (mShape.getPosition().y - view.getCenter().y > 0) {
            mShape.setPosition(mShape.getPosition().x, view.getCenter().y + view.getSize().y / 2 - 2 * mShape.getRadius());
        }
        else if (mShape.getPosition().y - view.getCenter().y < 0) {
            mShape.setPosition(mShape.getPosition().x, view.getCenter().y - view.getSize().y / 2);
        }
    }
    sf::Vector2f movement(mSpeed);
    mShape.move(movement * elapsedTime.asSeconds());

    if (status == RoundTargetStatus::Dying) {
        //pour que la cible retrecisse vers son centre lorsqu'elle meurt
        movement = { ogRadius, ogRadius };
        mShape.move(movement * elapsedTime.asSeconds());

        deathCountdown--;
        if (deathCountdown == 0) {
            status = RoundTargetStatus::Dead;
        }
    }
}

/// <summary>
/// renvoie vrai si la souris touche la cible
/// </summary>
/// <param name="mousePosition"></param>
/// /// <returns></returns>
bool RoundTarget::isHitByMouse(const sf::Vector2f &mousePosition) const {
    sf::Vector2f targetCenter = { mShape.getPosition().x + mShape.getRadius(), mShape.getPosition().y + mShape.getRadius() };
    float distanceClickFromCenter = (targetCenter.x - mousePosition.x)*(targetCenter.x - mousePosition.x) + (targetCenter.y - mousePosition.y)*(targetCenter.y - mousePosition.y);
    return (distanceClickFromCenter <= mShape.getRadius() * mShape.getRadius());
}

/// <summary>
/// enclenche l'animation de mort de la cible
/// </summary>
void RoundTarget::isDying() {
    mShape.setRadius(mShape.getRadius() - ogRadius / 60.f);
    mShape.setFillColor(sf::Color::Red);
    mSpeed.x = 0.f;
    mSpeed.y = 0.f;
    return;
}
