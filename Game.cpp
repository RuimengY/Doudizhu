#include "Game.h"
#include <algorithm>
#include <random>
#include <ctime>
#include <iostream>

Game::Game()
{
    initializeDeck();
    shuffleDeck();
    dealCards();
}

void Game::initializeDeck()
{
    // 初始化牌堆
}

void Game::shuffleDeck()
{
    std::shuffle(deck.begin(), deck.end(), std::default_random_engine(std::time(0)));
}

void Game::dealCards()
{
    // 发牌逻辑
}

void Game::startGame()
{
    // 游戏开始逻辑
}