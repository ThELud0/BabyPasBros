#include "Game.hpp"
#include "StringHelpers.hpp"
#include <random>
#include <iostream>
#include <vector>



const sf::Time Game::TimePerFrame = sf::seconds(1.f/60.f);


//random int from nbMin to nbMax
int rando(int const nbMin, int const nbMax)
{
	static std::random_device rd;
	static std::default_random_engine engine(rd());
	std::uniform_int_distribution<> distribution(nbMin, nbMax);
	return distribution(engine);
}


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

float invertOrNot(int x) {
	int choix = rando(0,1);
	if (choix == 0) {
		return static_cast<float>(x);
	}
	else {
		return static_cast<float>(-x);
	}
}



Game::Game()
{
	mFont.loadFromFile("media/Sansation.ttf");
	mStatisticsText.setFont(mFont);
	mStatisticsText.setFillColor(sf::Color::Black);
	
}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	mWindow.setVerticalSyncEnabled(true);
	
	//Initialise le fond d'écran de début de jeu
	sf::Texture levelStart;
	if (!levelStart.loadFromFile("media/sleeping_baby_art.jpg")) {
		std::cout << "texture load failed\n";
		exit(1);
	}
	levelStartScreen.setTexture(&levelStart);
	//attention, modifier les valeurs ci-dessous peut casser l'animation de départ... code à améliorer si temps libre
	levelStartScreen.setSize(sf::Vector2f(5269.f, 4176.f));
	levelStartScreen.setPosition(-2400.f,-800.f);

	//Initialise les autres éléments du jeu (textures, joueur, murs...)
	initTextures(textures);
	initSoundBuffers(soundBuffers);
	initialize(mTargets, textures,levels);


	// load harp sound from buffer table
	sf::Sound harpsound;
	for (auto const& [keyName, value] : soundBuffers) {
		if (keyName == "harp") {
			harpsound.setBuffer(value);
		}
	}
	harpsound.setVolume(10);
	harpsound.play();

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


void Game::processEvents()
{
    sf::Event event{sf::Event::Count}; // Initialization to an impossible value (in order to suppress Clang-Tidy warning)
	while (mWindow.pollEvent(event))
	{
		switch (event.type)
		{
			case sf::Event::Closed:
				mWindow.close();
				break;
			case sf::Event::MouseButtonPressed:
				// Tue les RoundTargets, et au passage harponne le personnage vers le RoundTarget cliqué
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
				// Passe au niveau suivant si clique droit, à changer pour que le passage de niveau soit déclenché par un autre évènement du jeu...
				else if (event.mouseButton.button == sf::Mouse::Right) {
					++curLevel;
					levels[curLevel]->setTexture(textures);
					levels[curLevel]->drawCurrent(mWindow);
					mWindow.setTitle(levels[curLevel]->returnName());
					altView.setCenter(sf::Vector2f(235.f, 1288.f));
					altView.setSize(sf::Vector2f(5270.f, 4176.f));
				}
				break;

			case sf::Event::KeyPressed :
				levels[curLevel]->handlePlayerInput(event.key.code, true);
				break;
			case sf::Event::KeyReleased :
				levels[curLevel]->handlePlayerInput(event.key.code, false);
				break;

            default:
                // We simply ignore all other events
                break;
		}
	}
}

/// <summary>
/// Ajoute toutes les textures des éléments à un tableau de textures qui sera récupérable
/// par toutes les instances de classes du jeu, pour que celles avec une texture en commun utilisent
/// toutes la même en pointant vers celle du tableau.
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

	if (!babyLeft.loadFromFile("media/babygoleft.png")) {
		std::cout << "texture load failed\n";
		exit(1);
	}

	if (!babyRight.loadFromFile("media/babygoright.png")) {
		std::cout << "texture load failed\n";
		exit(1);
	}

	if (!groundCloud.loadFromFile("media/nuage.png")) {
		std::cout << "texture load failed\n";

		exit(1);
	}
	if (!flippedCloud.loadFromFile("media/flipped_nuage.png")) {
		std::cout << "texture load failed\n";

		exit(1);
	}

	if (!closedDoor.loadFromFile("media/closed_door.png")) {
		std::cout << "texture load failed\n";
		exit(1);
	}

	if (!openedDoor.loadFromFile("media/opened_door.png")) {
		std::cout << "texture load failed\n";
		exit(1);
	}

	if (!openDoorText.loadFromFile("media/open_door_text.png")) {
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

}

/// <summary>
/// Ajoute toutes les buffer audio à un tableau de soundBuffer récupérable par toutes les instances de classes du jeu,
/// pour que celles avec des sons communs utilisent toutes le même en pointant vers celui du tableau.
/// </summary>
/// <param name="soundBuffersTable"></param>
void Game::initSoundBuffers(std::map<std::string, const sf::SoundBuffer, std::less<>>& soundBuffersTable) const {
	sf::SoundBuffer harp;
	sf::SoundBuffer door;
	sf::SoundBuffer babydie;
	sf::SoundBuffer jump;

	if (!harp.loadFromFile("media/harp.wav")) {
		std::cerr << "unable to load audio media\n";
		exit(1);
	}
	if (!door.loadFromFile("media/door.wav")) {
		std::cerr << "unable to load audio media\n";
		exit(1);
	}
	if (!babydie.loadFromFile("media/babydie.wav")) {
		std::cerr << "unable to load audio media\n";
		exit(1);
	}
	if (!jump.loadFromFile("media/jump.wav")) {
		std::cerr << "unable to load audio media\n";
		exit(1);
	}

	soundBuffersTable.try_emplace("harp", harp);
	soundBuffersTable.try_emplace("door", door);
	soundBuffersTable.try_emplace("babydie", babydie);
	soundBuffersTable.try_emplace("jump", jump);
}


void Game::initialize(std::vector<RoundTarget> &mTargetsTable, std::map<std::string, const sf::Texture, std::less<>> &texturesTable, std::vector<std::unique_ptr<Group>> &levelsTable) {
	//initialise les RoundTargets
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

	pugi::xml_document doc;
	if (auto result = doc.load_file("media/monde1.xml"); !result)
	{
		std::cerr << "Could not open file monde1.xml because " << result.description() << std::endl;
		exit(1);
	}

	//On lit le fichier xml du monde et on charge tous les éléments de chaque niveau dans un groupe respectivement, puis on ajoute tous ces groupes
	//à un vecteur de niveaux.
	for (auto const& child : doc.child("Monde"))
	{
		std::cout << child.name() << "\n";
		auto grp = std::make_unique<Group>(child);
		//grp->setTexture(texturesTable);
		grp->setTexture(textures);
		grp->setSoundBuffer(soundBuffers);
		levelsTable.push_back(std::move(grp));
	}

}

void Game::update(sf::Time elapsedTime)
{
	//laisser un temps pour charger les images au début
	if (loadingTime > 0)
		--loadingTime;

	///animation qui zoom vers la "vraie fenêtre" de jeu au début et à chaque changement de niveau
	if (loadingTime <= 0) {
		
		if (altView.getSize().x > 1024) {
			//on réduit la longueur de la vue
			altView.setSize(altView.getSize().x - (4246.f / startingAnimationTime), altView.getSize().y);
			//on garde la vue proche du personnage
			altView.setCenter(altView.getCenter().x + (levels[curLevel]->getPos().x + 77 - altView.getCenter().x) / startingAnimationTime,
				altView.getCenter().y);
			canStart = false;
		}
		if (altView.getSize().y > 768) {
			//on réduit la hauteur de la vue
			altView.setSize(altView.getSize().x, altView.getSize().y - (3408.f / startingAnimationTime));
			//on garde la vue proche du personnage
			altView.setCenter(altView.getCenter().x,
				(levels[curLevel]->getPos().y - 212 - altView.getCenter().y) / startingAnimationTime + altView.getCenter().y);
			canStart = false;
		}
		if ((altView.getSize().x <= 1024) && (altView.getSize().y <= 768)) {
			altView.setSize(sf::Vector2f(1024, 768));
			canStart = true;
		}
	}
	//gère l'animation lorsque les RoundTargets meurent
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
	if (canStart) {
		//on met à jour tous les éléments du niveau actuel
		levels[curLevel]->update(elapsedTime, altView, textures);
		levels[curLevel]->collide(levels[curLevel]->getPos(), levels[curLevel]->getSiz(), elapsedTime, true);
	}

}

void Game::render()
{

	mWindow.clear(sf::Color(248,250,245));//c'est la couleur de la bulle de rêve de l'image au début du jeu!
	mWindow.setView(altView);
	if (!canStart)
		mWindow.draw(levelStartScreen);

	for (auto const &target : mTargets) {
		target.drawCurrent(mWindow);
	}

	levels[curLevel]->drawCurrent(mWindow);

	mWindow.setView(mWindow.getDefaultView()); //on dessine les éléments du jeu qui vont glisser dans la vue alternative, et ceux de l'UI dans la vue par défaut qui ne bouge pas
	mWindow.draw(mStatisticsText);
	mWindow.display();
}

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