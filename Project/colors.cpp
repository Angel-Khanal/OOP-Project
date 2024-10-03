#include "colors.h"
const sf::Color darkgrey(105, 105, 105);
const sf::Color blue(0, 0, 255);
const sf::Color cyan(0, 255, 255);
const sf::Color green(0, 255, 0);
const sf::Color magenta(255, 0, 255);
const sf::Color red(255, 0, 0);
const sf::Color yellow(255, 255, 0);
const sf::Color purple(128, 0, 128);

std::vector<sf::Color>Getcolor()
{
	return { darkgrey, blue, cyan, green, magenta, red, yellow, purple };
}