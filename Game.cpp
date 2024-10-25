#include "Game.h"
#include "Card.h"
#include <algorithm>
#include <random>
#include <ctime>
#include <iostream>

Game::Game() : DEFAULT_BRAND(54), brands(54)
{
    Card::initializeDefaultBrand(DEFAULT_BRAND, brands);
    shuffleDeck();
    dealCards();
}

void Game::shuffleDeck()
{
    // 洗牌逻辑
}

void Game::dealCards()
{
    // 发牌逻辑
}