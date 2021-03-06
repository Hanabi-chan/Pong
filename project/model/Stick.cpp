//
//  Stick.cpp
//  bRenderer_ios
//
//  Created by Björn Hasselmann on 03.05.16.
//
//

#include "Stick.hpp"

/*
 * free helper functions
 */
namespace {
    
    /*
     * TODO: get field coordinates in pixels!
     * (Maybe somehow compute how many "translation points" the screenwidth/height has?)
     *
     * 320px is where the field "starts"
     * 500px where it ends, seen from the upper border.
     */
    GLfloat fieldHeightStart = 320,
    fieldHeightEnd = 450,
    fieldHeight = fieldHeightEnd - fieldHeightStart;
    
    /* known values */
    GLfloat yTransMin = -1.85f, // <-- field.dimension.y / 2
    yTransMax = 2.5f,
    yTrans = fabsf(yTransMin) + yTransMax; // field.dimension.y
    
    
    GLfloat computeStickPosition(GLfloat yPosition){
        std::cout << "Position: " << yPosition << "\n";
        if(yPosition > fieldHeightEnd) return yTransMin;
        else if(yPosition < fieldHeightStart) return yTransMax;
        else {
            return -(yTrans/fieldHeight * (yPosition - fieldHeightStart) - yTransMax);
        }
    }
}

/*
 * Copy constructor for second player's stick
 */
Stick::Stick(const Stick &stick, bool left) : Stick::Stick(stick) {
    this->left = left;
    this->translation.x = (left ? -1 : 1) * trans_x;
}

Stick::Stick(Field *field, GLfloat dimensionX, GLfloat dimensionY, GLfloat scale, bool left)
        : ObjectModel("stick"), xAxis(1,0,0), rotation(-0.9), trans_x(4.0), trans_z(1.0) {
    this->field = field;
    this->dimension.x = dimensionX * scale;
    this->dimension.y = dimensionY * scale;
    this->scale = vmml::Vector3f(scale);
    this->left = left;
            
    this->translation.x = (left ? -1 : 1) * trans_x;
    this->translation.y = 0.0;
    this->translation.z = trans_z;
            
}

void Stick::drawModel(Renderer &bRenderer, const std::string &cameraName = ObjectModel::CAMERA_NAME){
    this->makeMovement(bRenderer);
    vmml::Matrix4f stickModelMatrix = this->field->fieldMatrix
                        * vmml::create_translation(this->translation.toVector())
                        * vmml::create_scaling(scale)
                        * vmml::create_rotation(rotation, xAxis);
    ObjectModel::drawModel(bRenderer, MODEL_NAME, cameraName, stickModelMatrix, std::vector<std::string>({ }));
}

void Stick::drawModelReflection(Renderer &bRenderer){
    this->makeMovement(bRenderer);
    vmml::Matrix4f stickModelMatrix = this->field->fieldMatrix
    * vmml::create_translation(this->translation.toVector()) * vmml::create_translation(vmml::Vector3f(0.0f, -1.0f, 0.0f))
    * vmml::create_scaling(scale) * vmml::create_scaling(vmml::Vector3f(1.0f, -1.0f, 1.0f))
    * vmml::create_rotation(rotation, xAxis);
    ObjectModel::drawModel(bRenderer, MODEL_NAME, ObjectModel::CAMERA_NAME, stickModelMatrix, std::vector<std::string>({ }));
}

void Stick::makeMovement(Renderer &bRenderer){
    /* handle touch inputs */
        TouchMap touches = bRenderer.getInput()->getTouches();
        for(TouchMap::iterator it = touches.begin(); it != touches.end(); it++){
            Touch touch = it->second;
            if((touch.startPositionX < View::getScreenWidth() / 2 && this->left)
                    || (touch.startPositionX > View::getScreenWidth() / 2 && !this->left)) {
                this->translation.y = computeStickPosition(touch.lastPositionY);
            }
        }
}