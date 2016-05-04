//
//  Field.cpp
//  bRenderer_ios
//
//  Created by Björn Hasselmann on 03.05.16.
//
//

#include "Field.hpp"

Field::Field(GLfloat dimensionX, GLfloat dimensionY, GLfloat scale) : ObjectModel("field"), fieldMatrix(vmml::create_rotation(-0.5f, vmml::Vector3f(1,0,0))) {
    this->scale(scale);
    this->dimension.x = dimensionX;
    this->dimension.y = dimensionY;
}

void Field::drawModel(Renderer &bRenderer, const std::string &cameraName = ObjectModel::CAMERA_NAME){
    ShaderPtr fieldShader = bRenderer.getObjects()->getShader(MODEL_NAME);
    fieldShader->setUniform("NormalMatrix", vmml::Matrix3f(this->fieldMatrix));
    this->ObjectModel::drawModel(bRenderer, MODEL_NAME, cameraName, this->fieldMatrix, std::vector<std::string>({ }));
}