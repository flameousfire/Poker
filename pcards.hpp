#ifndef POKER_HPP
#define POKER_HPP

extern std::vector<int> deck;
std::string suit(int card);
int value(int card);
bool checkflush(std::vector<int>& hand);
int checkmultiples(std::vector<int>& hand, int hs = 5);
bool checkfullhouse(std::vector<int> hand);
bool checktwopairs(std::vector<int> hand);
bool checkstraight(std::vector<int> hand);
int combination(std::vector<int>& hand);
void dealcards(std::vector<int>& hand, int hs = 5);
void printhand(std::vector<int>& hand);
std::string combprint(int c);
bool greaterthan(int i, int j);
void sorthand(std::vector<int>& hand);
int comparehigh(std::vector<int>& hand1, std::vector<int>& hand2);
int comparehands(std::vector<int>& hand1, std::vector<int>& hand2);
void cardchanger(std::vector<int>& hand);


#endif