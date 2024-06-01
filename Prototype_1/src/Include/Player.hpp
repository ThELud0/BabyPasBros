#include "Entity.hpp"


class Player : Entity {
public:
    Player(float x, float y);
    void                    setTexture(const sf::Texture& initTexture);
    void					drawCurrent(sf::RenderWindow& window) const;
    void					handlePlayerInput(const sf::Keyboard::Key& key, const bool& isPressed);
    void					update(const sf::Time& elapsedTime, sf::View& view, std::map<std::string, sf::Texture>& babyTextures);
    sf::Sprite              getSelf();

private:
    float                   PlayerSpeed = 200.f;
    sf::Vector2f            mSpeed;
    sf::Sprite			    mChar;
    bool					mIsMovingUp{ false };
    bool					mIsMovingDown{ false };
    bool					mIsMovingRight{ false };
    bool					mIsMovingLeft{ false };
};