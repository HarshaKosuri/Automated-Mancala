//
//  s.h
//  f
//
//  Created by Harsha  Kosuri  on 5/27/23.
//

#ifndef s_h
#define s_h
enum Side { NORTH, SOUTH };

    const int NSIDES = 2;
    const int POT = 0;

    inline
    Side opponent(Side s)
    {
        return Side(NSIDES - 1 - s);
    }
const int MAX_DEPTH = 5;
const int MIN_EVAL = -1000000; 
const int MAX_EVAL = 1000000;
#endif /* s_h */
