#include "RenderProject.h"
#include <math.h>
#include <stdlib.h>
#include <iostream>

float translation = 0.0f;

bool cube1Reached = false;
bool cube2Reached = false;

float constantMovement = 0;
float movementDirection = -1.0;

float translationHockeypuck_Xpos = 0.0;
float translationHockeypuck_Ypos = 0.0;
float translationHockeypuck_Zpos = 0.0;

float translationCubePlayer_Xpos = 0.0;
float translationCubePlayer_Ypos = 0.0;
float translationCubePlayer_Zpos = 0.0;

float translationCubeEnemy_Xpos = 0.0;
float translationCubeEnemy_Ypos = 0.0;
float translationCubeEnemy_Zpos = 0.0;

/* Initialize the Project */
void RenderProject::init()
{
    bRenderer::loadConfigFile("config.json");	// load custom configurations replacing the default values in Configuration.cpp
    
    // let the renderer create an OpenGL context and the main window
    if(Input::isTouchDevice())
        bRenderer().initRenderer(true);										// full screen on iOS
    else
        bRenderer().initRenderer(1920, 1080, false, "Assignment 4");		// windowed mode on desktop
    //bRenderer().initRenderer(View::getScreenWidth(), View::getScreenHeight(), true);		// full screen using full width and height of the screen
    
    // start main loop
    bRenderer().runRenderer();
}

/* This function is executed when initializing the renderer */
void RenderProject::initFunction()
{
    // get OpenGL and shading language version
    bRenderer::log("OpenGL Version: ", glGetString(GL_VERSION));
    bRenderer::log("Shading Language Version: ", glGetString(GL_SHADING_LANGUAGE_VERSION));
    
    // initialize variables
    _offset = 0.0f;
    _randomOffset = 0.0f;
    _cameraSpeed = 40.0f;
    _running = true; _lastStateSpaceKey = bRenderer::INPUT_UNDEFINED;
    _viewMatrixHUD = Camera::lookAt(vmml::Vector3f(0.0f, 0.0f, 0.25f), vmml::Vector3f::ZERO, vmml::Vector3f::UP);
    
    // set shader versions (optional)
    bRenderer().getObjects()->setShaderVersionDesktop("#version 120");
    bRenderer().getObjects()->setShaderVersionES("#version 100");
    
    /* Loading Sphere Object */
    
    // load materials and shaders before loading the model
    ShaderPtr sphereShader = bRenderer().getObjects()->loadShaderFile("sphere", 0, false, true, true, false, false);				// load shader from file without lighting, the number of lights won't ever change during rendering (no variable number of lights)
    
    // create additional properties for a model
    PropertiesPtr sphereProperties = bRenderer().getObjects()->createProperties("sphereProperties");
    
    // load model
    bRenderer().getObjects()->loadObjModel("sphere.obj", true, true, true, 4, true, false, sphereProperties);								// automatically generates a shader with a maximum of 4 lights (number of lights may vary between 0 and 4 during rendering without performance loss)
    
    /* Loading Cube Object */
    
    // load materials and shaders before loading the model
    ShaderPtr cubeShader = bRenderer().getObjects()->loadShaderFile("cube", 0, false, true, true, false, false);				// load shader from file without lighting, the number of lights won't ever change during rendering (no variable number of lights)
    
    // create additional properties for a model
    PropertiesPtr cubeProperties = bRenderer().getObjects()->createProperties("cubeProperties");
    
    // load model
    bRenderer().getObjects()->loadObjModel("cube.obj", true, true, true, 4, true, false, cubeProperties);								// automatically generates a shader with a maximum of 4 lights (number of lights may vary between 0 and 4 during rendering without performance loss)
    
    /* Loading Hockeypuck Object */
    
    // load materials and shaders before loading the model
    ShaderPtr hockeypuckShader = bRenderer().getObjects()->loadShaderFile("hockeypuck", 0, false, true, true, false, false);				// load shader from file without lighting, the number of lights won't ever change during rendering (no variable number of lights)
    
    // create additional properties for a model
    PropertiesPtr hockeypuckProperties = bRenderer().getObjects()->createProperties("hockeypuckProperties");
    
    // load model
    bRenderer().getObjects()->loadObjModel("hockeypuck.obj", true, true, true, 4, true, false, hockeypuckProperties);								// automatically generates a shader with a maximum of 4 lights (number of lights may vary between 0 and 4 during rendering without performance loss)
    
    // create camera
    bRenderer().getObjects()->createCamera("camera", vmml::Vector3f(0.0f, 0.0f, 10.0f), vmml::Vector3f(0.f, 0.0f, 0.f));
    
    // Update render queue
    updateRenderQueue("camera", 0.0f);
    
}

/* Draw your scene here */
void RenderProject::loopFunction(const double &deltaTime, const double &elapsedTime)
{
    //	bRenderer::log("FPS: " + std::to_string(1 / deltaTime));	// write number of frames per second to the console every frame
    
    //// Draw Scene and do post processing ////
    
    /// Begin post processing ///
    //	GLint defaultFBO;
    //	if (!_running){
    //		bRenderer().getView()->setViewportSize(bRenderer().getView()->getWidth() / 5, bRenderer().getView()->getHeight() / 5);		// reduce viewport size
    //		defaultFBO = Framebuffer::getCurrentFramebuffer();	// get current fbo to bind it again after drawing the scene
    //		bRenderer().getObjects()->getFramebuffer("fbo")->bindTexture(bRenderer().getObjects()->getTexture("fbo_texture1"), false);	// bind the fbo
    //	}
    
    /// Draw scene ///
    
    bRenderer().getModelRenderer()->drawQueue(/*GL_LINES*/);
    bRenderer().getModelRenderer()->clearQueue();
    
    //// Camera Movement ////
    updateCamera("camera", deltaTime);
    
    /// Update render queue ///
    updateRenderQueue("camera", deltaTime);
    
    // Quit renderer when escape is pressed
    if (bRenderer().getInput()->getKeyState(bRenderer::KEY_ESCAPE) == bRenderer::INPUT_PRESS)
        bRenderer().terminateRenderer();
}

/* This function is executed when terminating the renderer */
void RenderProject::terminateFunction()
{
    bRenderer::log("I totally terminated this Renderer :-)");
}

/* Update render queue */
void RenderProject::updateRenderQueue(const std::string &camera, const double &deltaTime)
{
    // get input translation from touch
    TouchMap touchMap = bRenderer().getInput()->getTouches();
    int i = 0;
    for (auto t = touchMap.begin(); t != touchMap.end(); ++t)
    {
        Touch touch = t->second;
        translation = (touch.currentPositionY - touch.startPositionY)/50;
        if (++i > 1)
            break;
    }
    
    //translation positions of the objects
    translationHockeypuck_Xpos = 3.5f + constantMovement;
    translationHockeypuck_Zpos = 6.9f;
    
    translationCubePlayer_Xpos = 8.5f;
    translationCubePlayer_Ypos = 0.0f;
    if(translationCubePlayer_Ypos >= 0) translationCubePlayer_Ypos -= translation;
    else translationCubePlayer_Ypos += translation;
    translationCubePlayer_Zpos = 6.9f;
    
    translationCubeEnemy_Xpos = -4.8f;
    translationCubeEnemy_Ypos = 0.0f;
    translationCubeEnemy_Zpos = 6.9f;
    
    //calculate the collision of objects and movement of Sphere
    if (translationHockeypuck_Xpos > 8.5 or translationHockeypuck_Xpos < -4.8) {
        constantMovement = 0.1f;
        translationHockeypuck_Xpos = 0.0f;
        translationHockeypuck_Zpos = 6.9f;
    }
    
    if (translationHockeypuck_Xpos > translationCubePlayer_Xpos - 2.0f
        and (translationHockeypuck_Ypos > translationCubePlayer_Ypos - 1.0f
             and translationHockeypuck_Ypos < translationCubePlayer_Ypos + 1.0f)) {
        cube1Reached = true;
        cube2Reached = false;
        movementDirection = -1.0;
        constantMovement += 0.1f * movementDirection;
    }
    if (translationHockeypuck_Xpos < translationCubeEnemy_Xpos + 2.0f
        and (translationHockeypuck_Zpos > translationCubeEnemy_Zpos - 2.0f
             and translationHockeypuck_Zpos < translationCubeEnemy_Zpos + 2.0f)) {
        cube2Reached = true;
        cube1Reached = false;
        movementDirection = 1.0;
        constantMovement += 0.1f * movementDirection;

    }
    else {
        constantMovement += 0.1f * movementDirection;
    }
    
    /* Cube 1 */
    
    vmml::Matrix4f modelMatrixCube1 = vmml::create_scaling(vmml::Vector3f(0.7f)) * vmml::create_translation(vmml::Vector3f(translationCubePlayer_Xpos,translationCubePlayer_Ypos,translationCubePlayer_Zpos));
    
    ShaderPtr shaderCube1 = bRenderer().getObjects()->getShader("cube");
    shaderCube1->setUniform("NormalMatrix", vmml::Matrix3f(modelMatrixCube1));
    bRenderer().getModelRenderer()->drawModel("cube", "camera", modelMatrixCube1, std::vector<std::string>({ }));
    
    /* Cube 2 */
    
    vmml::Matrix4f modelMatrixCube2 = vmml::create_scaling(vmml::Vector3f(0.7f)) * vmml::create_translation(vmml::Vector3f(translationCubeEnemy_Xpos,translationCubeEnemy_Ypos,translationCubeEnemy_Zpos));
    
    ShaderPtr shaderCube2 = bRenderer().getObjects()->getShader("cube");
    shaderCube2->setUniform("NormalMatrix", vmml::Matrix3f(modelMatrixCube2));
    bRenderer().getModelRenderer()->drawModel("cube", "camera", modelMatrixCube2, std::vector<std::string>({ }));
    
    /* Moving Sphere */
    
//    vmml::Matrix4f modelMatrixSphere = vmml::create_scaling(vmml::Vector3f(0.7f)) * vmml::create_translation(vmml::Vector3f(translationHockeypuck_Xpos,translationHockeypuck_Ypos,translationHockeypuck_Zpos));
//    
//    ShaderPtr shaderSphere = bRenderer().getObjects()->getShader("sphere");
//    shaderSphere->setUniform("NormalMatrix", vmml::Matrix3f(modelMatrixSphere));
//    bRenderer().getModelRenderer()->drawModel("sphere", "camera", modelMatrixSphere, std::vector<std::string>({ }));
    
    /* Cube hockeypuck */
    
    vmml::Matrix4f modelMatrixHockeypuck = vmml::create_scaling(vmml::Vector3f(0.7f)) * vmml::create_translation(vmml::Vector3f(translationHockeypuck_Xpos,translationHockeypuck_Ypos,translationHockeypuck_Zpos));
    
    ShaderPtr shaderHockeypuck = bRenderer().getObjects()->getShader("hockeypuck");
    shaderHockeypuck->setUniform("NormalMatrix", vmml::Matrix3f(modelMatrixHockeypuck));
    shaderHockeypuck->setUniform("ModelMatrix", modelMatrixHockeypuck);
    //shaderHockeypuck->setUniform("color", vmml::Vector4f(1,1,0,1));
    bRenderer().getModelRenderer()->drawModel("hockeypuck", "camera", modelMatrixHockeypuck, std::vector<std::string>({ }));
}

/* Camera movement */
void RenderProject::updateCamera(const std::string &camera, const double &deltaTime)
{
    //// Adjust aspect ratio ////
    bRenderer().getObjects()->getCamera(camera)->setAspectRatio(bRenderer().getView()->getAspectRatio());
}

/* For iOS only: Handle device rotation */
void RenderProject::deviceRotated()
{
    if (bRenderer().isInitialized()){
        // set view to full screen after device rotation
        bRenderer().getView()->setFullscreen(true);
        bRenderer::log("Device rotated");
    }
}

/* For iOS only: Handle app going into background */
void RenderProject::appWillResignActive()
{
    if (bRenderer().isInitialized()){
        // stop the renderer when the app isn't active
        bRenderer().stopRenderer();
    }
}

/* For iOS only: Handle app coming back from background */
void RenderProject::appDidBecomeActive()
{
    if (bRenderer().isInitialized()){
        // run the renderer as soon as the app is active
        bRenderer().runRenderer();
    }
}

/* For iOS only: Handle app being terminated */
void RenderProject::appWillTerminate()
{
    if (bRenderer().isInitialized()){
        // terminate renderer before the app is closed
        bRenderer().terminateRenderer();
    }
}

/* Helper functions */
GLfloat RenderProject::randomNumber(GLfloat min, GLfloat max){
    return min + static_cast <GLfloat> (rand()) / (static_cast <GLfloat> (RAND_MAX / (max - min)));
}