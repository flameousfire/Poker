#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "pcards.hpp"

std::vector<int> deck = {};

std::string suit(int card)
{
if (card / 13 == 0 or card == 13)
    return "C";
else if (card / 13 == 1 or card == 26)
    return "D";
else if (card / 13 == 2 or card == 39)
    return "H";
else if (card / 13 == 3 or card == 52)
    return "S";
}

int value(int card)
{
    if (card % 13 == 1)
        return 14;
    else if (card % 13 == 0)
        return 13;
    else 
        return card % 13;
}

bool checkflush(std::vector<int>& hand)
{   
    int a = 0;
    for (int i = 1; i < hand.size(); i++)
    {   
        if (suit(hand[0]) == suit(hand[i]))
            ++a;
    }
    if (a == 4)
        return true;
    else
        return false;
}

int checkmultiples(std::vector<int>& hand, int hs)
{
    int a = 0;
    int tmp = 0;
    for (int j = 0; j < hs; ++j)
    {
        for (int i = 0; i < hs; ++i)
        {
            if (value(hand[j]) == value(hand[i]))
                ++tmp;
        }
        if (tmp > a)
            a = tmp;
        tmp = 0;
    }
    return a;
}

bool checkfullhouse(std::vector<int> hand)
{
    if (checkmultiples(hand) != 3)
        return false;
    else
    {
        for (int j = hand.size(); j >= 0; --j)
        {
            if (value(hand[0]) == value(hand[j]))
                hand.erase(hand.begin()+j);
        }
        if (hand.size() == 4)
            return false;
        if (checkmultiples(hand, hand.size()) == 1)
            return false;
        else 
            return true;
    }
}    

bool checktwopairs(std::vector<int> hand)
{
    if (checkmultiples(hand) != 2)
        return false;
    else
    {
        for (int i = 0; i < hand.size(); ++i)
        {
            for (int j = 0; j < i; ++j)
            {
                if (value(hand[j]) == value(hand[i]))
                {
                hand.erase(hand.begin()+i);
                hand.erase(hand.begin()+j);
                if (checkmultiples(hand, 3) == 1)
                    return false;
                else if (checkmultiples(hand, 3) == 2)
                    return true;
                }             
            }
        }
    }
}

bool checkstraight(std::vector<int> hand)
{
    if (checkmultiples(hand) > 1)
        return false;
    else
    {
        int high = 0;
        int low = 13;
        for (int i = 0; i < hand.size(); ++i)
        {
            if (value(hand[i]) < low)
            low = value(hand[i]);
            
            if (value(hand[i]) > high)
            high = value(hand[i]);
        }
        if (high - low == 4)
            return true;
        else if (high - low == 12) 
        {
            for (int j = 0; j < hand.size(); ++j)
            {
                if (value(hand[j]) != 14)
                    continue;
                else if (value(hand[j]) == 14)
                {
                    high = 1;
                    low = 13;
                    hand.erase(hand.begin()+j);

                    for (int k = 0; k < 4; ++k)
                    {
                        if (value(hand[k]) < low)
                        low = value(hand[k]);
                    }
                }
                   
            }
            if (high - low == 4)
            return true;
            else
            {
                return false;
            }
            
        }
        
        {
            return false;
        }
        
    }
    
}

int combination(std::vector<int>& hand)
{
    int a = checkmultiples(hand);
    if (checkflush(hand) == true && checkstraight(hand) == true)
        return 1;
    else if (a == 4)
        return 2;
    else if (checkfullhouse(hand) == true)
        return 3;
    else if (checkflush(hand) == true)
        return 4;
    else if (checkstraight(hand) == true)
        return 5;
    else if (a == 3)
        return 6;
    else if (checktwopairs(hand) == true)
        return 7;
    else if (a == 2)
        return 8;
    else
        return 9;    
}

void dealcards(std::vector<int>& hand, int hs)
{
    srand(time(0));
    for (int i = 0; i < hs; i++)
    {
        int size = deck.size();
        int b = rand() % size;
        int tmp = deck[b];
        deck.erase(deck.begin()+b);
        hand.push_back(tmp);
    }
}

void printhand(std::vector<int>& hand)
{
    for (int i = 0; i < hand.size(); i++)
    {
        if (hand[i] % 13 == 12)
            std::cout << suit(hand[i]) << "Q" << " ";
        
        else if (hand[i] % 13 == 11)
            std::cout << suit(hand[i]) << "J" << " ";

        else if (hand[i] % 13 == 0)
            std::cout << suit(hand[i]) << "K" << " ";
        
        else if (hand[i] % 13 == 1)
            std::cout << suit(hand[i]) << "A" << " ";

        else
            std::cout << suit(hand[i]) << hand[i] % 13 << " ";        
    }
    std::cout << "\n";
}

std::string combprint(int c)
{
    if (c == 1)
    return "a straight flush!";
    else if (c == 2)
    return "four of a kind!";
    else if (c == 3)
    return "a fullhouse!";
    else if (c == 4)
    return "a flush!";
    else if (c == 5)
    return "a straight!";
    else if (c == 6)
    return "three of a kind!";
    else if (c == 7)
    return "two pairs!";
    else if (c == 8)
    return "a pair!";
    else if (c == 9)
    return "high card";
}
bool greaterthan(int i, int j)
{
    return value(i) < value(j);
}

void sorthand(std::vector<int>& hand)
{
    sort(hand.begin(), hand.end(), greaterthan);
}

int comparehigh(std::vector<int>& hand1, std::vector<int>& hand2)
{   
    sorthand(hand1);
    sorthand(hand2);
    if (combination(hand1) == 1 || combination(hand1) == 4 || combination(hand1) == 5 || combination(hand1) == 9)
    {
        if (value(hand1[5]) < value(hand2[5]))
            return 2;
        else if (value(hand1[5]) > value(hand2[5]))
            return 1;
        else if (value(hand1[5]) == value(hand2[5]))
            return 0;
    }
    else if (combination(hand1) == 2)
    {
        int high1 = 0;
        int high2 = 0;
        if (value(hand1[0]) == value(hand1[1]))
            high1 = value(hand1[0]);
        else 
            high1 = value(hand1[1]);
        if (value(hand2[0]) == value(hand2[1]))
            high2 = value(hand2[0]);
        else 
            high2 = value(hand2[1]);   
        if (high2 > high1) 
            return 2;
        else if (high1 > high2)
            return 1;
        else
            return 0;   
    }
    else if (combination(hand1) == 3 || combination(hand1) == 6)
    {
        if (value(hand1[2]) < value(hand2[2]))
            return 2;
        else if (value(hand1[2]) > value(hand2[2]))
            return 1;
        else if (value(hand1[2]) == value(hand2[2]))
            return 0;
    }
    else if (combination(hand1) == 7)
    {
        int hp1 = 0;
        int lp1 = 0;
        int k1 = 0;
        int hp2 = 0;
        int lp2 = 0;
        int k2 = 0;

        if (value(hand1[0]) != value(hand1[1]))
        {
            k1 = value(hand1[0]);
            hp1 = value(hand1[3]);
            lp1 = value(hand1[1]);
        }
        else if (value(hand1[3]) != value(hand1[4]))
        {
            k1 = value(hand1[4]);
            hp1 = value(hand1[2]);
            lp1 = value(hand1[0]);
        }
        else
        {
            k1 = value(hand1[2]);
            hp1 = value(hand1[3]);
            lp1 = value(hand1[0]);
        }

        if (value(hand2[0]) != value(hand2[1]))
        {
            k2 = value(hand2[0]);
            hp2 = value(hand2[3]);
            lp2 = value(hand2[1]);
        }
        else if (value(hand2[3]) != value(hand2[4]))
        {
            k2 = value(hand2[4]);
            hp2 = value(hand2[2]);
            lp2 = value(hand2[0]);
        }
        else
        {
            k2 = value(hand2[2]);
            hp2 = value(hand2[3]);
            lp2 = value(hand2[0]);
        }

        if (hp1 < hp2)
            return 2;
        else if (hp1 > hp2)
            return 1;
        else
        {
            if (lp1 < lp2)
                return 2;
            else if (lp1 > lp2)
                return 1;
            else
            {
                if (k1 < k2)
                    return 2;
                else if (k1 > k2)
                    return 1;
                else
                    return 0;
            }
            
        }
        
    }
    else if (combination(hand1) == 8)
    {
        int p1 = 0;
        int k1 = 0;
        int p2 = 0;
        int k2 = 0;

        for (int i = 1; i < 5; ++i)
        {
            for (int j = 0; j < i; ++j)
            {
                if (value(hand1[i]) == value(hand1[j]))
                    p1 = value(hand1[i]);
                
                if (value(hand2[i]) == value(hand2[j]))
                    p2 = value(hand2[i]);
            }
        }
        if (p1 == value(hand1[4]))
            k1 = value(hand1[2]);
        else
            k1 = value(hand1[4]);
        if (p2 == value(hand2[4]))
            k2 = value(hand2[2]);
        else
            k2 = value(hand2[4]);
        
        if (p1 < p2)
            return 2;
        else if (p1 > p2)
            return 1;
        else
        {
            if (k1 < k2)
                return 2;
            else if (k1 > k2)
                return 1;
            else
                return 0;
        }
    }
}

int comparehands(std::vector<int>& hand1, std::vector<int>& hand2)
{
    if (combination(hand1) == combination(hand2))
    {
        if (comparehigh(hand1, hand2) == 1)
            return 1;
        else if (comparehigh(hand1, hand2) == 2)
            return 2;
        else
            return 0;    }
    else if (combination(hand1) < combination(hand2))
        return 1;
    else if (combination(hand1) > combination(hand2))
        return 2;
}

void cardchanger(std::vector<int>& hand)
{
    std::cout << "How many cards do you want to change?\n";
    int num = 0;
    std::cin >> num;
    for (int i = 0; i < num; ++i)
    {
        std::cout << "Which card do you want to toss?\n";
        int pos = 0;
        std::cin >> pos;
        hand.erase(hand.begin() +pos - 1);
        printhand(hand);
    }
    dealcards(hand, num);
    std::cout << "Your new hand is: \n";
    printhand(hand);
}
