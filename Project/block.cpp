#include "block.h"
Block::Block()
{
	id = 0;
	cellsize = 30;
	rotationstate = 0;
	rowoffset = 0;
	coloffset = 0;
	color = Getcolor();
}
// yo xai block haru draw garna ko lagi
void Block::Draw(sf::RenderWindow &window,int offsetX,int offsetY)
{
	std::vector<Position>tiles = getcellposition();
	for (Position item : tiles)
	{
		sf::RectangleShape rectangle(sf::Vector2f(cellsize - 1, cellsize - 1));
		rectangle.setFillColor(color[id]);
		rectangle.setPosition(item.col * cellsize + offsetX, item.row * cellsize + offsetY);
		window.draw(rectangle);
	}
}
void Block::Move(int row, int col)
{
	rowoffset += row;
	coloffset += col;
}
std::vector<Position>Block::getcellposition()
{
	std::vector<Position>tiles = cells[rotationstate];
	std::vector<Position>movedTitles;
	for (Position item : tiles)
	{
		Position newpos = Position(item.row + rowoffset, item.col + coloffset);
		movedTitles.push_back(newpos);
	}
	return movedTitles;
}

void Block::Rotate()
{
	rotationstate++;
	if (rotationstate == (int)cells.size())
	{
		rotationstate = 0;
	}
}

void Block::Undorotation()
{
	rotationstate--;
	if (rotationstate == -1)
	{
		rotationstate = cells.size()-1;
	}
}
