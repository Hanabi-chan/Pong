//
//  Puck.hpp
//  bRenderer_ios
//
//  Created by Björn Hasselmann on 03.05.16.
//
//

#ifndef Puck_hpp
#define Puck_hpp

#include <stdio.h>
#include "ObjectModel.h"
#include "Field.hpp"
#include "Player.hpp"
#include "IReflection.h"

class Puck : public ObjectModel, public IReflection {
    
public:
    
    Puck(Field *field, Player *one, Player *two, GLfloat dimensionX, GLfloat dimensionY, GLfloat scale);
    
    void drawModel(Renderer &bRenderer, const std::string &cameraName);
    
    virtual void drawModelReflection(Renderer &bRenderer);
    
private:
    
    const GLfloat initVelocity;
    
    const GLfloat trans_z;
    
    Field *field;
    
    Player *left, *right;
    
    GLfloat radius, velocity;
    
    struct Point {
        
        Point(GLfloat x, GLfloat y){
            this->x = x;
            this->y = y;
        }
        
        GLfloat x, y;
        
        bool isImpact = false, playerCollision = false;
        
        void addVelocity(GLfloat velocity, Point &last){
            this->x += ((this->x > last.x ? 1 : -1) * velocity);
            this->y += ((this->y > last.y ? 1 : -1) * velocity);
        }
        
    };
    Point current, last;
    
    void makeMovement();
    
    Point bounce(Point &last, Point &currentImpact, bool yAxis);
    
    bool checkPlayerCollision(Player *player, Point &next, GLfloat xBound, bool &collision);
    
};


#endif /* Puck_hpp */
