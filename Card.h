#ifndef CARD_H
#define CARD_H

#include <string>
#include <unordered_map>
#include <vector>

class Card
{
public:
    Card(const std::string &str);

    int getValue() const;
    std::string getString() const;

private:
    std::string strRepresentation; // 牌的字符串表示
    int value;                     // 牌的大小
};

#endif // CARD_H