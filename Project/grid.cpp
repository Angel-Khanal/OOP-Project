#include"grid.h"
#include<iostream>
#include"colors.h"

Grid::Grid()
{
	numofrows = 20;
	numofcolumns = 10;
	cellsize = 30;
	initialize();
	color = Getcolor();
}
//grid banauna
void Grid::initialize()
{
	for (int row = 0; row < numofrows; row++)
	{
		for (int col = 0; col < numofcolumns; col++)
		{
			grid[row][col] = 0;
		}
	}
}

	// grid dekhauna ko lagi
void Grid::draw(sf::RenderWindow& window)
{
	for (int row = 0; row < numofrows; row++)
	{
		for (int col = 0; col < numofcolumns; col++)
		{
			int cellvalue = grid[row][col];
			sf::RectangleShape rectangle(sf::Vector2f(cellsize - 1, cellsize - 1));
			rectangle.setFillColor(color[cellvalue]);
			rectangle.setPosition(col * cellsize + 5, row * cellsize + 5);
			window.draw(rectangle);
		}
	}
}

bool Grid::iscelloutside(int row, int col)
{
	if (row >= 0 && row < numofrows && col >= 0 && col < numofcolumns)
	{
		return false;
	}
	return true;
}

bool Grid::iscellempty(int row, int col)
{
	if (grid[row][col] == 0)
	{
		return true;
	}
	return false;
}

bool Grid::Isrowfull(int row)
{
	for (int col = 0; col < numofcolumns; col++)
	{
		if (grid[row][col] == 0)
		{
			return false;
		}
	}
		return true;
}
void Grid::clearrow(int row)
{
	for (int col = 0; col < numofcolumns; col++)
	{
		grid[row][col] = 0;
	}
}
void Grid::rowmovedown(int row, int numofrow)
{
	for (int col = 0; col < numofcolumns; col++)
	{
		grid[row + numofrow][col] = grid[row][col];
		grid[row][col] = 0;
	}
}
int Grid::clearfullrow()
{
	int completed = 0;
	for (int row = numofrows - 1; row >= 0; row--)
	{
		if (Isrowfull(row))
		{
			clearrow(row);
			completed++;
		}
		else if (completed > 0)
		{
			rowmovedown(row, completed);
		}
	}
	return completed;
}
