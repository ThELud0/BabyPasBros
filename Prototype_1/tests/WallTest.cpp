#include <gtest/gtest.h>

#include "Wall.hpp"
#include "Player.hpp"
#include "Group.hpp"

namespace Wall_test {



    TEST(WallTest, playerOnLeft_collision) {

		std::string sc = R"(<?xml version="1.0"?>
	<Monde>
		<Niveau label="M1_Niveau1">
			<!-- Crée un élément en position (x,y) de taille (w,h) -->     
			<Player x="200" y="-106" h="106" w="144"/>
			<Pacifier x="-42" y="-196" h="196" w="184"/>
			<Wall x="0" y="0" h="168" w="512" vertical="false"/>
			<Wall x="512" y="700" h="168" w="512" vertical="true"/>
		</Niveau>
	</Monde>)";
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_string(sc.c_str());
        ASSERT_TRUE(result) << result.description();
		for (auto const& child : doc.child("Monde"))
		{
			auto grp = std::make_unique<Group>(child);
		}

        std::string c_dump_ref = "Circle \"testCircle\", x: 0, y: 1, r: 2, color: Black\n";

    }

}