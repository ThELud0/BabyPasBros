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
		void					initialize(std::vector<RoundTarget> &mTargets, std::map<std::string, const sf::Texture, std::less<>> &textures, std::vector<std::unique_ptr<Group>> &levels);
		void					initTextures(std::map<std::string, const sf::Texture, std::less<>> &textures) const;
		void					updateStatistics(sf::Time elapsedTime);	

		static const sf::Time	TimePerFrame;
		

		std::map<std::string,const sf::Texture, std::less<>> textures;
		bool									canStart{ false };
		int										nbCercles = 10;

		// on veut une taille 1024,768 en jeu donc il faudra supprimer des pixels dans l'animation de départ
		// 5270 - 1024 = 4246  pixels horizontaux à éliminer au départ pour revenir à la taille de fenêtre normale
		// 4176 - 768 = 3408  pixels verticaux à éliminer au départ pour revenir à la taille de fenêtre normale
		//les valeurs d'initialisation ont été choisies pour une image de départ spécifique, attention aux modifications!!
		sf::View								altView{sf::Vector2f(235.f,1288.f),sf::Vector2f(5270.f, 4176.f)}; 

		sf::RenderWindow						mWindow{sf::VideoMode{1024,768}, "SFML Application", sf::Style::Close};

		int										loadingTime{ 120 };
		float									startingAnimationTime{ 240 }; // en 1/60e de secondes
		sf::RectangleShape						levelStartScreen;

		std::vector<std::unique_ptr<Group>>		levels;
		int										curLevel = 0;
        std::vector<RoundTarget>				mTargets;
		sf::Font								mFont;
		sf::Text								mStatisticsText;
		sf::Time								mStatisticsUpdateTime;
		std::size_t								mStatisticsNumFrames{0};
};

#endif // BOOK_GAME_HPP
