#include "Game.hpp"
#include <SFML/Audio.hpp>

int main()
{
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("media/harp.wav"))
        return -1;
    sf::Sound sound;
    sound.setBuffer(buffer);
    sound.play();

	Game game;
    game.run();
    return 0;
}
