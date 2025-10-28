#include "PowerupManager.h"


PowerupManager::PowerupManager(sf::RenderWindow* window, Paddle* paddle, Ball* ball)
    : _window(window), _paddle(paddle), _ball(ball)
{
}

PowerupManager::~PowerupManager()
{
    for (auto powerup : _powerups)
    {
        delete powerup;
    }
    _powerups.clear();
}

void PowerupManager::clear() 
{
    for (auto* p : _powerups)
    {
        delete p;
    }
    _powerups.clear();
    _powerupInEffect.reset();
}

void PowerupManager::update(float dt)
{
    // tick down powerup effect time. Reset if elapsed.
    if (_powerupInEffect)
    {
        _powerupInEffect->second -= dt;
        if (_powerupInEffect->second <= 0)
        {
            _powerupInEffect.reset();
        }
    }


    for (auto it = _powerups.begin(); it != _powerups.end(); )
    {
        checkCollision();
        
        // Delete powerups queued for removal
        (*it)->update(dt);
        if (!(*it)->isAlive())
        {
            delete* it;
            it = _powerups.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void PowerupManager::render()
{
    for (auto& powerup : _powerups)
    {
        powerup->render();
    }
}

void PowerupManager::spawnPowerupAt(POWERUPS type, const sf::Vector2f& pos)
{

    // TODO finish this.

    PowerupBase* p = nullptr;


    switch (type)
    {
    case bigPaddle:
        p = new PowerupBigPaddle(_window, _paddle, _ball);
        break;
    case smallPaddle:
        p = new PowerupSmallPaddle(_window, _paddle, _ball);
        break;
    case fastBall:
        p = new PowerupFastBall(_window, _paddle, _ball);
        break;
    case slowBall:
        p = new PowerupSlowBall(_window, _paddle, _ball);
        break;
    case fireBall:
        p = new PowerupFireBall(_window, _paddle, _ball);
        break;

    default: 
        break;
    //case 5:  //This should not have been included as it will only ever get to between 0 and 4.
    //   break;
    }

    if (p) 
    {
        // need a setter to place it at the brick — next step
        p->setPosition(pos);
        _powerups.push_back(p);
    }
}

void PowerupManager::checkCollision()
{
    for (auto& powerup : _powerups)
    {

        if (powerup->checkCollisionWithPaddle())
        {
            _powerupInEffect = powerup->applyEffect();
            powerup->setAlive(false);
        }
    }
}

int PowerupManager::getPowerupsSpawned()
{
    return _powerups.size();
}

std::pair<POWERUPS, float> PowerupManager::getPowerupInEffect()
{
    if (!_powerupInEffect) return { none, 0.f };
    return *_powerupInEffect;
}
