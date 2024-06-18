#include "Enemy.hpp"
#include <random>
using namespace std::literals;

Enemy::Enemy(float x, float y, int height, int width) : Entity{ x, y, height, width } {
	mChar.setPosition(x, y);
}

Enemy::Enemy(const pugi::xml_node& node) : Entity{node} {
	mChar.setPosition(x, y);
}

void Enemy::setTexture(std::map<std::string, const sf::Texture, std::less<>> &textures) {
	for (auto const & [key,value] : textures) {
		if (key == "enemy"sv) {
			mChar.setTexture(value);
		}
	}
	mChar.setTextureRect(sf::IntRect(0, 0, width, height));
}

void Enemy::setSoundBuffer(std::map<std::string, const sf::SoundBuffer, std::less<>>& soundBuffers) {
}

void Enemy::drawCurrent(sf::RenderWindow& window) const {
	window.draw(mChar);
}

void Enemy::handlePlayerInput(const sf::Keyboard::Key& key, const bool& isPressed) {
}

void Enemy::update(const sf::Time& elapsedTime, sf::View& view, std::map<std::string, const sf::Texture, std::less<>>& textures) {
}

sf::Sprite Enemy::getSelf() const{
	return mChar;
}

sf::Vector2f Enemy::getPos() {
	return mChar.getPosition();
}

sf::Vector2f Enemy::getSiz() {
	return sf::Vector2f(static_cast<float>(width), static_cast<float>(height));
}

void Enemy::collide(sf::Vector2f wallPos, sf::Vector2f wallSize, const sf::Time& elapsedTime, bool physical) {
}