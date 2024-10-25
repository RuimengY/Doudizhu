#include "card.h"
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <map>

// 默认的牌型一共有54张，其中前52张是普通牌，最后两张是大小王
const std::vector<std::string> Card::NUMBER = {"3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A", "2", "joker"};
const std::vector<std::string> Card::COLOUR = {"♥", "♠", "♣", "♦"};
std::vector<std::string> Card::DEFAULT_BRAND(54);

// 初始化默认的牌型
static void initializeDefaultBrand()
{
    Card::DEFAULT_BRAND[52] = "小王";
    Card::DEFAULT_BRAND[53] = "大王";
    for (int i = 0; i < 52; ++i)
    {
        Card::DEFAULT_BRAND[i] = Card::COLOUR[i % 4] + Card::NUMBER[i / 4];
    }

    // 填充哈希表
    for (int i = 0; i < 54; ++i)
    {
        Card::brandMap[Card::DEFAULT_BRAND[i]] = i;
    }
}

// 在接收到一副牌之后将牌对应成相应的数组序列
// 其中输入每张牌应该是例如"♠4","♣5"的形式
Card::Card(const std::vector<std::string> &brands)
{
    // 下面的函数为什么不能在Game.cpp当中执行?
    if (Card::DEFAULT_BRAND[0].empty())
    {
        initializeDefaultBrand();
    }

    brandLength = brands.size();
    brandSize.resize(brandLength); // brandSize是指牌的张数

    for (int i = 0; i < brandLength; ++i)
    {
        // 如果输入的牌不在哈希表中，说明输入的牌型错误
        while (true)
        {
            if (getBrandIndex(brands[i]) != -1)
            {
                // 将输入的每一张牌对应到相应的数字(1-54)
                brandSize[i] = getBrandIndex(brands[i]);
                break;
            }
            std::cout << "牌型输入错误" << std::endl;
            return;
        }
    }
    initializeBrandNum();
    sort();
}

// 使用哈希表查找牌型索引
// 不用数组索引直接找的原因：有可能输入的牌是错误的，即没有这种牌型
// 而且在数组中查找的时间复杂度为O(n)，而在哈希表中查找的时间复杂度为O(1)
int Card::getBrandIndex(const std::string &brand)
{
    auto it = brandMap.find(brand);
    if (it != brandMap.end())
    {
        return it->second;
    }
    return -1;
}

void Card::initializeBrandNum()
{
    brandNum.clear(); // 清空之前的内容
    for (int i = 0; i < brandLength; ++i)
    {
        brandNum[brandSize[i] / 4]++; // 代表某个张数的牌有多少张
    }
}

// 对brandSize向量进行排序，按照从小到大的方式排列
void Card::sort()
{
    std::sort(brandSize.begin(), brandSize.end());
}

// 这里的compare是指已经知道对方牌的情况，则如果this牌大，则返回true，否则返回false，表示不能出牌
bool Card::compareSize(Card &card)
{
    std::pair<int, int> thisCard = updateType();
    std::pair<int, int> cardCard = card.updateType();
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
    else
    {
        std::cout << "玩家可以出牌" << std::endl;
        return true;
    }
}
/*
结果是一个键值对，用来说明类型和对应的关键代码
*/
// 假设 Card 类和其他相关代码已经定义

std::pair<int, int> Card::updateType()
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
int Card::getCardValue() const
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

bool Card::isSeries(const std::vector<int> &a) const
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
bool Card::isRepeat(int repeatNum) const
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
bool Card::judgeFixedType(int len, int seriesNum, int repeatNum) const
{
    if (brandLength != len)
    {
        return false;
    }

    bool isSeriesResult = isSeries(brandSize);
    bool isRepeatResult = isRepeat(repeatNum);

    return isSeriesResult && isRepeatResult;
}

bool Card::isStraight() const
{
    if (brandLength < 5)
    {
        return false; // 顺子至少需要5张牌
    }
    return isSeries(brandSize);
}

bool Card::isConsecutivePairs() const
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
bool Card::isPlane() const
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
int Card::isPlaneWithWings() const
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

std::string Card::getBrandByIndex(int index)
{
    if (index == 52)
    {
        return "小王";
    }
    if (index == 53)
    {
        return "大王";
    }
    return COLOUR[index % 4] + NUMBER[index / 4];
}

int Card::getBrandSize(const std::string &brand)
{
    int index = getBrandIndex(brand);
    if (index > 51)
    {
        return index;
    }
    return index / 4;
}

bool Card::extent(const Card &card) const
{
    if (this->type == -1)
    {
        return false;
    }
    if (card.type == -1 || this->type == 2)
    {
        return true;
    }
    if (this->type == card.type && this->brandLength == card.brandLength)
    {
        return this->brandSize[0] > card.brandSize[0];
    }
    if (this->type == 7)
    {
        return true;
    }
    return false;
}

std::string Card::printBrand() const
{
    std::string result;
    for (int i = 0; i < brandLength; ++i)
    {
        result += getBrandByIndex(brandSize[i]);
    }
    return result;
}

bool Card::isOff() const
{
    return this->type == -1;
}