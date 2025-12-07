#include "Game.h"
#include <iostream>
#include <fstream>
#include <algorithm>

Game::Game()
    : m_pilka(320.f, 300.f, 4.f, -3.f, 8.f),
    m_paletka(320.f, 440.f, 100.f, 20.f, 8.f),
    m_currentState(State::Playing),
    m_frameCounter(0),
    m_menuSelection(0)
{
    reset();
}

float Game::calculateBallStartY() const
{
    return 300.f;
}

void Game::checkGameOver()
{
    if (m_pilka.getY() - m_pilka.getRadius() > HEIGHT + 50) {
        m_currentState = State::GameOver;
        std::cout << "MISS! KONIEC GRY\n";
        return;
    }

    bool allDestroyed = true;
    for (const auto& b : m_bloki) {
        if (!b.isDestroyed()) {
            allDestroyed = false;
            break;
        }
    }

    if (allDestroyed && !m_bloki.empty()) {
        m_currentState = State::GameOver;
        std::cout << "WYGRANA! Wszystkie bloki zniszczone!\n";
    }
}

void Game::reset()
{
    m_bloki.clear();

    const int ILOSC_KOLUMN = 6;
    const int ILOSC_WIERSZY = 7;
    const float ODSTEP = 2.f;
    const float BLOCK_H = 25.f;
    const float BLOCK_W = (800 - (ILOSC_KOLUMN - 1) * ODSTEP) / ILOSC_KOLUMN;

    for (int y = 0; y < ILOSC_WIERSZY; ++y)
    {
        for (int x = 0; x < ILOSC_KOLUMN; ++x)
        {
            float posX = x * (BLOCK_W + ODSTEP);
            float posY = 50.f + y * (BLOCK_H + ODSTEP);

            int L =
                (y < 1) ? 3 :
                (y < 3) ? 2 : 1;

            m_bloki.emplace_back(
                sf::Vector2f(posX, posY),
                sf::Vector2f(BLOCK_W, BLOCK_H),
                L
            );
        }
    }

    float ballStartY = calculateBallStartY();
    m_pilka = Pilka(320.f, ballStartY, 4.f, -3.f, 8.f);
    m_paletka = Paletka(320.f, 440.f, 100.f, 20.f, 8.f);
    m_currentState = State::Playing;
    m_frameCounter = 0;
    m_menuSelection = 0;
}

void Game::update(sf::Time dt)
{
    if (m_currentState != State::Playing) {
        return;
    }

    ++m_frameCounter;

    m_paletka.clampToBounds(WIDTH);
    m_pilka.move();
    m_pilka.collideWalls(WIDTH, HEIGHT);

    if (m_pilka.collidePaddle(m_paletka)) {
        
    }

    
    for (auto& b : m_bloki)
    {
        if (b.isDestroyed()) continue;

        float ballLeft = m_pilka.getX() - m_pilka.getRadius();
        float ballRight = m_pilka.getX() + m_pilka.getRadius();
        float ballTop = m_pilka.getY() - m_pilka.getRadius();
        float ballBottom = m_pilka.getY() + m_pilka.getRadius();

        float blockLeft = b.getPosition().x;
        float blockRight = b.getPosition().x + b.getSize().x;
        float blockTop = b.getPosition().y;
        float blockBottom = b.getPosition().y + b.getSize().y;

        if (ballRight > blockLeft && ballLeft < blockRight &&
            ballBottom > blockTop && ballTop < blockBottom)
        {
            b.trafienie();

            float overlapLeft = ballRight - blockLeft;
            float overlapRight = blockRight - ballLeft;
            float overlapTop = ballBottom - blockTop;
            float overlapBottom = blockBottom - ballTop;

            float minOverlap = std::min({ overlapLeft, overlapRight, overlapTop, overlapBottom });

            if (minOverlap == overlapLeft || minOverlap == overlapRight) {
                m_pilka.bounceX();
            }
            else {
                m_pilka.bounceY();
            }
        }
    }

    checkGameOver();
}

bool Game::saveState(const std::string& filename)
{
    ::GameState state;
    state.capture(m_paletka, m_pilka, m_bloki);
    return state.saveToFile(filename);
}

bool Game::loadState(const std::string& filename)
{
    ::GameState state;
    if (!state.loadFromFile(filename))
        return false;

    m_bloki.clear();
    state.apply(m_paletka, m_pilka, m_bloki);
    m_currentState = State::Playing;

    return true;
}

void Game::renderPauseMenu(sf::RenderTarget& target)
{
    
    sf::RectangleShape overlay(sf::Vector2f(WIDTH, HEIGHT));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    target.draw(overlay);

    
    sf::RectangleShape menuBackground(sf::Vector2f(300, 200));
    menuBackground.setFillColor(sf::Color(50, 50, 50, 200));
    menuBackground.setPosition(250, 200);
    target.draw(menuBackground);

    static sf::Font font;
    static bool fontLoaded = false;

    if (!fontLoaded) {
        fontLoaded = true;
        if (!font.loadFromFile("arial.ttf")) {
            return;
        }
    }

    
    sf::Text title;
    title.setFont(font);
    title.setCharacterSize(36);
    title.setFillColor(sf::Color::Yellow);
    title.setString("MENU PAUZY");
    title.setPosition(310, 210);
    target.draw(title);

    
    for (size_t i = 0; i < m_menuItems.size(); ++i) {
        sf::Text menuItem;
        menuItem.setFont(font);
        menuItem.setCharacterSize(28);
        menuItem.setString(m_menuItems[i]);
        menuItem.setPosition(280, 260 + i * 40);

        if (i == m_menuSelection) {
            menuItem.setFillColor(sf::Color::Cyan);
        }
        else {
            menuItem.setFillColor(sf::Color::White);
        }

        target.draw(menuItem);
    }
}

void Game::renderGameOver(sf::RenderTarget& target)
{
    static sf::Font font;
    static bool fontLoaded = false;

    if (!fontLoaded) {
        fontLoaded = true;
        if (!font.loadFromFile("arial.ttf")) {
            return;
        }
    }

    
    sf::RectangleShape overlay(sf::Vector2f(WIDTH, HEIGHT));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    target.draw(overlay);

    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(50);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setString("GAME OVER");
    gameOverText.setPosition(280, 200);
    target.draw(gameOverText);

    sf::Text restartText;
    restartText.setFont(font);
    restartText.setCharacterSize(30);
    restartText.setFillColor(sf::Color::White);
    restartText.setString("Press R to restart\nESC to menu");
    restartText.setPosition(280, 280);
    target.draw(restartText);
}

void Game::render(sf::RenderTarget& target)
{
    m_paletka.draw(target);
    m_pilka.draw(target);

    for (auto& b : m_bloki)
        b.draw(target);

    if (m_currentState == State::Paused) {
        renderPauseMenu(target);
    }

    if (m_currentState == State::GameOver) {
        renderGameOver(target);
    }
}

void Game::handleMenuInput(sf::Keyboard::Key key)
{
    if (key == sf::Keyboard::Up) {
        moveMenuUp();
    }
    else if (key == sf::Keyboard::Down) {
        moveMenuDown();
    }
    else if (key == sf::Keyboard::Enter) {
        bool returnToMainMenu = false;
        executeMenuAction(returnToMainMenu);
    }
}

void Game::moveMenuUp()
{
    m_menuSelection--;
    if (m_menuSelection < 0) {
        m_menuSelection = static_cast<int>(m_menuItems.size()) - 1;
    }
}

void Game::moveMenuDown()
{
    m_menuSelection++;
    if (m_menuSelection >= static_cast<int>(m_menuItems.size())) {
        m_menuSelection = 0;
    }
}

void Game::executeMenuAction(bool& returnToMainMenu)
{
    switch (m_menuSelection) {
    case 0:  
        togglePause();
        std::cout << "Wznowiono gre\n";
        break;

    case 1:  
        if (saveState("save.json")) {
            std::cout << "Gra zapisana pomyslnie z menu\n";
        }
        else {
            std::cout << "Nie udalo sie zapisac gry\n";
        }
        break;

    case 2:  
        if (loadState("save.json")) {
            std::cout << "Gra wczytana pomyslnie z menu\n";
        }
        else {
            std::cout << "Nie udalo sie wczytac gry\n";
        }
        break;

    case 3:  
        returnToMainMenu = true;
        togglePause();
        std::cout << "Powrot do glownego menu\n";
        break;
    }
} 





