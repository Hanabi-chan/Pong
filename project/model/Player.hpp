//
//  Player.hpp
//  bRenderer_ios
//
//  Created by Björn Hasselmann on 03.05.16.
//
//

#ifndef Player_hpp
#define Player_hpp

#include <stdio.h>
#include "Stick.hpp"

class Player {
    
public:
    
    int score;
    Stick *stick;
    
    Player(Stick *stick);
    
    
};
#endif /* Player_hpp */
