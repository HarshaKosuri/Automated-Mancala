#include "Player.h"
#include <iostream>
#include <vector>
#include <climits>


std::string Player::name() const {
    return m_name; //returns the name
}
bool Player::isInteractive() const {
    return false; //sets the base case to false here
}
bool HumanPlayer::isInteractive() const {
    return true; //returns true here because it is interactive
}

int HumanPlayer::chooseMove(const Board& b, Side s) const { //choose move makes you first enter a valid hole number, and if you don't enter one, akss you to do it again
    int hole;

    std::cout << "Enter a valid hole number: "; 

    if (!(std::cin >> hole) || hole < 1 || hole > b.holes() || b.beans(s, hole) == 0) {
        std::cin.clear();
        std::cout << "Invalid hole number. Please try again." << std::endl;
    }
    else {
        return hole;
    }

    return 0;
    }

bool BadPlayer::isInteractive() const {
    return false; //returns it false here
}
int BadPlayer::chooseMove(const Board& b, Side s) const {
    int holes = b.holes();
     for (int i = 1; i <= holes; i++) {
         if (b.beans(s, i) > 0) {
             return i;
         }
     }
     return -1; // moves to the very next non zero number
 }

bool SmartPlayer::isInteractive() const {
    return false;
}
Side opposieteSide(Side side) {
    if (side == SOUTH) {
        return NORTH;
    } else {
        return SOUTH;
    }
}

int evaluate(const Board& b, Side currentPlayerSide) {
    Side opponentPlayerSide = opposieteSide(currentPlayerSide);
 
      if (b.beansInPlay(NORTH) == 0 || b.beansInPlay(SOUTH) == 0) {
          if (b.beans(NORTH, 0) > b.beans(SOUTH, 0)) {
              return MAX_EVAL;
          } else if (b.beans(NORTH, 0) < b.beans(SOUTH, 0)) {
              return MIN_EVAL;
          } else {
              return 0;
          } //evaluate function that has a value based on teh nouth vs the north with each value becoming a int_max or int_min which are defined as the lowest and highest possible value in c++ which is helpful here
      }

     
      int evalValue = b.beans(SOUTH, 0) - b.beans(NORTH, 0); //easiest way to tell if we are in the move

      
      int holeHeuristic = 0; //herustic made from ensuring that we stay in our holes
      for (int hole = 1; hole <= b.holes(); ++hole) {
          int currentPlayerBeans = b.beans(currentPlayerSide, hole);
          int opponentPlayerBeans = b.beans(opponentPlayerSide, hole);
          holeHeuristic += currentPlayerBeans - opponentPlayerBeans;
      }
      evalValue += holeHeuristic;

      // herusitic made with a weightage on stealing the opponenets beans
      int captureHeuristic = 0;
      for (int hole = 1; hole <= b.holes(); ++hole) {
          if (b.beans(currentPlayerSide, hole) == 0) {
              int currentPlayerPot = b.beans(currentPlayerSide, 0);
              int opponentPlayerPot = b.beans(opponentPlayerSide, 0);
              captureHeuristic += currentPlayerPot - opponentPlayerPot;
          }
      }
      evalValue += captureHeuristic;

      // herusitic made that favor moves that lead to more available moves for the current player
      int mobilityHeuristic = 0;
      for (int hole = 1; hole <= b.holes(); ++hole) {
          if (b.beans(currentPlayerSide, hole) > 0) {
              Board copy(b);
              Side endSide;
              int endHole;
              copy.sow(currentPlayerSide, hole, endSide, endHole);
              int currentPlayerMoves = copy.beansInPlay(currentPlayerSide);
              int opponentPlayerMoves = copy.beansInPlay(opponentPlayerSide);
              mobilityHeuristic += currentPlayerMoves - opponentPlayerMoves;
          }
      }
      evalValue += mobilityHeuristic;

      //herustic made that checks to ensure that the difference between pots is always greater on one side
      int potHeuristic = b.beans(currentPlayerSide, 0) - b.beans(opponentPlayerSide, 0);
      evalValue += potHeuristic;
    
    int emptyHoleHeuristic = 0;
    for (int hole = 1; hole <= b.holes(); ++hole) {
        if (b.beans(currentPlayerSide, hole) == 0) {
            emptyHoleHeuristic -= 10;
        }
    }
    evalValue += emptyHoleHeuristic;
    //heursitic made that checks to make the best move to see if the opponents pot is empty or not
    int opponentEmptyHoleHeuristic = 0;
    for (int hole = 1; hole <= b.holes(); ++hole) {
        if (b.beans(opponentPlayerSide, hole) == 0) {
            opponentEmptyHoleHeuristic += 10;
        }
    }
    evalValue += opponentEmptyHoleHeuristic;
    // favors the player with more holes in their current hole
    int cornerOccupancyHeuristic = 0;
    int currentPlayerCornerBeans = b.beans(currentPlayerSide, 0);
    int opponentPlayerCornerBeans = b.beans(opponentPlayerSide, 0);
    cornerOccupancyHeuristic += currentPlayerCornerBeans - opponentPlayerCornerBeans;
    evalValue += cornerOccupancyHeuristic;
    // adds weightage to the idea of having more stuff in corner holes
    int frontierHoleHeuristic = 0;
    for (int hole = 1; hole <= b.holes(); ++hole) {
        if (b.beans(currentPlayerSide, hole) > 0 && b.beans(currentPlayerSide, hole + 1) == 0) {
            frontierHoleHeuristic += 5;
        }
    }
    evalValue += frontierHoleHeuristic;
     // adds weithtage to bringing the game when there is a greater difference in seeds between the two sides
    
    int seedDifferenceHeuristic = b.beansInPlay(SOUTH) - b.beansInPlay(NORTH);
    evalValue += seedDifferenceHeuristic;
    
    return evalValue;


  }

int SmartPlayer::chooseMove(const Board& b, Side s) const {
    Side currentPlayerSide = s; //initalzes currentPlayer side as s
      int bestHole = -1; //initalizes the besthole number as =1
      int bestValue = MIN_EVAL; //using limits, we make the bestValue start at int_min so we can see how far up it is capable of going
      int a = MIN_EVAL; // use
      int c = MAX_EVAL;

      for (int hole = 1; hole <= b.holes(); ++hole) { // iterate through all avaliable spaces
          if (b.beans(currentPlayerSide, hole) > 0) {
              Board copy(b); //create a copy of the bard and have an int and side endside chosen
              Side endSide;
              int endHole;
              copy.sow(currentPlayerSide, hole, endSide, endHole); //sow for each side called

              int moveValue;

              // Check if the last sow ended in the pot and if it does than recurisvly call it for both sides
              if (endSide == NORTH && currentPlayerSide == SOUTH) { // if
                  // Recursively call chooseMove again for the current player's side
                  moveValue = chooseMove(copy, currentPlayerSide);
              }
              if ((endSide == NORTH && m_currentPlayer == NORTH && endHole == POT) || (endSide == SOUTH && m_currentPlayer == SOUTH && endHole == POT)) {
                  moveValue = chooseMove(copy, currentPlayerSide);
              }
              else {
                  // Call minMax for the opponent's side to see what causes the greatest  value to occur
                  moveValue = minMax(copy, opposieteSide(currentPlayerSide), a, c, 0);
              }

              if (moveValue > bestValue) { // if the value is greater than the current value
                  bestValue = moveValue; // make it the move value and the hole
                  bestHole = hole;
              }

              a = std::max(a, bestValue);

              if (a >= c) {
                  break; // is the a value now becomes greater than the original max valye we set, then it is a viable option to move towards causing us to return that valey
              }
          }
      }

      return bestHole;
  }
int SmartPlayer::minMax(const Board& b, Side currentPlayerSide, int a, int c, int depth) const {
    if (b.beansInPlay(NORTH) == 0 || b.beansInPlay(SOUTH) == 0 || depth == MAX_DEPTH) { //base cause with the depth already figured out at 5 and when the game is won
        return evaluate(b, currentPlayerSide);
    }

    // Maximizing player's turn
    if (currentPlayerSide == m_currentPlayer) {
        int bestValue = MIN_EVAL;
        for (int hole = 1; hole <= b.holes(); ++hole) {
            if (b.beans(currentPlayerSide, hole) > 0) {
                Board copy(b);
                Side endSide;
                int endHole;
                copy.sow(currentPlayerSide, hole, endSide, endHole);

                int moveValue = minMax(copy, opposieteSide(currentPlayerSide), a, c, depth + 1);
                bestValue = std::max(bestValue, moveValue);

                a = std::max(a, bestValue);
                if (a >= c) {
                    break; // B
                }
            }
        }
        return bestValue;
    }
    // Minimizing player's turn
    else {
        int bestValue = MAX_EVAL;
        for (int hole = 1; hole <= b.holes(); ++hole) {
            if (b.beans(currentPlayerSide, hole) > 0) {
                Board copy(b);
                Side endSide;
                int endHole;
                copy.sow(currentPlayerSide, hole, endSide, endHole);

                int moveValue = minMax(copy, opposieteSide(currentPlayerSide), a, c, depth + 1);
                bestValue = std::min(bestValue, moveValue);

                c = std::min(c, bestValue);
                if (a >= c) {
                    break; //
                }
            }
        }
        return bestValue;
    }
}






