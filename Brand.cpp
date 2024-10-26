#include "Brand.h"
#include "card.h"
#include "Game.h"
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <map>

Brand::Brand(const std::vector<std::string> &cards) : cards(cards)
{
    brandLength = cards.size();
    brandSize.resize(brandLength); // brandSize是指牌的张数
    brandMap = Game::cardMap;      // 将Game中的哈希表(对应相应的键字符串和1-54赋值给Card中的brandMap
    for (const auto &cardStr : cards)
    {
        cardList.emplace_back(cardStr);
    }
    initializeBrandNum();
    sort();
}

// 使用哈希表查找牌型索引
// 不用数组索引直接找的原因：有可能输入的牌是错误的，即没有这种牌型
// 而且在数组中查找的时间复杂度为O(n)，而在哈希表中查找的时间复杂度为O(1)
int Brand::getBrandIndex(const std::string &brand)
{
    auto it = brandMap.find(brand);
    if (it != brandMap.end())
    {
        return it->second;
    }
    return -1;
}

void Brand::initializeBrandNum()
{
    brandNum.clear(); // 清空之前的内容
    for (int i = 0; i < brandLength; ++i)
    {
        brandNum[brandSize[i] / 4]++; // 代表某个张数的牌有多少张
    }
}

// 对brandSize向量进行排序，按照从小到大的方式排列
void Brand::sort()
{
    std::sort(brandSize.begin(), brandSize.end());
}

bool Brand::canPlay(Brand &enemyBrand)
{
    std::pair<int, int> thisCard = updateType();
    std::pair<int, int> cardCard = enemyBrand.updateType();
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
        std::cout << "牌太小，不能出牌" << std::endl;
        return false;
    }
    return true;
}

std::pair<int, int> Brand::updateType()
{
    std::pair<int, int> result;

    // 鬼牌(两张牌，且一张是小王，一张是大王)
    if (brandLength == 2 && brandMap.find("小王") != brandMap.end() && brandMap.find("大王") != brandMap.end())
    {
        result.first = 10; // 火箭
        result.second = 0; // 火箭没有关键值
        return result;
    }

    // 炸弹的情况(有四张相同的牌)
    if (judgeFixedType(4, 1, 4))
    {
        result.first = 9;               // 炸弹
        result.second = getCardValue(); // 获取牌的大小
        return result;
    }

    // 对子的情况(有两张相同的牌)
    if (judgeFixedType(2, 1, 2))
    {
        result.first = 8;               // 对子
        result.second = getCardValue(); // 获取牌的大小
        return result;
    }

    // 三张牌的情况(有三张相同的牌)
    if (judgeFixedType(3, 1, 3))
    {
        result.first = 7;               // 三张牌
        result.second = getCardValue(); // 获取牌的大小
        return result;
    }

    // 三带一的情况(三张相同的牌 + 一张随便的牌)
    if (judgeFixedType(4, 1, 3))
    {
        result.first = 6;               // 三带一
        result.second = getCardValue(); // 获取三张牌的大小
        return result;
    }

    // 三带二的情况(三张相同的牌 + 一副对子)
    if (judgeFixedType(5, 1, 3) && judgeFixedType(5, 0, 2))
    {
        result.first = 5;               // 三带二
        result.second = getCardValue(); // 获取三张牌的大小
        return result;
    }

    // 顺子的情况(至少5张连续的牌)
    if (isStraight())
    {
        result.first = 4;             // 顺子
        result.second = brandSize[0]; // 获取最小一张牌的大小
        return result;
    }

    // 连对的情况(三对或三对以上点数相连的牌)
    if (isConsecutivePairs())
    {
        result.first = 3;             // 连对
        result.second = brandSize[0]; // 获取最小一张牌的大小
        return result;
    }

    // 飞机的情况(两个或多个连续的三张牌)
    if (isPlane())
    {
        result.first = 2;             // 飞机
        result.second = brandSize[0]; // 获取最小一张牌的大小
        return result;
    }

    // 飞机带翅膀的情况(三个顺子，同数量的单牌或同数量的对牌)
    if (isPlaneWithWings() != -1)
    {
        result.first = 1;                   // 飞机带翅膀
        result.second = isPlaneWithWings(); // 获取最小一张牌的大小
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

bool Brand::isSeries(const std::vector<int> &a) const
{
    int seriesNum = a.size();
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

    bool isSeriesResult = isSeries(brandSize);
    bool isRepeatResult = isRepeat(repeatNum);

    return isSeriesResult && isRepeatResult;
}

bool Brand::isStraight() const
{
    if (brandLength < 5)
    {
        return false; // 顺子至少需要5张牌
    }
    return isSeries(brandSize);
}

bool Brand::isConsecutivePairs() const
{
    if (brandLength < 6 || brandLength % 2 != 0)
    {
        return false; // 连对至少需要6张牌，且牌的数量是偶数
    }
    std::vector<int> temp(brandLength / 2);
    for (int i = 0; i < brandLength - 1; i += 2)
    {
        temp[i] = brandSize[i];
        if (brandNum.at(brandSize[i] / 4) != 2)
        {
            return false;
        }
    }

    return isSeries(temp);
}
bool Brand::isPlane() const
{
    if (brandLength < 6 || brandLength % 3 != 0)
    {
        return false; // 连对至少需要6张牌，且牌的数量是偶数
    }
    std::vector<int> temp(brandLength / 3);
    for (int i = 0; i < brandLength - 1; i += 3)
    {
        temp[i] = brandSize[i];
        if (brandNum.at(brandSize[i] / 4) != 3)
        {
            return false;
        }
    }

    return isSeries(temp);
}
int Brand::isPlaneWithWings() const
{
    if (brandLength != 8)
        return false;
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

    return choose[0];
}
