#include "RenderProject.h"
#include <math.h>
#include <string>
#include<stdio.h>
#include <list>

static const GLint VIEW_WIDTH = View::getScreenWidth();
static const GLint VIEW_HEIGHT = View::getScreenHeight();

const int targetScore = 5;
bool waitForRestart = false;
int scoreId = std::numeric_limits<int>::min();
int lastScoreLeft, lastScoreRight;

static std::list<ObjectModel*> models;

Field field(10, 5, 1);
Stick stick1(&field, 200, 1000, 0.001), stick2(stick1, true);
Player player1(&stick1), player2(&stick2);
Puck puck(&field, &player1, &player2, 2.25, 2.25, 0.15);

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
    ShaderPtr fieldShader = bRenderer().getObjects()->loadShaderFile(field.MODEL_NAME, 0, false, true, true, false, false);
    // create additional properties for a model
    PropertiesPtr fieldProperties = bRenderer().getObjects()->createProperties("fieldProperties");
    // load model
    bRenderer().getObjects()->loadObjModel("field.obj", true, true, true, 4, true, false, fieldProperties);
    
    /* Loading stick */
    ShaderPtr stickShader = bRenderer().getObjects()->loadShaderFile(stick1.MODEL_NAME, 0, false, true, true, false, false);
    PropertiesPtr stickProperties = bRenderer().getObjects()->createProperties("stickProperties");
    bRenderer().getObjects()->loadObjModel("stick.obj", true, true, true, 4, true, false, stickProperties);
    
    /* Loading Hockeypuck Object */
    ShaderPtr hockeypuckShader = bRenderer().getObjects()->loadShaderFile(puck.MODEL_NAME, 0, false, true, true, false, false);
    PropertiesPtr hockeypuckProperties = bRenderer().getObjects()->createProperties("hockeypuckProperties");
    bRenderer().getObjects()->loadObjModel("hockeypuck.obj", true, true, true, 4, true, false, hockeypuckProperties);
    
    // create camera
    bRenderer().getObjects()->createCamera(ObjectModel::CAMERA_NAME, vmml::Vector3f(0.0f, 0.0f, 10.0f), vmml::Vector3f(0.f, 0.0f, 0.f));
    
    //font
    FontPtr font = bRenderer().getObjects()->loadFont("DJB Up on the Scoreboard.ttf", 50);
    bRenderer().getObjects()->createTextSprite("score", vmml::Vector3f(1.f, 1.f, 1.f), std::to_string(0) + " : " + std::to_string(0), font);
    
    // fill object model list
    models.push_back(&field);
    models.push_back(&puck);
    models.push_back(&stick1);
    models.push_back(&stick2);
    
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
    // wait 5 seconds before game restarts
    if(waitForRestart){
        sleep(5);
        waitForRestart = false;
    }

    if (player1.getScore() == targetScore || player2.getScore() == targetScore) {
        
        /* winning message */
        FontPtr fontWinner = bRenderer().getObjects()->loadFont("orange juice 2.0.ttf", 50);
        std::string winner(player2.getScore() == targetScore ? "Left Player" : "Right Player");
        bRenderer().getObjects()->createTextSprite("winner"+std::to_string(++scoreId), vmml::Vector3f(1.f, 1.f, 1.f), "Winner: " + winner + "\nRestart in 5 seconds!", fontWinner);
        
        player1.resetScore();
        player2.resetScore();
        
        GLfloat scaleTextWinner = 0.1f;
        vmml::Matrix4f scalingMatrixWinner = vmml::create_scaling(vmml::Vector3f(scaleTextWinner / bRenderer().getView()->getAspectRatio(), scaleTextWinner, scaleTextWinner));
        vmml::Matrix4f modelMatrixTextWinner = vmml::create_translation(vmml::Vector3f(-0.3f, 0.7f, 0.f)) * scalingMatrixWinner;
        vmml::Matrix4f viewMatrixWinner = Camera::lookAt(vmml::Vector3f(0.0f, 0.0f, 0.25f), vmml::Vector3f::ZERO, vmml::Vector3f::UP);
        vmml::Matrix4f projectionMatrixWinner = vmml::Matrix4f::IDENTITY;
        
        ModelPtr scoreWinner = bRenderer().getObjects()->getTextSprite("winner"+std::to_string(scoreId));
        // draw without lighting (empty vector of strings)
        bRenderer().getModelRenderer()->drawModel(scoreWinner, modelMatrixTextWinner, viewMatrixWinner, projectionMatrixWinner, std::vector<std::string>({}));
        waitForRestart = true;
    }
    else {
        FontPtr font = bRenderer().getObjects()->loadFont("DJB Up on the Scoreboard.ttf", 50);
        // only create new sprite, if score has changed
        if(player1.getScore() != lastScoreRight || player2.getScore() != lastScoreLeft || scoreId == std::numeric_limits<int>::min()) {
            lastScoreRight = player1.getScore();
            lastScoreLeft = player2.getScore();
            bRenderer().getObjects()->createTextSprite("score"+std::to_string(++scoreId), vmml::Vector3f(1.f, 1.f, 1.f), std::to_string(player2.getScore()) + " : " + std::to_string(player1.getScore()), font);
        }
        GLfloat scaleText = 0.1f;
        vmml::Matrix4f scalingMatrix = vmml::create_scaling(vmml::Vector3f(scaleText / bRenderer().getView()->getAspectRatio(), scaleText, scaleText));
        vmml::Matrix4f modelMatrixText = vmml::create_translation(vmml::Vector3f(-0.1f, 0.7f, 0.f)) * scalingMatrix;
        vmml::Matrix4f viewMatrix = Camera::lookAt(vmml::Vector3f(0.0f, 0.0f, 0.25f), vmml::Vector3f::ZERO, vmml::Vector3f::UP);
        vmml::Matrix4f projectionMatrix = vmml::Matrix4f::IDENTITY;

        ModelPtr score = bRenderer().getObjects()->getTextSprite("score"+std::to_string(scoreId));
        // draw without lighting (empty vector of strings)
        bRenderer().getModelRenderer()->drawModel(score, modelMatrixText, viewMatrix, projectionMatrix, std::vector<std::string>({}));
    }

    for(std::list<ObjectModel*>::const_iterator it = models.begin(); it != models.end(); ++it){
        (*it)->drawModel(bRenderer());
    }
    
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