#include "Player.hpp"



Player::Player(float x, float y) : Entity(x,y){
	mChar.setPosition(x, y);
}

void Player::setTexture(const sf::Texture& initTexture) {
	mChar.setTexture(initTexture);
}
void Player::drawCurrent(sf::RenderWindow& window) const {
	window.draw(mChar);
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
void	Player::update(const sf::Time& elapsedTime, sf::View& view, std::map<std::string, sf::Texture>& babyTextures) {
	sf::Vector2f movement(0.f, 0.f);
	std::string key("");

	if (mIsMovingUp)
		movement.y -= PlayerSpeed;
	if (mIsMovingDown)
		movement.y += PlayerSpeed;
	if (mIsMovingLeft) {
		movement.x -= PlayerSpeed;
		key = "babyleft";
	}
	if (mIsMovingRight) {
		movement.x += PlayerSpeed;
		key = "babyright";
	}

	for (auto& pair : babyTextures) {
		if (pair.first == key)
			mChar.setTexture(pair.second);
	}

	mChar.move(movement * elapsedTime.asSeconds());
	if ((mChar.getPosition().x < mChar.getTextureRect().getSize().x) || (mChar.getPosition().x > view.getSize().x - mChar.getTextureRect().getSize().x * 2)) {

		view.move(movement.x * elapsedTime.asSeconds(),0.f);
	}
	if ((mChar.getPosition().y > view.getSize().y - mChar.getTextureRect().getSize().y * 2) || (mChar.getPosition().y < mChar.getTextureRect().getSize().y)) {

		view.move(0.f, movement.y * elapsedTime.asSeconds());
	}
}

sf::Sprite Player::getSelf() {
	return mChar;
}