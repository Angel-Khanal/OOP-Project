#pragma once
#include<vector>
#include<SFML/Graphics.hpp>
class Grid
{
	int numofrows;
	int numofcolumns;
	int cellsize;
	std::vector<sf::Color>color;
	bool Isrowfull(int row);
	void clearrow(int row);
	void rowmovedown(int row, int numofrow);
public:
	Grid();
	int grid[20][10];
	void initialize();
	//void print();
	void draw(sf::RenderWindow& window);
	bool iscelloutside(int row, int col);
	bool iscellempty(int row, int col);
	int clearfullrow();
};