#include "Pilka.hpp"
#include <cmath>
#include <iostream>

Pilka::Pilka(float px, float py, float vx_, float vy_, float r)
    : x(px), y(py), vx(vx_), vy(vy_), radius(r)

{
    shape.setRadius(radius);
    shape.setOrigin(radius, radius);
    shape.setPosition(x, y);
    shape.setFillColor(sf::Color::White);
}

void Pilka::move()
{
    x += vx;
    y += vy;
    shape.setPosition(x, y);
}

void Pilka::bounceX() { vx = -vx; }
void Pilka::bounceY() { vy = -vy; }

void Pilka::collideWalls(float width, float height)
{
    
    if (x - radius <= 0.f)
    {
        x = radius;
        bounceX();
    }

    
    if (x + radius >= width)
    {
        x = width - radius;
        bounceX();
    }

   
    if (y - radius <= 0.f)
    {
        y = radius;
        bounceY();
    }
    shape.setPosition(x, y);
}

bool Pilka::collidePaddle(const Paletka& p)
{
    float palX = p.getX();
    float palY = p.getY();
    float palW = p.getSzerokosc();
    float palH = p.getWysokosc();

    float palLeft = palX - palW / 2.f;
    float palRight = palX + palW / 2.f;
    float palTop = palY - palH / 2.f;
    float palBottom = palY + palH / 2.f;

    
    if (y + radius < palTop && vy > 0) {
        
        float nextY = y + vy;

        if (nextY + radius >= palTop) {
            
            if (x + radius >= palLeft && x - radius <= palRight) {
                vy = -std::abs(vy);
                y = palTop - radius;

                
                float hitPos = (x - palX) / (palW / 2.f);
                vx += hitPos * 1.5f;

                
                if (vx > 8.0f) vx = 8.0f;
                if (vx < -8.0f) vx = -8.0f;

                shape.setPosition(x, y);
                return true;
            }
        }
    }

    return false;
}

void Pilka::reset(const sf::Vector2f& pos, const sf::Vector2f& vel)
{
    x = pos.x;
    y = pos.y;

    vx = vel.x;
    vy = vel.y;

    shape.setPosition(x, y);
    shape.setOrigin(radius, radius);
}

void Pilka::draw(sf::RenderTarget& target)
{
    target.draw(shape);
}

float Pilka::getX() const { return x; }
float Pilka::getY() const { return y; }
float Pilka::getVx() const { return vx; }
float Pilka::getVy() const { return vy; }
float Pilka::getRadius() const { return radius; }
