#include "Menu.h"

Menu::Menu(float width, float height)
{
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Nie moge zaladowac czcionki arial.ttf\n";
    }

    sf::Text t;
    t.setFont(font);
    t.setCharacterSize(40);

    //Nowa gra
    t.setString("Nowa gra");
    t.setFillColor(sf::Color::Cyan);
    t.setPosition(sf::Vector2f(width / 3, height / (MAX_LICZBA_POZIOMOW + 1) * 1));
    menu.push_back(t);

    //Wczytaj gre
    t.setString("Wczytaj gre");
    t.setFillColor(sf::Color::White);
    t.setPosition(sf::Vector2f(width / 3, height / (MAX_LICZBA_POZIOMOW + 1) * 2));
    menu.push_back(t);

    //Zapisz gre
    t.setString("Zapisz gre");
    t.setFillColor(sf::Color::White);
    t.setPosition(sf::Vector2f(width / 3, height / (MAX_LICZBA_POZIOMOW + 1) * 3));
    menu.push_back(t);

    //Wyniki
    t.setString("Ostatnie wyniki");
    t.setFillColor(sf::Color::White);
    t.setPosition(sf::Vector2f(width / 3, height / (MAX_LICZBA_POZIOMOW + 1) * 4));
    menu.push_back(t);

    //Wyjœcie
    t.setString("Wyjscie");
    t.setFillColor(sf::Color::White);
    t.setPosition(sf::Vector2f(width / 3, height / (MAX_LICZBA_POZIOMOW + 1) * 5));
    menu.push_back(t);

    selectedItem = 0;
}

void Menu::draw(sf::RenderWindow& window)
{
    for (int i = 0; i < MAX_LICZBA_POZIOMOW; i++) {
        window.draw(menu[i]);
    }
}

void Menu::przesunG()
{
    menu[selectedItem].setFillColor(sf::Color::White);
    selectedItem--;
    if (selectedItem < 0)
        selectedItem = MAX_LICZBA_POZIOMOW - 1;
    menu[selectedItem].setFillColor(sf::Color::Cyan);
}

void Menu::przesunD()
{
    menu[selectedItem].setFillColor(sf::Color::White);
    selectedItem++;
    if (selectedItem >= MAX_LICZBA_POZIOMOW)
        selectedItem = 0;
    menu[selectedItem].setFillColor(sf::Color::Cyan);
}

void Menu::setPosition(float x, float y)
{
    for (auto& item : menu) {
        sf::Vector2f pos = item.getPosition();
        item.setPosition(x, pos.y + y);
    }
}

void Menu::setCharacterSize(unsigned int size)
{
    for (auto& item : menu) {
        item.setCharacterSize(size);
    }
}