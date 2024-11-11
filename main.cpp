#include "Brand.h"
#include "Game.h"
#include "Player.h"
#include "Card.h"
#include <algorithm>
#include <iostream>

int main()
{
    char choice;
    do
    {
        Game game;

        game.playGame();
        std::cout << "Do you want to play again? (y/n): ";
        std::cin >> choice;
    } while (choice == 'y' || choice == 'Y');

    return 0;
}