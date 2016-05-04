//
//  Puck.cpp
//  bRenderer_ios
//
//  Created by Björn Hasselmann on 03.05.16.
//
//

#include "Puck.hpp"

Puck::Puck(Field *field, Player *one, Player *two, GLfloat dimensionX, GLfloat dimensionY, GLfloat scale) : ObjectModel("hockeypuck"), trans_z(0.5), current(0.0, 0.0), last(current) {
    this->field = field;
    this->dimension.x = dimensionX;
    this->dimension.y = dimensionY;
    this->scale = vmml::Vector3f(scale);
    this->radius = dimensionX / 2;
    if(one->stick->left){
        this->left = one;
        this->right = two;
    } else {
        this->left = two;
        this->right = one;
    }
    this->velocity = 0.5f;
}

void Puck::drawModel(Renderer &bRenderer, const std::string &cameraName = ObjectModel::CAMERA_NAME) {
    vmml::Matrix4f modelMatrixHockeypuck = this->field->fieldMatrix
                        * vmml::create_translation(vmml::Vector3f(this->current.x, trans_z, this->current.y))
                        * vmml::create_scaling(scale);
    ShaderPtr shaderHockeypuck = bRenderer.getObjects()->getShader(MODEL_NAME);
    shaderHockeypuck->setUniform("NormalMatrix", vmml::Matrix3f(modelMatrixHockeypuck));
    shaderHockeypuck->setUniform("ModelMatrix", modelMatrixHockeypuck);
    
    this->ObjectModel::drawModel(bRenderer, MODEL_NAME, cameraName, modelMatrixHockeypuck, std::vector<std::string>({ }));
}

void Puck::makeMovement(){
    Point next(this->current);
    next.addVelocity(this->velocity, this->last); // starts always the same from 0.0
    
    bool collision = false;
    // y axis
    // lower bound
    GLfloat yBound = this->field->dimension.y/2;
    if(next.y - radius <= -yBound) {
        float diff = next.y - -yBound;
        next.y = -yBound;
        collision = true;
    }
    
}

Puck::Point Puck::bounce(Point &current, Point &impact){
    return Point(0.0,0.0);
}
