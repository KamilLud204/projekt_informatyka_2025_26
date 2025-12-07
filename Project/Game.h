#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Paletka.hpp"
#include "Pilka.hpp"
#include "Stone.h"
#include "Gamestate.h"

class Game
{
public:
    enum class State { Playing, Paused, GameOver };

    Game();

    void update(sf::Time dt);
    void render(sf::RenderTarget& target);
    bool saveState(const std::string& filename);
    bool loadState(const std::string& filename);
    void reset();

    void togglePause() {
        m_currentState = (m_currentState == State::Playing) ? State::Paused : State::Playing;
    }
    bool isPaused() const { return m_currentState == State::Paused; }

    bool levelLoadedFromSave = false;
    State getCurrentState() const { return m_currentState; }

    
    void handleMenuInput(sf::Keyboard::Key key);
    int getMenuSelection() const { return m_menuSelection; }
    void moveMenuUp();
    void moveMenuDown();
    void executeMenuAction(bool& returnToMainMenu);

private:
    const float WIDTH = 800.f;
    const float HEIGHT = 600.f;

    int m_frameCounter = 0;
    State m_currentState = State::Playing;

    Paletka m_paletka;
    Pilka m_pilka;
    std::vector<Stone> m_bloki;

    
    int m_menuSelection = 0;
    std::vector<std::string> m_menuItems = { "Wznow gre", "Zapisz gre", "Wczytaj gre", "Wyjdz do menu" };

    
    void checkGameOver();
    float calculateBallStartY() const;
    void renderPauseMenu(sf::RenderTarget& target);
    void renderGameOver(sf::RenderTarget& target);
};
