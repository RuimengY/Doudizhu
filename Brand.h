#ifndef BRAND_H
#define BRAND_H

#include <vector>
#include <string>
#include <unordered_map>

class Brand
{
public:
    Brand(const std::vector<std::string> &cards);
    std::vector<std::string> getCardList() const;
    void initializeBrandMap();
    bool canPlay(Brand &enemyBrand);
    std::pair<int, int> updateType();
    std::pair<int, int> getCardType();
    int getCardValue() const;
    bool isSeries(int times) const;
    bool isRepeat(int repeatNum) const;
    bool isPlane() const;
    bool isConsecutivePairs() const;
    bool isStraight() const;

    std::vector<std::vector<std::string>> findStraights() const;
    std::vector<std::string> hasStraight() const;
    std::vector<std::string> hasStraight(const std::pair<int, int> &cardType, int size) const;

    std::vector<std::string> hasPlane(const std::pair<int, int> &cardType, int size) const;
    std::vector<std::string> hasPlane() const;
    std::vector<std::vector<std::string>> findPlanes() const;

    std::vector<std::vector<std::string>> findPlaneWithWings() const;
    std::vector<std::string> hasPlaneWithWings() const;
    std::vector<std::string> hasPlaneWithWings(const std::pair<int, int> &cardType) const;

    std::vector<std::string> hasConsecutivePairs(const std::pair<int, int> &cardType, int size) const;
    std::vector<std::string> hasConsecutivePairs() const;
    std::vector<std::vector<std::string>> findConsecutivePairs() const;

    std::vector<std::vector<std::string>> findThreeWithTwo() const;
    std::vector<std::string> hasThreeWithTwo() const;
    std::vector<std::string> hasThreeWithTwo(const std::pair<int, int> &cardType) const;

    std::vector<std::vector<std::string>> findThreeWithOne() const;
    std::vector<std::string> hasThreeWithOne() const;
    std::vector<std::string> hasThreeWithOne(const std::pair<int, int> &cardType) const;

    std::vector<std::vector<std::string>> findThreeOfAKind() const;
    std::vector<std::string> hasThreeOfAKind() const;
    std::vector<std::string> hasThreeOfAKind(const std::pair<int, int> &cardType) const;

    std::vector<std::vector<std::string>> findTwoOfAKind() const;
    std::vector<std::string> hasTwoOfAKind() const;
    std::vector<std::string> hasTwoOfAKind(const std::pair<int, int> &cardType) const;

    int isPlaneWithWings() const;
    std::vector<std::string> chooseCards(std::vector<std::string> &currentBrand);
    std::vector<std::string> autoChoose();
    bool judgeFixedType(int length, int num, int times) const;
    void printBrand() const;
    void sort(); // 对cardList进行排序

private:
    void initializeBrandNum();

    std::vector<std::string> cardList;             // 字符串牌型
    std::pair<int, int> result;                    // 牌型和关键值
    size_t brandLength;                            // 牌的数量
    std::vector<int> brandSize;                    // 每张牌对应的大小(0-53)
    std::unordered_map<std::string, int> brandMap; // 牌型哈希表(某种牌型和对应的数字)
    std::unordered_map<int, int> brandNum;         // 牌的数量哈希表(某个数字和对应的数量)
};

#endif // BRAND_H