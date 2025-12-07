#pragma once
#include <SFML/Graphics.hpp>
#include "Paletka.hpp"

class Pilka
{
private:
    float x, y;
    float vx, vy;
    float radius;
    sf::CircleShape shape;

public:
    Pilka(float px, float py, float vx_, float vy_, float r);

    void move();
    void bounceX();
    void bounceY();
    void collideWalls(float width, float height);
    bool collidePaddle(const Paletka& p);
    void draw(sf::RenderTarget& target);
   

    float getX() const;
    float getY() const;
    float getVx() const;
    float getVy() const;
    float getRadius() const;
    void reset(const sf::Vector2f& pos, const sf::Vector2f& vel);
};


