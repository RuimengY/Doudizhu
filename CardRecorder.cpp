#include "CardRecorder.h"
const std::vector<std::string> NUMBER = {"3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A", "2", "joker"};
const std::vector<std::string> COLOUR = {"♥", "♠", "♣", "♦"};
void CardRecorder::recordRound(const std::vector<std::string> &player1Cards, const std::vector<std::string> &player2Cards)
{
    rounds.push_back(std::make_pair(player1Cards, player2Cards));
}

const std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>> &CardRecorder::getRounds() const
{
    return rounds;
}

std::unordered_map<std::string, int> CardRecorder::getCardCountForRound()
{
    initializeCardCount();
    for (size_t i = 0; i < rounds.size(); ++i)
    {
        updateCardCount(rounds[i].first);
        updateCardCount(rounds[i].second);
    }
    return cardCounts;
}

void CardRecorder::updateCardCount(const std::vector<std::string> &cards)
{
    for (const auto &card : cards)
    {
        cardCounts[card]++;
    }
}

void CardRecorder::initializeCardCount()
{
    cardCounts.clear();
    int index = 0;
    for (const auto &number : NUMBER)
    {
        for (const auto &colour : COLOUR)
        {
            cardCounts[colour + number] = 0;
        }
    }
    cardCounts["小王"] = 0;
    cardCounts["大王"] = 0;
}

void CardRecorder::printCardCount()
{
    bool first = true;
    for (const auto &number : NUMBER)
    {
        for (const auto &colour : COLOUR)
        {
            if (!first)
            {
                std::cout << ", ";
            }
            std::cout << colour + number << ": " << cardCounts[colour + number];
            first = false;
        }
    }
    std::cout << ", 小王: " << cardCounts["小王"] << ", 大王: " << cardCounts["大王"] << std::endl;
}