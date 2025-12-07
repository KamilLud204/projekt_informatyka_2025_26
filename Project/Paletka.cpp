#include "Paletka.hpp"

Paletka::Paletka(float px, float py, float s, float w, float p)
    : x(px), y(py), szerokosc(s), wysokosc(w), predkosc(p)
{
    
    shape.setSize(sf::Vector2f(szerokosc, wysokosc));
    shape.setOrigin(szerokosc / 2.f, wysokosc / 2.f);
    shape.setPosition(x, y);
    shape.setFillColor(sf::Color(0, 200, 220)); 
}

void Paletka::moveLeft()
{
    x -= predkosc;
    shape.setPosition(x, y);
}

void Paletka::moveRight()
{
    x += predkosc;
    shape.setPosition(x, y);
}

void Paletka::clampToBounds(float width)
{
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        moveLeft();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        moveRight();
    }

    
    float half = szerokosc / 2.f;
    if (x - half < 0.f) x = half;
    if (x + half > width) x = width - half;

    
    shape.setPosition(x, y);
}
void Paletka::setPosition(const sf::Vector2f& pos)
{
    x = pos.x;
    y = pos.y;
    shape.setPosition(pos);
}


void Paletka::draw(sf::RenderTarget& target)
{
    target.draw(shape);
}

float Paletka::getX() const { return x; }
float Paletka::getY() const { return y; }
float Paletka::getSzerokosc() const { return szerokosc; }
float Paletka::getWysokosc() const { return wysokosc; }
