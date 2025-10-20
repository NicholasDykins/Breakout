#pragma once

#include <SFML/Audio.hpp>
#include <array>
#include <random>
#include <cstddef>


class SoundManager
{

public:
	bool initiate();
	void bounceWall();
	void bouncePaddle();
	void bounceBrick();
	void fireBallStart();
	void fireBallEnd();


private:

	sf::Sound& getFreeVoice();

	sf::SoundBuffer _bufferWallBounce, _bufferPaddleBounce, _bufferBrickBounce, _bufferFireBallStart, _bufferFireBallEnd; //Creating the buffer to store the sound clips

	std::array<sf::Sound, 12> _voices;  //Makes sure sounds aren't cut off

	std::mt19937 _rng{ std::random_device{}() };  //Beter random gen than rand()

	std::size_t _nextVoice = 0;

};

