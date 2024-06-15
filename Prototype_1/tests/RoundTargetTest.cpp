#include <gtest/gtest.h>

#include "RoundTarget.hpp"

namespace RoundTarget_test {

    static const sf::Time elapsedTime = sf::seconds(1.f / 60.f);
    constexpr auto notScreenBoundary = 200.f;
    constexpr auto radius = 50.f;
    constexpr auto speed = 100.f;
    const sf::View altView{ sf::Vector2f(0.f,0.f),sf::Vector2f(1000.f, 1000.f) };

    TEST(RoundTargetTest, update_noBounce) {
        RoundTarget target{radius, sf::Color::Cyan, notScreenBoundary, notScreenBoundary, speed, speed };

        target.update(elapsedTime, altView);
        EXPECT_FLOAT_EQ(notScreenBoundary + speed * elapsedTime.asSeconds(), target.getShape().getPosition().x);
        EXPECT_FLOAT_EQ(notScreenBoundary + speed * elapsedTime.asSeconds(), target.getShape().getPosition().y);

        target.update(elapsedTime, altView);
        EXPECT_FLOAT_EQ(notScreenBoundary + 2 * speed * elapsedTime.asSeconds(), target.getShape().getPosition().x);
        EXPECT_FLOAT_EQ(notScreenBoundary + 2 * speed * elapsedTime.asSeconds(), target.getShape().getPosition().y);
    }

    TEST(RoundTargetTest, update_bounceLeft) {
        RoundTarget target{ radius, sf::Color::Cyan, -500.f, notScreenBoundary, -speed, -speed };

        target.update(elapsedTime, altView);
        EXPECT_FLOAT_EQ(-500.f + speed * elapsedTime.asSeconds(), target.getShape().getPosition().x);
        EXPECT_FLOAT_EQ(notScreenBoundary - speed * elapsedTime.asSeconds(), target.getShape().getPosition().y);

        target.update(elapsedTime, altView);
        EXPECT_FLOAT_EQ(-500.f + 2 * speed * elapsedTime.asSeconds(), target.getShape().getPosition().x);
        EXPECT_FLOAT_EQ(notScreenBoundary - 2 * speed * elapsedTime.asSeconds(), target.getShape().getPosition().y);
    }

    TEST(RoundTargetTest, update_bounceUp) {
        RoundTarget target{ radius, sf::Color::Cyan, notScreenBoundary, -500.f, speed, -speed };

        target.update(elapsedTime, altView);
        EXPECT_FLOAT_EQ(notScreenBoundary + speed * elapsedTime.asSeconds(), target.getShape().getPosition().x);
        EXPECT_FLOAT_EQ(-500.f + speed * elapsedTime.asSeconds(), target.getShape().getPosition().y);

        target.update(elapsedTime, altView);
        EXPECT_FLOAT_EQ(notScreenBoundary + 2 * speed * elapsedTime.asSeconds(), target.getShape().getPosition().x);
        EXPECT_FLOAT_EQ(-500.f + 2 * speed * elapsedTime.asSeconds(), target.getShape().getPosition().y);
    }

    TEST(RoundTargetTest, update_bounceRight) {
        RoundTarget target{ radius, sf::Color::Cyan, altView.getCenter().x + altView.getSize().x/2 - 2 * radius, notScreenBoundary, speed, speed};

        target.update(elapsedTime, altView);
        EXPECT_FLOAT_EQ(altView.getCenter().x + altView.getSize().x / 2 - 2 * target.getShape().getRadius() - speed * elapsedTime.asSeconds(), target.getShape().getPosition().x);
        EXPECT_FLOAT_EQ(notScreenBoundary + speed * elapsedTime.asSeconds(), target.getShape().getPosition().y);

        target.update(elapsedTime, altView);
        EXPECT_FLOAT_EQ(altView.getCenter().x + altView.getSize().x / 2 - 2 * target.getShape().getRadius() - 2 * speed * elapsedTime.asSeconds(), target.getShape().getPosition().x);
        EXPECT_FLOAT_EQ(notScreenBoundary + 2 * speed * elapsedTime.asSeconds(), target.getShape().getPosition().y);
    }

    TEST(RoundTargetTest, update_bounceBottom) {
        RoundTarget target{ radius, sf::Color::Cyan, notScreenBoundary, altView.getCenter().y + altView.getSize().y / 2 - 2 * radius, speed, speed };

        target.update(elapsedTime, altView);
        EXPECT_FLOAT_EQ(notScreenBoundary + speed * elapsedTime.asSeconds(), target.getShape().getPosition().x);
        EXPECT_FLOAT_EQ(altView.getCenter().y + altView.getSize().y / 2 - 2 * target.getShape().getRadius() - speed * elapsedTime.asSeconds(), target.getShape().getPosition().y);

        target.update(elapsedTime, altView);
        EXPECT_FLOAT_EQ(notScreenBoundary + 2 * speed * elapsedTime.asSeconds(), target.getShape().getPosition().x);
        EXPECT_FLOAT_EQ(altView.getCenter().y + altView.getSize().y / 2 - 2 * target.getShape().getRadius() - 2 * speed * elapsedTime.asSeconds(), target.getShape().getPosition().y);
    }

    TEST(RoundTargetTest, isHitByMouse_true) {
        RoundTarget target{ radius, sf::Color::Cyan, notScreenBoundary, notScreenBoundary, speed, speed };
        sf::Vector2f mousePosition{ notScreenBoundary + radius / 2.f, notScreenBoundary + radius / 2.f };
        
        EXPECT_TRUE( target.isHitByMouse(mousePosition));
        
    }

    TEST(RoundTargetTest, isHitByMouse_false) {
        RoundTarget target{ radius, sf::Color::Cyan, notScreenBoundary, notScreenBoundary, speed, speed };

        sf::Vector2f mousePosition{ notScreenBoundary + 2.f * radius + 1.f, notScreenBoundary + radius / 2.f };

        EXPECT_FALSE(target.isHitByMouse(mousePosition));

    }

    

}