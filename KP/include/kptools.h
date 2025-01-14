#ifndef __KPTOOLS_H__
#define __KPTOOLS_H__

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <sys/stat.h>
#include <mutex>

const std::string mainFileName = "main.back";
const std::string mainSemName = "main.semaphore";
int accessPerm = S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH;

int semSetvalue(sem_t *semaphore, int state)
{
    std::mutex mx;
    int s = 0;
    sem_getvalue(semaphore, &s);
    mx.lock();
    while (s++ < state) {
        sem_post(semaphore);
    }
    while (s-- > state + 1) {
        sem_wait(semaphore);
    }
    mx.unlock();
    return s;
}

namespace gametools
{
    class Player
    {
    public:
        std::string name;
        int bulls;
        int cows;
        std::string ans;
        friend std::ostream& operator<<(std::ostream& cout, const Player &p) {
            cout << "name: " << p.name << "\n";
            cout << "bulls: " << p.bulls << "\n";
            cout << "cows: " << p.cows << "\n";
            cout << "ans: " << p.ans << "\n";
            return cout;
        }
        Player();
        ~Player();
    };

    class Session
    {
    public:
        std::string sessionName;
        unsigned int cntOfPlayers;
        int curPlayerIndex = 0;
        std::vector<Player> playerList;
        std::string hiddenNum;
        friend std::ostream& operator<<(std::ostream& cout, const Session &s) {
            cout << "Name of session: " << s.sessionName << "\n";
            cout << "Count of players: " << s.cntOfPlayers << "\n";
            cout << "Turn of player: " << s.curPlayerIndex << "\n";
            cout << "Players:\n";
            for (auto i : s.playerList) {
                cout << i << "\n";
            }
            cout << "hidden Number: " << s.hiddenNum << "\n";
            return cout;
        }
        Session();
        ~Session();
    }; 

    Session::Session()
    {
    }
    
    Session::~Session()
    {
    }

    Player::Player()
    {
    }

    Player::~Player()
    {
    }
}

#endif
