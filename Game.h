#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>
#include "Player.h"
#include <unordered_map>
#include "CardRecorder.h"

class Game
{
public:
    static std::unordered_map<std::string, int> cardMap; // 哈希表，用于存储牌的字符串表示和大小的映射
    Game();
    std::vector<std::string> getCurrentBrand() const;

    void initialPage();
    void playGame();
    std::vector<std::string> playerTurn();
    std::vector<std::string> computerTurn();
    void recordPlay(const std::vector<std::string> &playerCards, const std::vector<std::string> &opponentCards);
    bool isGameOver();
    void printResult();

    void cheatMenu(); // 作弊菜单

    void setRemainingBrands(const std::vector<std::string> &brands);

private:
    // 当前牌桌子上的牌
    std::vector<std::string> currentBrand;
    static void initializeCardMap();
    static void initializeDefaultBrand(std::vector<std::string> &defaultBrand, std::vector<std::string> &brands);
    Player player;
    Player computer;
    void shuffleDeck();
    void dealCards();
    bool isBombOrJoker(const std::vector<std::string> &cards) const;
    bool isValidPlay(const std::vector<std::string> &cards);
    bool isValidHand(const std::vector<std::string> &cards);

    // 记牌器，记录每一局出的牌
    CardRecorder cardRecorder;
    std::vector<std::string> lastPlayerCards;
    std::vector<std::string> lastOpponentCards;

    std::vector<std::string> DEFAULT_BRAND;
    std::vector<std::string> brands;

    // 存储剩余的牌
    std::vector<std::string> remainingBrands;
};

#endif // GAME_H