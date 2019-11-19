#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "pservernw.hpp"
#include "pcards.hpp"
#include "gameround.hpp"

struct Player
{
    std::string name;
    int idnumber;
    int stack;
    int bet;
    std::vector<int> hand;

Player()
{
    static int id = 0;
    ++id;
    idnumber = id;
    name = "Player" + std::to_string(id);
    stack = 1000;
    bet = 0;
    dealcards(hand, 5);
}
};

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

Gameround()
{
    std::cout << "New round begins.\n\n";
    sendstring(new_socket, "New round begins.\n");
    static int round = 0;
    ++round;
    playersnum = playerlist.size();
    dealer = (2*playersnum - 4 + round) % playersnum;
    pot = 0;
    for (int i = 0; i < playersnum; ++i)
        roundplayers.push_back(&playerlist[i]);
    sblind = 50 * (1 + round / 5);
    bblind = 2 * sblind;
    smallblind(playerlist[(dealer + 1) % playersnum]);
    bigblind(playerlist[(dealer + 2) % playersnum]);
    highestbet = bblind;
    minraise = bblind;
    counter = 0;
    turnorder = dealer + 2;
    firstbid = true;
}

void turncaller()
{
    ++turnorder;
    if (turnorder >= playersnum)
        turnorder %= playersnum;

    if (counter == playersnum || (counter == playersnum - 1 && highestbet != 0 && firstbid == false))
    {
        turnorder = dealer;
        highestbet = 0;
        counter = 0;
        for (int i = 0; i < playerlist.size(); ++i)
        {
            pot += playerlist[i].bet;
            playerlist[i].bet = 0;
        }
    }
    else if (roundplayers[turnorder]->stack == 0)
    {
        ++counter;
        turncaller();
    }
    else
    {
        if (roundplayers[turnorder]->idnumber == 1)
            turn(*roundplayers[turnorder]);  
        else
            sturn(*roundplayers[turnorder]);
    }
}

void smallblind(Player& player)
{
    player.stack -= sblind;
    player.bet = sblind;
    std::string tmp = player.name + " has the small blind: " + std::to_string(sblind);
    sendstring(new_socket, tmp);
    std::cout << tmp << "\n";
}

void bigblind(Player& player)
{
    player.stack -= bblind;
    player.bet = bblind;
    std::string tmp = player.name + " has the big blind: " + std::to_string(bblind);    
    sendstring(new_socket, tmp);
    std::cout << tmp << "\n";
}

void sturn(Player& who)
{
    std::string start = "Players fighting for the pot: " ;
    for (int i = 0; i < playersnum; ++i)
        start += roundplayers[i]->name + " ";
    start += "\nPlayer to act: " + who.name + "\n";
    sendstring(new_socket, start);
    sendstring(new_socket, serverprinthand(who.hand));   
    int amount = highestbet - who.bet;
    std::string bid = "Highest bet is " + std::to_string(highestbet) + ". You must pay " + std::to_string(amount) + " to call. \n";
    sendstring(new_socket, bid);
    sendstring(new_socket, "TURN");
    std::string choice = recvstring(new_socket);
    if (choice == "f")
        sfold(&who);
    else if (choice == "c")
        scall(who);
    else if (choice == "r")
        sraise(who);

    turncaller();
}


void sraise(Player& who)
{
    sendstring(new_socket, "How much do you want to raise?\n");
    sendstring(new_socket, "RAISE");
    counter = 0;
    firstbid = false;
    sendstring(new_socket, std::to_string(minraise));
    int raise = std::stoi(recvstring(new_socket));
    int amount = highestbet - who.bet;
    if (who.stack < amount + raise)
    {
        sendstring(new_socket, "All in!\n");
        std::cout << who.name << " is all in!\n";
        who.bet += who.stack;
        who.stack = 0;
        if (minraise < raise)
            minraise = raise;
        if (who.bet > highestbet)
            highestbet = who.bet;
    }
    else
    {
    who.stack -= amount + raise;
    who.bet += amount + raise;
    highestbet = highestbet + raise;
    if (minraise < raise)
        minraise = raise;
    std::string rais = "Raise " + std::to_string(raise) + "!\n";
    sendstring(new_socket, rais);
    std::cout << who.name << " raises " << raise;
    }
}

void scall(Player& who)
{
    int amount = highestbet - who.bet;
    ++counter;
    if (who.stack < amount)
    {
        std::cout << "All in!\n";
        who.bet = who.stack;
        who.stack = 0;
    }
    else
    {
        who.stack -= amount;
        who.bet += amount;
        if (amount == 0)
        {
            std::cout << "Check. \n";
            sendstring(new_socket, "You checked.\n");
        }
        else
        {
            std::cout << "Call.\n"; 
            sendstring(new_socket, "You called.\n");
        }
    }
}


void sfold(Player* who)
{
    std::string out = who->name + " is out.\n";
    std::cout << out;
    sendstring(new_socket, out);
    std::vector<Player*>::iterator it = std::find(roundplayers.begin(), roundplayers.end(), who);
    roundplayers.erase(it);
    --playersnum;
    --turnorder;
}

void raise(Player& who)
{
    std::cout << "How much will you raise?\n";
    int raise = 0;
    counter = 0;
    firstbid = false;
    while (raise < minraise)
    {
        if (!(std::cin >> raise))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        if (raise < minraise)
        std::cout << "That raise is too small.\n";
    }
    int amount = highestbet - who.bet;
    if (who.stack < amount + raise)
    {
        std::cout << "All in!\n";
        sendstring(new_socket, who.name + " is all in!");
        who.bet += who.stack;
        who.stack = 0;
        if (who.bet > highestbet)
        highestbet = who.bet;
        if (minraise < raise)
            minraise = raise;
    }
    else
    {
    who.stack -= amount + raise;
    who.bet += amount + raise;
    highestbet = highestbet + raise;
    if (minraise < raise)
        minraise = raise;
    std::cout << "Raise " << raise << "!\n";
    sendstring(new_socket, who.name + " raises " + std::to_string(raise));

    }
}

void call(Player& who)
{
    int amount = highestbet - who.bet;
    ++counter;
    if (who.stack < amount)
    {
        std::cout << "All in!\n";
        sendstring(new_socket, who.name + " is all in!");
        who.bet = who.stack;
        who.stack = 0;
    }
    else
    {
        who.stack -= amount;
        who.bet += amount;
        if (amount == 0)
        {
            std::cout << "Check. \n";
            sendstring(new_socket, who.name + " checks.");
        }
        else
        {
            std::cout << "Call.\n"; 
            sendstring(new_socket, who.name + " calls.");
        }
    }
}


void fold(Player* who)
{
    std::cout << "I'm out. \n";
    sendstring(new_socket, who->name + " folds.");
    std::vector<Player*>::iterator it = std::find(roundplayers.begin(), roundplayers.end(), who);
    roundplayers.erase(it);
    --playersnum;
    --turnorder;
}


void turn(Player& who)
{
    std::cout << "Players fighting for the pot: ";
    for (int i = 0; i < playersnum; ++i)
        std::cout << roundplayers[i]->name << " ";
    std::cout << "\n";
    printhand(who.hand);
    std::cout << "\n" << who.name << " to act.\n";
    sendstring(new_socket, who.name + " to act.");
    int amount = highestbet - who.bet;
    std::cout << "Highest bid is " << highestbet << ". You must pay " << amount << " to call. \n";
    std::cout << "Choose to fold(f), call/check(c) or raise(r): \n";
    std::string choice = "";
    while (choice != "f" && choice != "c" && choice != "r")
        if (!(std::cin >> choice))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');    
        }
    if (choice == "f")
        fold(&who);
    else if (choice == "c")
        call(who);
    else
        raise(who);

    turncaller();
}

void comparetable()
{
    std::vector<int> besthand = roundplayers[0]->hand;
    int winnerindex = 0;
    std::vector<int> tieindex = {};
    for (int i = 1; i < playersnum; ++i)
    {
        if (comparehands(besthand, roundplayers[i]->hand) == 2)
        {
            besthand = roundplayers[i]->hand;
            winnerindex = i;
            tieindex.clear();
        }
        else if (comparehands(besthand, roundplayers[i]->hand) == 0)
        {
            tieindex.emplace_back(i);
        }
    }
    potdivider(winnerindex, tieindex);

}

void potdivider(int win, std::vector<int> tie)
{
    if (tie.empty() == true)
    {
        roundplayers[win]->stack += pot;
        std::cout << roundplayers[win]->name << " wins with " << combprint(combination(roundplayers[win]->hand)) << "\n";
        sendstring(new_socket, roundplayers[win]->name + " wins with " + combprint(combination(roundplayers[win]->hand)));
    }

    else
    {   std::cout << "Players: ";
        for (int n : tie)
        {
            roundplayers[n]->stack += pot / tie.size();
            std::cout << roundplayers[n]->name << " ";
        }
        std::cout << "tie the hand with " << combprint(combination(roundplayers[win]->hand)) << "\n";
    }
    
}

void winlosscheck()
{
    for (int i = 0; i < playerlist.size(); ++i)
    {
        if (playerlist[i].stack == 0)
        {
            std::cout << playerlist[i].name << " is out!\n";
            sendstring(new_socket, playerlist[i].name + " is out!");
            playerlist.erase(playerlist.begin() + i);
        }
    }
    
    if (playerlist.size() == 1)
    {
        std::cout << playerlist[0].name << " has won the game! \n";
        sendstring(new_socket, playerlist[0].name + " has won the game!");
    }
}

void roundrun()
{
    turncaller();
    if (playersnum > 1)
    {
        for (int i = 0; i < roundplayers.size(); ++i)
        {
            if (roundplayers[i]->idnumber == 1)
            {
                std::cout << roundplayers[i]->name << "\n";
                printhand(roundplayers[i]->hand);
                sendstring(new_socket, roundplayers[i]->name);
                cardchanger(roundplayers[i]->hand);
            }
            else
            {
                servercardchanger(new_socket, roundplayers[i]->hand);
            }
            
        }
        if (roundplayers[dealer]->idnumber == 1)
            turn(*roundplayers[dealer]);
        else
            sturn(*roundplayers[dealer]);
           
    }
    comparetable();
    
    for (int i = 0; i < roundplayers.size(); ++i)
    {
        std::cout << roundplayers[i]->name + "'s hand: ";
        printhand(roundplayers[i]->hand);
        std::cout << "Stack: " << roundplayers[i]->stack << "\n";
        sendstring(new_socket, roundplayers[i]->name + "'s hand: " + serverprinthand(roundplayers[i]->hand) + " stack: " + std::to_string(roundplayers[i]->stack));
    }
    deck.clear();
    for (int i = 1; i <= 52; ++i)
    deck.emplace_back(i);

    winlosscheck();
}

};