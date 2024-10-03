#pragma once
#include<vector>
#include<map>
#include"position.h"
#include"colors.h"
#include<SFML/Graphics.hpp>
class Block
{
	int cellsize;
	int rotationstate;
	int rowoffset;
	int coloffset;
	std::vector<sf::Color>color;

public:
	Block();
	void Draw(sf::RenderWindow &window,int offsetX,int offsetY);
	void Move(int row, int col);
	std::vector<Position> getcellposition();
	void Rotate();
	void Undorotation();
	int id;
	std::map<int, std::vector<Position>>cells;
	
};
	