#include <gtest/gtest.h>

#include "Door.hpp"
#include "Wall.hpp"
#include "Player.hpp"
#include "Group.hpp"

namespace Door_test {

	std::map<std::string, const sf::Texture, std::less<>> textures;
	static const sf::Time elapsedTime = sf::seconds(1.f / 60.f);
	const auto PlayerSpeed = 200.f;
	sf::View altView{ sf::Vector2f(0.f,0.f),sf::Vector2f(1000.f, 1000.f) };

	TEST(DoorTest, playerOnLeft_collisionDoorClosed) {

		std::string sc = R"(<?xml version="1.0"?>
	<Monde>
		<Niveau label="M1_Niveau1">
			<!-- Crée un élément en position (x,y) de taille (w,h) -->     
			<Player x="0" y="-106" h="106" w="144"/>
			<Pacifier x="-420" y="-196" h="196" w="184"/>
			<Wall x="0" y="0" h="168" w="512" vertical="false"/>
			<Door x="144" y="-700" h="700" w="512" vertical="true"/>
		</Niveau>
	</Monde>)";
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_string(sc.c_str());
		ASSERT_TRUE(result) << result.description();
		for (auto const& child : doc.child("Monde"))
		{
			auto grp = std::make_unique<Group>(child);
			float position_initiale = grp->getPos().x;
			grp->update(elapsedTime, altView, textures);
			grp->collide(grp->getPos(), grp->getSiz(), elapsedTime, true);
			EXPECT_EQ(grp->getPos().x, position_initiale);

			grp->handlePlayerInput(sf::Keyboard::D, true);
			grp->update(elapsedTime, altView, textures);
			grp->collide(grp->getPos(), grp->getSiz(), elapsedTime, true);
			EXPECT_EQ(grp->getPos().x, position_initiale);


		}

	}

	TEST(DoorTest, playerOnLeft_noCollisionDoorOpened) {

		std::string sc = R"(<?xml version="1.0"?>
	<Monde>
		<Niveau label="M1_Niveau1">
			<!-- Crée un élément en position (x,y) de taille (w,h) -->     
			<Player x="0" y="-106" h="106" w="144"/>
			<Pacifier x="-420" y="-196" h="196" w="184"/>
			<Wall x="0" y="0" h="168" w="512" vertical="false"/>
			<Door x="144" y="-700" h="700" w="512" vertical="true"/>
		</Niveau>
	</Monde>)";
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_string(sc.c_str());
		ASSERT_TRUE(result) << result.description();
		for (auto const& child : doc.child("Monde"))
		{
			auto grp = std::make_unique<Group>(child);
			float position_initiale = grp->getPos().x;
			grp->update(elapsedTime, altView, textures);
			grp->collide(grp->getPos(), grp->getSiz(), elapsedTime, true);
			EXPECT_EQ(grp->getPos().x, position_initiale);

			grp->handlePlayerInput(sf::Keyboard::E, true); //cliquer sur "E" quand on est proche de la porte permet de l'ouvrir
			grp->update(elapsedTime, altView, textures);
			grp->collide(grp->getPos(), grp->getSiz(), elapsedTime, true);

			grp->handlePlayerInput(sf::Keyboard::D, true);
			grp->update(elapsedTime, altView, textures);
			grp->collide(grp->getPos(), grp->getSiz(), elapsedTime, true);
			EXPECT_EQ(grp->getPos().x, position_initiale + PlayerSpeed * elapsedTime.asSeconds());

		}

	}

	TEST(DoorTest, playerOnLeft_openingDoorFromTooFar) {

		std::string sc = R"(<?xml version="1.0"?>
	<Monde>
		<Niveau label="M1_Niveau1">
			<!-- Crée un élément en position (x,y) de taille (w,h) -->     
			<Player x="0" y="-106" h="106" w="144"/>
			<Pacifier x="-420" y="-196" h="196" w="184"/>
			<Wall x="0" y="0" h="168" w="512" vertical="false"/>
			<Door x="219" y="-700" h="700" w="512" vertical="true"/>
		</Niveau>
	</Monde>)";
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_string(sc.c_str());
		ASSERT_TRUE(result) << result.description();
		for (auto const& child : doc.child("Monde"))
		{
			auto grp = std::make_unique<Group>(child);
			float position_initiale = grp->getPos().x;
			grp->update(elapsedTime, altView, textures);
			grp->collide(grp->getPos(), grp->getSiz(), elapsedTime, true);
			EXPECT_EQ(grp->getPos().x, position_initiale);

			grp->handlePlayerInput(sf::Keyboard::E, true); //cliquer sur "E" quand on est proche de la porte permet de l'ouvrir (ici le joueur est trop loin)
			grp->update(elapsedTime, altView, textures);
			grp->collide(grp->getPos(), grp->getSiz(), elapsedTime, true);

			grp->handlePlayerInput(sf::Keyboard::D, true);
			for (int i = 0;i < 50;++i) {
				grp->update(elapsedTime, altView, textures);
				grp->collide(grp->getPos(), grp->getSiz(), elapsedTime, true);
			}
			//Joueur bloqué à la porte
			EXPECT_LT(grp->getPos().x, position_initiale + PlayerSpeed * elapsedTime.asSeconds() * 50);
			EXPECT_LT(grp->getPos().x, 219.f - grp->getSiz().x);

		}

	}

	TEST(DoorTest, playerOnLeft_openingDoorFromCloseEnough) {

		std::string sc = R"(<?xml version="1.0"?>
	<Monde>
		<Niveau label="M1_Niveau1">
			<!-- Crée un élément en position (x,y) de taille (w,h) -->     
			<Player x="0" y="-106" h="106" w="144"/>
			<Pacifier x="-420" y="-196" h="196" w="184"/>
			<Wall x="0" y="0" h="168" w="512" vertical="false"/>
			<Door x="219" y="-700" h="700" w="512" vertical="true"/>
		</Niveau>
	</Monde>)";
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_string(sc.c_str());
		ASSERT_TRUE(result) << result.description();
		for (auto const& child : doc.child("Monde"))
		{
			auto grp = std::make_unique<Group>(child);
			float position_initiale = grp->getPos().x;
			grp->update(elapsedTime, altView, textures);
			grp->collide(grp->getPos(), grp->getSiz(), elapsedTime, true);
			EXPECT_EQ(grp->getPos().x, position_initiale);

			grp->handlePlayerInput(sf::Keyboard::E, true); //cliquer sur "E" quand on est proche de la porte permet de l'ouvrir (ici le joueur est trop loin)
			grp->update(elapsedTime, altView, textures);
			grp->collide(grp->getPos(), grp->getSiz(), elapsedTime, true);

			grp->handlePlayerInput(sf::Keyboard::D, true); //on s'approche juste assez
			grp->update(elapsedTime, altView, textures);
			grp->collide(grp->getPos(), grp->getSiz(), elapsedTime, true);
			
			grp->handlePlayerInput(sf::Keyboard::E, true); //cliquer sur "E" quand on est proche de la porte permet de l'ouvrir (assez proche maintenant)

			for (int i = 0;i < 49;++i) {
				grp->update(elapsedTime, altView, textures);
				grp->collide(grp->getPos(), grp->getSiz(), elapsedTime, true);
			}
			EXPECT_NEAR(grp->getPos().x, position_initiale + PlayerSpeed * elapsedTime.asSeconds() * 50, 0.0001);
			EXPECT_GT(grp->getPos().x, 219.f - grp->getSiz().x); //got past the door

		}
	}

}