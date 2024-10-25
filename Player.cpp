#include "Player.h"
#include <algorithm>
#include <stdexcept>

Player::Player(const std::string &name) : name(name) {}

void Player::addCard(const std::string &card)
{
    hand.push_back(card);
}

void Player::removeCard(const std::string &card)
{
    auto it = std::find(hand.begin(), hand.end(), card);
    if (it != hand.end())
    {
        hand.erase(it);
    }
    else
    {
        throw std::invalid_argument("Card not found in hand");
    }
}

bool Player::canPlayCard(const std::string &card) const
{
    return Brand::canPlay(card); // 假设Brand类有静态方法Judge
}

const std::vector<std::string> &Player::getHand() const
{
    return hand;
}