#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "Card.h"

class Player
{
public:
    std::vector<Card> hand;

    void showHand() const;
};

#endif // PLAYER_H