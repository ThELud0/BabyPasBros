#include "Player.hpp"

Player::Player(float radius, sf::Color color, float x, float y) {
	mShape.setRadius(radius);
	mShape.setFillColor(color);
	mShape.setPosition(x, y);

}
void Player::drawCurrent(sf::RenderWindow& window) const {
	window.draw(mShape);
}

void Player::handlePlayerInput(const sf::Keyboard::Key& key, const bool& isPressed) {
	if (key == sf::Keyboard::Z)
		mIsMovingUp = isPressed;
	else if (key == sf::Keyboard::S)
		mIsMovingDown = isPressed;
	else if (key == sf::Keyboard::Q)
		mIsMovingLeft = isPressed;
	else if (key == sf::Keyboard::D)
		mIsMovingRight = isPressed;
}
void	Player::update(const sf::Time& elapsedTime, sf::View& view) {
	sf::Vector2f movement(0.f, 0.f);
	if (mIsMovingUp)
		movement.y -= PlayerSpeed;
	if (mIsMovingDown)
		movement.y += PlayerSpeed;
	if (mIsMovingLeft)
		movement.x -= PlayerSpeed;
	if (mIsMovingRight)
		movement.x += PlayerSpeed;

	mShape.move(movement * elapsedTime.asSeconds());
	if ((mShape.getPosition().x < mShape.getRadius() * 2) || (mShape.getPosition().x > view.getSize().x - mShape.getRadius() * 4)) {

		view.move(movement.x * elapsedTime.asSeconds(),0.f);
	}
	if ((mShape.getPosition().y > view.getSize().y - mShape.getRadius() * 4) || (mShape.getPosition().y < mShape.getRadius() * 2)) {

		view.move(0.f, movement.y * elapsedTime.asSeconds());
	}
}

sf::CircleShape Player::getShape() {
	return mShape;
}