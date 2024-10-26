#ifndef BRAND_H
#define BRAND_H

#include <string>
#include <vector>
#include "Card.h"

class Brand
{
public:
    Brand(const std::vector<std::string> &cards);
    int getBrandIndex(const std::string &brand);
    void initializeBrandNum();
    void sort();

    bool canPlay(Brand &enemyBrand);

    std::pair<int, int> updateType();
    bool judgeFixedType(int len, int seriesNum, int repeatNum) const;
    bool isSeries(const std::vector<int> &a) const;
    bool isRepeat(int repeatNum) const;
    int getCardValue() const;

    bool isStraight() const;
    bool isConsecutivePairs() const;
    bool isPlane() const;
    int isPlaneWithWings() const;

private:
    std::vector<std::string> cards; // cards是一个字符串数组，里面存储了所有的牌(字符串)
    std::vector<Card> cardList;     // cardList是一个牌组，里面存储了所有的牌(Card类)

    int brandLength = 0; // brandLength指一个牌组的长度

    std::vector<int> brandSize; // brandSize是牌组中每一个输入牌的大小(1-54)

    // brandNum是一个哈希表，键是牌的大小(1-13)，值是牌的数量
    std::unordered_map<int, int> brandNum;

    int type = -1; // type指牌型
    static std::unordered_map<std::string, int> brandMap;
};

#endif // BRAND_H