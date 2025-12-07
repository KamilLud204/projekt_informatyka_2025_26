
#pragma once
#include <SFML/Graphics.hpp>


class Paletka
{
private:
    float x;
    float y;
    float szerokosc;
    float wysokosc;
    float predkosc;
    sf::RectangleShape shape;

public:
    Paletka(float px, float py, float s, float w, float p);

    void moveLeft();
    void moveRight();

    
    void clampToBounds(float width);

    void draw(sf::RenderTarget& target);

    float getX() const;
    float getY() const;
    float getSzerokosc() const;
    float getWysokosc() const;
    void setPosition(const sf::Vector2f& pos);
};

