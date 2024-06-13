#include "Player.hpp"
using namespace std::literals;


/// <summary>
/// Initialise le joueur � une position (x,y) avec une taille (width,height).
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="height"></param>
/// <param name="width"></param>
Player::Player(float x, float y, int height, int width) : Entity{ x, y, height, width } {
	mChar.setPosition(x, y);
}
/// <summary>
/// Initialise le jour � une position (x,y) avec une taille (width,height),
/// ces param�tres �tant r�cup�r�s � partir d'un node xml.
/// </summary>
/// <param name="node"></param>
Player::Player(const pugi::xml_node& node) : Entity{node} {
	mChar.setPosition(x, y);
}

/// <summary>
/// Permet de donner la texture initiale du joueur lors de son apparition.
/// </summary>
/// <param name="textures"></param>
void Player::setTexture(std::map<std::string, const sf::Texture, std::less<>> &textures) {
	///on parcourt la map des textures
	for (auto const & [key,value] : textures) {
		///on r�cup�re une texture du b�b� par d�faut
		if (key == "babyright"sv) {
			mChar.setTexture(value);
		}
	}
	///on ajuste la taille de la texture r�cup�r�e aux valeurs (width,height) avec lesquelles le jouer a �t� initialis�
	mChar.setTextureRect(sf::IntRect(0, 0, width, height));
}

/// <summary>
/// dessine le joueur
/// </summary>
/// <param name="window"></param>
void Player::drawCurrent(sf::RenderWindow& window) const {
	window.draw(mChar);
}

/// <summary>
/// G�re les �v�nements que doit traiter le joueur
/// </summary>
/// <param name="key"></param>
/// <param name="isPressed"></param>
void Player::handlePlayerInput(const sf::Keyboard::Key& key, const bool& isPressed) {
 
	///Un saut n'est possible que si le joueur touche le sol (collideDown)
	if ((key == sf::Keyboard::Z) && collideDown) {
		mIsMovingUp = true;
	}
	else if (key == sf::Keyboard::S)
		mIsMovingDown = isPressed;
	else if (key == sf::Keyboard::Q)
		mIsMovingLeft = isPressed;
	else if (key == sf::Keyboard::D)
		mIsMovingRight = isPressed;

}

/// <summary>
/// Met � jour le joueur selon les informations disponibles
/// </summary>
/// <param name="elapsedTime"></param>
/// <param name="view"></param>
/// <param name="babyTextures"></param>
void	Player::update(const sf::Time& elapsedTime, sf::View& view, std::map<std::string,const sf::Texture, std::less<>>& babyTextures) {
	sf::Vector2f movement(0.f, 0.f);
	std::string key("");

	///Si le joueur ne touche pas le sol, il est sujet � la gravit�
	if (!collideDown)
	{
		///� chaque frame, la gravit� acc�lere la vitesse � laquelle le personnage tombe jusqu'� atteindre un maximum
		///la gravit� du jeu n'est pas r�aliste: elle n'est pas constante... 
		/// raison: on veut que le personnage puisse sauter plus haut initialement, puis qu'il retombe plus vite � mesure 
		/// qu'il atteint le sommet de son saut.
		if (acceleration < maxGravity / 4)
			acceleration += 10.f;
		if ((acceleration >= maxGravity / 4) && (acceleration < maxGravity ))
			acceleration += 25.f;
		///un mouvement vers le bas consiste juste � faire tomber le personnage plus vite 
		/// lorsqu'il est en l'air, limit� au maximum de la gravit�
		if (mIsMovingDown && (acceleration + 50.f < maxGravity))
			acceleration += 50.f;
		movement.y += acceleration;
	}
	
	///si un saut a �t� enclench� et qu'il n'y a pas d'obstacle en hauteur, le personnage saute.
	if (mIsMovingUp&&(!collideUp))
		movement.y -= PlayerSpeed * 3;

	if (mIsMovingLeft && (!collideLeft)) {
		movement.x -= PlayerSpeed;
	}
	if (mIsMovingRight && (!collideRight)) {
		movement.x += PlayerSpeed;
	}

	if (movement.x > 0)
		key = "babyright";
	else if (movement.x < 0)
		key = "babyleft";

	///on change l'apparence du joueur en fonction de son mouvement
	for (auto const&  [keyName, value] : babyTextures) {
		if (keyName == key) {
			mChar.setTexture(value);
		}
	}

	if (dragging) {
		movement = dragMovement;
	}

	mChar.move(movement * elapsedTime.asSeconds());
	
	///si le joueur s'approche des bords horizontaux de la vue, on la "glisse" pour suivre le joueur et se d�placer au del� 
	///de ce qui est visible dans la fen�tre initiale
	if ((mChar.getPosition().x < view.getCenter().x - view.getSize().x / 2 + static_cast<float>(mChar.getTextureRect().getSize().x)) || (mChar.getPosition().x > view.getCenter().x + view.getSize().x / 2 - static_cast<float>(mChar.getTextureRect().getSize().x * 2))) {
		///la vue coulisse � la m�me vitesse que le joueur.
		view.move(movement.x * elapsedTime.asSeconds(),0.f);

		///si le joueur fini hors de l'�cran par exemple � cause d'un respawn suite au changement de niveau
		///ou d'un autre bug, la vue se recentre sur le joueur.
		if ((mChar.getPosition().x < view.getCenter().x - view.getSize().x / 2 + static_cast<float>(mChar.getTextureRect().getSize().x) - 1) || (mChar.getPosition().x > view.getCenter().x + view.getSize().x / 2 - static_cast<float>(mChar.getTextureRect().getSize().x) * 2 + 1)) {
			view.move(mChar.getPosition().x - view.getCenter().x, 0.f);
		}
	}
	///idem pour les bords verticaux de la vue.
	if ((mChar.getPosition().y > view.getCenter().y + view.getSize().y / 2 - static_cast<float>(mChar.getTextureRect().getSize().y * 2)) || (mChar.getPosition().y < view.getCenter().y - view.getSize().y / 2 + static_cast<float>(mChar.getTextureRect().getSize().y))) {

		view.move(0.f, movement.y * elapsedTime.asSeconds());
		if ((mChar.getPosition().y > view.getCenter().y + view.getSize().y / 2 - static_cast<float>(mChar.getTextureRect().getSize().y * 2) + 1) || (mChar.getPosition().y < view.getCenter().y - view.getSize().y / 2 + static_cast<float>(mChar.getTextureRect().getSize().y) - 1)) {
			view.move(0.f, mChar.getPosition().y - view.getCenter().y);
		}
	}

	///on remet les informations de collision et drag � false, les autres fonctions les remettront �
	///true si n�c�ssaire pour le traitement du prochain update.
	collideUp = false;
	collideDown = false;
	collideLeft = false;
	collideRight = false;
	dragging = false;
}

/// <summary>
/// retourne le sprite qui repr�sente le joueur
/// </summary>
/// <returns></returns>
sf::Sprite Player::getSelf() const{
	return mChar;
}
/// <summary>
/// retourne la position (x,y) du sprite du joueur
/// </summary>
/// <returns></returns>
sf::Vector2f Player::getPos() {
	return mChar.getPosition();
}
/// <summary>
/// retourne la taille (width,height) du sprite du joueur
/// </summary>
/// <returns></returns>
sf::Vector2f Player::getSiz() {
	return sf::Vector2f(static_cast<float>(width), static_cast<float>(height));
}

/// <summary>
/// permet de g�rer les collisions avec un mur donn�
/// </summary>
/// <param name="wallPos"></param>
/// <param name="wallSize"></param>
/// <param name="elapsedTime"></param>
void Player::collide(sf::Vector2f wallPos, sf::Vector2f wallSize, const sf::Time& elapsedTime, bool physical) {
	///on r�cup�re la position actuelle du joueur et les informations pertinentes au mur trait�
	float xPlayer = mChar.getPosition().x;
	float yPlayer = mChar.getPosition().y;
	float xWall = wallPos.x;
	float yWall = wallPos.y;
	float widWall = wallSize.x;
	float heiWall = wallSize.y;

	//si l'obstacle est un obstacle physique (le joueur passe au travers les objets non physique)
	if (physical) {

		//joueur au dessus du mur
		if ((yPlayer < yWall) && (xPlayer + static_cast<float>(width) > xWall) && (xPlayer < xWall + widWall)
			/// on laisse le joueur s'enfoncer l�g�rement dans le mur (� un quart de sa taille) purement pour l'aspect esthetique.
			/// on regarde si le prochain mouvement vers le bas du joueur rentre en collision avec le mur. 
			/// le mouvement le plus rapide dont le joueur est capable vers le bas est � la vitesse maxGravity, att�nu�e par sa vitesse de saut
			&& (yPlayer + static_cast<float>(height) + maxGravity*elapsedTime.asSeconds() >= yWall + static_cast<float>(height) / 4)) 
		{
			collideDown = true;
			///si le joueur touche le sol, son acceleration due � la gravit� retourne � z�ro 
			///de plus il arr�te son mouvement de saut vers le haut jusqu'� nouvel ordre.
			acceleration = 0;
			mIsMovingUp = false;
		}

		//joueur en dessous du mur
		if ((yPlayer + static_cast<float>(height) > yWall + heiWall) && (xPlayer + static_cast<float>(width) > xWall) && (xPlayer < xWall + widWall)
			///au plus vite, un joueur se d�place vers le haut � la vitesse de son saut.
			&& (yPlayer - 3 * PlayerSpeed * elapsedTime.asSeconds() <= yWall + heiWall - static_cast<float>(height) / 6)) 
		{
			collideUp = true;
		}

		//joueur � gauche du mur
		if ((xPlayer < xWall) && (yPlayer < yWall + heiWall - static_cast<float>(height) / 6) && (yPlayer + static_cast<float>(height) > yWall + static_cast<float>(height) / 4)
			///idem que pour le haut et bas, sauf qu'on ne laisse pas le joueur s'enfoncer dans les cot�s verticaux d'un mur.
			///si son prochain mouvement le fait rentrer dans le mur, on l'interdit.
			&& (xPlayer + static_cast<float>(width) + PlayerSpeed * elapsedTime.asSeconds() >= xWall)) 
		{
			collideRight = true;
		}
		//joueur � droite du mur
		if ((xPlayer + static_cast<float>(width) > xWall + widWall) && (yPlayer < yWall + heiWall - static_cast<float>(height) / 6) && (yPlayer + static_cast<float>(height) > yWall + static_cast<float>(height) / 4)
			///m�me raisonnement que pour la collision pr�c�dente.
			&& (xPlayer - PlayerSpeed * elapsedTime.asSeconds() <= xWall + widWall)) 
		{
			collideLeft = true;
		}
	}

}

void Player::dragTowards(sf::Vector2f targetPos) {
	dragging = true;
	sf::Vector2f movement(0.f, 0.f);

	if ((targetPos.y < mChar.getPosition().y) && (!collideUp))
		movement.y -= PlayerSpeed * 2;

	if ((targetPos.x < mChar.getPosition().x + static_cast<float>(width) / 3.f) && (!collideLeft)) {
		movement.x -= PlayerSpeed;
	}
	if ((targetPos.x > mChar.getPosition().x + static_cast<float>(width) * 2.f / 3.f) && (!collideRight)) {
		movement.x += PlayerSpeed;
	}

	dragMovement = movement;
}