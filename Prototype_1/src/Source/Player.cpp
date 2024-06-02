#include "Player.hpp"
using namespace std::literals;


Player::Player(float x, float y, int height, int width) : Entity{ x, y, height, width } {
	mChar.setPosition(x, y);
}

Player::Player(const pugi::xml_node& node) : Entity{node} {
	mChar.setPosition(x, y);
}

void Player::setTexture(std::map<std::string, const sf::Texture> &textures) {
	for (auto &pair : textures) {
		if (pair.first == "babyright"sv) {
			mChar.setTexture(pair.second);
		}
	}
	mChar.setTextureRect(sf::IntRect(0, 0, width, height));
}

void Player::drawCurrent(sf::RenderWindow& window) const {
	window.draw(mChar);
}

void Player::handlePlayerInput(const sf::Keyboard::Key& key, const bool& isPressed) {
 
	if ((key == sf::Keyboard::Z) && (collideDown)) {
		mIsMovingUp = true;
	}
	else if (key == sf::Keyboard::S)
		mIsMovingDown = isPressed;
	else if (key == sf::Keyboard::Q)
		mIsMovingLeft = isPressed;
	else if (key == sf::Keyboard::D)
		mIsMovingRight = isPressed;

}
void	Player::update(const sf::Time& elapsedTime, sf::View& view, std::map<std::string,const sf::Texture>& babyTextures) {
	sf::Vector2f movement(0.f, 0.f);
	std::string key("");

	if (!collideDown)
	{
		if (acceleration < maxGravity / 4)
			acceleration += 10.f;
		if ((acceleration >= maxGravity / 4) && (acceleration < maxGravity ))
			acceleration += 25.f;
		if ((mIsMovingDown) && (acceleration + 24.f < maxGravity))
			acceleration += 50.f;

		movement.y += acceleration;
	}
	
	if ((mIsMovingUp)&&(!collideUp))
		movement.y -= PlayerSpeed * 3;

	if ((mIsMovingLeft) && (!collideLeft)) {
		movement.x -= PlayerSpeed;
		key = "babyleft";
	}
	if ((mIsMovingRight) && (!collideRight)) {
		movement.x += PlayerSpeed;
		key = "babyright";
	}

	for (auto& pair : babyTextures) {
		if (pair.first == key) {
			mChar.setTexture(pair.second);
		}
	}

	mChar.move(movement * elapsedTime.asSeconds());
	
	if ((mChar.getPosition().x < view.getCenter().x - view.getSize().x/2 + mChar.getTextureRect().getSize().x) || (mChar.getPosition().x > view.getCenter().x + view.getSize().x/2 - mChar.getTextureRect().getSize().x * 2)) {

		view.move(movement.x * elapsedTime.asSeconds(),0.f);
		if ((mChar.getPosition().x < view.getCenter().x - view.getSize().x / 2) || (mChar.getPosition().x > view.getCenter().x + view.getSize().x / 2 - mChar.getTextureRect().getSize().x)) {
			view.move(mChar.getPosition().x - view.getCenter().x, 0.f);
		}

	}
	if ((mChar.getPosition().y > view.getCenter().y + view.getSize().y/2 - mChar.getTextureRect().getSize().y * 2) || (mChar.getPosition().y < view.getCenter().y - view.getSize().y/2 + mChar.getTextureRect().getSize().y)) {

		view.move(0.f, movement.y * elapsedTime.asSeconds());
		if ((mChar.getPosition().y > view.getCenter().y + view.getSize().y / 2 - mChar.getTextureRect().getSize().y) || (mChar.getPosition().y < view.getCenter().y - view.getSize().y / 2)) {
			view.move(0.f, mChar.getPosition().y - view.getCenter().y);
		}
	}

	collideUp = false;
	collideDown = false;
	collideLeft = false;
	collideRight = false;

}

sf::Sprite Player::getSelf() {
	return mChar;
}

sf::Vector2f Player::getPos() {
	return mChar.getPosition();
}

sf::Vector2f Player::getSiz() {
	return sf::Vector2f(width,height);
}

void Player::collide(sf::Vector2f wallPos, sf::Vector2f wallSize, const sf::Time& elapsedTime) {

	float xPlayer = mChar.getPosition().x;
	float yPlayer = mChar.getPosition().y;
	float xWall = wallPos.x;
	float yWall = wallPos.y;
	int widWall = wallSize.x;
	int heiWall = wallSize.y;

	//joueur au dessus du mur
	if ((yPlayer < yWall) && (xPlayer + width > xWall) && (xPlayer < xWall + widWall) ) {
		if (yPlayer + static_cast<float>(height) + (maxGravity - PlayerSpeed * 3) * elapsedTime.asSeconds() >= yWall + static_cast<float>(height) / 4) {
		
			collideDown = true;
			acceleration = 0;
			mIsMovingUp = false;
		}
	}
	//joueur en dessous du mur
	if ((yPlayer + static_cast<float>(height) > yWall + heiWall) && (xPlayer + width > xWall) && (xPlayer < xWall + widWall)) {
		if (yPlayer - PlayerSpeed * elapsedTime.asSeconds() <= yWall + heiWall - static_cast<float>(height) / 6) {
			collideUp = true;
		}
	}
	//joueur à gauche du mur
	if ((xPlayer < xWall) && (yPlayer < yWall + heiWall - static_cast<float>(height) / 6) && (yPlayer + height > yWall + static_cast<float>(height) / 4)) {
		if (xPlayer + width + PlayerSpeed * elapsedTime.asSeconds() >= xWall) {
			collideRight = true;
		}
	}
	//joueur à droite du mur
	if ((xPlayer + width > xWall + widWall) && (yPlayer < yWall + heiWall - static_cast<float>(height) / 6) && (yPlayer + height > yWall + static_cast<float>(height) / 4)) {
		if (xPlayer - PlayerSpeed * elapsedTime.asSeconds() <= xWall + widWall) {
			collideLeft = true;
		}
	}

}