#ifndef BRAND_H
#define BRAND_H

#include <vector>
#include <string>
#include <unordered_map>

class Brand
{
public:
    Brand(const std::vector<std::string> &cards);
    void initializeBrandMap();
    bool canPlay(Brand &enemyBrand);
    std::pair<int, int> updateType();
    std::pair<int, int> getCardType();
    int getCardValue() const;
    bool isSeries(const std::vector<int> &a) const;
    bool isRepeat(int repeatNum) const;
    bool isPlane() const;
    bool isConsecutivePairs() const;
    bool isStraight() const;
    int isPlaneWithWings() const;
    bool judgeFixedType(int length, int num, int times) const;

private:
    void initializeBrandNum();
    void sort();

    std::pair<int, int> result;                    // 牌型和关键值
    size_t brandLength;                            // 牌的数量
    std::vector<int> brandSize;                    // 每张牌对应的大小(1-54)
    std::unordered_map<std::string, int> brandMap; // 牌型哈希表(某种牌型和对应的数字)
    std::unordered_map<int, int> brandNum;         // 牌的数量哈希表(某个数字和对应的数量)
    std::vector<std::string> cardList;             // 字符串牌型
};

#endif // BRAND_H