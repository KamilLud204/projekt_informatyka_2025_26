#include "Stone.h"

const std::array<sf::Color, 4> Stone::m_colorLUT = {
    sf::Color::Transparent, // 0
    sf::Color::Red,         // 1
    sf::Color::Yellow,      // 2
    sf::Color::Blue         // 3
};

Stone::Stone(sf::Vector2f startPos, sf::Vector2f size, int L)
    : m_punktyZycia(L), m_jestZniszczony(false)
{
    this->setPosition(startPos);
    this->setSize(size);
    this->setOutlineThickness(2.f);
    this->setOutlineColor(sf::Color::Black);

    aktualizujKolor();
}

void Stone::trafienie() {
    if (m_jestZniszczony) return;

    m_punktyZycia--;
    if (m_punktyZycia <= 0) {
        m_jestZniszczony = true;
    }

    aktualizujKolor();
}

void Stone::aktualizujKolor() {
    int hp = m_punktyZycia;

    if (hp < 0) hp = 0;
    if (hp > 3) hp = 3;

    this->setFillColor(m_colorLUT[hp]);
}

void Stone::draw(sf::RenderTarget& target) const {
    if (!m_jestZniszczony) {
        target.draw(static_cast<sf::RectangleShape>(*this));
    }
}