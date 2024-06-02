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
	int choix = rando(0,5);
	switch (choix) {
	case 0:
		return sf::Color::White;
	case 1:
		return sf::Color::Yellow;
	case 2:
		return sf::Color::Green;
	case 3:
		return sf::Color::Blue;
	case 4:
		return sf::Color::Magenta;
	case 5:
		return sf::Color::Cyan;
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
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setCharacterSize(10);
}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	mWindow.setVerticalSyncEnabled(true);
	
	initTextures(textures);

	initialize(mTargets, textures);

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
					for (auto target = mTargets.rbegin(); target != mTargets.rend(); ++target) {
						if ((target->isHitByMouse(sf::Mouse::getPosition(mWindow)))&&(target->getStatus()==RoundTargetStatus::Alive)) {
							std::next(target).base() -> setStatus(RoundTargetStatus::Dying);
							break;
						}
					}
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

	textures.insert(std::make_pair("babyleft", babyLeft));
	textures.insert(std::make_pair("babyright", babyRight));
	textures.insert(std::make_pair("groundCloud", groundCloud));
	textures.insert(std::make_pair("flippedCloud", flippedCloud));

}

void Game::initialize(std::vector<RoundTarget> &mTargets, std::map<std::string, const sf::Texture> &textures) {
	
	for (int i = 0;i < nbCercles;++i) {
		float radius = rando(10, 50);
		sf::Color couleur = couleurAleatoire();
		float x = rando(0, mWindow.getSize().x - 2 * radius);
		float y = rando(mWindow.getSize().y/2, mWindow.getSize().y - 2 * radius);
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

		levels.push_back(std::move(grp));
	}
	
	mWindow.setTitle(levels[0] -> returnName());
	
	levels[0]->setTexture(textures);
	levels[0]->drawCurrent(mWindow);
	
}

void Game::update(sf::Time elapsedTime)
{
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
		}
		target -> update(elapsedTime, altView);
	}

	levels[curLevel]->update(elapsedTime, altView, textures);

}

void Game::render()
{

	mWindow.clear();
	mWindow.setView(altView);


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

	if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	{

			mStatisticsText.setString(
				"Frames / Second = " + toString(mStatisticsNumFrames) + "\n" +
				"Time / Update = " + toString(mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames) + "us\n" +

				"View pos = " + toString(altView.getCenter().x) + "  " + toString(altView.getCenter().y)

			);
							 
		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;
	}
}