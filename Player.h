#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include "Brand.h" // 假设Brand类在Brand.h中定义

class Player
{
public:
    Player(const std::string &name);

    void addCard(const std::string &card);
    void removeCard(const std::string &card);
    bool canPlayCard(const std::string &card) const;

    const std::vector<std::string> &getHand() const;

private:
    std::string name;
    std::vector<std::string> hand;
};

#endif // PLAYER_H