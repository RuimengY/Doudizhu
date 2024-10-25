#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>

class Game
{
public:
    Game();

private:
    void shuffleDeck();
    void dealCards();

    std::vector<std::string> DEFAULT_BRAND;
    std::vector<std::string> brands;
};

#endif // GAME_H