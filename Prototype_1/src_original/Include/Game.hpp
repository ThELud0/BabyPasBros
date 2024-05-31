#ifndef BOOK_GAME_HPP
#define BOOK_GAME_HPP

#include "SFML/Graphics.hpp"


class Game : private sf::NonCopyable
{
	public:
								Game();
		void					run();
		

	private:
		void					processEvents();
		void					update(sf::Time elapsedTime);
		void					render();

		void					updateStatistics(sf::Time elapsedTime);	
		void					handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
		

		static const float		PlayerSpeed;
		static const sf::Time	TimePerFrame;

		sf::RenderWindow		mWindow{sf::VideoMode{640, 480}, "SFML Application", sf::Style::Close};
		sf::Texture				mTexture;
        sf::CircleShape			mTarget;
		sf::Font				mFont;
		sf::Text				mStatisticsText;
		sf::Time				mStatisticsUpdateTime;

		std::size_t				mStatisticsNumFrames{0};
		bool					mIsMovingUp{false};
		bool					mIsMovingDown{false};
		bool					mIsMovingRight{false};
		bool					mIsMovingLeft{false};
};

#endif // BOOK_GAME_HPP
