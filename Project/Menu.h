#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

#define MAX_LICZBA_POZIOMOW 5  

class Menu
{
private:
	sf::Font font;
	std::vector<sf::Text> menu;  
	int selectedItem = 0;

public:
	Menu(float width, float height);
	~Menu() {};
	void przesunG();  
	void przesunD();  
	int getSelectedItem() { return selectedItem; }  
	void draw(sf::RenderWindow& window);  
	void setPosition(float x, float y);  
	void setCharacterSize(unsigned int size);  
};
