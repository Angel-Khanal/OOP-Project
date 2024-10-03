#pragma once
#include "grid.h"
#include "blocks.cpp"
class Game
{
	std::vector<Block> block;
	Block currentBlock;
	Block nextBlock;
	Block GetrandomBlock();
	bool isblockoutside();
	void Rotateblock();
	void lockblock();
	void MoveblockLeft();
	bool blockfits();
	void Reset();
	void updatescore(int lineclearedpoint , int blockmovedpoint);

public:
	Game();
	Grid grid;
	std::vector<Block> GetallBlock();
	void Draw(sf::RenderWindow& window);
	void MoveblockRight();
	void MoveblockDown();
	void handleinput();
	bool gameover;
	int score;
	int highscore;
};

