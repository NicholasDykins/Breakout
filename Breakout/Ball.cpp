#include "Ball.h"
#include "GameManager.h" // avoid cicular dependencies
#include "SoundManager.h"

Ball::Ball(sf::RenderWindow* window, float velocity, GameManager* gameManager)
    : _window(window), _velocity(velocity), _gameManager(gameManager),
    _timeWithPowerupEffect(0.f), _isFireBall(false), _isAlive(true), _direction({1,1})
{
    _sprite.setRadius(RADIUS);
    _sprite.setFillColor(sf::Color::Cyan);
    _sprite.setPosition(0, 300);
    
}

Ball::~Ball()
{
}

void Ball::update(float dt)
{
    // check for powerup, tick down or correct
    if (_timeWithPowerupEffect > 0.f)
    {
        _timeWithPowerupEffect -= dt;
    }
    else
    {
        if (_velocity != VELOCITY)
            _velocity = VELOCITY;   // reset speed.
        else
        {
            setFireBall(0);    // disable fireball
            _sprite.setFillColor(sf::Color::Cyan);  // back to normal colour.       
        }        
    }

    static float currentIntensity = 1.0f;  //Base line of intensity for fireball.

    // Fireball effect
    if (_isFireBall)
    {
        // Flickering effect
        
        float flicker = (rand() % 101)/100.0f * 1.4f; // Random value between 0.0 and 1.4

        currentIntensity += (flicker - currentIntensity)*0.2;  //Smooth more real fire
                    
        //                                                         Red                                                Green                                    Blue
        _sprite.setFillColor(sf::Color(static_cast<sf::Uint8>(255 * currentIntensity), static_cast<sf::Uint8>(120 * currentIntensity), static_cast<sf::Uint8>(30 * currentIntensity))); // fire flickering color
    }

    // Update position with a subtle floating-point error
    _sprite.move(_direction * _velocity * dt);

    // check bounds and bounce
    sf::Vector2f position = _sprite.getPosition();
    sf::Vector2u windowDimensions = _window->getSize();

    // bounce on walls
    if ((position.x >= windowDimensions.x - 2 * RADIUS && _direction.x > 0) || (position.x <= 0 && _direction.x < 0))
    {

        if ( _direction.x < 0 ||  _direction.x > 0)
        {
            if (auto* sfx = _gameManager->getSFX())   
            {
                sfx->bounceWall();                         //Play the bounce sound when the wall is hit
            }
        }
        _direction.x *= -1;
    }

    // bounce on ceiling
    if (position.y <= 0 && _direction.y < 0)
    {
        if (_direction.y < 0.f)
        {
            if (auto* sfx = _gameManager->getSFX())
            {
                sfx->bounceWall();
            }
        }

        _direction.y *= -1;
    }

    // lose life bounce
    if (position.y > windowDimensions.y)
    {
        _sprite.setPosition(0, 300);
        _direction = { 1, 1 };
        _gameManager->loseLife();
    }

    // collision with paddle
    if (_sprite.getGlobalBounds().intersects(_gameManager->getPaddle()->getBounds()))
    {
        if (_direction.y > 0.f) 
        {
            if (auto* sfx = _gameManager->getSFX()) 
            {
                sfx->bouncePaddle();
            }
        }



        _direction.y *= -1; // Bounce vertically

        float paddlePositionProportion = (_sprite.getPosition().x - _gameManager->getPaddle()->getBounds().left) / _gameManager->getPaddle()->getBounds().width;
        _direction.x = paddlePositionProportion * 2.0f - 1.0f;

        // Adjust position to avoid getting stuck inside the paddle
        _sprite.setPosition(_sprite.getPosition().x, _gameManager->getPaddle()->getBounds().top - 2 * RADIUS);


    }

    // collision with bricks
    int collisionResponse = _gameManager->getBrickManager()->checkCollision(_sprite, _direction);
    if (_isFireBall)
    {
        return; // no collisisons when in fireBall mode.
    }
        
    if (collisionResponse == 1)
    {

            if (auto* sfx = _gameManager->getSFX())
            {
                sfx->bounceBrick();
            }
        
        _direction.x *= -1; // Bounce horizontally
    }
    else if (collisionResponse == 2)
    {

        if (auto* sfx = _gameManager->getSFX())
        {
            sfx->bounceBrick();
        }
        _direction.y *= -1; // Bounce vertically
    }
}

void Ball::render()
{
    _window->draw(_sprite);
}

void Ball::setVelocity(float coeff, float duration)
{
    _velocity = coeff * VELOCITY;
    _timeWithPowerupEffect = duration;
}

void Ball::setFireBall(float duration)
{
    if (duration> 0.f) 
    {
        if (!_isFireBall)
        {
            _isFireBall = true;
            _timeWithPowerupEffect = duration;

            if (auto* sfx = _gameManager->getSFX())              //This block checks that the sound clip is not already playing to provent it overloading the voice that has 12 available slots, it only activates the sound at the start of the power up
            {
                sfx->fireBallStart();
            }
            else
            {
                _timeWithPowerupEffect = duration;
            }
            return;
        }


        if (_isFireBall)
        {
            _isFireBall = false;
            _timeWithPowerupEffect = 0.f;
        }
        
        
        //if (auto* sfx = _gameManager->getSFX())
        //{
        //    sfx->fireBallEnd();
        //}

        
    }
    _isFireBall = false;
    _timeWithPowerupEffect = 0.f;
    return;
     
}
