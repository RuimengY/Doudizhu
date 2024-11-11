#ifndef CARDRECORDER_H
#define CARDRECORDER_H

#include <vector>
#include <string>
#include <utility>
#include <unordered_map>
#include <iostream>

class CardRecorder
{
public:
    // 记录每一局的出牌
    void recordRound(const std::vector<std::string> &player1Cards, const std::vector<std::string> &player2Cards);
    std::unordered_map<std::string, int> getCardCountForRound();
    void updateCardCount(const std::vector<std::string> &cards);
    void printCardCount();

    void initializeCardCount();

    // 获取所有记录的出牌
    const std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>> &getRounds() const;

private:
    std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>> rounds;
    std::unordered_map<std::string, int> cardCounts;
};

#endif // CARDRECORDER_H