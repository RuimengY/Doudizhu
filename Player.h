#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include "Brand.h"
#include <iostream>

class Player
{
public:
    Player();
    Player(const std::string &name);

    void addCard(const std::string &card);
    void removeCard(const std::vector<std::string> &cards);

    const std::vector<std::string> &getHand() const;
    void printHand() const;
    void setName(const std::string &name);
    void setHand(const std::vector<std::string> &hand);
    std::string getName() const;

private:
    std::string name;
    std::vector<std::string> hand;
};

#endif // PLAYER_H