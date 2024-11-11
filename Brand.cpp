#include "Brand.h"
#include "card.h"
#include "Game.h"
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <map>

const std::vector<std::string> NUMBER = {"3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A", "2", "joker"};
const std::vector<std::string> COLOUR = {"♥", "♠", "♣", "♦"};
Brand::Brand(const std::vector<std::string> &cards)
{
    brandLength = cards.size();
    brandSize.resize(brandLength); // brandSize是每张牌对应的大小
    initializeBrandMap();
    for (const auto &cardStr : cards)
    {
        cardList.emplace_back(cardStr);
    }
    for (int i = 0; i < brandLength; ++i)
    {
        brandSize[i] = brandMap[cardList[i]];
    }
    std::sort(brandSize.begin(), brandSize.end()); // 对brandSize进行排序
    initializeBrandNum();
    sort();
}
void Brand::initializeBrandMap()
{
    if (brandMap.size() != 0)
        return;
    int index = 0;
    for (const auto &number : NUMBER)
    {
        for (const auto &colour : COLOUR)
        {
            brandMap[colour + number] = index++;
        }
    }
    brandMap["小王"] = 52;
    brandMap["大王"] = 53;
}

void Brand::initializeBrandNum()
{
    brandNum.clear(); // 清空之前的内容
    for (int i = 0; i < brandLength; ++i)
    {
        brandNum[brandSize[i] / 4]++; // 代表某个张数的牌有多少张
    }
}

// 对cardList进行排序
void Brand::sort()
{
    for (int i = 0; i < brandLength; ++i)
    {
        if (brandSize[i] == 52)
        {
            cardList[i] = "小王";
        }
        else if (brandSize[i] == 53)
        {
            cardList[i] = "大王";
        }
        else
        {
            cardList[i] = COLOUR[brandSize[i] % 4] + NUMBER[brandSize[i] / 4];
        }
    }
}

bool Brand::canPlay(Brand &tableBrand)
{
    std::pair<int, int> thisCard = updateType();
    std::pair<int, int> cardCard = tableBrand.updateType();

    // 如果出的牌是鬼牌或者炸弹，一定可以出牌
    if (thisCard.first == 10 || thisCard.first == 9)
    {
        return true;
    }
    // 如果长度不一样，不能出牌
    if (brandLength != tableBrand.brandLength)
    {
        std::cout << "长度不一样，不能出牌" << std::endl;
        return false;
    }
    if (thisCard.first == -1 || cardCard.first == -1)
    {
        std::cout << "不能这么出牌，请重新出牌" << std::endl;
        // 关于重新出牌的逻辑
        return false;
    }
    if (thisCard.first != cardCard.first)
    {
        std::cout << "牌型不一样，不能出牌" << std::endl;
        return false;
    }
    if (thisCard.second <= cardCard.second)
    {
    std:
        std::cout << "要出的牌的最小值" << std::endl;
        std::cout << thisCard.second << std::endl;
        std::cout << "牌太小，不能出牌" << std::endl;
        return false;
    }
    return true;
}

std::vector<std::vector<std::string>> Brand::findStraights() const
{
    std::vector<std::string> straightCards;
    std::vector<std::vector<std::string>> allStraights;
    int brandLength = cardList.size();

    // 滑动窗口的大小可以是5到13
    const std::vector<int> windowSizes = {5, 6, 7, 8, 9, 10, 11, 12, 13};

    // 遍历所有可能的窗口大小
    for (const int windowSize : windowSizes)
    {
        // 遍历所有可能的起始位置
        for (int i = 0; i <= brandLength - windowSize; ++i)
        {
            for (int j = 0; j < windowSize; ++j)
            {
                straightCards.emplace_back(cardList[i + j]);
            }
            Brand temp(straightCards);
            if (temp.isStraight())
            {
                allStraights.push_back(straightCards);
            }
            straightCards.clear();
        }
    }
    return allStraights;
}

std::vector<std::string> Brand::hasStraight() const
{
    if (brandLength < 5)
    {
        return {};
    }
    auto allStraights = findStraights();
    if (!allStraights.empty())
    {
        return allStraights[0]; // 返回找到的第一个顺子
    }
    return {};
}

std::vector<std::string> Brand::hasStraight(const std::pair<int, int> &cardType, int size) const
{
    if (brandLength < 5)
    {
        return {};
    }
    auto allStraights = findStraights();
    for (const auto &straight : allStraights)
    {
        if (straight.size() == size)
        {
            Brand temp(straight);
            if (temp.updateType().second > cardType.second)
            {
                return straight;
            }
        }
    }
    return {};
}

std::vector<std::vector<std::string>> Brand::findConsecutivePairs() const
{
    std::vector<std::vector<std::string>> allPairs;
    int brandLength = cardList.size();
    if (brandLength < 6)
    {
        return allPairs;
    }

    std::vector<std::string> pairs;

    // 遍历牌组，寻找相邻两个牌值相等的牌
    for (int i = 0; i < brandLength - 1; ++i)
    {
        if (brandSize[i] / 4 == brandSize[i + 1] / 4)
        {
            pairs.push_back(cardList[i]);
            pairs.push_back(cardList[i + 1]);

            ++i; // 跳过下一个牌，因为已经处理过
        }
    }
    // 将pairs数组分成所有可能的6, 8, 10, 12组合
    for (int windowSize = 6; windowSize <= pairs.size(); windowSize += 2)
    {
        for (int i = 0; i <= pairs.size() - windowSize; i += 2)
        {
            std::vector<std::string> window(pairs.begin() + i, pairs.begin() + i + windowSize);

            Brand temp(window);
            if (temp.isConsecutivePairs())
            {
                allPairs.push_back(window);
            }
        }
    }

    return allPairs;
}

std::vector<std::string> Brand::hasConsecutivePairs() const
{
    if (brandLength < 6)
    {
        return {};
    }
    auto allPairs = findConsecutivePairs();
    if (!allPairs.empty())
    {
        return allPairs[0]; // 返回找到的第一个连对
    }
    return {};
}

std::vector<std::string> Brand::hasConsecutivePairs(const std::pair<int, int> &cardType, int size) const
{
    if (brandLength < 6)
    {
        return {};
    }
    auto allPairs = findConsecutivePairs();
    for (const auto &pair : allPairs)
    {
        if (pair.size() == size)
        {
            Brand temp(pair);
            if (temp.updateType().second > cardType.second)
            {
                return pair;
            }
        }
    }
    return {};
}

std::vector<std::vector<std::string>> Brand::findPlanes() const
{
    std::vector<std::string> planeCards;
    std::vector<std::vector<std::string>> allPlanes;
    int brandLength = cardList.size();

    // 遍历所有可能的窗口位置
    for (int i = 0; i <= brandLength - 6; ++i)
    {
        for (int windowSize = 6; windowSize <= brandLength - i; windowSize += 3)
        {
            for (int j = 0; j < windowSize; ++j)
            {
                planeCards.emplace_back(cardList[i + j]);
            }
            Brand temp(planeCards);
            if (temp.isPlane())
            {
                allPlanes.push_back(planeCards);
            }
            planeCards.clear();
        }
    }
    return allPlanes;
}

std::vector<std::string> Brand::hasPlane() const
{
    if (brandLength < 6)
    {
        return {};
    }
    auto allPlanes = findPlanes();
    if (!allPlanes.empty())
    {
        return allPlanes[0]; // 返回找到的第一个飞机
    }
    return {};
}

std::vector<std::string> Brand::hasPlane(const std::pair<int, int> &cardType, int size) const
{
    if (brandLength < 6)
    {
        return {};
    }
    auto allPlanes = findPlanes();
    for (const auto &plane : allPlanes)
    {
        if (plane.size() == size)
        {
            Brand temp(plane);
            if (temp.updateType().second > cardType.second)
            {
                return plane;
            }
        }
    }
    return {};
}

std::vector<std::vector<std::string>> Brand::findPlaneWithWings() const
{
    std::vector<std::string> planeWithWingsCards;
    std::vector<std::vector<std::string>> allPlanesWithWings;
    const int windowSize = 8;
    int brandLength = cardList.size();

    // 遍历所有可能的窗口位置
    for (int i = 0; i <= brandLength - windowSize; ++i)
    {
        for (int j = 0; j < windowSize; ++j)
        {
            planeWithWingsCards.emplace_back(cardList[i + j]);
        }
        Brand temp(planeWithWingsCards);
        if (temp.isPlaneWithWings() != -1)
        {
            allPlanesWithWings.push_back(planeWithWingsCards);
        }
        planeWithWingsCards.clear();
    }

    return allPlanesWithWings;
}

std::vector<std::string> Brand::hasPlaneWithWings() const
{
    if (brandLength < 8)
    {
        return {};
    }
    auto allPlanesWithWings = findPlaneWithWings();
    if (!allPlanesWithWings.empty())
    {
        return allPlanesWithWings[0]; // 返回找到的第一个结果
    }
    return {};
}

std::vector<std::string> Brand::hasPlaneWithWings(const std::pair<int, int> &cardType) const
{
    if (brandLength < 8)
    {
        return {};
    }
    auto allPlanesWithWings = findPlaneWithWings();
    for (const auto &plane : allPlanesWithWings)
    {
        Brand temp(plane);
        if (temp.updateType().second > cardType.second)
        {
            return plane;
        }
    }
    return {};
}

std::vector<std::vector<std::string>> Brand::findThreeWithTwo() const
{
    std::vector<std::vector<std::string>> allThreeWithTwo;
    std::vector<std::string> result;

    // 遍历牌组，寻找三带二的牌型
    for (size_t i = 0; i + 2 < cardList.size(); ++i)
    {
        if (brandNum.at(brandSize[i] / 4) == 3)
        {
            for (size_t j = 0; j + 1 < cardList.size(); ++j)
            {
                if (i != j && brandNum.at(brandSize[j] / 4) == 2)
                {
                    result.push_back(cardList[i]);
                    result.push_back(cardList[i + 1]);
                    result.push_back(cardList[i + 2]);
                    result.push_back(cardList[j]);
                    result.push_back(cardList[j + 1]);

                    allThreeWithTwo.push_back(result);
                    result.clear();
                }
            }
        }
    }

    return allThreeWithTwo;
}
std::vector<std::string> Brand::hasThreeWithTwo() const
{
    if (brandLength < 5)
    {
        return {};
    }
    auto allThreeWithTwo = findThreeWithTwo();
    if (!allThreeWithTwo.empty())
    {
        return allThreeWithTwo[0]; // 返回找到的第一个结果
    }
    return {};
}
std::vector<std::string> Brand::hasThreeWithTwo(const std::pair<int, int> &cardType) const
{
    if (brandLength < 5)
    {
        return {};
    }
    auto allThreeWithTwo = findThreeWithTwo();
    for (const auto &threeWithTwo : allThreeWithTwo)
    {
        Brand temp(threeWithTwo);
        if (temp.updateType().second > cardType.second)
        {
            return threeWithTwo;
        }
    }
    return {};
}

std::vector<std::vector<std::string>> Brand::findThreeWithOne() const
{
    if (brandLength < 4)
    {
        return {};
    }
    std::vector<std::vector<std::string>> allThreeWithOne;
    std::vector<std::string> result;
    // 遍历牌组，寻找三带一的牌型
    for (size_t i = 0; i + 2 < cardList.size(); ++i)
    {
        if (brandNum.at(brandSize[i] / 4) == 3)
        {
            for (size_t j = 0; j < cardList.size(); ++j)
            {
                if (i != j && brandNum.at(brandSize[j] / 4) == 1)
                {
                    result.push_back(cardList[i]);
                    result.push_back(cardList[i + 1]);
                    result.push_back(cardList[i + 2]);
                    result.push_back(cardList[j]);

                    allThreeWithOne.push_back(result);
                    result.clear();
                }
            }
        }
    }

    return allThreeWithOne;
}
std::vector<std::string> Brand::hasThreeWithOne() const
{
    if (brandLength < 4)
    {
        return {};
    }
    auto allThreeWithOne = findThreeWithOne();
    if (!allThreeWithOne.empty())
    {
        return allThreeWithOne[0]; // 返回找到的第一个结果
    }
    return {};
}
std::vector<std::string> Brand::hasThreeWithOne(const std::pair<int, int> &cardType) const
{
    if (brandLength < 4)
    {
        return {};
    }
    auto allThreeWithOne = findThreeWithOne();
    for (const auto &threeWithOne : allThreeWithOne)
    {
        Brand temp(threeWithOne);
        if (temp.updateType().second > cardType.second)
        {
            return threeWithOne;
        }
    }
    return {};
}

std::vector<std::vector<std::string>> Brand::findThreeOfAKind() const
{
    std::vector<std::vector<std::string>> allThreeOfAKind;
    std::vector<std::string> result;
    if (cardList.size() < 3)
    {
        return allThreeOfAKind;
    }
    // 遍历牌组，寻找三张相同的牌型
    for (int i = 0; i < cardList.size() - 2; ++i)
    {
        if (brandNum.at(brandSize[i] / 4) == 3)
        {
            result.push_back(cardList[i]);
            result.push_back(cardList[i + 1]);
            result.push_back(cardList[i + 2]);

            allThreeOfAKind.push_back(result);
            result.clear();
        }
    }

    return allThreeOfAKind;
}
std::vector<std::string> Brand::hasThreeOfAKind() const
{
    if (brandLength < 3)
    {
        return {};
    }
    auto allThreeOfAKind = findThreeOfAKind();
    if (!allThreeOfAKind.empty())
    {
        return allThreeOfAKind[0]; // 返回找到的第一个结果
    }
    return {};
}
std::vector<std::string> Brand::hasThreeOfAKind(const std::pair<int, int> &cardType) const
{
    if (brandLength < 3)
    {
        return {};
    }
    auto allThreeOfAKind = findThreeOfAKind();
    if (allThreeOfAKind.empty())
    {
        return {};
    }
    for (const auto &threeOfAKind : allThreeOfAKind)
    {
        Brand temp(threeOfAKind);
        if (temp.updateType().second > cardType.second)
        {
            return threeOfAKind;
        }
    }
    return {};
}

std::vector<std::vector<std::string>> Brand::findTwoOfAKind() const
{
    std::vector<std::vector<std::string>> allTwoOfAKind;
    std::vector<std::string> result;
    if (cardList.size() < 2)
    {
        return allTwoOfAKind;
    }
    // 遍历牌组，寻找两张相同的牌型
    for (size_t i = 0; i < cardList.size() - 1; ++i)
    {
        // 判断当前牌和下一张牌是否相同
        if (brandSize[i] / 4 == brandSize[i + 1] / 4)
        {
            result.push_back(cardList[i]);
            result.push_back(cardList[i + 1]);

            allTwoOfAKind.push_back(result);
            result.clear();

            // 跳过下一张牌，因为已经处理过
            ++i;
        }
    }
    return allTwoOfAKind;
}
std::vector<std::string> Brand::hasTwoOfAKind() const
{
    if (brandLength < 2)
    {
        return {};
    }
    auto allTwoOfAKind = findTwoOfAKind();
    if (!allTwoOfAKind.empty())
    {
        return allTwoOfAKind[0]; // 返回找到的第一个结果
    }
    return {};
}
std::vector<std::string> Brand::hasTwoOfAKind(const std::pair<int, int> &cardType) const
{
    if (brandLength < 2)
    {
        return {};
    }
    auto allTwoOfAKind = findTwoOfAKind();
    if (allTwoOfAKind.empty())
    {
        return {};
    }
    for (const auto &twoOfAKind : allTwoOfAKind)
    {
        Brand temp(twoOfAKind);
        if (temp.updateType().second > cardType.second)
        {
            return twoOfAKind;
        }
    }
    return {};
}

std::vector<std::string> Brand::autoChoose()
{
    std::vector<std::string> autoCards;
    // 寻找有没有鬼牌(大王和小王)，有鬼牌先出
    // 大王和小王是最后两张牌
    if (brandLength >= 2 && brandSize[brandLength - 1] == 53 && brandSize[brandLength - 2] == 52)
    {
        autoCards.emplace_back("大王");
        autoCards.emplace_back("小王");
        return autoCards;
    }

    // 寻找有没有炸弹，有炸弹先出
    if (brandLength >= 4)
    {
        for (int i = 0; i < brandLength - 3; ++i)
        {
            if (brandNum[brandSize[i] / 4] == 4)
            {
                if (i + 3 < brandLength)
                {
                    for (int j = 0; j < 4; ++j)
                    {
                        autoCards.emplace_back(cardList[i + j]);
                    }
                    return autoCards;
                }
            }
        }
    }
    // 寻找有没有连牌
    autoCards = hasStraight();
    if (!autoCards.empty())
    {
        return autoCards;
    }
    // 寻找有没有连对
    autoCards = hasConsecutivePairs();
    if (!autoCards.empty())
    {
        return autoCards;
    }
    // 寻找有没有飞机
    autoCards = hasPlane();
    if (!autoCards.empty())
    {
        return autoCards;
    }
    // 寻找有没有飞机带翅膀
    autoCards = hasPlaneWithWings();
    if (!autoCards.empty())
    {
        return autoCards;
    }
    // 寻找有没有三带二
    autoCards = hasThreeWithTwo();
    if (!autoCards.empty())
    {
        return autoCards;
    }
    // 寻找有没有三张相同的牌
    autoCards = hasThreeOfAKind();
    if (!autoCards.empty())
    {
        return autoCards;
    }
    // 寻找有没有对子
    autoCards = hasTwoOfAKind();
    if (!autoCards.empty())
    {
        return autoCards;
    }
    // 随便返回一张单牌
    for (int i = 0; i < brandLength; ++i)
    {
        if (brandNum[brandSize[i] / 4] == 1)
        {
            autoCards.emplace_back(cardList[i]);
            return autoCards;
        }
    }
    return {};
}

std::vector<std::string> Brand::chooseCards(std::vector<std::string> &currentBrand)
{
    if (currentBrand.empty())
    {
        return autoChoose();
    }
    // 确定当前桌子上的牌的类型
    Brand tableBrand(currentBrand);
    std::pair<int, int> cardType = tableBrand.updateType();

    std::vector<std::string> autoCards;
    switch (cardType.first)
    {
    case 10:
    case 9:
        return {};
    case 8:
        // 寻找有没有顺子
        autoCards = hasStraight(cardType, currentBrand.size());
        if (!autoCards.empty())
        {
            return autoCards;
        }

        return {};
    case 7:
        // 寻找有没有连对
        autoCards = hasConsecutivePairs(cardType, currentBrand.size());
        if (!autoCards.empty())
        {
            return autoCards;
        }

        return {};
    case 6:
        // 寻找有没有飞机
        autoCards = hasPlane(cardType, currentBrand.size());
        if (!autoCards.empty())
        {
            return autoCards;
        }
        return {};

    case 5:
        // 寻找有没有飞机带翅膀
        autoCards = hasPlaneWithWings(cardType);
        if (!autoCards.empty())
        {
            return autoCards;
        }

        return {};
    case 4:
        // 寻找有没有三带二
        autoCards = hasThreeWithTwo(cardType);
        if (!autoCards.empty())
        {
            return autoCards;
        }
        return {};
    case 3:
        // 寻找有没有三带一
        autoCards = hasThreeWithOne(cardType);
        if (!autoCards.empty())
        {
            return autoCards;
        }
        return {};

    case 2:
        // 寻找有没有三张一样的牌
        autoCards = hasThreeOfAKind(cardType);
        if (!autoCards.empty())
        {
            return autoCards;
        }
        return {};
    case 1:
        // 寻找有没有对子
        autoCards = hasTwoOfAKind(cardType);
        if (!autoCards.empty())
        {
            return autoCards;
        }
        return {};
    case 0:
        // 寻找有没有单牌
        for (int i = 0; i < brandLength; ++i)
        {
            if (brandSize[i] / 4 > cardType.second)
            {
                autoCards.emplace_back(cardList[i]);
                return autoCards;
            }
        }

    default:
        return {};
    }
}

std::pair<int, int> Brand::updateType()
{
    // 鬼牌(两张牌，且一张是小王，一张是大王)
    if (brandLength == 2 && brandSize[0] == 52 && brandSize[1] == 53)
    {
        result.first = 10; // 鬼牌
        result.second = 0; // 鬼牌没有关键值
        return result;
    }

    // 炸弹的情况(有四张相同的牌)
    if (judgeFixedType(4, 1, 4))
    {
        result.first = 9;               // 炸弹
        result.second = getCardValue(); // 获取牌的大小
        return result;
    }

    // 顺子的情况(至少5张连续的牌)
    if (isStraight())
    {
        result.first = 8;                 // 顺子
        result.second = brandSize[0] / 4; // 获取最小一张牌的大小
        return result;
    }

    // 连对的情况(三对或三对以上点数相连的牌)
    if (isConsecutivePairs())
    {
        result.first = 7;                 // 连对
        result.second = brandSize[0] / 4; // 获取最小一张牌的大小
        return result;
    }
    // 飞机的情况(两个或多个连续的三张牌)
    if (isPlane())
    {
        result.first = 6;                 // 飞机
        result.second = brandSize[0] / 4; // 获取最小一张牌的大小
        return result;
    }

    // 飞机带翅膀的情况(三个顺子，同数量的单牌或同数量的对牌)
    if (isPlaneWithWings() != -1)
    {
        result.first = 5;                   // 飞机带翅膀
        result.second = isPlaneWithWings(); // 获取最小一张牌的大小
        return result;
    }

    // 三带二的情况(三张相同的牌 + 一副对子)
    if (judgeFixedType(5, 1, 3) && judgeFixedType(5, 1, 2))
    {
        result.first = 4;               // 三带二
        result.second = getCardValue(); // 获取三张牌的大小
        return result;
    }
    // 三带一的情况(三张相同的牌 + 一张随便的牌)
    if (judgeFixedType(4, 1, 3))
    {
        result.first = 3;               // 三带一
        result.second = getCardValue(); // 获取三张牌的大小
        return result;
    }
    // 三张牌的情况(有三张相同的牌)
    if (judgeFixedType(3, 1, 3))
    {
        result.first = 2;                 // 三张牌
        result.second = brandSize[0] / 4; // 获取牌的大小
        return result;
    }
    // 对子的情况(有两张相同的牌)
    if (judgeFixedType(2, 1, 2))
    {
        result.first = 1;                 // 对子
        result.second = brandSize[0] / 4; // 获取牌的大小
        return result;
    }
    if (brandLength == 1)
    {
        result.first = 0; // 单牌

        // 如果是大王，将值加1
        if (brandSize[0] == 53)
        {
            result.second = 14;
            return result;
        }
        result.second = brandSize[0] / 4; // 获取牌的大小
        return result;
    }

    // 未知类型
    result.first = -1;
    result.second = 0;
    return result;
}

// 将出现了三次以及以上的牌的牌值返回
int Brand::getCardValue() const
{
    for (const auto &pair : brandNum)
    {
        if (pair.second >= 3)
        {
            return pair.first;
        }
    }
    return -1; // 如果没有找到出现三次的牌，返回一个特殊值
}

bool Brand::isSeries(int seriesNum) const
{
    if (seriesNum == 1)
    {
        return true;
    }
    int count = 1;
    for (int i = 1; i < brandNum.size(); ++i)
    {
        if (brandSize[i] / 4 == brandSize[i - 1] / 4 + 1)
        {
            count++;
            if (count >= seriesNum)
            {
                return true;
            }
        }
        else
        {
            count = 1;
        }
    }
    return false;
}

// 在输入的牌型中找到是否刚好有n个相同的元素
bool Brand::isRepeat(int repeatNum) const
{
    if (repeatNum == 1)
    {
        return true;
    }
    for (const auto &pair : brandNum)
    {
        if (pair.second == repeatNum)
        {
            return true;
        }
    }
    return false;
}

// 判断数量确定的类型
bool Brand::judgeFixedType(int len, int seriesNum, int repeatNum) const
{
    if (brandLength != len)
    {
        return false;
    }
    bool isSeriesResult = isSeries(seriesNum);
    bool isRepeatResult = isRepeat(repeatNum);
    return isSeriesResult && isRepeatResult;
}

bool Brand::isStraight() const
{
    if (brandLength < 5)
    {
        return false; // 顺子至少需要5张牌
    }
    return isSeries(brandLength);
}

bool Brand::isConsecutivePairs() const
{
    if (brandLength < 6 || brandLength % 2 != 0)
    {
        return false; // 连对至少需要6张牌，且牌的数量是偶数
    }

    std::vector<std::string> temp;
    for (int i = 0; i < brandLength; i += 2)
    {
        if (brandSize[i] / 4 != brandSize[i + 1] / 4)
        {
            return false;
        }
        temp.push_back(cardList[i]);
    }
    // 重新构造一个 brand
    Brand newBrand(temp);

    return newBrand.isSeries(temp.size());
}
bool Brand::isPlane() const
{
    if (brandLength < 6 || brandLength % 3 != 0)
    {
        return false; // 飞机至少需要6张牌，且牌的数量是3的倍数
    }

    std::vector<std::string> temp;
    for (int i = 0; i < brandLength; i += 3)
    {
        if (brandNum.at(brandSize[i] / 4) != 3)
        {
            return false;
        }
        temp.push_back(cardList[i]);
    }

    // 重新构造一个 brand
    Brand newBrand(temp);

    return newBrand.isSeries(temp.size());
}
int Brand::isPlaneWithWings() const
{
    if (brandLength != 8)
        return -1;
    std::vector<int> choose(6);
    int count = 0;
    // 从brandSize中找到六个三张牌
    for (int i = 0; i < brandLength - 1; i++)
    {
        if (brandNum.at(brandSize[i] / 4) == 3)
            choose[count++] = brandSize[i];
    }
    if (count != 6)
        return -1;
    std::sort(choose.begin(), choose.end());
    // 此时choose 0 1 2应该为一个值，choose3 4 5分别除以4后应该为一个值
    if (choose[3] / 4 - choose[0] / 4 != 1)
        return -1;

    return choose[0] / 4;
}

void Brand::printBrand() const
{
    for (const auto &card : cardList)
    {
        std::cout << card << " ";
    }
    std::cout << std::endl;
}

std::pair<int, int> Brand::getCardType()
{
    return result;
}

std::vector<std::string> Brand::getCardList() const
{
    return cardList;
}