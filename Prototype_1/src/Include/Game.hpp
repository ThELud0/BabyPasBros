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
		

		sf::RectangleShape		windowBounds{ sf::Vector2f(640,480) };
		int						nbCercles = 10;
		Player					ludo{ 40, sf::Color::Blue, 280, 200 };
		sf::View				altView{sf::Vector2f(320.f,240.f),sf::Vector2f(640,480)};
		sf::RenderWindow		mWindow{sf::VideoMode{640, 480}, "SFML Application", sf::Style::Close};
		sf::Texture				mTexture;
        std::vector<RoundTarget> mTargets;
		sf::Font				mFont;
		sf::Text				mStatisticsText;
		sf::Time				mStatisticsUpdateTime;
		std::size_t				mStatisticsNumFrames{0};
};

#endif // BOOK_GAME_HPP
