#include "game.h"
#include <random>
#include<iostream>
#include<fstream>
Game::Game()
{
	grid = Grid();
	block = GetallBlock();
	currentBlock = GetrandomBlock();
	nextBlock = GetrandomBlock();
	gameover = false;
	score = 0;
}
Block Game::GetrandomBlock()
{
	if (block.empty())
	{
		block = GetallBlock();
	}
	int randomIndex = rand() % block.size();
	Block selectedblock = block[randomIndex];
	block.erase(block.begin() + randomIndex);
	return selectedblock;
}
std::vector<Block>Game::GetallBlock()
{
	return { IBlock(),JBlock(),LBlock(),ZBlock(),OBlock(),SBlock(),TBlock() };

}
void Game::Draw(sf::RenderWindow &window)
{
	grid.draw(window);
	currentBlock.Draw(window,5,5);
	nextBlock.Draw(window,220, 280);
}

void Game::handleinput()
{
	if (gameover && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
	{
		gameover = false;
		Reset();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		
		MoveblockLeft();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		MoveblockRight();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		MoveblockDown();
		updatescore(0, 1);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		Rotateblock();
	}
}
void Game::MoveblockLeft()
{
	if (!gameover)
	{
		currentBlock.Move(0, -1);
		if (isblockoutside() || blockfits() == false)
		{
			currentBlock.Move(0, 1);
		}
	}
}
void Game::MoveblockRight()
	{
	if (!gameover)
	{
		currentBlock.Move(0, 1);
		if (isblockoutside() || blockfits() == false)
		{
			currentBlock.Move(0, -1);
		}
	}
	}
void Game::MoveblockDown()
{
	if (!gameover)
	{
		currentBlock.Move(1, 0);
		if (isblockoutside() || blockfits() == false)
		{
			currentBlock.Move(-1, 0);
			lockblock();
		}
	}
}
bool Game::isblockoutside()
{
	std::vector<Position>tiles = currentBlock.getcellposition();
	for (Position item : tiles)
	{
		if (grid.iscelloutside(item.row, item.col))
		{
			return true;
		}
	}
	return false;
}
void Game::Rotateblock()
{
	if (!gameover)
	{
		currentBlock.Rotate();
		if (isblockoutside())
		{
			currentBlock.Undorotation();
		}
	}
}
//last ma gayera block lai steady rakhna
void Game::lockblock()
{
	std::vector<Position>tiles = currentBlock.getcellposition();
	for (Position item : tiles)
	{
		grid.grid[item.row][item.col] = currentBlock.id;
	}
	currentBlock = nextBlock;
	if (blockfits() == false)
	{
		gameover = true;
		
	}
	nextBlock = GetrandomBlock();
	int rowscleared = grid.clearfullrow();
	updatescore(rowscleared, 0);
}

bool Game::blockfits()
{
	std::vector<Position> tiles = currentBlock.getcellposition();
	for (Position item : tiles)
	{
		if (grid.iscellempty(item.row, item.col) == false)
		{
			return false;
		}
	}
	return true;;
}

void Game::Reset()
{
	grid.initialize();
	block = GetallBlock();
	currentBlock = GetrandomBlock();
	nextBlock = GetrandomBlock();
	score = 0;
}

void Game::updatescore(int linesclearedpoint, int blockmovedpoint)
{
	switch (linesclearedpoint)
	{
	case 1:
		score += 100;
		break;
	case 2:
		score += 200;
	case3:
		score += 300;
	default:
		break;
	}
	score += blockmovedpoint;
}