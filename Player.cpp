#include "Player.h"
#include <iostream>

void Player::showHand() const
{
    for (const auto &card : hand)
    {
        std::cout << card.toString() << " ";
    }
    std::cout << std::endl;
}