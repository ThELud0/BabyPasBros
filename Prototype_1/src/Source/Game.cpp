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

float invertOrNot(float x) {
	int choix = rando(0,1);
	if (choix == 0) {
		return x;
	}
	else {
		return -x;
	}
}



Game::Game()
{
	mFont.loadFromFile("media/Sansation.ttf");
	mStatisticsText.setFont(mFont);
	mStatisticsText.setColor(sf::Color::Black);
	
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
	initialize(mTargets, textures,levels);




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
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					mWindow.setView(altView);
					for (auto target = mTargets.rbegin(); target != mTargets.rend(); ++target) {
						if ((target->isHitByMouse(mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow))))&&(target->getStatus()==RoundTargetStatus::Alive)) {
							std::next(target).base() -> setStatus(RoundTargetStatus::Dying);
							break;
						}
					}
					mWindow.setView(mWindow.getDefaultView());
				}
				else if (event.mouseButton.button == sf::Mouse::Right) {
					
					++curLevel;
					levels[curLevel]->setTexture(textures);
					levels[curLevel]->drawCurrent(mWindow);
					mWindow.setTitle(levels[curLevel]->returnName());
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

void Game::initTextures(std::map<std::string, const sf::Texture> &textures) {
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

	textures.insert(std::make_pair("babyleft", babyLeft));
	textures.insert(std::make_pair("babyright", babyRight));
	textures.insert(std::make_pair("groundCloud", groundCloud));
	textures.insert(std::make_pair("flippedCloud", flippedCloud));
	textures.insert(std::make_pair("closedDoor", closedDoor));
	textures.insert(std::make_pair("openedDoor", openedDoor));
	textures.insert(std::make_pair("openDoorText", openDoorText));

}

void Game::initialize(std::vector<RoundTarget> &mTargets, std::map<std::string, const sf::Texture> &textures, std::vector<std::unique_ptr<Group>> &levels) {
	
	for (int i = 0;i < nbCercles;++i) {
		float radius = rando(10, 50);
		sf::Color couleur = couleurAleatoire();
		float x = rando(0, mWindow.getSize().x - 2 * radius);
		float y = rando(0, mWindow.getSize().y - 2 * radius);
		float xspeed = invertOrNot(rando(1, 100));
		float yspeed = invertOrNot(sqrt(100 * 100 - xspeed * xspeed));
		RoundTarget target{ radius, couleur, x, y, xspeed, yspeed };
		mTargets.push_back(target);
	}

	pugi::xml_document doc;
	if (auto result = doc.load_file("media/monde1.xml"); !result)
	{
		std::cerr << "Could not open file monde1.xml because " << result.description() << std::endl;
		exit(1);
	}

	const pugi::xml_node& node = doc.child("Monde");
	
	for (auto const& child : node)
	{
		std::cout << child.name() << "\n";
		auto grp = std::make_unique<Group>(child);
		grp->setTexture(textures);
		levels.push_back(std::move(grp));
	}
	
	mWindow.setTitle(levels[0] -> returnName());
	
	levels[0]->setTexture(textures);
	
}

void Game::update(sf::Time elapsedTime)
{
	//laisser un temps pour charger les images au début
	if (loadingTime > 0)
		--loadingTime;

	///animation de départ qui zoom vers la "vraie fenêtre" de jeu
	if (loadingTime <= 0) {
		if (altView.getSize().x > 1024) {
			altView.setSize(altView.getSize().x - (4246.f / startingAnimationTime), altView.getSize().y);
			altView.setCenter(altView.getCenter().x + (levels[curLevel]->getPos().x + 77 - altView.getCenter().x) / startingAnimationTime,
				altView.getCenter().y);
		}
		if (altView.getSize().y > 768) {
			altView.setSize(altView.getSize().x, altView.getSize().y - (3408.f / startingAnimationTime));
			altView.setCenter(altView.getCenter().x,
				(levels[curLevel]->getPos().y - 212 - altView.getCenter().y) / startingAnimationTime + altView.getCenter().y);
		}

		if ((altView.getSize().x <= 1024) && (altView.getSize().y <= 768)) {
			altView.setSize(sf::Vector2f(1024, 768));
			canStart = true;
		}
	}


	if (mTargets.empty()) {
		mWindow.close();
	}
	for (auto target = mTargets.begin(); target != mTargets.end(); ++target) {
		if (target -> getStatus() == RoundTargetStatus::Dead) {
			mTargets.erase(target);
			break;	
		}
		else if (target -> getStatus() == RoundTargetStatus::Dying) {
			target -> isDying();
			levels[curLevel]->dragTowards(target->getShape().getPosition());
		}
		target -> update(elapsedTime, altView);
	}
	if (canStart) {
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

	mWindow.setView(mWindow.getDefaultView());
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
		mStatisticsText.setPosition(sf::Vector2f(mWindow.getSize().x / 2.0f, mWindow.getSize().y / 2.0f));
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