#include "Player.h"
#include <iostream>
Player::Player(Role r, const std::vector<Card> &initialHand)
    : role(r), hand(initialHand)
{
}

void Player::showHand() const
{
    // 显示手牌的逻辑
}
/*
void Player::showHand() const
{
    for (const auto &card : hand)
    {
        std::cout << card.toString() << " ";
    }
    std::cout << std::endl;
}

*/
