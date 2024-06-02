#ifndef BOOK_GAME_HPP
#define BOOK_GAME_HPP

#include "SFML/Graphics.hpp"
#include "RoundTarget.hpp"
#include "Player.hpp"
#include "Wall.hpp"
#include "Group.hpp"
#pragma once

class Game : private sf::NonCopyable
{
	public:
								Game();
		void					run();

	private:
		void					processEvents();
		void					update(sf::Time elapsedTime);
		void					render();
		void					initialize(std::vector<RoundTarget> &mTargets, std::map<std::string, const sf::Texture> &textures);
		void					initTextures(std::map<std::string, const sf::Texture> &textures);
		void					updateStatistics(sf::Time elapsedTime);	

		static const sf::Time	TimePerFrame;
		

		std::map<std::string,const sf::Texture> textures;

		int										nbCercles = 10;

		sf::View								altView{sf::Vector2f(512.f,384.f),sf::Vector2f(1024,768)};
		sf::RenderWindow						mWindow{sf::VideoMode{1024,768}, "SFML Application", sf::Style::Close};
	
		std::vector<std::unique_ptr<Group>>		levels;
		int										curLevel = 0;
        std::vector<RoundTarget>				mTargets;
		sf::Font								mFont;
		sf::Text								mStatisticsText;
		sf::Time								mStatisticsUpdateTime;
		std::size_t								mStatisticsNumFrames{0};
};

#endif // BOOK_GAME_HPP
