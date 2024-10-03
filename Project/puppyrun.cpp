#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
#include <fstream>

// Constants
const int WINDOW_WIDTH = 500;
const int WINDOW_HEIGHT = 400;
const float GROUND_HEIGHT = 100.f;
const float GRAVITY = 0.5f;
const float JUMP_STRENGTH = -14.f;
float OBSTACLE_SPEED = -5.f;
const float SPEED_INCREASE_INTERVAL = 10.f;
const float SPEED_INCREMENT = 0.5f;
const float MIN_OBSTACLE_SPEED = -10.f;
const std::string SCORE_FILE = "highscore.txt";

// Class for the Puppy character with animations
class Puppy {
public:
    sf::Sprite sprite;
    std::vector<sf::Texture> textures;
    float velocityY;
    bool isJumping;
    size_t currentTextureIndex;
    sf::Clock textureClock;

    Puppy(float x, float y) : currentTextureIndex(0) {
        std::vector<std::string> textureFiles = { "puppyee11.png", "puppyee22.png", "puppyee31.png","puppyee32.png","puppyee34.png","puppyee33.png" };

        for (const auto& file : textureFiles) {
            sf::Texture texture;
            if (!texture.loadFromFile(file)) {
                std::cerr << "Error: Could not load texture '" << file << "'!" << std::endl;
                exit(-1);
            }
            textures.push_back(texture);
        }

        sprite.setTexture(textures[currentTextureIndex]);
        sprite.setPosition(x, y);
        sprite.setScale(0.3f, 0.3f);

        velocityY = 0.f;
        isJumping = false;
    }

    void jump() {
        if (!isJumping) {
            isJumping = true;
            velocityY = JUMP_STRENGTH;
        }
    }

    void update() {
        velocityY += GRAVITY;
        sprite.move(0, velocityY);

        if (sprite.getPosition().y + sprite.getGlobalBounds().height >= WINDOW_HEIGHT - GROUND_HEIGHT) {
            sprite.setPosition(sprite.getPosition().x, WINDOW_HEIGHT - GROUND_HEIGHT - sprite.getGlobalBounds().height);
            velocityY = 0.f;
            isJumping = false;
        }

        if (textureClock.getElapsedTime().asSeconds() > .2f) {
            currentTextureIndex = (currentTextureIndex + 1) % textures.size();
            sprite.setTexture(textures[currentTextureIndex]);
            textureClock.restart();
        }
    }

    float getDistance() const {
        return sprite.getPosition().x;
    }

    void resetPosition() {
        sprite.setPosition(50.f, WINDOW_HEIGHT - GROUND_HEIGHT - sprite.getGlobalBounds().height);
        velocityY = 0.f;
        isJumping = false;
    }
};

// Class for Obstacles with image
class Obstacle {
public:
    sf::Sprite sprite;
    sf::Texture texture;
    bool isPassed;

    Obstacle(float x, float y) : isPassed(false) {
        if (!texture.loadFromFile("obstacle.png")) {
            std::cerr << "Error: Could not load obstacle texture 'obstacle.png'!" << std::endl;
            exit(-1);
        }

        sprite.setTexture(texture);
        sprite.setScale(0.5f, 0.5f);

        float obstacleHeight = texture.getSize().y * sprite.getScale().y;
        sprite.setPosition(x, WINDOW_HEIGHT - GROUND_HEIGHT - obstacleHeight);
    }

    void update(float speed) {
        sprite.move(speed, 0);
    }
};

// Function to load the high score from a file
int loadHighScore() {
    std::ifstream infile(SCORE_FILE);
    int highScore = 0;
    if (infile.is_open()) {
        infile >> highScore;
        infile.close();
    }
    return highScore;
}

// Function to save the high score to a file
void saveHighScore(int score) {
    std::ofstream outfile(SCORE_FILE);
    if (outfile.is_open()) {
        outfile << score;
        outfile.close();
    }
}

// Function to reset the game state
void resetGame(std::vector<Obstacle>& obstacles, Puppy& puppy, float& score, float& obstacleSpeed, int highScore) {
    obstacles.clear();
    puppy.resetPosition();
    score = 0.0f;
    obstacleSpeed = -6.f; // Reset to initial speed
}

void playPuppyRun() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    int highScore = loadHighScore();
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Puppy Endless Runner");
    window.setFramerateLimit(60);

    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    if (!backgroundTexture.loadFromFile("background.png")) {
        std::cerr << "Error: Could not load background texture 'background.png'!" << std::endl;
        return;
    }
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setPosition(0, 0);

    sf::Texture groundTexture;
    if (!groundTexture.loadFromFile("ground.png")) {
        std::cerr << "Error: Could not load ground texture 'ground.png'!" << std::endl;
        return;
    }

    sf::Sprite groundSprite;
    groundSprite.setTexture(groundTexture);

    float groundWidth = static_cast<float>(groundTexture.getSize().x);
    float groundHeight = static_cast<float>(groundTexture.getSize().y);
    float scaleX = WINDOW_WIDTH / groundWidth;
    float scaleY = GROUND_HEIGHT / groundHeight;
    float scale = std::max(scaleX, scaleY);
    groundSprite.setScale(scale, scale);
    groundSprite.setPosition(0, WINDOW_HEIGHT - GROUND_HEIGHT);

    Puppy puppy(50.f, WINDOW_HEIGHT - GROUND_HEIGHT - 40.f);
    std::vector<Obstacle> obstacles;
    sf::Clock obstacleClock;

    float score = 0.0f;
    sf::Font font;
    sf::Text scoreText;
    sf::Text highScoreText;
    sf::Text gameOverText;

    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error: Could not load font 'arial.ttf'. Using default sans-serif font." << std::endl;
        scoreText.setString("Font Load Failed");
        highScoreText.setString("Font Load Failed");
        gameOverText.setString("Font Load Failed");
    }
    else {
        scoreText.setFont(font);
        highScoreText.setFont(font);
        gameOverText.setFont(font);
        scoreText.setCharacterSize(10);
        scoreText.setFillColor(sf::Color::Black);
        scoreText.setPosition(5, 5);

        highScoreText.setCharacterSize(14);
        highScoreText.setFillColor(sf::Color::Black);
        highScoreText.setPosition(10, 40);

        gameOverText.setCharacterSize(20);
        gameOverText.setFillColor(sf::Color::Red);
        gameOverText.setPosition(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 50);
        gameOverText.setString("Game Over! Press R to Restart");
    }

    sf::Clock speedClock;

    sf::Music backgroundMusic;
    sf::Music obstaclePassedMusic;
    sf::Music gameOverMusic;

    if (!backgroundMusic.openFromFile("background_music.mp3")) {
        std::cerr << "Error: Could not load background music!" << std::endl;
        return;
    }
    if (!obstaclePassedMusic.openFromFile("obstacle_passed_music.mp3")) {
        std::cerr << "Error: Could not load obstacle passed music!" << std::endl;
        return;
    }
    if (!gameOverMusic.openFromFile("game_over_music.mp3")) {
        std::cerr << "Error: Could not load game over music!" << std::endl;
        return;
    }

    backgroundMusic.setLoop(true);
    backgroundMusic.play();

    bool gameOver = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space && !gameOver) {
                    puppy.jump();
                }

                if (event.key.code == sf::Keyboard::R && gameOver) {
                    resetGame(obstacles, puppy, score, OBSTACLE_SPEED, highScore);
                    gameOver = false;
                    backgroundMusic.play();
                }
            }
        }

        if (!gameOver) {
            puppy.update();

            if (speedClock.getElapsedTime().asSeconds() > SPEED_INCREASE_INTERVAL) {
                OBSTACLE_SPEED -= SPEED_INCREMENT;
                if (OBSTACLE_SPEED < MIN_OBSTACLE_SPEED) {
                    OBSTACLE_SPEED = MIN_OBSTACLE_SPEED;
                }
                speedClock.restart();
            }

            if (obstacleClock.getElapsedTime().asSeconds() > 2.0f) {
                float randomY = WINDOW_HEIGHT - GROUND_HEIGHT - 30;
                obstacles.emplace_back(WINDOW_WIDTH, randomY);
                obstacleClock.restart();
            }

            for (auto& obstacle : obstacles) {
                obstacle.update(OBSTACLE_SPEED);

                if (puppy.sprite.getGlobalBounds().intersects(obstacle.sprite.getGlobalBounds())) {
                    gameOver = true;
                    backgroundMusic.stop();
                    gameOverMusic.play();
                    if (score > highScore) {
                        highScore = static_cast<int>(score);
                        saveHighScore(highScore);
                    }
                }

                if (!obstacle.isPassed && obstacle.sprite.getPosition().x + obstacle.sprite.getGlobalBounds().width < puppy.sprite.getPosition().x) {
                    score += 10;
                    obstacle.isPassed = true;
                    obstaclePassedMusic.play();
                }
            }

            obstacles.erase(
                std::remove_if(obstacles.begin(), obstacles.end(), [](const Obstacle& obs) {
                    return obs.sprite.getPosition().x + obs.sprite.getGlobalBounds().width < 0;
                    }),
                obstacles.end()
            );

            scoreText.setString("Score: " + std::to_string(static_cast<int>(score)));
            highScoreText.setString("High Score: " + std::to_string(highScore));
        }

        window.clear();
        window.draw(backgroundSprite);
        window.draw(groundSprite);
        window.draw(puppy.sprite);
        for (const auto& obstacle : obstacles) {
            window.draw(obstacle.sprite);
        }
        window.draw(scoreText);
        window.draw(highScoreText);

        if (gameOver) {
            window.draw(gameOverText);
        }

        window.display();
    }
}
