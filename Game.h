#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>
#include "Player.h"
#include <unordered_map>

class Game
{
public:
    static std::unordered_map<std::string, int> cardMap; // 哈希表，用于存储牌的字符串表示和大小的映射
    Game();
    void playGame();
    void playerTurn(std::vector<std::string> &playerHand);
    void computerTurn(std::vector<std::string> &computerHand);
    bool isGameOver(const std::vector<std::string> &playerHand, const std::vector<std::string> &computerHand);
    void printResult(const std::vector<std::string> &playerHand, const std::vector<std::string> &computerHand);
    bool playAgain();

private:
    // 当前牌桌子上的牌
    std::vector<std::string> currentBrand;
    static void initializeCardMap();
    static void initializeDefaultBrand(std::vector<std::string> &defaultBrand, std::vector<std::string> &brands);
    Player player;
    Player computer;
    void shuffleDeck();
    void dealCards();
    bool isValidMove(const std::string &str);
    bool isValidHand(const std::vector<std::string> &cards);

    std::vector<std::string> DEFAULT_BRAND;
    std::vector<std::string> brands;

    // 存储剩余的牌
    std::vector<std::string> remainingBrands;
};

#endif // GAME_H