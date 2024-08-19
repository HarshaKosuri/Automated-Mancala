#ifndef player_h
#define player_h

#include <string>
#include "Board.h"

class Player { //class player that has everything needed to  be derived from later one as well as a constructor for the name
public:
    Player(std::string name) : m_name(name) {}
    std::string name() const;
    virtual bool isInteractive() const;
    virtual int chooseMove(const Board& b, Side s) const = 0;
    virtual ~Player() = default;
private:
    std::string m_name;
};

class HumanPlayer : public Player { //derived class that overrides interactive and choosemove in order to allow for a human to play
public:
    HumanPlayer(const std::string& name) : Player(name) {}
    virtual  bool isInteractive() const override;
   virtual  int chooseMove(const Board& b, Side s) const override;
};

class BadPlayer : public Player { //derived class called BadPlayer that allows a computer BadPlayer to paly
public:
    BadPlayer(const std::string& name) : Player(name) {}
    virtual bool isInteractive() const;
    virtual int chooseMove(const Board& b, Side s) const ;
};


class SmartPlayer : public Player { //derived class called SmartPlayer that looks at how effective each move is before making it
public:
    SmartPlayer(const std::string& name)
          : Player(name), m_currentPlayer(SOUTH) {}
    virtual bool isInteractive() const;
    virtual int chooseMove(const Board& b, Side s) const;

private:
    Side m_currentPlayer;   
    int minMax(const Board& b, Side currentPlayerSide, int a, int c, int depth) const;
};


#endif /* player_h */
