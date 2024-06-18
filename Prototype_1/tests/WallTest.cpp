#include <gtest/gtest.h>

#include "Wall.hpp"
#include "Player.hpp"
#include "Group.hpp"

namespace Wall_test {

	std::map<std::string, const sf::Texture, std::less<>> textures;
	static const sf::Time elapsedTime = sf::seconds(1.f / 60.f);
	const auto PlayerSpeed = 200.f;
	sf::View altView{ sf::Vector2f(0.f,0.f),sf::Vector2f(1000.f, 1000.f) };

    TEST(WallTest, playerOnLeft_collision) {

		std::string sc = R"(<?xml version="1.0"?>
	<Monde>
		<Niveau label="M1_Niveau1">
			<!-- Crée un élément en position (x,y) de taille (w,h) -->     
			<Player x="0" y="-106" h="106" w="144"/>
			<Pacifier x="-420" y="-196" h="196" w="184"/>
			<Wall x="0" y="0" h="168" w="512" vertical="false"/>
			<Wall x="144" y="-700" h="700" w="512" vertical="true"/>
		</Niveau>
	</Monde>)";
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_string(sc.c_str());
        ASSERT_TRUE(result) << result.description();
		for (auto const& child : doc.child("Monde"))
		{
			auto grp = std::make_unique<Group>(child);
			float position_initiale = grp->getPos().x;
			EXPECT_EQ(grp->getPos().x, position_initiale);

			grp->handlePlayerInput(sf::Keyboard::D, false);
			grp->update(elapsedTime, altView, textures);
			grp->collide(grp->getPos(), grp->getSiz(), elapsedTime, true);
			EXPECT_EQ(grp->getPos().x, position_initiale);

			grp->handlePlayerInput(sf::Keyboard::D, true);
			grp->update(elapsedTime, altView, textures);
			grp->collide(grp->getPos(), grp->getSiz(), elapsedTime, true);
			EXPECT_EQ(grp->getPos().x, position_initiale);
		}
    }

	TEST(WallTest, playerOnLeft_noCollision) {

		std::string sc = R"(<?xml version="1.0"?>
	<Monde>
		<Niveau label="M1_Niveau1">
			<!-- Crée un élément en position (x,y) de taille (w,h) -->     
			<Player x="0" y="-106" h="106" w="144"/>
			<Pacifier x="-420" y="-196" h="196" w="184"/>
			<Wall x="0" y="0" h="168" w="512" vertical="false"/>
			<Wall x="150" y="-700" h="700" w="512" vertical="true"/>
		</Niveau>
	</Monde>)";
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_string(sc.c_str());
		ASSERT_TRUE(result) << result.description();
		for (auto const& child : doc.child("Monde"))
		{
			
			auto grp = std::make_unique<Group>(child);
			float position_initiale = grp->getPos().x;
			EXPECT_EQ(grp->getPos().x, position_initiale);

			grp->handlePlayerInput(sf::Keyboard::D, false);
			grp->update(elapsedTime, altView, textures);
			grp->collide(grp->getPos(), grp->getSiz(), elapsedTime, true);
			EXPECT_EQ(grp->getPos().x, position_initiale);

			grp->handlePlayerInput(sf::Keyboard::D, true);
			grp->update(elapsedTime, altView, textures);
			grp->collide(grp->getPos(), grp->getSiz(), elapsedTime, true);
			EXPECT_EQ(grp->getPos().x, position_initiale + PlayerSpeed * elapsedTime.asSeconds());
		}
	}

	TEST(WallTest, playerOnRight_collision) {
		std::string sc = R"(<?xml version="1.0"?>
							<Monde>
								<Niveau label="M1_Niveau1">
									<!-- Crée un élément en position (x,y) de taille (w,h) -->     
									<Player x="0" y="-106" h="106" w="144"/>
									<Pacifier x="-420" y="-196" h="196" w="184"/>
									<Wall x="0" y="0" h="168" w="512" vertical="false"/>
									<Wall x="-512" y="-700" h="700" w="512" vertical="true"/>
								</Niveau>
							</Monde>)";
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_string(sc.c_str());
		ASSERT_TRUE(result) << result.description();

		for (auto const& child : doc.child("Monde"))
		{
			auto grp = std::make_unique<Group>(child);
			float position_initiale = grp->getPos().x;
			EXPECT_EQ(grp->getPos().x, position_initiale);

			grp->handlePlayerInput(sf::Keyboard::Q, false);
			grp->update(elapsedTime, altView, textures);
			grp->collide(grp->getPos(), grp->getSiz(), elapsedTime, true);
			EXPECT_EQ(grp->getPos().x, position_initiale);

			grp->handlePlayerInput(sf::Keyboard::Q, true);
			grp->update(elapsedTime, altView, textures);
			grp->collide(grp->getPos(), grp->getSiz(), elapsedTime, true);
			EXPECT_EQ(grp->getPos().x, position_initiale);
		}
	}

	TEST(WallTest, playerOnRight_noCollision) {
		std::string sc = R"(<?xml version="1.0"?>
							<Monde>
								<Niveau label="M1_Niveau1">
									<!-- Crée un élément en position (x,y) de taille (w,h) -->     
									<Player x="0" y="-106" h="106" w="144"/>
									<Pacifier x="-420" y="-196" h="196" w="184"/>
									<Wall x="0" y="0" h="168" w="512" vertical="false"/>
									<Wall x="-550" y="-700" h="700" w="512" vertical="true"/>
								</Niveau>
							</Monde>)";
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_string(sc.c_str());
		ASSERT_TRUE(result) << result.description();

		for (auto const& child : doc.child("Monde"))
		{
			auto grp = std::make_unique<Group>(child);
			float position_initiale = grp->getPos().x;
			EXPECT_EQ(grp->getPos().x, position_initiale);

			grp->handlePlayerInput(sf::Keyboard::Q, false);
			grp->update(elapsedTime, altView, textures);
			grp->collide(grp->getPos(), grp->getSiz(), elapsedTime, true);
			EXPECT_EQ(grp->getPos().x, position_initiale);

			grp->handlePlayerInput(sf::Keyboard::Q, true);
			grp->update(elapsedTime, altView, textures);
			grp->collide(grp->getPos(), grp->getSiz(), elapsedTime, true);
			EXPECT_EQ(grp->getPos().x, position_initiale - PlayerSpeed * elapsedTime.asSeconds());
		}
	}

	TEST(WallTest, playerAbove_noCollision) {
		std::string sc = R"(<?xml version="1.0"?>
							<Monde>
								<Niveau label="M1_Niveau1">
									<!-- Crée un élément en position (x,y) de taille (w,h) -->     
									<Player x="0" y="0" h="106" w="144"/>
									<Pacifier x="-420" y="-196" h="196" w="184"/>
								</Niveau>
							</Monde>)";
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_string(sc.c_str());
		ASSERT_TRUE(result) << result.description();

		for (auto const& child : doc.child("Monde"))
		{
			auto grp = std::make_unique<Group>(child);
			float position_initiale = grp->getPos().y;
			EXPECT_EQ(grp->getPos().y, position_initiale);

			grp->handlePlayerInput(sf::Keyboard::S, true);
			grp->update(elapsedTime, altView, textures);
			grp->collide(grp->getPos(), grp->getSiz(), elapsedTime, true);
			EXPECT_TRUE(grp->getPos().y > position_initiale);
		}
	}
}