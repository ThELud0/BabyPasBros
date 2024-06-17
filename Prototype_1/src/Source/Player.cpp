#include "Player.hpp"
#include <random>
using namespace std::literals;


/// <summary>
/// Initialise le joueur à une position (x,y) avec une taille (width,height).
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="height"></param>
/// <param name="width"></param>
Player::Player(float x, float y, int height, int width) : Entity{ x, y, height, width } {
	mChar.setPosition(x, y);
}
/// <summary>
/// Initialise le jour à une position (x,y) avec une taille (width,height),
/// ces paramètres étant récupérés à partir d'un node xml.
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
		///on récupère une texture du bébé par défaut
		if (key == "babyright"sv) {
			mChar.setTexture(value);
		}
	}
	///on ajuste la taille de la texture récupérée aux valeurs (width,height) avec lesquelles le jouer a été initialisé
	mChar.setTextureRect(sf::IntRect(0, 0, width, height));
}


/// <summary>
/// Permet de charger les sons initiaux du joueur lors de son apparition.
/// </summary>
/// <param name="soundBuffers"></param>
void Player::setSoundBuffer(std::map<std::string, const sf::SoundBuffer, std::less<>>& soundBuffers) {
	///on parcourt la map des buffer audio
	for (auto const& [key, value] : soundBuffers) {
		if (key == "jump"sv) {
			mJumpSound.setBuffer(value);
		}
		if (key == "knock1"sv) {
			mWalkSound1.setBuffer(value);
		}
		if (key == "knock2"sv) {
			mWalkSound2.setBuffer(value);
		}
		if (key == "knock3"sv) {
			mWalkSound3.setBuffer(value);
		}
		if (key == "hit"sv) {
			mHitSound.setBuffer(value);
		}
		if (key == "rope"sv) {
			mRopeSound.setBuffer(value);
		}
	}
	///on ajuste le volume
	mJumpSound.setVolume(10);
	mWalkSound1.setVolume(20);
	mWalkSound2.setVolume(20);
	mWalkSound3.setVolume(20);
	mHitSound.setVolume(40);
	mRopeSound.setVolume(20);
}

/// <summary>
/// dessine le joueur
/// </summary>
/// <param name="window"></param>
void Player::drawCurrent(sf::RenderWindow& window) const {
	window.draw(mChar);
}

/// <summary>
/// Gère les événements que doit traiter le joueur
/// </summary>
/// <param name="key"></param>
/// <param name="isPressed"></param>
void Player::handlePlayerInput(const sf::Keyboard::Key& key, const bool& isPressed) {
 
	///Un saut n'est possible que si le joueur touche le sol (collideDown)
	if ((key == sf::Keyboard::Z) && collideDown) {
		mIsMovingUp = true;
		mJumpSound.play();
	}
	else if (key == sf::Keyboard::S)
		mIsMovingDown = isPressed;
	else if (key == sf::Keyboard::Q)
		mIsMovingLeft = isPressed;
	else if (key == sf::Keyboard::D)
		mIsMovingRight = isPressed;

}

/// <summary>
/// Met à jour le joueur selon les informations disponibles
/// </summary>
/// <param name="elapsedTime"></param>
/// <param name="view"></param>
/// <param name="babyTextures"></param>
void	Player::update(const sf::Time& elapsedTime, sf::View& view, std::map<std::string,const sf::Texture, std::less<>>& babyTextures) {
	sf::Vector2f movement(0.f, 0.f);
	std::string key("");

	///Si le joueur ne touche pas le sol, il est sujet à la gravité
	if (!collideDown)
	{
		if (mIsMovingUp)
			maxGravity = 1200;
		else
			maxGravity = 1200 - PlayerSpeed * 3;
		///à chaque frame, la gravité accèlere la vitesse à laquelle le personnage tombe jusqu'à atteindre un maximum
		///la gravité du jeu n'est pas réaliste: elle n'est pas constante... 
		/// raison: on veut que le personnage puisse sauter plus haut initialement, puis qu'il retombe plus vite à mesure 
		/// qu'il atteint le sommet de son saut. 
		if (acceleration < maxGravity / 4)
			acceleration += 10.f;
		if ((acceleration >= maxGravity / 4) && (acceleration + 25.f < maxGravity))
			acceleration += 25.f;
		///un mouvement vers le bas consiste juste à faire tomber le personnage plus vite 
		/// lorsqu'il est en l'air, limité au maximum de la gravité
		if (mIsMovingDown && (acceleration + 50.f < maxGravity))
			acceleration += 50.f;
		
	}
	
	///si un saut a été enclenché et qu'il n'y a pas d'obstacle en hauteur, le personnage saute.
	if (mIsMovingUp&&(!collideUp))
		movement.y -= PlayerSpeed * 3;

	if (mIsMovingLeft && (!collideLeft)) {
		movement.x -= PlayerSpeed;
		playWalkSound();
	}
	if (mIsMovingRight && (!collideRight)) {
		movement.x += PlayerSpeed;
		playWalkSound();
	}

	//on "override" les instructions provenant du clavier pour harponner le joueur vers un RoundTarget si un a été cliqué
	if (dragging) {
		movement = dragMovement;
	}

	movement.y += acceleration;

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

	mChar.move(movement * elapsedTime.asSeconds());
	
	///si le joueur s'approche des bords horizontaux de la vue, on la "glisse" pour suivre le joueur et se déplacer au delà 
	///de ce qui est visible dans la fenêtre initiale
	if ((mChar.getPosition().x < view.getCenter().x - view.getSize().x / 2 + static_cast<float>(mChar.getTextureRect().getSize().x * 1.5)) || (mChar.getPosition().x > view.getCenter().x + view.getSize().x / 2 - static_cast<float>(mChar.getTextureRect().getSize().x * 3.5))) {
		///la vue coulisse à la même vitesse que le joueur.
		view.move(movement.x * elapsedTime.asSeconds(),0.f);

		///si le joueur fini hors de l'écran par exemple à cause d'un respawn suite au changement de niveau
		///ou d'un autre bug, la vue se recentre sur le joueur.
		if ((mChar.getPosition().x < view.getCenter().x - view.getSize().x / 2 + static_cast<float>(mChar.getTextureRect().getSize().x * 1.5) - 1) || (mChar.getPosition().x > view.getCenter().x + view.getSize().x / 2 - static_cast<float>(mChar.getTextureRect().getSize().x) * 3.5 + 1)) {
			view.move(mChar.getPosition().x - view.getCenter().x, 0.f);
		}
	}
	///idem pour les bords verticaux de la vue.
	if ((mChar.getPosition().y > view.getCenter().y + view.getSize().y / 2 - static_cast<float>(mChar.getTextureRect().getSize().y * 3.5)) || (mChar.getPosition().y < view.getCenter().y - view.getSize().y / 2 + static_cast<float>(mChar.getTextureRect().getSize().y * 1.5))) {
		view.move(0.f, movement.y * elapsedTime.asSeconds());

		if ((mChar.getPosition().y > view.getCenter().y + view.getSize().y / 2 - static_cast<float>(mChar.getTextureRect().getSize().y * 3.5) + 1) || (mChar.getPosition().y < view.getCenter().y - view.getSize().y / 2 + static_cast<float>(mChar.getTextureRect().getSize().y * 1.5) - 1)) {
			view.move(0.f, mChar.getPosition().y - view.getCenter().y);
		}
	}

	///on remet les informations de collision et drag à false, les autres fonctions les remettront à
	///true si nécéssaire pour le traitement du prochain update.
	collideUp = false;
	collideDown = false;
	collideLeft = false;
	collideRight = false;
	dragging = false;
}

/// <summary>
/// retourne le sprite qui représente le joueur
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
/// permet de gérer les collisions avec un mur donné
/// </summary>
/// <param name="wallPos"></param>
/// <param name="wallSize"></param>
/// <param name="elapsedTime"></param>
void Player::collide(sf::Vector2f wallPos, sf::Vector2f wallSize, const sf::Time& elapsedTime, bool physical) {
	///on récupère la position actuelle du joueur et les informations pertinentes au mur traité
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
			/// on laisse le joueur s'enfoncer légèrement dans le mur (à un quart de sa taille) purement pour l'aspect esthetique.
			/// on regarde si le prochain mouvement vers le bas du joueur rentre en collision avec le mur. 
			/// le mouvement le plus rapide dont le joueur est capable vers le bas est à la vitesse maxGravity
			&& (yPlayer + static_cast<float>(height) + maxGravity*elapsedTime.asSeconds() > yWall + static_cast<float>(height) / 4)) 
		{
			collideDown = true;
			///si le joueur touche le sol, son acceleration due à la gravité retourne à zéro 
			///de plus il arrête son mouvement de saut vers le haut jusqu'à nouvel ordre.
			acceleration = 0;
			mIsMovingUp = false;
		}

		//joueur en dessous du mur
		if ((yPlayer + static_cast<float>(height) > yWall + heiWall) && (xPlayer + static_cast<float>(width) > xWall) && (xPlayer < xWall + widWall)
			///au plus vite, un joueur se déplace vers le haut à la vitesse de son saut.
			&& (yPlayer - 3 * PlayerSpeed * elapsedTime.asSeconds() <= yWall + heiWall - static_cast<float>(height) / 6)) 
		{
			collideUp = true;
			mHitSound.play();
			if (mIsMovingUp) {
				acceleration = PlayerSpeed * 3 + 10;
				
			}

		}

		//joueur à gauche du mur
		if ((xPlayer < xWall) && (yPlayer < yWall + heiWall - static_cast<float>(height) / 6) && (yPlayer + static_cast<float>(height) > yWall + static_cast<float>(height) / 4)
			///idem que pour le haut et bas, sauf qu'on ne laisse pas le joueur s'enfoncer dans les cotés verticaux d'un mur.
			///si son prochain mouvement le fait rentrer dans le mur, on l'interdit.
			&& (xPlayer + static_cast<float>(width) + PlayerSpeed * elapsedTime.asSeconds() >= xWall)) 
		{
			collideRight = true;

				
		}
		//joueur à droite du mur
		if ((xPlayer + static_cast<float>(width) > xWall + widWall) && (yPlayer < yWall + heiWall - static_cast<float>(height) / 6) && (yPlayer + static_cast<float>(height) > yWall + static_cast<float>(height) / 4)
			///même raisonnement que pour la collision précédente.
			&& (xPlayer - PlayerSpeed * elapsedTime.asSeconds() <= xWall + widWall)) 
		{
			collideLeft = true;

			
		}
	}

}

//tire le joueur vers la position d'un RoundTarget mourant (sans traverser les obstacles sur son chemin)
void Player::dragTowards(sf::Vector2f targetPos) {
	//mRopeSound.play();
	dragging = true;
	sf::Vector2f movement(0.f, 0.f);

	//si le joueur a atteint la hauteur du RoundTarget on se contente de l'empécher de tomber
	if ((targetPos.y <= mChar.getPosition().y + PlayerSpeed * 2)&& (targetPos.y >= mChar.getPosition().y)) {
		movement.y -= acceleration;
	}
	//sinon on le tire vers le RoundTarget
	else if ((targetPos.y < mChar.getPosition().y) && (!collideUp)) {
		movement.y -= PlayerSpeed * 2 + acceleration;
	}

	if ((targetPos.x < mChar.getPosition().x + static_cast<float>(width) / 3.f) && (!collideLeft)) {
		movement.x -= PlayerSpeed;
	}
	if ((targetPos.x > mChar.getPosition().x + static_cast<float>(width) * 2.f / 3.f) && (!collideRight)) {
		movement.x += PlayerSpeed;
	}
	dragMovement = movement;
}


void Player::playWalkSound()  {
	if (collideDown && (clock.getElapsedTime().asMilliseconds() > 350.f)) {
		clock.restart();
		//random int from 0 to 2
		static std::random_device rd;
		static std::default_random_engine engine(rd());
		std::uniform_int_distribution<> distribution(0, 2);
		int drawnNumber = distribution(engine);
		if (drawnNumber == 0) {
			mWalkSound1.play();
		}
		else if (drawnNumber == 1) {
			mWalkSound2.play();
		}
		else {
			mWalkSound3.play();
		}
	}
}
