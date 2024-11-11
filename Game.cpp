#include "Game.h"
#include "Player.h"
#include "Card.h"
#include <algorithm>
#include <random>
#include <ctime>
#include <iostream>
#include <sstream>

using namespace std;

std::unordered_map<std::string, int> Game::cardMap;

const std::vector<std::string> NUMBER = {"3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A", "2", "joker"};
const std::vector<std::string> COLOUR = {"♥", "♠", "♣", "♦"};

Game::Game() : DEFAULT_BRAND(54), brands(54), player(), computer()
{

    initializeCardMap();                           // 初始化键值对
    initializeDefaultBrand(DEFAULT_BRAND, brands); // 初始化手牌

    initialPage(); // 欢迎界面
    dealCards();   // 发牌
}
void Game::initialPage()
{
    std::cout << "欢迎来到斗地主游戏!" << std::endl;
    std::cout << "游戏规则: " << std::endl;
    std::cout << "1. 一副牌有54张，包括13种点数，4种花色，大小王" << std::endl;
    std::cout << "2. 一局游戏有3个玩家，其中一个是地主，另外两个是农民" << std::endl;
    std::cout << "3. 一开始，地主和农民分别获得17张牌，剩下的20张牌作为底牌" << std::endl;
    std::cout << "4. 地主首先出牌，然后其他玩家依次出牌，直到有一方出完手牌" << std::endl;
    std::cout << "5. 出牌时，玩家可以选择跳过，或者出一张或多张牌" << std::endl;
    std::cout << "6. 出牌时，玩家必须出比上家大的牌，或者出炸弹" << std::endl;
    std::cout << "7. 出牌时，玩家可以选择不出牌，此时会跳过该玩家" << std::endl;
    std::cout << "8. 出完手牌的玩家获得胜利" << std::endl;
    std::cout << "祝你好运!" << std::endl;
}
void Game::initializeCardMap()
{
    int index = 0;
    for (const auto &number : NUMBER)
    {
        for (const auto &colour : COLOUR)
        {
            cardMap[colour + number] = index++;
        }
    }
    cardMap["小王"] = 52;
    cardMap["大王"] = 53;
}
void Game::initializeDefaultBrand(std::vector<std::string> &defaultBrand, std::vector<std::string> &brands)
{
    defaultBrand[52] = brands[52] = "小王";
    defaultBrand[53] = brands[53] = "大王";
    for (int i = 0; i < 52; ++i)
    {
        defaultBrand[i] = COLOUR[i % 4] + NUMBER[i / 4];
        brands[i] = COLOUR[i % 4] + NUMBER[i / 4];
    }
}

void Game::shuffleDeck()
{
    // 洗牌逻辑
    std::srand(std::time(0));
    std::random_shuffle(Game::brands.begin(), Game::brands.end());
}
void Game::setRemainingBrands(const std::vector<std::string> &brands)
{
    remainingBrands = brands;
}

void Game::dealCards()
{
    // 发牌逻辑
    // 随机选择一个大于40且小于54的偶数作为N
    std::srand(std::time(0));
    int N = 42 + (std::rand() % 7) * 2; // 生成42, 44, 46, 48, 50, 52中的一个
    // 洗牌
    shuffleDeck();
    // 初始化玩家和地主的手牌容器
    // 询问玩家是否原因成为地主
    std::cout << "是否成为地主？(y/n)" << std::endl;
    char choice;
    std::cin >> choice;
    if (choice == 'y')
    {
        // 玩家成为地主
        player.setName("Landlord");
        computer.setName("Slave");
        // 分配牌，玩家比电脑多两张
        for (int i = 0; i < N; ++i)
        {
            if (i < (N - 2) / 2) // 前(N-2)/2张牌给地主
            {
                computer.addCard(brands[i]);
            }
            else // 剩下的(N+2)/2张牌给玩家
            {
                player.addCard(brands[i]);
            }
        }
        // 存储剩余的牌
        for (int i = N; i < 54; ++i)
        {
            remainingBrands.push_back(brands[i]);
        }
        player.sort();
        computer.sort();
        Brand brand(remainingBrands);
        setRemainingBrands(brand.getCardList());
    }
    else
    {
        // 电脑成为地主
        player.setName("Slave");
        computer.setName("Landlord");

        // 分配牌，地主比玩家多两张
        for (int i = 0; i < N; ++i)
        {
            if (i < (N - 2) / 2) // 前(N-2)/2张牌给玩家
            {
                player.addCard(brands[i]);
            }
            else // 剩下的(N+2)/2张牌给地主
            {
                computer.addCard(brands[i]);
            }
        }
        // 存储剩余的牌
        for (int i = N; i < 54; ++i)
        {
            remainingBrands.push_back(brands[i]);
        }
        player.sort();
        computer.sort();
        Brand brand(remainingBrands);
        setRemainingBrands(brand.getCardList());
    }
    // 打印玩家手牌
    player.printHand();
}

void Game::playGame()
{
    // 游戏逻辑
    // 玩家和电脑轮流出牌，直到有一方出完手牌
    // 每次出牌前，询问玩家是否要出牌
    // 出牌后，判断是否符合规则，不符合规则则重新出牌
    // 判断是否有一方出完手牌，如果有则游戏结束
    // 输出游戏结果
    // 询问是否再来一局
    // 如果是，则重新开始游戏
    // 如果不是，则退出游戏
    srand(static_cast<unsigned int>(time(0)));

    bool gameRunning = true;

    bool playerIsLandLord = (player.getName() == "Landlord");

    while (!isGameOver())
    {
        // 检查是否需要进入作弊菜单
        char cheatInput;
        std::cout << "是否进入作弊菜单？(Y/N): ";
        std::cin >> cheatInput;
        if (cheatInput == 'Y' || cheatInput == 'y')
        {
            cheatMenu();
        }
        if (playerIsLandLord)
        {
            lastPlayerCards = playerTurn();
            std::cout << "玩家的回合结束" << std::endl;
            if (lastPlayerCards.empty())
            {
                currentBrand.clear();
            }

            if (isGameOver())
                break;

            // 判断是否是鬼牌或炸弹
            if (isBombOrJoker(currentBrand))
            {
                std::cout << "鬼牌/炸弹大了" << std::endl;
                currentBrand.clear();
                recordPlay(lastPlayerCards, {"pass"});
                continue; // 继续由玩家出牌
            }
            std::cout << "电脑的回合" << std::endl;
            lastOpponentCards = computerTurn();
            if (isGameOver())
                break;
            if (lastOpponentCards.empty())
                currentBrand.clear();
            recordPlay(lastPlayerCards, lastOpponentCards);
        }
        else
        {
            lastOpponentCards = computerTurn();
            if (lastOpponentCards.empty())
                currentBrand.clear();
            if (isGameOver())
                break;

            // 判断是否是鬼牌或炸弹
            if (isBombOrJoker(currentBrand))
            {
                std::cout << "鬼牌/炸弹大了" << std::endl;
                currentBrand.clear();
                recordPlay(lastOpponentCards, {"pass"});
                continue; // 继续由电脑出牌
            }

            lastPlayerCards = playerTurn();
            if (isGameOver())
                break;
            if (lastPlayerCards.empty())
                currentBrand.clear();
            recordPlay(lastOpponentCards, lastPlayerCards);
        }
    }
    printResult();
    std::cout << "本局游戏结束!" << std::endl;
}

bool Game::isBombOrJoker(const std::vector<std::string> &cards) const
{
    if (cards.size() == 2 && ((cards[0] == "Joker" && cards[1] == "joker") || (cards[0] == "joker" && cards[1] == "Joker")))
    {
        return true; // 鬼牌
    }

    if (cards.size() == 4 && cards[0] == cards[1] && cards[1] == cards[2] && cards[2] == cards[3])
    {
        return true; // 炸弹
    }

    return false;
}

bool Game::isValidHand(const std::vector<std::string> &cards)
{
    Brand tryBrand(cards);
    Brand TableBrand(currentBrand);
    if (currentBrand.empty())
    {
        return true;
    }

    return tryBrand.canPlay(TableBrand);
}

// 定义一个映射表，将中文花色转换为符号
std::unordered_map<std::string, std::string> cardMap = {
    {"R", "♥"},
    {"BO", "♦"},
    {"BA", "♠"},
    {"F", "♣"},
    {"joker", "小王"},
    {"JOKER", "大王"}};

std::string changCards(const std::string &input)
{
    // 遍历映射表，查找并替换花色
    for (const auto &pair : cardMap)
    {
        size_t pos = input.find(pair.first);
        if (pos != std::string::npos)
        {
            std::string result = input;
            result.replace(pos, pair.first.length(), pair.second);
            return result;
        }
    }
    return input; // 如果输入不在映射表中，返回原输入
}

std::vector<std::string> convertCards(std::vector<std::string> &cards)
{
    for (auto &card : cards)
    {
        card = changCards(card);
    }
    return cards;
}
bool Game::isValidPlay(const std::vector<std::string> &cards)
{
    std::vector<std::string> tempHand = player.getHand();
    for (const auto &card : cards)
    {
        auto it = std::find(tempHand.begin(), tempHand.end(), card);
        if (it == tempHand.end())
        {
            std::cout << "牌 " << card << " 不在手牌中或数量不足，请重新输入。" << std::endl;
            return false;
        }
        else
        {
            tempHand.erase(it); // 从临时手牌中移除已找到的牌
        }
    }
    // 检查整个牌型是否符合规则
    if (!isValidHand(cards))
    {
        std::cout << "手牌不符合规则，请重新出牌。" << std::endl;
        return false;
    }
    // 从玩家手牌中删除已出的牌
    player.removeCard(cards);
    // 更新桌子上的牌的类型
    currentBrand = cards;
    return true;
}

void Game::recordPlay(const std::vector<std::string> &playerCards, const std::vector<std::string> &opponentCards)
{
    if (playerCards.empty())
    {
        cardRecorder.recordRound({"pass"}, opponentCards);
    }
    else if (opponentCards.empty())
    {
        cardRecorder.recordRound(playerCards, {"pass"});
    }
    else
        cardRecorder.recordRound(playerCards, opponentCards);
}
std::vector<std::string> Game::playerTurn()
{
    while (true)
    {
        // 输出桌子上的牌
        std::cout << "桌子上的牌: ";
        for (const auto &card : currentBrand)
        {
            std::cout << card << " ";
        }
        std::cout << std::endl;

        // 输出玩家现有的牌
        player.printHand();
        std::vector<std::string> cards;
        std::cout << "请输入你要出的牌(用R,BA,F,BO,joker,JOKER表示花色,输入-1表示放弃): ";

        std::string input;
        while (std::cin >> input)
        {
            if (input == "-1")
            {
                std::cout << "玩家放弃，由电脑继续出牌。" << std::endl;
                return {};
            }
            cards.push_back(input);
            // 检查是否按下回车键结束输入
            if (std::cin.peek() == '\n')
            {
                break;
            }
        }
        // 转化牌的花色
        cards = convertCards(cards);
        // 判断是否可以出牌
        if (isValidPlay(cards))
        {
            std::cout << "成功出牌" << std::endl;
            return cards;
            break;
        }
        else
        {
            std::cout << "出牌无效，请重新输入。" << std::endl;
            // 清空输入缓冲区
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

std::vector<std::string> Game::computerTurn()
{
    std::vector<std::string> computerHand = computer.getHand();
    Brand brand(computerHand);

    std::vector<std::string> chosenCardsStr = brand.chooseCards(currentBrand);
    if (chosenCardsStr.empty())
    {
        std::cout << "电脑跳过了这一轮" << std::endl;
        return {};
    }
    else
    {
        // 将这些牌从电脑手牌中删除
        for (const auto &card : chosenCardsStr)
        {
            computerHand.erase(std::remove(computerHand.begin(), computerHand.end(), card), computerHand.end());
        }
        // 更新电脑手牌
        computer.setHand(computerHand);
        // 更新桌子上的牌
        currentBrand = chosenCardsStr;
        return chosenCardsStr;
    }
}

bool Game::isGameOver()
{
    std::vector<std::string> playerHand = player.getHand();
    std::vector<std::string> computerHand = computer.getHand();
    return playerHand.empty() || computerHand.empty();
}

void Game::printResult()
{
    std::vector<std::string> playerHand = player.getHand();
    if (playerHand.empty())
    {
        std::cout << "你赢了!" << std::endl;
    }
    else
    {
        std::cout << "电脑赢了!" << std::endl;
    }
}

std::vector<std::string> Game::getCurrentBrand() const
{
    return currentBrand;
}

void Game::cheatMenu()
{
    std::cout << "作弊菜单" << std::endl;
    std::cout << "1. 输入T/t显示电脑手牌" << std::endl;
    std::cout << "2. 输入Y/y显示剩余的牌(没被用的牌)" << std::endl;
    std::cout << "3. 输入U/u显示记牌器的结果" << std::endl;
    std::cout << "4. 输入N/n显示已经出了的牌的张数" << std::endl;
    std::cout << "请选择: ";
    char choice;
    std::cin >> choice;
    switch (choice)
    {
    case 'T':
    case 't':
        computer.printHand();
        break;
    case 'Y':
    case 'y':
        for (const auto &card : remainingBrands)
        {
            std::cout << card << " ";
        }
        std::cout << std::endl;
        break;
    case 'U':
    case 'u':
        std::cout << "记牌器结果：" << std::endl;
        if (player.getName() == "Landlord")
        {
            for (const auto &round : cardRecorder.getRounds())
            {
                std::cout << "玩家出牌：";
                for (const auto &card : round.first)
                {
                    std::cout << card << " ";
                }
                std::cout << " | 对手出牌：";
                for (const auto &card : round.second)
                {
                    std::cout << card << " ";
                }
                std::cout << std::endl;
            }
        }
        else
        {
            for (const auto &round : cardRecorder.getRounds())
            {
                std::cout << "对手出牌：";
                for (const auto &card : round.first)
                {
                    std::cout << card << " ";
                }
                std::cout << " | 玩家出牌：";
                for (const auto &card : round.second)
                {
                    std::cout << card << " ";
                }
                std::cout << std::endl;
            }
        }
        break;
    case 'N':
    case 'n':
        std::cout << "已经出了的牌的张数：" << std::endl;
        cardRecorder.getCardCountForRound();
        cardRecorder.printCardCount();
        break;
    default:
        std::cout << "无效选择" << std::endl;
        break;
    }
}