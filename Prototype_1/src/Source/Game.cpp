#include "Game.hpp"
#include "StringHelpers.hpp"
#include <random>
#include <iostream>
#include <vector>

const sf::Time Game::TimePerFrame = sf::seconds(1.f/60.f);

/// <summary>
/// G�n�re un nombre al�atoire dans l'intervalle donn�
/// </summary>
/// <param name="nbMin"></param>
/// <param name="nbMax"></param>
/// <returns></returns>
int rando(int const nbMin, int const nbMax)
{
	static std::random_device rd;
	static std::default_random_engine engine(rd());
	std::uniform_int_distribution<> distribution(nbMin, nbMax);
	return distribution(engine);
}

/// <summary>
/// G�n�re une couleur al�atoire
/// </summary>
/// <returns></returns>
sf::Color couleurAleatoire() {
	int choix = rando(0,4);
	switch (choix) {
	case 0:
		return sf::Color::Cyan;
	case 1:
		return sf::Color::Yellow;
	case 2:
		return sf::Color::Green;
	case 3:
		return sf::Color::Blue;
	case 4:
		return sf::Color::Magenta;
	default :
		//returns Orange color but technically this should never happen
		return sf::Color(255, 165, 0);
	}
}

/// <summary>
/// Change al�atoirement le signe d'un nombre
/// </summary>
/// <param name="x"></param>
/// <returns></returns>
float invertOrNot(int x) {
	int choix = rando(0,1);
	if (choix == 0) {
		return static_cast<float>(x);
	}
	else {
		return static_cast<float>(-x);
	}
}

/// <summary>
/// Initialise le jeu
/// </summary>
Game::Game()
{
	mFont.loadFromFile("resources/Sansation.ttf");
	mStatisticsText.setFont(mFont);
	mStatisticsText.setFillColor(sf::Color::Black);
}

/// <summary>
/// Lance le jeu
/// </summary>
void Game::run()
{
	//Initialise l'horloge du jeu
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	mWindow.setVerticalSyncEnabled(true);
	
	//Initialise le fond d'�cran de d�but de jeu
	sf::Texture levelStart;
	if (!levelStart.loadFromFile("resources/sleeping_baby_art.jpg")) {
		std::cout << "texture load failed\n";
		exit(1);
	}
	levelStartScreen.setTexture(&levelStart);
	//attention, modifier les valeurs ci-dessous peut casser l'animation de d�part... code � am�liorer si temps libre
	levelStartScreen.setSize(sf::Vector2f(5269.f, 4176.f));
	levelStartScreen.setPosition(-2400.f,-800.f);

	//Initialise les autres �l�ments du jeu (textures, joueur, murs...)
	initTextures(textures);
	initSoundBuffers(soundBuffers);
	initialize(mTargets,levels);

	//charge le son de d�part depuis la table des sons
	sf::Sound harpsound;
	for (auto const& [keyName, value] : soundBuffers) {
		if (keyName == "harp") {
			harpsound.setBuffer(value);
		}
	}
	harpsound.setVolume(20);
	harpsound.play();

	//charge et joue la musique de fond
	sf::Music lullaby;
	if (!lullaby.openFromFile("resources/lullaby.mp3"))
		std::cerr << "unable to load music\n";
	lullaby.setLoop(true);
	lullaby.setVolume(20);
	lullaby.play();

	//boucle de jeu
	while (mWindow.isOpen())
	{
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;
			processEvents();
			update(TimePerFrame);
		}
		updateStatistics(elapsedTime);
		render();
	}
}

/// <summary>
/// G�re les �v�nements du joueur
/// </summary>
void Game::processEvents()
{
    sf::Event event{sf::Event::Count}; // Initialization to an impossible value (in order to suppress Clang-Tidy warning)
	while (mWindow.pollEvent(event))
	{
		switch (event.type)
		{
			case sf::Event::Closed: // fermer la fenetre
				mWindow.close();
				break;

			case sf::Event::MouseButtonPressed:	// click (sur une cible)
				// Tue les RoundTargets, et au passage harponne le personnage vers le RoundTarget cliqu�
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					mWindow.setView(altView); // Pour avoir la bonne position de la souris par rapport au Roundtarget quon voit, il faut se mettre sur la bonne vue
					for (auto target = mTargets.rbegin(); target != mTargets.rend(); ++target) {
						if ((target->isHitByMouse(mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow))))&&(target->getStatus()==RoundTargetStatus::Alive)) {
							std::next(target).base() -> setStatus(RoundTargetStatus::Dying);
							break;
						}
					}
					mWindow.setView(mWindow.getDefaultView());
				}

			case sf::Event::KeyPressed:	//appuie sur une touche du clavier
				levels[curLevel]->handlePlayerInput(event.key.code, true); // on redistribue aux �l�ments qui composent le niveau
				
				//si le joueur se trouve sur la t�tine du niveau actuel et qu'il en donne l'indication, on passe au niveau suivant!
				if (levels[curLevel]->nextLevel()) {
					++curLevel;
					levels[curLevel]->setTexture(textures);
					levels[curLevel]->drawCurrent(mWindow);
					mWindow.setTitle(levels[curLevel]->returnName());
					altView.setCenter(sf::Vector2f(235.f, 1288.f));
					altView.setSize(sf::Vector2f(5270.f, 4176.f));
				}
				break;

			case sf::Event::KeyReleased: //relachement d'une touche
				levels[curLevel]->handlePlayerInput(event.key.code, false);
				break;

            default:
                // We simply ignore all other events
                break;
		}
	}
}

/// <summary>
/// Ajoute toutes les textures des �l�ments � un tableau de textures qui sera r�cup�rable
/// par toutes les instances de classes du jeu, pour que celles avec une texture en commun utilisent
/// toutes la m�me en pointant vers celle du tableau.
/// </summary>
/// <param name="texturesTable"></param>
void Game::initTextures(std::map<std::string, const sf::Texture, std::less<>> &texturesTable) const {
	sf::Texture	babyLeft;
	sf::Texture	babyRight;
	sf::Texture groundCloud;
	sf::Texture flippedCloud;
	sf::Texture closedDoor;
	sf::Texture openedDoor;
	sf::Texture openDoorText;
	sf::Texture babyPacifier;
	sf::Texture pacifierText;

	if (!babyLeft.loadFromFile("resources/babygoleft.png")) {
		std::cout << "texture load failed\n";
		exit(1);
	}
	if (!babyRight.loadFromFile("resources/babygoright.png")) {
		std::cout << "texture load failed\n";
		exit(1);
	}
	if (!groundCloud.loadFromFile("resources/nuage.png")) {
		std::cout << "texture load failed\n";
		exit(1);
	}
	if (!flippedCloud.loadFromFile("resources/flipped_nuage.png")) {
		std::cout << "texture load failed\n";
		exit(1);
	}
	if (!closedDoor.loadFromFile("resources/closed_door.png")) {
		std::cout << "texture load failed\n";
		exit(1);
	}
	if (!openedDoor.loadFromFile("resources/opened_door.png")) {
		std::cout << "texture load failed\n";
		exit(1);
	}
	if (!openDoorText.loadFromFile("resources/open_door_text.png")) {
		std::cout << "texture load failed\n";
		exit(1);
	}
	if (!babyPacifier.loadFromFile("resources/baby_pacifier.png")) {
		std::cout << "texture load failed\n";
		exit(1);
	}
	if (!pacifierText.loadFromFile("resources/pacifier_text.png")) {
		std::cout << "texture load failed\n";
		exit(1);
	}
	texturesTable.try_emplace("babyleft", babyLeft);
	texturesTable.try_emplace("babyright", babyRight);
	texturesTable.try_emplace("groundCloud", groundCloud);
	texturesTable.try_emplace("flippedCloud", flippedCloud);
	texturesTable.try_emplace("closedDoor", closedDoor);
	texturesTable.try_emplace("openedDoor", openedDoor);
	texturesTable.try_emplace("openDoorText", openDoorText);
	texturesTable.try_emplace("babyPacifier", babyPacifier);
	texturesTable.try_emplace("pacifierText", pacifierText);
}

/// <summary>
/// Ajoute toutes les buffer audio � un tableau de soundBuffer r�cup�rable par toutes les instances de classes du jeu,
/// pour que celles avec des sons communs utilisent toutes le m�me en pointant vers celui du tableau.
/// </summary>
/// <param name="soundBuffersTable"></param>
void Game::initSoundBuffers(std::map<std::string, const sf::SoundBuffer, std::less<>>& soundBuffersTable) const {
	sf::SoundBuffer harp;
	sf::SoundBuffer door;
	sf::SoundBuffer babydie;
	sf::SoundBuffer jump;
	sf::SoundBuffer knock1;
	sf::SoundBuffer knock2;
	sf::SoundBuffer knock3;
	sf::SoundBuffer hit;
	sf::SoundBuffer rope;

	if (!harp.loadFromFile("resources/harp.mp3")) {
		std::cerr << "unable to load audio media\n";
		exit(1);
	}
	if (!door.loadFromFile("resources/door.mp3")) {
		std::cerr << "unable to load audio media\n";
		exit(1);
	}
	if (!babydie.loadFromFile("resources/babydie.mp3")) {
		std::cerr << "unable to load audio media\n";
		exit(1);
	}
	if (!jump.loadFromFile("resources/jump.mp3")) {
		std::cerr << "unable to load audio media\n";
		exit(1);
	}
	if (!knock1.loadFromFile("resources/knock1.mp3")) {
		std::cerr << "unable to load audio media\n";
		exit(1);
	}
	if (!knock2.loadFromFile("resources/knock2.mp3")) {
		std::cerr << "unable to load audio media\n";
		exit(1);
	}
	if (!knock3.loadFromFile("resources/knock3.mp3")) {
		std::cerr << "unable to load audio media\n";
		exit(1);
	}
	if (!hit.loadFromFile("resources/hit.mp3")) {
		std::cerr << "unable to load audio media\n";
		exit(1);
	}
	if (!rope.loadFromFile("resources/rope.mp3")) {
		std::cerr << "unable to load audio media\n";
		exit(1);
	}

	soundBuffersTable.try_emplace("harp", harp);
	soundBuffersTable.try_emplace("door", door);
	soundBuffersTable.try_emplace("babydie", babydie);
	soundBuffersTable.try_emplace("jump", jump);
	soundBuffersTable.try_emplace("knock1", knock1);
	soundBuffersTable.try_emplace("knock2", knock2);
	soundBuffersTable.try_emplace("knock3", knock3);
	soundBuffersTable.try_emplace("hit", hit);
	soundBuffersTable.try_emplace("rope", rope);
}

/// <summary>
/// Initialise les RoundTarget et le niveau
/// </summary>
/// <param name="soundBuffersTable"></param>
void Game::initialize(std::vector<RoundTarget> &mTargetsTable, std::vector<std::unique_ptr<Group>> &levelsTable) {
	// On initialise les RoundTarget
	for (int i = 0;i < nbCercles;++i) {
		int radius = rando(10, 50);
		sf::Color couleur = couleurAleatoire();
		int x = rando(0, static_cast<int>(mWindow.getSize().x - 2 * radius));
		int y = rando(0, static_cast<int>(mWindow.getSize().y - 2 * radius));
		float xspeed = invertOrNot(rando(1, 100));
		float yspeed = invertOrNot(static_cast<int>(sqrt(100 * 100 - xspeed * xspeed)));
		RoundTarget target{ static_cast<float>(radius), couleur, static_cast<float>(x), static_cast<float>(y), xspeed, yspeed };
		mTargetsTable.push_back(target);
	}

	//On lit le fichier xml du monde et on charge tous les �l�ments de chaque niveau dans un groupe respectivement, puis on ajoute tous ces groupes
	//� un vecteur de niveaux.
	pugi::xml_document doc;
	if (auto result = doc.load_file("resources/monde1.xml"); !result)
	{
		std::cerr << "Could not open file monde2.xml because " << result.description() << std::endl;
		exit(1);
	}
	for (auto const& child : doc.child("Monde"))
	{
		auto grp = std::make_unique<Group>(child);
		grp->setTexture(textures);
		grp->setSoundBuffer(soundBuffers);
		levelsTable.push_back(std::move(grp));
	}
}

/// <summary>
/// Met � jour le jeu
/// </summary>
/// <param name="elapsedTime"></param>
void Game::update(sf::Time elapsedTime)
{
	//laisser un temps pour charger les images au d�but
	if (loadingTime > 0)
		--loadingTime;

	///animation qui zoom vers la "vraie fen�tre" de jeu au d�but et � chaque changement de niveau
	if (loadingTime <= 0) {		
		if (altView.getSize().x > 1024) {
			//on r�duit la longueur de la vue
			altView.setSize(altView.getSize().x - (4246.f / startingAnimationTime), altView.getSize().y);
			//on garde la vue proche du personnage
			altView.setCenter(altView.getCenter().x + (levels[curLevel]->getPos().x + levels[curLevel]->getSiz().x/2 - altView.getCenter().x) / (startingAnimationTime * 1024 * 2 / altView.getSize().y),
				altView.getCenter().y);
			canStart = false;
		}
		if (altView.getSize().y > 768) {
			//on r�duit la hauteur de la vue
			altView.setSize(altView.getSize().x, altView.getSize().y - (3408.f / startingAnimationTime));
			//on garde la vue proche du personnage
			altView.setCenter(altView.getCenter().x,
				(levels[curLevel]->getPos().y - altView.getCenter().y - levels[curLevel]->getSiz().y/2 ) / (startingAnimationTime * 768 * 2 / altView.getSize().y) + altView.getCenter().y);
			canStart = false;
		}
		if ((altView.getSize().x <= 1024) && (altView.getSize().y <= 768)) {
			altView.setSize(sf::Vector2f(1024, 768));
			canStart = true;
		}
	}

	//g�re l'animation lorsque les RoundTargets meurent
	for (auto target = mTargets.begin(); target != mTargets.end(); ++target) {
		//on retire de la liste un RoundTarget mort
		if (target -> getStatus() == RoundTargetStatus::Dead) {
			mTargets.erase(target);
			break;	
		}
		//on anime le roundTarget entrain de mourir
		else if (target -> getStatus() == RoundTargetStatus::Dying) {
			target -> isDying();
			levels[curLevel]->dragTowards(target->getShape().getPosition());
		}
		target -> update(elapsedTime, altView);
	}

	//on met � jour tous les �l�ments du niveau actuel
	if (canStart) {
		levels[curLevel]->update(elapsedTime, altView, textures);
		levels[curLevel]->collide(levels[curLevel]->getPos(), levels[curLevel]->getSiz(), elapsedTime, true);
	}
}

/// <summary>
/// G�n�re les visuels du jeu
/// </summary>
void Game::render()
{
	mWindow.clear(sf::Color(248,250,245));//c'est la couleur de la bulle de r�ve de l'image au d�but du jeu



	mWindow.setView(altView); //on dessine les �l�ments du jeu qui vont glisser dans la vue alternative...
	if (!canStart) {
		mWindow.clear(sf::Color(64, 145, 162));//c'est la couleur du ciel dans l'image au d�but du jeu
		mWindow.draw(levelStartScreen);
	}

	// on dessine les cibles
	for (auto const &target : mTargets) {
		target.drawCurrent(mWindow);
	}

	// on dessine le niveau et ses �l�ments
	levels[curLevel]->drawCurrent(mWindow);

	mWindow.setView(mWindow.getDefaultView()); //... et ceux de l'UI dans la vue par d�faut qui ne bouge pas
	mWindow.draw(mStatisticsText);
	mWindow.display();
}

/// <summary>
/// Met � jour les param�tres de d�boguage
/// </summary>
/// <param name="elapsedTime"></param>
void Game::updateStatistics(sf::Time elapsedTime)
{
	mStatisticsUpdateTime += elapsedTime;
	mStatisticsNumFrames += 1;

	if (loadingTime > 0) {
		//center text
		mStatisticsText.setCharacterSize(60);
		sf::FloatRect textRect = mStatisticsText.getLocalBounds();
		mStatisticsText.setOrigin(textRect.left + textRect.width / 2.0f,textRect.top + textRect.height / 2.0f);
		mStatisticsText.setPosition(sf::Vector2f(static_cast<float>(mWindow.getSize().x) / 2.0f, static_cast<float>(mWindow.getSize().y) / 2.0f));
		mStatisticsText.setString("Loading...");
	}

	if ((mStatisticsUpdateTime >= sf::seconds(1.0f))&&(loadingTime<=0))
	{
		mStatisticsText.setPosition(5.f, 5.f);
		mStatisticsText.setCharacterSize(10);
		mStatisticsText.setString(
				"Frames / Second = " + toString(mStatisticsNumFrames) + "\n" +
				"Time / Update = " + toString(mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames) + "us\n" +

				"View center pos = " + toString(altView.getCenter().x) + "  " + toString(altView.getCenter().y) + "\n" +
				"View size = " + toString(altView.getSize().x) + "  " + toString(altView.getSize().y)
			);
							 
		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;
	}
}