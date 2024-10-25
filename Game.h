#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>
#include "Card.h"
#include "Player.h"

class Game
{
private:
    static std::vector<std::string> brands; // 表示一副牌
    std::vector<Player> players;

public:
    static std::vector<std::string> DEFAULT_BRAND;
    // brandmap中存储了所有的牌型以及对应的索引，键是牌型，值是索引(1-54)
    static std::unordered_map<std::string, int> brandMap;
    static const std::vector<std::string> NUMBER;
    static const std::vector<std::string> COLOUR;

    Game();
    void initializeDEFAULT_BRAND();
    void shuffleDeck();
    void dealCards();
    void startGame();
};

#endif // GAME_H