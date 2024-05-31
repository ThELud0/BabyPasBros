#include <gtest/gtest.h>

#include "RoundTarget.hpp"

namespace RoundTarget_test {

    static const sf::Time elapsedTime = sf::seconds(1.f / 60.f);
    constexpr auto notScreenBoundary = 200.f;
    constexpr auto radius = 50.f;
    constexpr auto speed = 100.f;
    const sf::Vector2u windowSize{ 640, 480 };

    TEST(RoundTargetTest, update_noBounce) {
        RoundTarget target{radius, sf::Color::Cyan, notScreenBoundary, notScreenBoundary, speed, speed };

        target.update(elapsedTime, windowSize);
        EXPECT_FLOAT_EQ(notScreenBoundary + speed * elapsedTime.asSeconds(), target.getShape().getPosition().x);
        EXPECT_FLOAT_EQ(notScreenBoundary + speed * elapsedTime.asSeconds(), target.getShape().getPosition().y);

        target.update(elapsedTime, windowSize);
        EXPECT_FLOAT_EQ(notScreenBoundary + 2 * speed * elapsedTime.asSeconds(), target.getShape().getPosition().x);
        EXPECT_FLOAT_EQ(notScreenBoundary + 2 * speed * elapsedTime.asSeconds(), target.getShape().getPosition().y);
    }

    TEST(RoundTargetTest, update_bounceLeft) {
        RoundTarget target{ radius, sf::Color::Cyan, 0.f, notScreenBoundary, -speed, -speed };

        target.update(elapsedTime, windowSize);
        EXPECT_FLOAT_EQ(speed * elapsedTime.asSeconds(), target.getShape().getPosition().x);
        EXPECT_FLOAT_EQ(notScreenBoundary - speed * elapsedTime.asSeconds(), target.getShape().getPosition().y);

        target.update(elapsedTime, windowSize);
        EXPECT_FLOAT_EQ(2 * speed * elapsedTime.asSeconds(), target.getShape().getPosition().x);
        EXPECT_FLOAT_EQ(notScreenBoundary - 2 * speed * elapsedTime.asSeconds(), target.getShape().getPosition().y);
    }

    TEST(RoundTargetTest, update_bounceUp) {
        RoundTarget target{ radius, sf::Color::Cyan, notScreenBoundary, 0.f, speed, -speed };

        target.update(elapsedTime, windowSize);
        EXPECT_FLOAT_EQ(notScreenBoundary + speed * elapsedTime.asSeconds(), target.getShape().getPosition().x);
        EXPECT_FLOAT_EQ(speed * elapsedTime.asSeconds(), target.getShape().getPosition().y);

        target.update(elapsedTime, windowSize);
        EXPECT_FLOAT_EQ(notScreenBoundary + 2 * speed * elapsedTime.asSeconds(), target.getShape().getPosition().x);
        EXPECT_FLOAT_EQ(2 * speed * elapsedTime.asSeconds(), target.getShape().getPosition().y);
    }

    TEST(RoundTargetTest, update_bounceRight) {
        RoundTarget target{ radius, sf::Color::Cyan, static_cast<float>(windowSize.x) - 2 * radius, notScreenBoundary, speed, speed };

        target.update(elapsedTime, windowSize);
        EXPECT_FLOAT_EQ(static_cast<float>(windowSize.x) - 2 * target.getShape().getRadius() - speed * elapsedTime.asSeconds(), target.getShape().getPosition().x);
        EXPECT_FLOAT_EQ(notScreenBoundary + speed * elapsedTime.asSeconds(), target.getShape().getPosition().y);

        target.update(elapsedTime, windowSize);
        EXPECT_FLOAT_EQ(static_cast<float>(windowSize.x) - 2 * target.getShape().getRadius() - 2 * speed * elapsedTime.asSeconds(), target.getShape().getPosition().x);
        EXPECT_FLOAT_EQ(notScreenBoundary + 2 * speed * elapsedTime.asSeconds(), target.getShape().getPosition().y);
    }

    TEST(RoundTargetTest, update_bounceBottom) {
        RoundTarget target{ radius, sf::Color::Cyan, notScreenBoundary, static_cast<float>(windowSize.y) - 2 * radius, speed, speed };

        target.update(elapsedTime, windowSize);
        EXPECT_FLOAT_EQ(notScreenBoundary + speed * elapsedTime.asSeconds(), target.getShape().getPosition().x);
        EXPECT_FLOAT_EQ(static_cast<float>(windowSize.y) - 2 * target.getShape().getRadius() - speed * elapsedTime.asSeconds(), target.getShape().getPosition().y);

        target.update(elapsedTime, windowSize);
        EXPECT_FLOAT_EQ(notScreenBoundary + 2 * speed * elapsedTime.asSeconds(), target.getShape().getPosition().x);
        EXPECT_FLOAT_EQ(static_cast<float>(windowSize.y) - 2 * target.getShape().getRadius() - 2 * speed * elapsedTime.asSeconds(), target.getShape().getPosition().y);
    }

    TEST(RoundTargetTest, isHitByMouse_true) {
        RoundTarget target{ radius, sf::Color::Cyan, notScreenBoundary, notScreenBoundary, speed, speed };
        int mousex = (int)notScreenBoundary + radius / 2;
        int mousey = (int)notScreenBoundary + radius / 2;
        sf::Mouse::setPosition({ mousex, mousey });
        
        EXPECT_TRUE( target.isHitByMouse(sf::Mouse::getPosition()) );
        
    }

    TEST(RoundTargetTest, isHitByMouse_false) {
        RoundTarget target{ radius, sf::Color::Cyan, notScreenBoundary, notScreenBoundary, speed, speed };
        int mousex = (int)notScreenBoundary + 2 * radius + 1;
        int mousey = (int)notScreenBoundary + radius / 2;
        sf::Mouse::setPosition({ mousex, mousey });

        EXPECT_FALSE(target.isHitByMouse(sf::Mouse::getPosition()));

    }

    

}