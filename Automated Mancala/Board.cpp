//
//  Board.cpp
//  f
//
//  Created by Harsha  Kosuri  on 5/27/23.
//
#include "Board.h"
#include <vector>
#include "Side.h"
#include <stdio.h>
Board::Board(int nHoles, int nInitialBeansPerHole) { //constructor for the Board that initalizes the potbeans, the holes, and the inital beans that we need for everything as well as the pot beans
     if (nHoles <= 0) {
         nHoles = 1;
     }
     
     if (nInitialBeansPerHole < 0) {
         nInitialBeansPerHole = 0;
     }
    m_potBeans[NORTH] = 0;
    m_potBeans[SOUTH] = 0;
     m_Holes[NORTH] = nHoles;
     m_Holes[SOUTH] = nHoles;
     m_initalBeans[NORTH] = nInitialBeansPerHole;
     m_initalBeans[SOUTH] = nInitialBeansPerHole;

     m_BoardBeans.resize(NSIDES);
     for (int i = 0; i < NSIDES; ++i) {
         m_BoardBeans[i].resize(nHoles + 1, nInitialBeansPerHole);
     }
     
     m_potBeans[NORTH] = 0;
     m_potBeans[SOUTH] = 0;
 }

int Board::holes() const { //returns only the holes for the north side because holes just wants to know the holes for one side
        return m_Holes[NORTH];
   }
int Board::beans(Side s, int hole) const { //returns the beans for each specific hole
    
    if (hole < 0 || hole > m_Holes[s]) {
         return -1;
     }
     
     if (hole == POT) {
         return m_potBeans[s];
     }
     
    return m_BoardBeans[s][hole];
    
}

int Board::beansInPlay(Side s) const { //sees how many beans are in each side
    int totalBeans = 0;
     for (int hole = 1; hole <= m_Holes[s]; ++hole) {
         totalBeans += m_BoardBeans[s][hole];
     }
     return totalBeans;
 }

int Board::totalBeans() const { // counts the total beans in total from both sides
    int totalBeanss=0;
    for(int hole = 1; hole <= m_Holes[NORTH]; ++hole) {
        totalBeanss += m_BoardBeans[NORTH][hole];
    }
    for(int hole = 1; hole <= m_Holes[SOUTH]; ++hole) {
        totalBeanss += m_BoardBeans[SOUTH][hole];
    }
    return totalBeanss += m_potBeans[NORTH] + m_potBeans[SOUTH];
}

bool Board::sow(Side s, int hole, Side& endSide, int& endHole) { //sow function sows the necessary values each time it is called
    if (hole <= 0 || hole > holes() || beans(s, hole) == 0)
        return false;
    
    int numBeans = beans(s, hole);
    setBeans(s, hole, 0);
    Side currentSide = s;
    int currentHole = hole;
    
    while (numBeans > 0) {
        if (currentSide == NORTH) {
            currentHole--;
            if (currentHole == 0) {
                m_potBeans[NORTH]++;
                numBeans--;
                if (numBeans == 0) {
                    endSide = NORTH;
                    endHole = POT;
                    return true;
                }
                currentSide = SOUTH;
                currentHole = 1;
            }
        } else {
            currentHole++;
            if (currentHole > holes()) {
                m_potBeans[SOUTH]++;
                numBeans--;
                if (numBeans == 0) {
                    endSide = SOUTH;
                    endHole = POT;
                    return true;
                }
                currentSide = NORTH;
                currentHole = holes();
            }
        }
        setBeans(currentSide, currentHole, beans(currentSide, currentHole) + 1);
        numBeans--;
    }
    
    endSide = currentSide;
    endHole = currentHole;
    return true;
}


bool Board::moveToPot(Side s, int hole, Side potOwner) { //move the beans at the pot at a certain place to a new POT
    if (hole == POT || hole < 1 || hole > m_Holes[s]) {
        return false;
    }
    int beans2move = m_BoardBeans[s][hole];
    m_BoardBeans[s][hole] = 0;
    
    m_potBeans[potOwner] += beans2move;
    return true;
}

bool Board:: setBeans(Side s, int hole, int beans) { //set the beans at a certain hole to whatever value we desire
    
    if (hole < 1 || hole > m_Holes[s] || beans < 0) {
        return false;
    }

    if (hole == POT) {
        m_potBeans[s] = beans;
        return true;
    } else if (hole >= 0)
        m_BoardBeans[s][hole] = beans;
    return true;
}
