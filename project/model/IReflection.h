//
//  IReflection.hpp
//  bRenderer_ios
//
//  Created by Björn Hasselmann on 28.05.16.
//
//

#ifndef IReflection_h
#define IReflection_h

/* Interface for indicating that a certain model is going to be reflected */
class IReflection {

public:
    
    virtual void drawModelReflection(Renderer &bRenderer) = 0;
    
};

#endif /* IReflection_h */
