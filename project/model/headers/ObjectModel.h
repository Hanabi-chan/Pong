//
//  Model.h
//  bRenderer_ios
//
//  Created by Björn Hasselmann on 03.05.16.
//
//
#ifndef Model_h
#define Model_h

#include "Renderer.h"

class ObjectModel
{
public:
    
    static const std::string CAMERA_NAME;
    
    const std::string MODEL_NAME;
    
    struct Dimension {
        GLfloat x;
        GLfloat y;
    };
    Dimension dimension;
    
    void drawModel(Renderer &bRenderer, const std::string &modelName, const std::string &cameraName, const vmml::Matrix4f & modelMatrix, const std::vector<std::string> &lightNames);

//    vmml::Vector3f getScale();
//    
//    vmml::Vector3f getTranslation();
    
    vmml::Vector3f scale;
    
    vmml::Vector3f translation;
    
    virtual void drawModel(Renderer &bRenderer, const std::string &cameraName = ObjectModel::CAMERA_NAME);
    
    

protected:
    
    ObjectModel(const std::string &model_name);
    
   
    
};

#endif /* Model_h */
