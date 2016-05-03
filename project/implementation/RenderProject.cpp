#include "RenderProject.h"
#include <math.h>
#include <string>
#include<stdio.h>

float translation = 0.0f;
int scoreId = 0;

float constantMovement = 0;
float movementDirection = -1.0;

float translationHockeypuck_Xpos = 0.0;
float translationHockeypuck_Ypos = 0.0;
float translationHockeypuck_Zpos = 0.0;

bool scoredPuck = false;
bool endOfGame = false;
bool showEndgameText = false;

std::string winner = "";

int scoreLeftPlayer = 0;
int scoreRightPlayer = 0;

static GLint VIEW_WIDTH = View::getScreenWidth();
static GLint VIEW_HEIGHT = View::getScreenHeight();

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
    
    /* Loading field */
    // load materials and shaders before loading the model
    // load shader from file without lighting, the number of lights won't ever change during rendering (no variable number of lights)
    ShaderPtr fieldShader = bRenderer().getObjects()->loadShaderFile("field", 0, false, true, true, false, false);
    // create additional properties for a model
    PropertiesPtr fieldProperties = bRenderer().getObjects()->createProperties("fieldProperties");
    // load model
    bRenderer().getObjects()->loadObjModel("field.obj", true, true, true, 4, true, false, fieldProperties);
    
    /* Loading stick */
    ShaderPtr stickShader = bRenderer().getObjects()->loadShaderFile("stick", 0, false, true, true, false, false);
    PropertiesPtr stickProperties = bRenderer().getObjects()->createProperties("stickProperties");
    bRenderer().getObjects()->loadObjModel("stick.obj", true, true, true, 4, true, false, stickProperties);
    
    /* Loading Hockeypuck Object */
    ShaderPtr hockeypuckShader = bRenderer().getObjects()->loadShaderFile("hockeypuck", 0, false, true, true, false, false);
    PropertiesPtr hockeypuckProperties = bRenderer().getObjects()->createProperties("hockeypuckProperties");
    bRenderer().getObjects()->loadObjModel("hockeypuck.obj", true, true, true, 4, true, false, hockeypuckProperties);
    
    // create camera
    bRenderer().getObjects()->createCamera("camera", vmml::Vector3f(0.0f, 0.0f, 10.0f), vmml::Vector3f(0.f, 0.0f, 0.f));
    
    //font
    FontPtr font = bRenderer().getObjects()->loadFont("DJB Up on the Scoreboard.ttf", 50);
    bRenderer().getObjects()->createTextSprite("score", vmml::Vector3f(1.f, 1.f, 1.f), std::to_string(scoreLeftPlayer) + " : " + std::to_string(scoreRightPlayer), font);
    
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

/* 
 * TODO: get field coordinates.
 * 320 is where the field "starts", seen from the upper right corner.
 * 500 where it ends.
 * -5 and 5 for the y translation are only estimated values.
 */
GLfloat fieldHeight = 180,
            fieldHeightStart = 320,
            fieldHeightEnd = 500,
            yTransMin = -3.0f,
            yTransMax = -yTransMin,
            yTrans = fabsf(yTransMin) + yTransMax; // because of 0

GLfloat move_1 = 0.0,
        move_2 = 0.0;

GLfloat computeStickPosition(GLfloat yPosition){
    std::cout << "Position: " << yPosition << "\n";
    if(yPosition > fieldHeightEnd) return yTransMin;
    else if(yPosition < fieldHeightStart) return yTransMax;
    else {
        return -(yTrans/fieldHeight * (yPosition - fieldHeightStart) - yTransMax);
    }
}

/* Update render queue */
void RenderProject::updateRenderQueue(const std::string &camera, const double &deltaTime)
{
    
    /* Sticks Variables */
    vmml::Vector3f xAxis = vmml::Vector3f(1,0,0);
    float scale = 0.001;
    float trans_x = 4.0, trans_z = 0.5;
    float rotation_x = -0.9;
    
    /* handle touch inputs */
    TouchMap touches = bRenderer().getInput()->getTouches();
    for(TouchMap::iterator it = touches.begin(); it != touches.end(); it++){
        Touch touch = it->second;
        // Apply movement to the correct screen side
        if(touch.startPositionX < VIEW_WIDTH / 2){
            move_2 = computeStickPosition(touch.lastPositionY);
        } else {
            move_1 = computeStickPosition(touch.lastPositionY);
            std::cout << "Move 1 :" << move_1 << "\n";
        }
    }
    
    //translation positions of hockeypuck
    translationHockeypuck_Xpos = 0.0f + constantMovement;
    translationHockeypuck_Zpos = 0.3f;
    
    /* Pauses in the Game */
    
    //pause 2 seconds after a scored point when the puck is in the middle of the field again
    if (scoredPuck) {
        sleep(2);
        scoredPuck = false;
        movementDirection *= -1;
        // reset sticks
        move_1 = 0.0f;
        move_2 = 0.0f;
        if (scoreLeftPlayer == 5) {
            endOfGame = true;
            winner = "Left Player";
        }
        if (scoreRightPlayer == 5) {
            endOfGame = true;
            winner = "Right Player";
        }
    }
    
    //pause 5 seconds when showing the winning text
    if (showEndgameText) {
        sleep(5);
        scoreLeftPlayer = 0;
        scoreRightPlayer = 0;
        endOfGame = false;
        showEndgameText = false;
    }
    
    /* Movement of Puck */
    
    //reset hockeypuck after reaching the goal
    if (translationHockeypuck_Xpos > trans_x + 0.5f) {
        constantMovement = 0.1f;
        translationHockeypuck_Xpos = 0.0f + constantMovement;
        scoredPuck = true;
        scoreLeftPlayer += 1;
        scoreId += 1;
    }
    
    if (translationHockeypuck_Xpos < -trans_x - 0.5f) {
        constantMovement = 0.1f;
        translationHockeypuck_Xpos = 0.0f + constantMovement;
        scoredPuck = true;
        scoreRightPlayer += 1;
        scoreId += 1;
    }
    
    /* Collison of Puck */
    
    //collision with right stick
    if (translationHockeypuck_Xpos > trans_x
        and translationHockeypuck_Zpos < move_1 + .5f
        and translationHockeypuck_Zpos > move_1 - .3f) {
        movementDirection = -1.0;
        constantMovement += 0.1f * movementDirection;
    }
    
    //collision with left stick
    if (translationHockeypuck_Xpos < -trans_x
        and translationHockeypuck_Zpos < move_2 + .5f
        and translationHockeypuck_Zpos > move_2 - .3f) {
        movementDirection = 1.0;
        constantMovement += 0.1f * movementDirection;
    }
    
    //movement of hockeypuck without collision
    else {
        constantMovement += 0.1f * movementDirection;
    }
    
    /* Score */
    if (endOfGame == false) {
        FontPtr font = bRenderer().getObjects()->loadFont("DJB Up on the Scoreboard.ttf", 50);
        bRenderer().getObjects()->createTextSprite("score"+std::to_string(scoreId), vmml::Vector3f(1.f, 1.f, 1.f), std::to_string(scoreLeftPlayer) + " : " + std::to_string(scoreRightPlayer), font);
        
        GLfloat scaleText = 0.1f;
        vmml::Matrix4f scalingMatrixText = vmml::create_scaling(vmml::Vector3f(scaleText / bRenderer().getView()->getAspectRatio(), scaleText, scaleText));
        
        vmml::Matrix4f modelMatrixText = vmml::create_translation(vmml::Vector3f(-0.1f, 0.7f, 0.f)) * scalingMatrixText;
        
        vmml::Matrix4f viewMatrixText = Camera::lookAt(vmml::Vector3f(0.0f, 0.0f, 0.25f), vmml::Vector3f::ZERO, vmml::Vector3f::UP);
        
        vmml::Matrix4f projectionMatrixText = vmml::Matrix4f::IDENTITY;
        
        ModelPtr score = bRenderer().getObjects()->getTextSprite("score"+std::to_string(scoreId));
        // draw without lighting (empty vector of strings)
        bRenderer().getModelRenderer()->drawModel(score, modelMatrixText, viewMatrixText, projectionMatrixText, std::vector<std::string>({}));
    }
    else {
        scoreLeftPlayer = 0;
        scoreRightPlayer = 0;
        
        /* winning message */
        FontPtr fontWinner = bRenderer().getObjects()->loadFont("orange juice 2.0.ttf", 50);
        bRenderer().getObjects()->createTextSprite("winner"+std::to_string(scoreId), vmml::Vector3f(1.f, 1.f, 1.f), "Winner: "+winner+"\nRestart in 5 seconds!", fontWinner);
        
        GLfloat scaleTextWinner = 0.1f;
        vmml::Matrix4f scalingMatrixWinner = vmml::create_scaling(vmml::Vector3f(scaleTextWinner / bRenderer().getView()->getAspectRatio(), scaleTextWinner, scaleTextWinner));
        
        vmml::Matrix4f modelMatrixTextWinner = vmml::create_translation(vmml::Vector3f(-0.3f, 0.7f, 0.f)) * scalingMatrixWinner;
        
        vmml::Matrix4f viewMatrixWinner = Camera::lookAt(vmml::Vector3f(0.0f, 0.0f, 0.25f), vmml::Vector3f::ZERO, vmml::Vector3f::UP);
        
        vmml::Matrix4f projectionMatrixWinner = vmml::Matrix4f::IDENTITY;
        
        ModelPtr scoreWinner = bRenderer().getObjects()->getTextSprite("winner"+std::to_string(scoreId));
        // draw without lighting (empty vector of strings)
        bRenderer().getModelRenderer()->drawModel(scoreWinner, modelMatrixTextWinner, viewMatrixWinner, projectionMatrixWinner, std::vector<std::string>({}));
        
        showEndgameText = true;
        scoreId += 1;
    }
    
    /* field */
    vmml::Matrix4f fieldModelMatrix = vmml::create_scaling(vmml::Vector3f(1.0f))
        * vmml::create_rotation(-0.5f, vmml::Vector3f(1,0,0));
    
    ShaderPtr fieldShader = bRenderer().getObjects()->getShader("field");
    fieldShader->setUniform("NormalMatrix", vmml::Matrix3f(fieldModelMatrix));
    bRenderer().getModelRenderer()->drawModel("field", "camera", fieldModelMatrix, std::vector<std::string>({ }));
    
    /* Stick 1 */
    vmml::Matrix4f stickModelMatrix = fieldModelMatrix
        * vmml::create_translation(vmml::Vector3f(trans_x, trans_z, move_1))
        * vmml::create_scaling(vmml::Vector3f(scale))
        * vmml::create_rotation(rotation_x, xAxis);
   
    ShaderPtr stickShader = bRenderer().getObjects()->getShader("stick");
    stickShader->setUniform("NormalMatrix", vmml::Matrix3f(stickModelMatrix));
    bRenderer().getModelRenderer()->drawModel("stick", "camera", stickModelMatrix, std::vector<std::string>({ }));
    
    /* Stick2 */
    vmml::Matrix4f stick2ModelMatrix = fieldModelMatrix
        * vmml::create_translation(vmml::Vector3f(-trans_x, trans_z, move_2))
        * vmml::create_scaling(vmml::Vector3f(scale))
        * vmml::create_rotation(rotation_x, xAxis);
    
    ShaderPtr stick2Shader = bRenderer().getObjects()->getShader("stick");
    stick2Shader->setUniform("NormalMatrix", vmml::Matrix3f(stick2ModelMatrix));
    bRenderer().getModelRenderer()->drawModel("stick", "camera", stick2ModelMatrix, std::vector<std::string>({ }));
    
    /* Hockeypuck */
    vmml::Matrix4f modelMatrixHockeypuck = vmml::create_translation(vmml::Vector3f(vmml::Vector3f(translationHockeypuck_Xpos, 0.0f, translationHockeypuck_Zpos))) * vmml::create_scaling(vmml::Vector3f(0.2f)) * vmml::create_rotation(rotation_x + 0.25f, xAxis);
    
    vmml::Matrix4f viewMatrixHockeypuck = bRenderer().getObjects()->getCamera("camera")->getViewMatrix();

    ShaderPtr shaderHockeypuck = bRenderer().getObjects()->getShader("hockeypuck");
    if (shaderHockeypuck.get())
    {
        shaderHockeypuck->setUniform("ProjectionMatrix", vmml::Matrix4f::IDENTITY);
        shaderHockeypuck->setUniform("ViewMatrix", viewMatrixHockeypuck);
        shaderHockeypuck->setUniform("ModelMatrix", modelMatrixHockeypuck);
        
        vmml::Matrix3f normalMatrixHockeypuck;
        vmml::compute_inverse(vmml::transpose(vmml::Matrix3f(modelMatrixHockeypuck)), normalMatrixHockeypuck);
        shaderHockeypuck->setUniform("NormalMatrix", normalMatrixHockeypuck);
        
        vmml::Vector4f eyePos = vmml::Vector4f(0.0f, 0.0f, 10.0f, 1.0f);
        shaderHockeypuck->setUniform("EyePos", eyePos);
        
        shaderHockeypuck->setUniform("LightPos", vmml::Vector4f(0.f, 1.f, .5f, 1.f));
        shaderHockeypuck->setUniform("Ia", vmml::Vector3f(1.f));
        shaderHockeypuck->setUniform("Id", vmml::Vector3f(1.f));
        shaderHockeypuck->setUniform("Is", vmml::Vector3f(1.f));
    }
    else
    {
        bRenderer::log("No shader available.");
    }
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