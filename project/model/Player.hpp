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
    
    Stick *stick;
    
    Player(Stick *stick);
    
    void scorePoint();
    
    int getScore();
    
    void resetScore();
    
private:
    
    int score;
};
#endif /* Player_hpp */
