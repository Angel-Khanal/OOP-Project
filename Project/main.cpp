#include <iostream>
#include<cstdlib>
#include<SFML/Graphics.hpp>
void playTetris();
void playTicTacToe();  
void playPuppyRun(); 
void playHangman();

int main() {
    int choice;
    do {
        std::cout << "Select a game to play:\n";
        std::cout << "1. Tetris\n";
        std::cout << "2. Tic-Tac-Toe\n";
        std::cout << "3. Puppy Run\n";
        std::cout << "4. Hangman\n";
        std::cout << "5. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            playTetris();
            break;
        case 2:
            playTicTacToe();  
            break;
        case 3:
            playPuppyRun();  
            break;
        case 4:
            playHangman();
            break;
        case 5:
            std::cout << "Exiting...\n";
            exit(0);
        default:
            std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 0);

}