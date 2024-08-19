//
//  board.h
//  f
//
//  Created by Harsha  Kosuri  on 5/27/23.
//

#ifndef board_h
#define board_h
#include "Side.h"
#include <vector>
class Board {
    
public:
    Board(int nHoles, int nInitialBeansPerHole);
    int holes() const;
    int beans(Side s, int hole) const;
    int beansInPlay(Side s) const;
    int totalBeans() const;
    bool sow(Side s, int hole, Side& endSide, int& endHole);
    bool moveToPot(Side s, int hole, Side potOwner);
    bool setBeans(Side s, int hole, int beans);
private:
    int m_Holes[NSIDES];
    int m_initalBeans[NSIDES];
    std::vector<std::vector<int>> m_BoardBeans;
    int m_potBeans[NSIDES];
    Side m_currentSide;
};

#endif /* board_h */
