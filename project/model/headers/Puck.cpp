//
//  Puck.cpp
//  bRenderer_ios
//
//  Created by Björn Hasselmann on 03.05.16.
//
//

#include "Puck.hpp"

Puck::Puck(Field *field, GLfloat dimensionX, GLfloat dimensionY, GLfloat scale) : ObjectModel("hockeypuck"), trans_z(0.5) {
    this->field = field;
    this->dimension.x = dimensionX;
    this->dimension.y = dimensionY;
    this->scale = vmml::Vector3f(scale);
    this->radius = dimensionX / 2;
}

void Puck::drawModel(Renderer &bRenderer, const std::string &cameraName = ObjectModel::CAMERA_NAME) {
    vmml::Matrix4f modelMatrixHockeypuck = this->field->fieldMatrix
                        * vmml::create_translation(vmml::Vector3f(0.0f, trans_z, 0.0f))
                        * vmml::create_scaling(scale);
//                        * vmml::create_scaling(vmml::Vector3f(1.0));
    
    ShaderPtr shaderHockeypuck = bRenderer.getObjects()->getShader(MODEL_NAME);
    shaderHockeypuck->setUniform("NormalMatrix", vmml::Matrix3f(modelMatrixHockeypuck));
    shaderHockeypuck->setUniform("ModelMatrix", modelMatrixHockeypuck);
    //shaderHockeypuck->setUniform("color", vmml::Vector4f(1,1,0,1));
    
    this->ObjectModel::drawModel(bRenderer, MODEL_NAME, cameraName, modelMatrixHockeypuck, std::vector<std::string>({ }));
}