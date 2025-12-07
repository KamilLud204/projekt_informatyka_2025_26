#include <SFML/Graphics.hpp>
#include "Menu.h"
#include "Game.h"
#include <iostream>

enum class AppState { MainMenu, Playing, Scores, Exiting };

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Arkanoid");
    window.setFramerateLimit(60);

    //G³ówne menu
    Menu mainMenu(
        static_cast<float>(window.getSize().x),
        static_cast<float>(window.getSize().y)
    );

    //Inicjalizacja gry
    Game game;
    AppState currentAppState = AppState::MainMenu;

    sf::Clock deltaClock;

    while (window.isOpen())
    {
        sf::Time dt = deltaClock.restart();
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            //g³owne menu obs³uga
            if (currentAppState == AppState::MainMenu)
            {
                if (event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code == sf::Keyboard::Up) mainMenu.przesunG();
                    if (event.key.code == sf::Keyboard::Down) mainMenu.przesunD();

                    if (event.key.code == sf::Keyboard::Enter)
                    {
                        int choice = mainMenu.getSelectedItem();

                        if (choice == 0) {  // Nowa gra
                            game.reset();
                            currentAppState = AppState::Playing;
                            std::cout << "Rozpoczeto nowa gre\n";
                        }
                        else if (choice == 1) {  // Wczytaj grê
                            if (game.loadState("save.json")) {
                                currentAppState = AppState::Playing;
                                std::cout << "Gra wczytana pomyslnie\n";
                            }
                            else {
                                std::cout << "Nie udalo sie wczytac gry\n";
                            }
                        }
                        else if (choice == 2) {  // Zapisz grê
                            if (game.saveState("save.json")) {
                                std::cout << "Gra zapisana pomyslnie\n";
                            }
                            else {
                                std::cout << "Nie udalo sie zapisac gry\n";
                            }
                        }
                        else if (choice == 3) {  // Wyniki
                            currentAppState = AppState::Scores;
                            std::cout << "Pokaz wyniki\n";
                        }
                        else if (choice == 4) {  // Wyjœcie
                            currentAppState = AppState::Exiting;
                            window.close();
                        }
                    }
                }
            }
            //Obs³uga gry
            else if (currentAppState == AppState::Playing)
            {
                if (event.type == sf::Event::KeyPressed)
                {
                    //menu pauzy/wznowienie
                    if (event.key.code == sf::Keyboard::Escape) {
                        //prze³¹cz pauzê
                        if (game.getCurrentState() == Game::State::Playing) {
                            game.togglePause();
                        }
                        //powrót do menu
                        else if (game.getCurrentState() == Game::State::GameOver) {
                            currentAppState = AppState::MainMenu;
                        }
                        //wznów grê
                        else if (game.isPaused()) {
                            game.togglePause();
                        }
                    }

                    
                    if (!game.isPaused() && game.getCurrentState() == Game::State::Playing) {
                        
                        if (event.key.code == sf::Keyboard::F5) {
                            if (game.saveState("save.json")) {
                                std::cout << "Gra zapisana (F5)\n";
                            }
                        }
                        else if (event.key.code == sf::Keyboard::F9) {
                            if (game.loadState("save.json")) {
                                std::cout << "Gra wczytana (F9)\n";
                            }
                        }
                    }

                    //obs³uga menu pauzy
                    if (game.isPaused()) {
                        game.handleMenuInput(event.key.code);
                    }

                    //Restart 
                    if (event.key.code == sf::Keyboard::R) {
                        if (game.getCurrentState() == Game::State::GameOver) {
                            game.reset();
                            std::cout << "Restart gry\n";
                        }
                    }
                }
            }
            
            //Obs³uga wyników
            else if (currentAppState == AppState::Scores)
            {
                if (event.type == sf::Event::KeyPressed)
                {
                    //powrót
                    if (event.key.code == sf::Keyboard::Escape) {
                        currentAppState = AppState::MainMenu;
                    }
                }
            }
        }

        // Update gry 
        if (currentAppState == AppState::Playing &&
            !game.isPaused() &&
            game.getCurrentState() == Game::State::Playing) {
            game.update(dt);
        }

        // Render
        window.clear();

        if (currentAppState == AppState::MainMenu) {
            mainMenu.draw(window);
        }
        else if (currentAppState == AppState::Playing) {
            game.render(window);
        }
        else if (currentAppState == AppState::Scores) {
            // Ekran wyników
            sf::Font font;
            if (font.loadFromFile("arial.ttf")) {
                // T³o
                sf::RectangleShape background(sf::Vector2f(800, 600));
                background.setFillColor(sf::Color(30, 30, 50));
                window.draw(background);

                sf::Text text;
                text.setFont(font);
                text.setCharacterSize(40);
                text.setFillColor(sf::Color::Yellow);
                text.setString("NAJLEPSZE WYNIKI");
                text.setPosition(250, 100);
                window.draw(text);

                sf::Text scores;
                scores.setFont(font);
                scores.setCharacterSize(30);
                scores.setFillColor(sf::Color::White);
                scores.setString("1. 1250 pkt\n\n2. 980 pkt\n\n3. 760 pkt\n\n4. 640 pkt\n\n5. 520 pkt");
                scores.setPosition(300, 180);
                window.draw(scores);

                sf::Text info;
                info.setFont(font);
                info.setCharacterSize(24);
                info.setFillColor(sf::Color::Cyan);
                info.setString("ESC - powrot do menu");
                info.setPosition(300, 500);
                window.draw(info);
            }
        }

        window.display();
    }

    return 0;
}