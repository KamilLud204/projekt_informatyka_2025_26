#include "GameState.h"
#include "Paletka.hpp"
#include "Pilka.hpp"
#include "Stone.h"
#include <fstream>
#include <iostream>
#include <algorithm>

void GameState::capture(const Paletka& paddle,
    const Pilka& ball,
    const std::vector<Stone>& stones)
{
    
    paddlePosition = sf::Vector2f(paddle.getX(),
        paddle.getY());

    ballPosition = sf::Vector2f(ball.getX(),
        ball.getY());

    ballVelocity = sf::Vector2f(ball.getVx(),
        ball.getVy());

    blocks.clear();
    for (const auto& s : stones) {
        if (!s.isDestroyed()) {  // Zapisujemy  niezniszczone bloki
            blocks.push_back({ s.getPosition().x, s.getPosition().y, s.getHP() });
        }
    }
}

bool GameState::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Nie mozna otworzyc pliku do zapisu: " << filename << std::endl;
        return false;
    }

    // Zapis 
    file.precision(6);
    file << std::fixed;

    file << "PADDLE " << paddlePosition.x << " " << paddlePosition.y << "\n";
    file << "BALL " << ballPosition.x << " " << ballPosition.y << " "
        << ballVelocity.x << " " << ballVelocity.y << "\n";
    file << "BLOCKS_COUNT " << blocks.size() << "\n";

    for (const auto& b : blocks) {
        file << b.x << " " << b.y << " " << b.hp << "\n";
    }

    file.close();
    std::cout << "Zapisano stan gry do " << filename << std::endl;
    return true;
}

bool GameState::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Nie mozna otworzyc pliku do odczytu: " << filename << std::endl;
        return false;
    }

    std::string label;

    // Paletka
    if (!(file >> label >> paddlePosition.x >> paddlePosition.y)) {
        std::cerr << "Blad odczytu pozycji paletki" << std::endl;
        return false;
    }

    if (label != "PADDLE") {
        std::cerr << "Nieprawidlowy format pliku - brak PADDLE" << std::endl;
        return false;
    }

    // Pi³ka
    if (!(file >> label >> ballPosition.x >> ballPosition.y
        >> ballVelocity.x >> ballVelocity.y)) {
        std::cerr << "Blad odczytu pozycji pilki" << std::endl;
        return false;
    }

    if (label != "BALL") {
        std::cerr << "Nieprawidlowy format pliku - brak BALL" << std::endl;
        return false;
    }

    // Bloki
    int count = 0;
    if (!(file >> label >> count)) {
        std::cerr << "Blad odczytu liczby blokow" << std::endl;
        return false;
    }

    if (label != "BLOCKS_COUNT") {
        std::cerr << "Nieprawidlowy format pliku - brak BLOCKS_COUNT" << std::endl;
        return false;
    }

    blocks.clear();
    for (int i = 0; i < count; i++) {
        BlockData b;
        if (!(file >> b.x >> b.y >> b.hp)) {
            std::cerr << "Blad odczytu bloku " << i << std::endl;
            return false;
        }
        // SprawdŸ czy s¹ poprawne
        if (b.x < 0 || b.x > 800 || b.y < 0 || b.y > 600 || b.hp < 1 || b.hp > 3) {
            std::cerr << "Nieprawidlowe dane bloku " << i
                << ": x=" << b.x << " y=" << b.y << " hp=" << b.hp << std::endl;
            return false;
        }
        blocks.push_back(b);
    }

    file.close();
    std::cout << "Wczytano stan gry z " << filename
        << " (blokow: " << blocks.size() << ")" << std::endl;
    return true;
}

void GameState::apply(Paletka& paddle,
    Pilka& ball,
    std::vector<Stone>& stones)
{
    //pozycjê paletki
    paddle.setPosition(paddlePosition);

    //pozycjê, prêdkoœæ pi³ki
    ball.reset(ballPosition, ballVelocity);

    // Wyczyœæ, odtwórz bloki
    stones.clear();

    // Rozmiar bloku 
    const float BLOCK_H = 25.f;
    const float BLOCK_W = (800 - (6 - 1) * 2.f) / 6;  

    for (const auto& b : blocks) {
        // SprawdŸ pozycja bloku poprawna
        if (b.x >= 0 && b.x <= 800 && b.y >= 0 && b.y <= 600 && b.hp > 0 && b.hp <= 3) {
            stones.emplace_back(
                sf::Vector2f(b.x, b.y),
                sf::Vector2f(BLOCK_W, BLOCK_H),
                b.hp
            );
        }
        else {
            std::cerr << "Omietyto blok z nieprawidlowymi danymi: "
                << "x=" << b.x << " y=" << b.y << " hp=" << b.hp << std::endl;
        }
    }

    std::cout << "Zastosowano stan gry (blokow: " << stones.size() << ")" << std::endl;
}