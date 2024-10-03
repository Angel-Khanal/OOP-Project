#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

class HangmanDrawer {
public:
    HangmanDrawer(sf::RenderWindow& window) : window(window) {
        if (!font.loadFromFile("arial.ttf")) {
            throw runtime_error("Error loading font");
        }
        text.setFont(font);
        text.setCharacterSize(18);
        text.setFillColor(sf::Color::Green);
        text.setPosition(40, 40);
    }

    void updateText(const std::string& newText) {
        text.setString(newText);
    }

    void drawHangman(int incorrectGuesses) {
        sf::Vector2u windowSize = window.getSize();
        float centerX = windowSize.x / 2.0f;
        float centerY = window.getSize().y / 2.0f;

        drawGallows(centerX, centerY);
        drawHead(centerX, centerY, incorrectGuesses);
        drawBody(centerX, centerY, incorrectGuesses);
        drawArms(centerX, centerY, incorrectGuesses);
        drawLegs(centerX, centerY, incorrectGuesses);
    }

    void displayGame(const std::string& guessed, const std::vector<char>& incorrectGuesses, int attempts, const std::string& word) {
        std::stringstream ss;
        ss << "WORD: " << guessed << "\n";
        ss << "INCORRECT GUESSES: ";
        for (char ch : incorrectGuesses) {
            ss << ch << ' ';
        }
        ss << "\nAttempts left: " << attempts << "\n";

        updateText(ss.str());

        window.clear(sf::Color::Black);
        drawHangman(6 - attempts);
        window.draw(text);

        if (attempts == 0 || guessed == word) {
            std::string gameOverText = guessed == word ? "Congratulations! You've guessed the word!" : "Game Over! The word was " + word;
            sf::Text gameOver(gameOverText, font, 30);
            gameOver.setFillColor(sf::Color::Red);
            gameOver.setPosition(40, window.getSize().y / 2.0f - 20);
            window.draw(gameOver);
        }

        window.display();
    }

    sf::Font& getFont() {
        return font;
    }

private:
    sf::RenderWindow& window;
    sf::Font font;
    sf::Text text;

    void drawGallows(float centerX, float centerY) {
        sf::VertexArray gallowsBase(sf::LinesStrip, 2);
        gallowsBase[0].position = sf::Vector2f(centerX - 25, centerY + 100);
        gallowsBase[1].position = sf::Vector2f(centerX + 25, centerY + 100);
        gallowsBase[0].color = sf::Color::White;
        gallowsBase[1].color = sf::Color::White;
        window.draw(gallowsBase);

        sf::VertexArray gallowsPole(sf::LinesStrip, 2);
        gallowsPole[0].position = sf::Vector2f(centerX, centerY + 100);
        gallowsPole[1].position = sf::Vector2f(centerX, centerY - 100);
        gallowsPole[0].color = sf::Color::White;
        gallowsPole[1].color = sf::Color::White;
        window.draw(gallowsPole);

        sf::VertexArray gallowsTop(sf::LinesStrip, 2);
        gallowsTop[0].position = sf::Vector2f(centerX, centerY - 100);
        gallowsTop[1].position = sf::Vector2f(centerX + 50, centerY - 100);
        gallowsTop[0].color = sf::Color::White;
        gallowsTop[1].color = sf::Color::White;
        window.draw(gallowsTop);

        sf::VertexArray gallowsRope(sf::LinesStrip, 2);
        gallowsRope[0].position = sf::Vector2f(centerX + 50, centerY - 100);
        gallowsRope[1].position = sf::Vector2f(centerX + 50, centerY - 50);
        gallowsRope[0].color = sf::Color::White;
        gallowsRope[1].color = sf::Color::White;
        window.draw(gallowsRope);
    }

    void drawHead(float centerX, float centerY, int incorrectGuesses) {
        if (incorrectGuesses > 0) {
            sf::CircleShape head(15);
            head.setFillColor(sf::Color::White);
            head.setOutlineThickness(2);
            head.setOutlineColor(sf::Color::White);
            head.setPosition(centerX + 35, centerY - 90);
            window.draw(head);
        }
    }

    void drawBody(float centerX, float centerY, int incorrectGuesses) {
        if (incorrectGuesses > 1) {
            sf::VertexArray body(sf::LinesStrip, 2);
            body[0].position = sf::Vector2f(centerX + 50, centerY - 75);
            body[1].position = sf::Vector2f(centerX + 50, centerY - 35);
            body[0].color = sf::Color::White;
            body[1].color = sf::Color::White;
            window.draw(body);
        }
    }

    void drawArms(float centerX, float centerY, int incorrectGuesses) {
        if (incorrectGuesses > 2) {
            sf::VertexArray leftArm(sf::LinesStrip, 2);
            leftArm[0].position = sf::Vector2f(centerX + 50, centerY - 70);
            leftArm[1].position = sf::Vector2f(centerX + 20, centerY - 70);
            leftArm[0].color = sf::Color::White;
            leftArm[1].color = sf::Color::White;
            window.draw(leftArm);
        }

        if (incorrectGuesses > 3) {
            sf::VertexArray rightArm(sf::LinesStrip, 2);
            rightArm[0].position = sf::Vector2f(centerX + 50, centerY - 70);
            rightArm[1].position = sf::Vector2f(centerX + 80, centerY - 70);
            rightArm[0].color = sf::Color::White;
            rightArm[1].color = sf::Color::White;
            window.draw(rightArm);
        }
    }

    void drawLegs(float centerX, float centerY, int incorrectGuesses) {
        if (incorrectGuesses > 4) {
            sf::VertexArray leftLeg(sf::LinesStrip, 2);
            leftLeg[0].position = sf::Vector2f(centerX + 50, centerY - 35);
            leftLeg[1].position = sf::Vector2f(centerX + 20, centerY + 20);
            leftLeg[0].color = sf::Color::White;
            leftLeg[1].color = sf::Color::White;
            window.draw(leftLeg);
        }

        if (incorrectGuesses > 5) {
            sf::VertexArray rightLeg(sf::LinesStrip, 2);
            rightLeg[0].position = sf::Vector2f(centerX + 50, centerY - 35);
            rightLeg[1].position = sf::Vector2f(centerX + 80, centerY + 20);
            rightLeg[0].color = sf::Color::White;
            rightLeg[1].color = sf::Color::White;
            window.draw(rightLeg);
        }
    }
};

class HangmanGame {
public:
    HangmanGame(sf::RenderWindow& window, HangmanDrawer& drawer)
        : window(window), drawer(drawer), attempts(6), gameRunning(true) {}

    void startGame() {
        while (window.isOpen()) {
            if (gameRunning) {
                playRound(); // Run the main game loop
            }
            else {
                handleEvents(); // Handle events for restarting or exiting
                displayMessage("Press 'R' to restart \n O R \n 'E' to exit.", true);

                // Wait for user input to restart or exit
                while (true) {
                    handleEvents(); // Process events (including user input)
                    if (inputBuffer == "r") { // Enter key pressed
                        resetGame(); // Restart the game
                        inputBuffer.clear(); // Clear buffer after processing
                        break; // Exit the loop and restart the game
                    }
                    else if (inputBuffer == "e") { // Escape key pressed
                        window.close(); // Exit the game
                        inputBuffer.clear(); // Clear buffer after processing
                        break; // Exit the loop and close the window
                    }
                }
            }
        }
    }



private:
    sf::RenderWindow& window;
    HangmanDrawer& drawer;
    std::string inputBuffer;
    std::string word;
    std::string guessed;
    std::vector<char> incorrectGuesses;
    int attempts;
    bool gameRunning;

    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close(); // Close the window
            }
            else if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128) {
                    char enteredChar = static_cast<char>(event.text.unicode);
                    if (enteredChar == '\b') { // Backspace
                        if (!inputBuffer.empty()) {
                            inputBuffer.pop_back();
                        }
                    }
                    else if (enteredChar == '\r' || enteredChar == '\n') { // Enter key
                        processInput(); // Process input when Enter is pressed
                    }
                    else if (enteredChar == '\x1B') { // Escape key
                        gameRunning = false; // Stop the game loop
                    }
                    else if (isalpha(enteredChar)) { // Only accept alphabetic characters
                        inputBuffer += tolower(enteredChar);
                    }
                }
            }
        }
    }



    void processInput() {
        if (word.empty()) {
            word = inputBuffer;
            inputBuffer.clear();
            guessed.assign(word.size(), '_');
            incorrectGuesses.clear();
            attempts = 6;
        }
        else {
            if (inputBuffer.length() == word.length()) {
                if (inputBuffer == word) {
                    guessed = word; // Correct guess
                }
                else {
                    attempts = 0; // End game on wrong full word guess
                }
            }
            else if (inputBuffer.length() == 1) {
                char guess = inputBuffer[0];
                if (word.find(guess) != std::string::npos) {
                    for (size_t i = 0; i < word.size(); ++i) {
                        if (word[i] == guess) {
                            guessed[i] = guess;
                        }
                    }
                }
                else {
                    if (find(incorrectGuesses.begin(), incorrectGuesses.end(), guess) == incorrectGuesses.end()) {
                        incorrectGuesses.push_back(guess);
                        --attempts;
                    }
                }
            }
            inputBuffer.clear();
        }
    }

    void displayMessage(const std::string& message, bool waitForInput) {
        sf::Text text(message, drawer.getFont(), 20);
        text.setFillColor(sf::Color::White);
        text.setPosition(10, 10);

        window.clear(sf::Color::Black);
        window.draw(text);
        window.display();

        if (waitForInput) {
            bool inputReceived = false;
            while (!inputReceived) {
                handleEvents();
                if (!inputBuffer.empty()) {
                    inputReceived = true;
                }
            }
        }
        else {
            sf::sleep(sf::seconds(3)); // Show message for 3 seconds
        }
    }

    void playRound() {
        displayMessage("Enter a word to guess:- ", true);

        while (word.empty()) {
            handleEvents();
        }

        while (attempts > 0 && guessed != word) {
            drawer.displayGame(guessed, incorrectGuesses, attempts, word);
            handleEvents();
        }

        if (guessed == word) {
            displayMessage("| C O N G U R A T S | \n You've guessed the word!", false);
        }
        else {
            displayMessage("| G A M E  O V E R |\n The word was : " + word, false);
        }

        gameRunning = false;
    }

    void resetGame() {
        word.clear();
        guessed.clear();
        incorrectGuesses.clear();
        attempts = 6;
        gameRunning = true; // Start a new game
    }

};

void playHangman() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Hangman Game");

    try {
        HangmanDrawer drawer(window);
        HangmanGame game(window, drawer);
        game.startGame();
    }
    catch (const std::exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
}