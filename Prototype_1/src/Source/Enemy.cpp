#include "Enemy.hpp"

using namespace std::literals;

Enemy::Enemy(float x, float y, int height, int width) : Entity{ x, y, height, width }
{
	wShape.setPosition(x, y);
}

Enemy::Enemy(const pugi::xml_node& node) : Entity{ node }
{
	wShape.setPosition(x, y);
}

void Enemy::drawCurrent(sf::RenderWindow& window) const {
	window.draw(wShape);
}

//void Enemy::setTexture(std::map<std::string, const sf::Texture, std::less<>>& textures) {
//	for (auto const& [keyName, value] : textures) {
//		if ((keyName == "robot") && vertical) {
//			wShape.setTexture(&value);
//		}
//	}
//}

//void Enemy::setSoundBuffer(std::map<std::string, const sf::SoundBuffer, std::less<>>& soundBuffers) {
//	for (auto const& [key, value] : soundBuffers) {
//		if (key == "door"sv) {
//			mEnemySound.setBuffer(value);
//		}
//	}
//	mEnemySound.setVolume(50);
//}
//
//sf::Vector2f Enemy::getPos() {
//	return wShape.getPosition();
//}
//
//
//sf::Vector2f Enemy::getSiz() {
//	return wShape.getSize();
//}

///// <summary>
///// Player::collide s'occupe déjà de la vraie collision, Enemy::collide() sert donc à voir si le joueur est proche 
///// de la porte pour intéragir avec.
///// </summary>
///// <param name="mcPos"></param>
///// <param name="mcSize"></param>
///// <param name="elapsedTime"></param>
///// <param name="physical"></param>
//void Enemy::collide(sf::Vector2f mcPos, sf::Vector2f mcSize, const sf::Time& elapsedTime, bool physical) {
//	//on récupère la position actuelle et taille du joueur 
//	float xPlayer = mcPos.x;
//	float yPlayer = mcPos.y;
//	float widPlayer = mcSize.x;
//	float heiPlayer = mcSize.y;
//
//	float xWall = x;
//	float yWall = y;
//	auto widWall = static_cast<float>(width);
//	auto heiWall = static_cast<float>(height);
//
//	//joueur au dessus de la porte
//	if ((yPlayer < yWall) && (xPlayer + widPlayer > xWall) && (xPlayer < xWall + widWall)
//		&& (yPlayer + heiPlayer * 1.5 >= yWall)) {
//		isNear = true;
//	}
//
//	//joueur en dessous de la porte
//	if ((yPlayer + heiPlayer > yWall + heiWall) && (xPlayer + widPlayer > xWall)
//		&& (xPlayer < xWall + widWall) && (yPlayer - heiPlayer * 0.25 <= yWall + heiWall)) {
//		isNear = true;
//	}
//
//	//joueur à gauche de la porte
//	if ((xPlayer < xWall) && (yPlayer <= yWall + heiWall)
//		&& (yPlayer + heiPlayer >= yWall) && (xPlayer + widPlayer * 1.5 >= xWall)) {
//		isNear = true;
//	}
//	//joueur à droite du mur
//	if ((xPlayer + widPlayer > xWall + widWall) && (yPlayer <= yWall + heiWall)
//		&& (yPlayer + heiPlayer >= yWall) && (xPlayer - widPlayer * 0.5 <= xWall + widWall)) {
//		isNear = true;
//	}
//
//
//}
//
//void Enemy::update(const sf::Time& elapsedTime, sf::View& view, std::map<std::string, const sf::Texture, std::less<>>& textures) {
//
//	for (auto const& [keyName, value] : textures) {
//		wShape.setTexture(&value, true);
//		wShape.setSize(sf::Vector2f(static_cast<float>(width) * static_cast<float>(1.5), static_cast<float>(height)));
//	}
//
//	//on reset isNear à false, update() la mettra à true s'il le faut
//	isNear = false;
//}
//
//void Enemy::handlePlayerInput(const sf::Keyboard::Key& key, const bool& isPressed) {
//	//si le joueur est proche et qu'il a cliqué sur la touche indiquée, la porte s'ouvre
//	if ((key == sf::Keyboard::E) && physical && isNear && isPressed) {
//		physical = false;
//		mEnemySound.play();
//	}
//}