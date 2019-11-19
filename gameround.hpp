#ifndef POKER_HPP
#define POKER_HPP
#include <vector>

struct Player
{
    std::string name;
    int idnumber;
    int stack;
    int bet;
    std::vector<int> hand;

    Player();
};

extern std::vector<Player> playerlist;

struct Gameround
{
    int playersnum;
    int dealer;
    int pot;
    std::vector<Player*> roundplayers;
    int sblind;
    int bblind;
    int highestbet;
    int minraise;
    int counter;
    int turnorder;
    bool firstbid;


Gameround();
void turncaller();
void smallblind(Player& player);
void bigblind(Player& player);
void sturn(Player& who);
void sraise(Player& who);
void scall(Player& who);
void sfold(Player* who);
void raise(Player& who);
void call(Player& who);
void fold(Player* who);
void turn(Player& who);
void comparetable();
void potdivider(int win, std::vector<int> tie);
void winlosscheck();
void roundrun();

};


#endif