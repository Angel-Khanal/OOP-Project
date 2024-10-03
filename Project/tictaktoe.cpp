#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

const int BOARD_SIZE = 3;
const int CELL_SIZE = 170;
const int WINDOW_SIZE = BOARD_SIZE * CELL_SIZE;

// Define colors
const sf::Color LINE_COLOR(0, 0, 0); // Black
const sf::Color X_COLOR(255, 0, 0);  // Red
const sf::Color O_COLOR(0, 0, 255);  // Blue
const sf::Color MESSAGE_COLOR(0, 128, 0); // Dark Green for messages

class TicTacToeGame
{
public:
    TicTacToeGame();
    void run();

private:
    void drawBoard();
    void handleInput(int& currentPlayer);
    bool checkWin();
    bool isDraw();
    void displayMessage(const std::string& message);
    void resetGame();
    void saveGameResult(const std::string& result);
    void inputPlayerNames();

    // SFML objects
    sf::RenderWindow window;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::SoundBuffer xBuffer, oBuffer, winBuffer, drawBuffer;
    sf::Sound xSound, oSound, winSound, drawSound;

    // Game board array
    char board[BOARD_SIZE][BOARD_SIZE];

    // Game state variables
    bool gameWon;
    bool gameDraw;
    std::string endMessage;
    std::string player1Name;
    std::string player2Name;
};
// Constructor
TicTacToeGame::TicTacToeGame()
    : window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Tic Tac Toe"),
    gameWon(false), gameDraw(false), endMessage("")
{
    // Load the background texture
    if (!backgroundTexture.loadFromFile("background.jpg"))
    {
        std::cerr << "Error loading background image!" << std::endl;
        exit(-1);
    }
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(
        static_cast<float>(WINDOW_SIZE) / backgroundTexture.getSize().x,
        static_cast<float>(WINDOW_SIZE) / backgroundTexture.getSize().y
    );

    // Load sound buffers
    if (!xBuffer.loadFromFile("x_sound.wav") || !oBuffer.loadFromFile("o_sound.wav") ||
        !winBuffer.loadFromFile("win_sound.wav") || !drawBuffer.loadFromFile("draw_sound.wav"))
    {
        std::cerr << "Error loading sound!" << std::endl;
        exit(-1);
    }
    xSound.setBuffer(xBuffer);
    oSound.setBuffer(oBuffer);
    winSound.setBuffer(winBuffer);
    drawSound.setBuffer(drawBuffer);

    // Initialize board
    resetGame();
}

void TicTacToeGame::inputPlayerNames()
{
    sf::String input;
    std::string name;
    bool inputDone = false;

    // Create a window for text input
    sf::RenderWindow inputWindow(sf::VideoMode(400, 300), "Enter Player Names");

    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
    {
        std::cerr << "Error loading font!" << std::endl;
        return;
    }

    sf::Text prompt("Enter Player 1 Name:", font, 20);
    prompt.setPosition(10, 10);

    sf::Text player1Text("", font, 20);
    player1Text.setPosition(10, 50);

    sf::Text player2Text("", font, 20);
    player2Text.setPosition(10, 150);

    sf::Text inputText("", font, 20);
    inputText.setPosition(10, 200);

    sf::String currentInput;

    while (inputWindow.isOpen())
    {
        sf::Event event;
        while (inputWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                inputWindow.close();

            if (event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode < 128)
                {
                    char enteredChar = static_cast<char>(event.text.unicode);
                    if (enteredChar == '\b' && !currentInput.isEmpty())
                    {
                        currentInput.erase(currentInput.getSize() - 1, 1);
                    }
                    else if (enteredChar >= ' ' && enteredChar <= '~')
                    {
                        currentInput += enteredChar;

                    }
                }
                if (event.text.unicode == '\r')
                {
                    if (player1Name.empty())
                    {
                        player1Name = currentInput.toAnsiString();
                        currentInput = "";
                        prompt.setString("Enter Player 2 Name:");
                    }
                    else
                    {
                        player2Name = currentInput.toAnsiString();
                        inputWindow.close();
                        inputDone = true;
                    }
                }
            }
        }

        inputWindow.clear();
        inputWindow.draw(prompt);
        inputText.setString(currentInput);
        inputWindow.draw(inputText);
        player1Text.setString("Player 1: " + player1Name);
        player2Text.setString("Player 2: " + player2Name);
        inputWindow.draw(player1Text);
        inputWindow.draw(player2Text);
        inputWindow.display();
    }
}

void TicTacToeGame::run()
{
    inputPlayerNames(); // Get player names

    int currentPlayer = 1; // 1 for X, 2 for O

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R && (gameWon || gameDraw))
            {
                resetGame();
                currentPlayer = 1; // Reset to player 1 (X)
                continue; // Skip the rest of the loop to restart the game
            }
        }

        if (!gameWon && !gameDraw)
        {
            handleInput(currentPlayer);
            gameWon = checkWin();
            gameDraw = isDraw();
            if (gameWon)
            {
                endMessage = (currentPlayer == 1) ? player2Name + " Wins!" : player1Name + " Wins!";
                winSound.play(); // Play win sound
                saveGameResult(endMessage); // Save the game result
            }
            else if (gameDraw)
            {
                endMessage = "Draw!";
                drawSound.play(); // Play draw sound
                saveGameResult(endMessage); // Save the game result
            }
        }

        window.clear(sf::Color::White);
        drawBoard();
        if (gameWon || gameDraw)
        {
            displayMessage(endMessage);
        }
        window.display();

        if (gameWon || gameDraw)
        {
            // Add a delay before closing to show the end state
            sf::sleep(sf::seconds(2)); // Pause for 2 seconds
        }
    }
}

void TicTacToeGame::drawBoard()
{
    // Draw the background image
    window.draw(backgroundSprite);

    // Draw the grid
    sf::RectangleShape line(sf::Vector2f(WINDOW_SIZE, 5));
    line.setFillColor(LINE_COLOR);

    // Draw vertical lines
    line.setSize(sf::Vector2f(5, WINDOW_SIZE));
    for (int i = 1; i < BOARD_SIZE; ++i)
    {
        line.setPosition(i * CELL_SIZE, 0);
        window.draw(line);
    }

    // Draw horizontal lines
    line.setSize(sf::Vector2f(WINDOW_SIZE, 5));
    for (int i = 1; i < BOARD_SIZE; ++i)
    {
        line.setPosition(0, i * CELL_SIZE);
        window.draw(line);
    }

    // Load font
    static sf::Font font;
    static bool fontLoaded = false;
    if (!fontLoaded)
    {
        if (!font.loadFromFile("arial.ttf"))
        {
            std::cerr << "Error loading font!" << std::endl;
            return;
        }
        fontLoaded = true;
    }

    // Prepare text settings
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(80);  // Ensure this size is appropriate for your cell size
    text.setStyle(sf::Text::Bold);

    // Draw X and O
    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        for (int j = 0; j < BOARD_SIZE; ++j)
        {
            if (board[i][j] != ' ')
            {
                text.setString(std::string(1, board[i][j])); // Convert char to string

                // Center the text within each cell
                sf::FloatRect textRect = text.getLocalBounds();
                text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
                text.setPosition(j * CELL_SIZE + CELL_SIZE / 2.0f, i * CELL_SIZE + CELL_SIZE / 2.0f);
                text.setFillColor(board[i][j] == 'X' ? X_COLOR : O_COLOR);
                window.draw(text);
            }
        }
    }
}

void TicTacToeGame::handleInput(int& currentPlayer)
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        int row = mousePos.y / CELL_SIZE;
        int col = mousePos.x / CELL_SIZE;

        if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE && board[row][col] == ' ')
        {
            board[row][col] = (currentPlayer == 1) ? 'X' : 'O';
            if (currentPlayer == 1) {
                xSound.play();
            }
            else {
                oSound.play();
            }
            currentPlayer = (currentPlayer % 2) + 1; // Switch player

            // Prevent multiple marks from one click
            sf::sleep(sf::milliseconds(200));
        }
    }
}

bool TicTacToeGame::checkWin()
{
    // Check rows and columns
    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        if (board[i][0] != ' ' && board[i][0] == board[i][1] && board[i][1] == board[i][2])
            return true;
        if (board[0][i] != ' ' && board[0][i] == board[1][i] && board[1][i] == board[2][i])
            return true;
    }

    // Check diagonals
    if (board[0][0] != ' ' && board[0][0] == board[1][1] && board[1][1] == board[2][2])
        return true;
    if (board[0][2] != ' ' && board[0][2] == board[1][1] && board[1][1] == board[2][0])
        return true;

    return false;
}

bool TicTacToeGame::isDraw()
{
    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        for (int j = 0; j < BOARD_SIZE; ++j)
        {
            if (board[i][j] == ' ')
                return false;
        }
    }
    return true;
}

void TicTacToeGame::displayMessage(const std::string& message)
{
    // Load font
    static sf::Font font;
    static bool fontLoaded = false;
    if (!fontLoaded)
    {
        if (!font.loadFromFile("arial.ttf"))
        {
            std::cerr << "Error loading font!" << std::endl;
            return;
        }
        fontLoaded = true;
    }

    // Prepare text settings
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(60);
    text.setStyle(sf::Text::Bold);
    text.setFillColor(MESSAGE_COLOR);
    text.setString(message);

    // Center the text
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    text.setPosition(WINDOW_SIZE / 2.0f, WINDOW_SIZE / 2.0f);

    window.draw(text);
}

void TicTacToeGame::resetGame()
{
    // Reset the board
    for (int i = 0; i < BOARD_SIZE; ++i)
        for (int j = 0; j < BOARD_SIZE; ++j)
            board[i][j] = ' ';

    // Reset game state
    gameWon = false;
    gameDraw = false;
    endMessage.clear();
}

void TicTacToeGame::saveGameResult(const std::string& result)
{
    std::ofstream file("game_result.txt", std::ios::app);
    if (file.is_open())
    {
        file << result << std::endl;
        file.close();
    }
    else
    {
        std::cerr << "Error opening file for writing game result!" << std::endl;
    }
}

// Main function to create and run the game
void playTicTacToe()
{
    TicTacToeGame game;
    game.run();
}