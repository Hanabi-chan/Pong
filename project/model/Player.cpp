//
//  Player.cpp
//  bRenderer_ios
//
//  Created by Björn Hasselmann on 03.05.16.
//
//

#include "Player.hpp"

Player::Player(Stick *stick){
    this->stick = stick;
}

void Player::scorePoint(){
    ++this->score;
    this->stick->translation.y = 0.0;
}

int Player::getScore(){
    return this->score;
}