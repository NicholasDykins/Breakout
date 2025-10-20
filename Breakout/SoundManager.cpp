
#include "SoundManager.h"
#include <algorithm>
#include <random>


bool SoundManager::initiate()
{
	bool ok = true;

	ok &= _bufferWallBounce.loadFromFile("assets/sfx/boing-101318.wav");

	ok &= _bufferPaddleBounce.loadFromFile("assets/sfx/boing-101318.wav");

	ok &= _bufferBrickBounce.loadFromFile("assets/sfx/bang-43964.wav");

	ok &= _bufferFireBallStart.loadFromFile("assets/sfx/fire-effect-367659.wav");

	for (auto& v : _voices)
	{
		v.setVolume(45.f);
		v.setAttenuation(0.f);
		
		
		//return ok;
	}
	return ok;

}

sf::Sound& SoundManager::getFreeVoice()
{
	for (auto& v : _voices)
	{
		if (v.getStatus() != sf::Sound::Playing)
		{
			return v;
		}
	}

	
}

static float randRange(std::mt19937& rng, float a, float b) 
{
	std::uniform_real_distribution<float> d(a, b);
	return d(rng);
}

void SoundManager::bounceWall() 
{
	auto& v = getFreeVoice();
	v.setBuffer(_bufferWallBounce);
	v.setPitch(randRange(_rng, 0.95f, 1.05f));  // slight variation feels nicer
	v.play();
}
void SoundManager::bouncePaddle() 
{
	auto& v = getFreeVoice();
	v.setBuffer(_bufferPaddleBounce);
	v.setPitch(randRange(_rng, 0.98f, 1.08f));
	v.play();
}
void SoundManager::bounceBrick() 
{
	auto& v = getFreeVoice();
	v.setBuffer(_bufferBrickBounce);
	v.setPitch(randRange(_rng, 0.95f, 1.12f));
	v.play();
}

void SoundManager::fireBallStart() 
{
	auto& v = getFreeVoice();
	v.setBuffer(_bufferFireBallStart);
	v.setPitch(randRange(_rng, 0.95f, 1.12f));
	v.play();
}

void SoundManager::fireBallEnd()
{
	auto& v = getFreeVoice();
	v.setBuffer(_bufferBrickBounce);
	v.setPitch(randRange(_rng, 0.95f, 1.12f));
	v.play();
}