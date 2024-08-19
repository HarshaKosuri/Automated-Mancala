//
//  Game.cpp
//  f
//
//  Created by Harsha  Kosuri  on 5/30/23.
//

#include <stdio.h>
#include "Game.h"
#include "Player.h"
Game::Game(const Board& b, Player* south, Player* north) //game constructor
    : m_board(b), m_southPlayer(south), m_northPlayer(north), m_currentPlayer(SOUTH)
{}

void Game::display() const { //calls out display of the beans and board
    std::cout << "    " << m_northPlayer->name() << std::endl;
    std::cout << " ";
    for (int hole = 1; hole <= m_board.holes(); ++hole) {
        std::cout << " " << m_board.beans(NORTH, hole) << "  ";
    }
    std::cout << std::endl;
    
    std::cout << m_board.beans(NORTH, POT);
    for (int i = 0; i < 3 * m_board.holes() + 2; ++i) {
        std::cout << " ";
    }
    std::cout << m_board.beans(SOUTH, POT) << std::endl;
    
    std::cout << " ";
    for (int hole = 1; hole <= m_board.holes(); ++hole) {
        std::cout << " " << m_board.beans(SOUTH, hole) << "  ";
    }
    std::cout << std::endl;
    
    std::cout << "    " << m_southPlayer->name() << std::endl;
}
void Game::status(bool& over, bool& hasWinner, Side& winner)const  { //calls out how we win

    over = (m_board.beansInPlay(NORTH) == 0) || (m_board.beansInPlay(SOUTH) == 0);
    if (over) {
        int northBeans = m_board.beans(NORTH, POT);
        
        int southBeans = m_board.beans(SOUTH, POT);
         if (northBeans == southBeans) {
             hasWinner = false;  // Tie
         } else {
             hasWinner = true;
             winner = (northBeans > southBeans) ? NORTH : SOUTH;
         }
     }
 }
Side oppositeSide(Side side) {
    if (side == SOUTH) {
        return NORTH;
    } else {
        return SOUTH;
    }
}
bool Game::move(Side s) { //move function that sows the bean depending on which player, north or south goes and then sows it again if we run into a pot
    int hole;
    Side endSide;
    int endHole;
    bool over, hasWinner;
    Side winner;
    status(over, hasWinner, winner);

    if (s == m_currentPlayer && m_board.beansInPlay(s) > 0) {
        // Choose a hole to sow beans from
        if (m_currentPlayer == SOUTH) {
            hole = m_southPlayer->chooseMove(m_board, m_currentPlayer);
        } else {
            hole = m_northPlayer->chooseMove(m_board, m_currentPlayer);
        }
        // Attempt to make a complete move
        bool validMove = m_board.sow(s, hole, endSide, endHole);

        if (validMove && endSide == SOUTH && m_currentPlayer == SOUTH && endHole == POT) {
            if (m_board.beansInPlay(NORTH) > 0 || m_board.beansInPlay(SOUTH) > 0) {
                hole = m_southPlayer->chooseMove(m_board, m_currentPlayer);
               m_board.sow(s, hole, endSide, endHole);
            }
        }

        if (validMove && endSide == NORTH && m_currentPlayer == NORTH && endHole == POT) {
            if (m_board.beansInPlay(NORTH) != 0 || m_board.beansInPlay(SOUTH) != 0) {
                hole = m_northPlayer->chooseMove(m_board, m_currentPlayer);
                m_board.sow(s, hole, endSide, endHole);
            }
        }


        if (validMove && endSide == s && endHole != 0 && m_board.beans(s, endHole) == 1 && m_board.beans(oppositeSide(s), endHole) > 0) {
            m_board.moveToPot(s, endHole, s);
            m_board.moveToPot(oppositeSide(s), endHole, s);
        }

        if (m_board.beansInPlay(NORTH) == 0 || m_board.beansInPlay(SOUTH) == 0) {
            for (int hole = 1; hole <= m_board.holes(); ++hole) {
                m_board.moveToPot(NORTH, hole, NORTH);
                m_board.moveToPot(SOUTH, hole, SOUTH);
            }
            display();
        } else {
            m_currentPlayer = (m_currentPlayer == SOUTH) ? NORTH : SOUTH;
        }

        return validMove;
    }

    return false;
    }

void Game::play() { //play function that plays the game and continues displaying it as needed
    bool over, hasWinner;
       Side winner;
       
       // Play the game until it is over
       while (true) {
           display();

           status(over, hasWinner, winner);
           if (over) {
               break;
           }
           
           if (!(m_southPlayer->isInteractive()) && !(m_northPlayer->isInteractive())) {
               std::cout << "Press ENTER to continue..." << std::endl;
               std::cin.ignore();
           }
           
           move(m_currentPlayer);
       }
       
       
       // Announce the winner
       if (hasWinner) {
           std::cout << "The winner is ";
           if (winner == SOUTH) {
               std::cout << "South!" << std::endl;
           } else {
               std::cout << "North!" << std::endl;
           }
       } else {
           std::cout << "The game resulted in a tie." << std::endl;
       }
   }
int Game::beans(Side s, int hole) const {
        return m_board.beans(s, hole);
    }
    



