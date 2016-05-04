//
//  Stick.cpp
//  bRenderer_ios
//
//  Created by Björn Hasselmann on 03.05.16.
//
//

#include "Stick.hpp"

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
    this->dimension.x = dimensionX;
    this->dimension.y = dimensionY;
    this->scale = vmml::Vector3f(scale);
    this->left = left;
            
    this->move = 0.0;
    this->translation.x = (left ? -1 : 1) * trans_x;
    this->translation.y = 0.0;
    this->translation.z = trans_z;
            
}

void Stick::drawModel(Renderer &bRenderer, const std::string &cameraName = ObjectModel::CAMERA_NAME){
    vmml::Matrix4f stickModelMatrix = this->field->fieldMatrix
                        * vmml::create_translation(this->translation.toVector())
                        * vmml::create_scaling(scale)
                        * vmml::create_rotation(rotation, xAxis);
    ShaderPtr stickShader = bRenderer.getObjects()->getShader(MODEL_NAME);
    stickShader->setUniform("NormalMatrix", vmml::Matrix3f(stickModelMatrix));
    this->ObjectModel::drawModel(bRenderer, MODEL_NAME, cameraName, stickModelMatrix, std::vector<std::string>({ }));
}

/*
 * TODO:
 * -1 * trans_x for left player
 * movement
 */