#ifndef GAME_H
#define GAME_H

#include <vector>
#include "Card.h"
#include "Player.h"

class Game
{
private:
    std::vector<Card> deck;
    std::vector<Player> players;

public:
    Game();
    void initializeDeck();
    void shuffleDeck();
    void dealCards();
    void startGame();
};

#endif // GAME_H