//
//  Header.h
//  f
//
//  Created by Harsha  Kosuri  on 5/27/23.
//

#ifndef Header_h
#define Header_h
#include "Player.h"
#include "Board.h"
#include <stdio.h>
#include <iostream>
class Game {
public:
    Game(const Board& b, Player* south, Player* north);
    void display() const;
    void status(bool& over, bool& hasWinner, Side& winner) const;
    bool move(Side s);
    void play();
    int beans(Side s, int hole) const;

private:
    Board m_board;
    Player* m_southPlayer;   
    Player* m_northPlayer;
    Side m_currentPlayer;
};

//game class with 4 private variables with the south player, nroth player, and the currentplayer






#endif /* Header_h */
