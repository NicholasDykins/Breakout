#include "GameManager.h"
#include "Ball.h"
#include "PowerupManager.h"
#include <iostream>
#include "SoundManager.h"

GameManager::GameManager(sf::RenderWindow* window)
    : _window(window), _paddle(nullptr), _ball(nullptr), _brickManager(nullptr), _powerupManager(nullptr),
    _messagingSystem(nullptr), _ui(nullptr), _pause(false), _time(0.f), _lives(3), _pauseHold(0.f), _levelComplete(false),
    _powerupInEffect({ none,0.f }), _timeLastPowerupSpawned(0.f)
{
    _font.loadFromFile("font/montS.ttf");
    _masterText.setFont(_font);
    _masterText.setPosition(50, 400);
    _masterText.setCharacterSize(48);
    _masterText.setFillColor(sf::Color::Yellow);

}

void GameManager::initialize()
{
    _paddle = new Paddle(_window);
    _brickManager = new BrickManager(_window, this);
    _messagingSystem = new MessagingSystem(_window);
    _ball = new Ball(_window, 400.0f, this); 
    _powerupManager = new PowerupManager(_window, _paddle, _ball);
    _ui = new UI(_window, _lives, this);

    // Create bricks
    _brickManager->createBricks(5, 10, 80.0f, 30.0f, 5.0f);

    
    _sfx = std::make_unique<SoundManager>();
    
    if (!_sfx->initiate()) 
    {
        
    }

}

void GameManager::restartLevel()
{
    _levelComplete = false;
    _masterText.setString("");

    _powerupManager->clear();

    _brickManager->clear();

    _brickManager->createBricks(5, 10, 80.0f, 30.0f, 5.0f);

    _paddle->resetToCenter();

    //_ball->reset({ _paddle->getBounds().left + _paddle->getBounds().width * 0.5f, _paddle->getBounds().top - 40.f });
    _ball->reset();

    _time = 0.f;
    _timeLastPowerupSpawned = 0.f;
}

void GameManager::update(float dt)
{
    _powerupInEffect = _powerupManager->getPowerupInEffect();
    _ui->updatePowerupText(_powerupInEffect);
    _powerupInEffect.second -= dt;


    if (_lives <= 0)
    {
        _masterText.setString("Game Over — [R] Restart Game");


        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) 
        { 
            restartLevel(); 
            _lives = 3;
            _ui->reLives(_lives);
        }
        return;
    }
    if (_levelComplete)
    {
        _masterText.setString("Level completed — [R] Restart Game");

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
        {
            restartLevel();
            _lives = 3;
            _ui->reLives(_lives);
        }
        return;
    }
    // pause and pause handling
    if (_pauseHold > 0.f) _pauseHold -= dt;


    if (sf::Keyboard::isKeyPressed(sf::Keyboard::P) && _pauseHold <= 0.f) 
    {
        _pause = !_pause;
        _pauseHold = PAUSE_TIME_BUFFER;
        _masterText.setString(_pause ? "Paused — [R] Restart Level   [P] Resume" : "");
    }


    if (_pause) 
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) 
        {
            restartLevel();
            _pause = false;
            _masterText.setString("");
            _pauseHold = PAUSE_TIME_BUFFER; 
            _lives = 3;
            _ui->reLives(_lives);
        }
        return;
    }


        // timer.
        _time += dt;


        if (_time > _timeLastPowerupSpawned + POWERUP_FREQUENCY && rand() % 700 == 0)      // TODO parameterise
        {
            _powerupManager->spawnPowerup();
            _timeLastPowerupSpawned = _time;
        }

        //// move paddle
        //if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) _paddle->moveRight(dt);   //Using Mouse so not needed
        //if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) _paddle->moveLeft(dt);






        // update everything 
       
    
    else
    {
        _paddle->update(dt);
        _ball->update(dt);
        _powerupManager->update(dt);
    }
}




void GameManager::loseLife()
{
    _lives--;
    _ui->lifeLost(_lives);

    // TODO screen shake.
}

void GameManager::render()
{
    _paddle->render();
    _ball->render();
    _brickManager->render();
    _powerupManager->render();
    _window->draw(_masterText);
    _ui->render();
}

void GameManager::levelComplete()
{
    _levelComplete = true;
}

sf::RenderWindow* GameManager::getWindow() const { return _window; }
UI* GameManager::getUI() const { return _ui; }
Paddle* GameManager::getPaddle() const { return _paddle; }
BrickManager* GameManager::getBrickManager() const { return _brickManager; }
PowerupManager* GameManager::getPowerupManager() const { return _powerupManager; }


GameManager::~GameManager() = default;