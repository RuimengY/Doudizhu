#ifndef CARD_H
#define CARD_H

#include <string>
#include <unordered_map>
#include <vector>

class Card
{
public:
    static std::unordered_map<std::string, int> cardMap; // 哈希表，用于存储牌的字符串表示和大小的映射
    Card(const std::string &str);

    int getValue() const;
    std::string getString() const;

    static void initializeCardMap();
    static void initializeDefaultBrand(std::vector<std::string> &defaultBrand, std::vector<std::string> &brands);

private:
    std::string strRepresentation; // 牌的字符串表示
    int value;                     // 牌的大小
};

#endif // CARD_H