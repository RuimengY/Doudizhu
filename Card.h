#ifndef CARD_H
#define CARD_H

#include <string>
#include <vector>
#include <unordered_map>
#include <map>

class Card
{
public:
    // brandmap中存储了所有的牌型以及对应的索引，键是牌型，值是索引(1-54)
    static std::unordered_map<std::string, int> brandMap;

    static const std::vector<std::string> NUMBER;
    static const std::vector<std::string> COLOUR;
    static std::vector<std::string> DEFAULT_BRAND;

    Card(const std::vector<std::string> &brands);

    void sort();

    bool Card::compareSize(Card &card);
    std::pair<int, int> Card::updateType();

    bool judgeFixedType(int len, int seriesNum, int repeatNum) const;
    bool Card::isSeries(const std::vector<int> &a) const;
    bool isRepeat(int repeatNum) const;
    int Card::getCardValue() const;

    bool Card::isStraight() const;
    bool Card::isConsecutivePairs() const;
    bool Card::isPlane() const;
    int Card::isPlaneWithWings() const;

    static int getBrandIndex(const std::string &brand);
    static std::string getBrandByIndex(int index);
    static int getBrandSize(const std::string &brand);
    bool extent(const Card &card) const;
    std::string printBrand() const;
    bool isOff() const;

private:
    int brandLength = 0; // brandLength指一个牌组的长度

    std::vector<int> brandSize; // brandSize是牌组中每一个输入牌的大小(1-54)

    // brandNum是一个哈希表，键是牌的大小(1-13)，值是牌的数量
    std::unordered_map<int, int> brandNum;

    int type = -1; // type指牌型

    // 为什么是私有化变量？是不是要在Game.cpp中用
    void initializeBrandNum();
};

#endif