#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "Card.h"

class Player
{
public:
    enum Role
    {
        PLAYER,
        LANDLORD
    };

    std::vector<Card> hand;
    Role role;

    Player(Role r, const std::vector<Card> &initialHand);

    void showHand() const;
};

#endif // PLAYER_H