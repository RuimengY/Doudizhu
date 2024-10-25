#include "Game.h"
#include <algorithm>
#include <random>
#include <ctime>
#include <iostream>

Game::Game()
{
    initializeDEFAULT_BRAND();
    shuffleDeck();
    dealCards();
}

void Game::initializeDEFAULT_BRAND()
{
    // 初始化牌堆
    DEFAULT_BRAND[52] = brands[52] = "小王";
    DEFAULT_BRAND[53] = brands[53] = "大王";
    for (int i = 0; i < 52; ++i)
    {
        DEFAULT_BRAND[i] = COLOUR[i % 4] + NUMBER[i / 4];
        brands[i] = COLOUR[i % 4] + NUMBER[i / 4];
    }

    // 填充哈希表
    for (int i = 0; i < 54; ++i)
    {
        brandMap[Game::DEFAULT_BRAND[i]] = i;
    }
}

// 将brands数组中的牌打乱
void Game::shuffleDeck()
{
    // 洗牌逻辑
    std::srand(std::time(0));
    std::random_shuffle(Game::brands.begin(), Game::brands.end());
}

void Game::dealCards()
{
    // 随机选择一个大于40且小于54的偶数作为N
    std::srand(std::time(0));
    int N = 42 + (std::rand() % 7) * 2; // 生成42, 44, 46, 48, 50, 52中的一个

    // 洗牌
    shuffleDeck();

    // 初始化玩家和地主的手牌容器
    std::vector<int> player, landlord;

    // 分配牌，地主比玩家多两张
    for (int i = 0; i < N; ++i)
    {
        if (i < (N - 2) / 2) // 前(N-2)/2张牌给玩家
        {
            player.push_back(Game::brands[i]);
        }
        else // 剩下的(N+2)/2张牌给地主
        {
            landlord.push_back(Game::brands[i]);
        }
    }
}

void Game::startGame()
{
    // 游戏开始逻辑
}