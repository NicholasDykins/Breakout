#include "Paddle.h"
#include <iostream>

Paddle::Paddle(sf::RenderWindow* window)
    : _window(window), _width(PADDLE_WIDTH), _timeInNewSize(0.0f), _isAlive(true)
{
    _sprite.setFillColor(sf::Color::Cyan);
    _sprite.setPosition((window->getSize().x - _width) / 2.0f, window->getSize().y - 50.0f);
    _sprite.setSize(sf::Vector2f(_width, PADDLE_HEIGHT));
}

Paddle::~Paddle()
{

}


void Paddle::resetToCenter() //reset paddle to center of the screen
{
    _sprite.setPosition((_window->getSize().x - PADDLE_WIDTH) * 0.5f, _window->getSize().y - 50.f);
}

void Paddle::moveLeft(float dt)
{
    float position = _sprite.getPosition().x;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && position > 0)
    {
        _sprite.move(sf::Vector2f(-dt * PADDLE_SPEED, 0));
    }
}

void Paddle::moveRight(float dt)
{
    float position = _sprite.getPosition().x;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && position < _window->getSize().x - _width)
    {
        _sprite.move(sf::Vector2f(dt * PADDLE_SPEED, 0));
    }
}



void Paddle::update(float dt)
{
    if (_timeInNewSize > 0)
    {
        _timeInNewSize -= dt;
    }
    else if(_width != PADDLE_WIDTH)   // Changed to an else if so that it doesn't reset the size every frame
    {
        setWidth(1.0f, 0.0f); // Reset to default width after duration
    }


    

    sf::Vector2i mousePosition = sf::Mouse::getPosition(*_window);  //Get's mouse position on window

    float currentX = _sprite.getPosition().x; //Trak the current x position of the paddle 

    float posX = static_cast<float>(mousePosition.x) - (_width * 0.5f); //sets the x position of the paddle to the mouse position - half the width to centralise it

    float lerpSpeed = 1.f;  //Speed that can be adjusted for smothing the movment

    float newX = currentX + (posX - currentX) * lerpSpeed * dt;

    posX = std::clamp(newX, 0.0f, static_cast<float>(_window->getSize().x) - _width); //clamps the position of the pad from moving outside of the visable window.
 
    _sprite.setPosition(posX, _sprite.getPosition().y);    //Sets the sprite position to the mouse

}

void Paddle::render()
{
    _window->draw(_sprite);
}

sf::FloatRect Paddle::getBounds() const
{
    return _sprite.getGlobalBounds();
}

// update width by SF of coeff. 
// ensure centre remains consistent.
void Paddle::setWidth(float coeff, float duration)
{
    _width = coeff * PADDLE_WIDTH;
    _sprite.setSize(sf::Vector2f(_width, _sprite.getSize().y));
    _timeInNewSize = duration;
    float newX = _sprite.getPosition().x + (_width - PADDLE_WIDTH) / 2;
    _sprite.setPosition(newX, _sprite.getPosition().y);
}
