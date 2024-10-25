#include "Card.h"
#include <stdexcept>

std::unordered_map<std::string, int> Card::cardMap;

const std::vector<std::string> NUMBER = {"3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A", "2", "joker"};
const std::vector<std::string> COLOUR = {"♥", "♠", "♣", "♦"};

Card::Card(const std::string &str) : strRepresentation(str)
{
    if (cardMap.empty())
    {
        initializeCardMap();
    }

    auto it = cardMap.find(str);
    if (it != cardMap.end())
    {
        value = it->second;
    }
    else
    {
        throw std::invalid_argument("Invalid card string representation");
    }
}

int Card::getValue() const
{
    return value;
}

std::string Card::getString() const
{
    return strRepresentation;
}

void Card::initializeCardMap()
{
    int index = 1;
    for (const auto &number : NUMBER)
    {
        for (const auto &colour : COLOUR)
        {
            cardMap[colour + number] = index++;
        }
    }
    cardMap["小王"] = 53;
    cardMap["大王"] = 54;
}

void Card::initializeDefaultBrand(std::vector<std::string> &defaultBrand, std::vector<std::string> &brands)
{
    defaultBrand[52] = brands[52] = "小王";
    defaultBrand[53] = brands[53] = "大王";
    for (int i = 0; i < 52; ++i)
    {
        defaultBrand[i] = COLOUR[i % 4] + NUMBER[i / 4];
        brands[i] = COLOUR[i % 4] + NUMBER[i / 4];
    }
}