#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

struct BlockData {
    float x, y;
    int hp;
};

class GameState {
private:
    sf::Vector2f paddlePosition;
    sf::Vector2f ballPosition;
    sf::Vector2f ballVelocity;
    std::vector<BlockData> blocks;

    
    bool m_showPauseMenu = false;
    

public:
    GameState() = default;

    void capture(const class Paletka& paddle,
        const class Pilka& ball,
        const std::vector<class Stone>& stones);

    bool saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename);

    void apply(class Paletka& paddle,
        class Pilka& ball,
        std::vector<class Stone>& stones);
};