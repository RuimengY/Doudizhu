#include "Game.h"
#include "Player.h"
#include "Card.h"
#include <algorithm>
#include <random>
#include <ctime>
#include <iostream>
std::unordered_map<std::string, int> Game::cardMap;

const std::vector<std::string> NUMBER = {"3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A", "2", "joker"};
const std::vector<std::string> COLOUR = {"♥", "♠", "♣", "♦"};

Game::Game() : DEFAULT_BRAND(54), brands(54), player(), computer()
{
    if (cardMap.empty())
    {
        initializeCardMap();
    }
    shuffleDeck();
    dealCards();
    playGame();
}
void Game::initializeCardMap()
{
    int index = 1;
    for (const auto &number : NUMBER)
    {
        for (const auto &colour : COLOUR)
        {
            cardMap[colour + number] = index++;
        }
    }
    cardMap["小王"] = 53;
    cardMap["大王"] = 54;
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
            if (i < (N - 2) / 2) // 前(N-2)/2张牌给玩家
            {
                computer.addCard(brands[i]);
            }
            else // 剩下的(N+2)/2张牌给地主
            {
                player.addCard(brands[i]);
            }
        }
        // 存储剩余的牌
        for (int i = N; i < 54; ++i)
        {
            remainingBrands.push_back(brands[i]);
        }
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
    }
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
    std::vector<std::string> playerHand, computerHand;

    bool gameRunning = true;
    while (gameRunning)
    {
        playerHand = player.getHand();
        computerHand = computer.getHand();

        bool playerIsLandLord = (player.getName() == "LandLord");

        while (!isGameOver(playerHand, computerHand))
        {
            if (playerIsLandLord)
            {
                playerTurn(playerHand);
                if (isGameOver(playerHand, computerHand))
                    break;
                computerTurn(computerHand);
            }
            else
            {
                computerTurn(computerHand);
                if (isGameOver(playerHand, computerHand))
                    break;
                playerTurn(playerHand);
            }
        }

        printResult(playerHand, computerHand);
        gameRunning = playAgain();
    }

    std::cout << "游戏结束!" << std::endl;
}

bool Game::isValidMove(const std::string &str)
{
    std::vector<std::string> playerHand = player.getHand();
    // 判断出的牌是否在手牌中
    auto it = std::find(playerHand.begin(), playerHand.end(), str);
    if (it == playerHand.end())
    {
        return false;
    }

    // 判断出的牌是否在 cardMap 中
    if (cardMap.find(str) == cardMap.end())
    {
        return false;
    }

    return true;
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
void Game::playerTurn(std::vector<std::string> &playerHand)
{
    std::vector<std::string> cards;
    std::string card;
    std::cout << "请输入你要出的牌（以空格分隔，输入结束后按回车）: ";
    while (std::cin >> card)
    {
        cards.push_back(card);
        if (std::cin.peek() == '\n')
        {
            break;
        }
    }
    // 判断每个元素是否满足条件
    for (const auto &card : cards)
    {
        if (!isValidMove(card))
        {
            std::cout << "存在单张牌不在手牌内，请重新出牌: ";
            return;
        }
    }

    // 判断整个字符串数组是否满足条件
    if (!isValidHand(cards))
    {
        std::cout << "手牌不符合规则，请重新出牌: ";
        return;
    }
    // 更新拍桌子上的牌
    currentBrand = cards;
    // 将出的每一张牌从手牌中删除
    for (const auto &card : cards)
    {
        playerHand.erase(std::remove(playerHand.begin(), playerHand.end(), card), playerHand.end());
    }
    std::cout << "你出了牌: ";
    for (const auto &card : cards)
    {
        std::cout << card << " ";
    }
    std::cout << std::endl;
    // 更新手牌
    player.setHand(playerHand);
}

void Game::computerTurn(std::vector<std::string> &computerHand)
{
    /*
    // 简单的电脑出牌逻辑，随机出一张牌
    int card = computerHand[rand() % computerHand.size()];
    std::cout << "电脑出了牌: " << card << std::endl;
    computerHand.erase(std::remove(computerHand.begin(), computerHand.end(), card), computerHand.end());
    */
}

bool Game::isGameOver(const std::vector<std::string> &playerHand, const std::vector<std::string> &computerHand)
{
    return playerHand.empty() || computerHand.empty();
}

void Game::printResult(const std::vector<std::string> &playerHand, const std::vector<std::string> &computerHand)
{
    if (playerHand.empty())
    {
        std::cout << "你赢了!" << std::endl;
    }
    else
    {
        std::cout << "电脑赢了!" << std::endl;
    }
}

bool Game::playAgain()
{
    char choice;
    std::cout << "是否再来一局? (y/n): ";
    std::cin >> choice;
    return choice == 'y' || choice == 'Y';
}

int main()
{
    Game game;
    return 0;
}