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

void Player::sort()
{
    Brand brand(hand); // 借助Brand中的排序函数对手牌进行排序
    hand = brand.getCardList();
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

std::vector<std::string> Player::getHand() const // 查看自己的手牌
{
    return hand;
}

// 打印手牌
void Player::printHand() const
{
    std::cout << name << "'s hand: ";
    for (const auto &card : hand)
    {
        std::cout << card << " ";
    }
    std::cout << std::endl;
}

void Player::setHand(const std::vector<std::string> &hand)
{
    Brand brand(hand); // 借助Brand中的排序函数对手牌进行排序
    this->hand = brand.getCardList();
}

std::string Player::getName() const
{
    return name;
}