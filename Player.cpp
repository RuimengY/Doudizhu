#include "Player.h"
#include <algorithm>
#include <stdexcept>

Player::Player(const std::string &name) : name(name) {}
Player::Player() : name("") {}

void Player::setName(const std::string &name)
{
    this->name = name;
}
void Player::addCard(const std::string &card)
{
    hand.push_back(card);
}

void Player::removeCard(const std::vector<std::string> &cards)
{
    for (const auto &card : cards)
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
}
const std::vector<std::string> &Player::getHand() const // 查看自己的手牌
{
    return hand;
}

void Player::printHand() const
{
    for (const auto &card : hand)
    {
        std::cout << card << " ";
    }
    std::cout << std::endl;
}

void Player::setHand(const std::vector<std::string> &hand)
{
    this->hand = hand;
}

std::string Player::getName() const
{
    return name;
}