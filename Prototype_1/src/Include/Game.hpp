#ifndef BOOK_GAME_HPP
#define BOOK_GAME_HPP

#include "SFML/Graphics.hpp"
#include "RoundTarget.hpp"
#include "Player.hpp"


class Game : private sf::NonCopyable
{
	public:
								Game();
		void					run();

	private:
		void					processEvents();
		void					update(sf::Time elapsedTime);
		void					render();
		void					initialize(std::vector<RoundTarget> &mTargets);

		void					updateStatistics(sf::Time elapsedTime);	

		static const sf::Time	TimePerFrame;
		

		std::map<std::string,sf::Texture> babyTextures;

		sf::RectangleShape		windowBounds{ sf::Vector2f(1024,768) };
		sf::RectangleShape		testCloud{ sf::Vector2f(280,70)};
		int						nbCercles = 10;
		Player					ludo{ 400,300 };
		sf::View				altView{sf::Vector2f(512.f,384.f),sf::Vector2f(1024,768)};
		sf::RenderWindow		mWindow{sf::VideoMode{1024,768}, "SFML Application", sf::Style::Close};
	
        std::vector<RoundTarget> mTargets;
		sf::Font				mFont;
		sf::Text				mStatisticsText;
		sf::Time				mStatisticsUpdateTime;
		std::size_t				mStatisticsNumFrames{0};
};

#endif // BOOK_GAME_HPP
