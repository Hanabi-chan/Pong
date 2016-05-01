#include "RenderProject.h"
#include <math.h>

/* angle for the rotation of the planets */
float angle;
float constant = 0;
bool endOfScreenReached = false;

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
    ShaderPtr fieldShader = bRenderer().getObjects()->loadShaderFile("field", 0, false, true, true, false, false);
    // create additional properties for a model
    PropertiesPtr fieldProperties = bRenderer().getObjects()->createProperties("fieldProperties");
    
    // load model
    bRenderer().getObjects()->loadObjModel("field.obj", true, true, true, 4, true, false, fieldProperties);
    
    /* Loading stick */
    
    // load materials and shaders before loading the model
    ShaderPtr stickShader = bRenderer().getObjects()->loadShaderFile("stick", 0, false, true, true, false, false);
    // create additional properties for a model
    PropertiesPtr stickProperties = bRenderer().getObjects()->createProperties("stickProperties");
    
    // load model
    bRenderer().getObjects()->loadObjModel("stick.obj", true, true, true, 4, true, false, stickProperties);
    
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
    
    /* field */
    vmml::Matrix4f fieldModelMatrix = vmml::create_scaling(vmml::Vector3f(1.0f))
        //* vmml::create_translation(vmml::Vector3f(0,0,10.0f))
        * vmml::create_rotation(-0.5f, vmml::Vector3f(1,0,0))
    ;//* vmml::create_rotation(1.0f, vmml::Vector3f(0,1,0));
    
    ShaderPtr fieldShader = bRenderer().getObjects()->getShader("field");
    fieldShader->setUniform("NormalMatrix", vmml::Matrix3f(fieldModelMatrix));
    bRenderer().getModelRenderer()->drawModel("field", "camera", fieldModelMatrix, std::vector<std::string>({ }));
    
    angle += deltaTime;
    
    //calculate the constant for the movement. When the Object reaches the end of the Screen, then move it to the other side.
    if(constant > 0) {
        endOfScreenReached = false;
    }
    
    if(constant > -9.5f and endOfScreenReached == false){
        constant -= 0.1f;
    }
    else {
        endOfScreenReached = true;
    }
    if (endOfScreenReached == true and constant <= 0.1) {
        constant += 0.1f;
    }
    
    /* Moving Sphere */
//    vmml::Matrix4f modelMatrix = vmml::create_scaling(vmml::Vector3f(0.7f)) * vmml::create_translation(vmml::Vector3f(6.5f+constant,.5f,6.9f));
//    
//    ShaderPtr shader = bRenderer().getObjects()->getShader("sphere");
//    shader->setUniform("NormalMatrix", vmml::Matrix3f(modelMatrix));
//    bRenderer().getModelRenderer()->drawModel("sphere", "camera", modelMatrix, std::vector<std::string>({ }));
    
//    /* Cube 1 */
//    vmml::Matrix4f modelMatrixCube1 = vmml::create_scaling(vmml::Vector3f(10.0f))
//    ; //* vmml::create_translation(vmml::Vector3f(8.5f,.5f,6.9f));
//    
//    ShaderPtr shaderCube1 = bRenderer().getObjects()->getShader("cube");
//    shaderCube1->setUniform("NormalMatrix", vmml::Matrix3f(modelMatrixCube1));
////    shaderCube1->setUniform("color", vmml::Vector4f(0,1,0.7,1));
//    bRenderer().getModelRenderer()->drawModel("cube", "camera", modelMatrixCube1, std::vector<std::string>({ }));
//    
//    /* Cube 2 */
//    vmml::Matrix4f modelMatrixCube2 = vmml::create_scaling(vmml::Vector3f(0.7f))
//    ;//* vmml::create_translation(vmml::Vector3f(-4.8f,.5f,6.9f));
//    
//    ShaderPtr shaderCube2 = bRenderer().getObjects()->getShader("cube");
//    shaderCube2->setUniform("NormalMatrix", vmml::Matrix3f(modelMatrixCube2));
////    shaderCube2->setUniform("color", vmml::Vector4f(1,0,0.7,1));
//    bRenderer().getModelRenderer()->drawModel("cube", "camera", modelMatrixCube2, std::vector<std::string>({ }));
    
    // handle touch inputs
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
    
    /* Sticks */
    vmml::Vector3f xAxis = vmml::Vector3f(1,0,0);
    float scale = 0.001;
    float trans_x = 4.0, trans_z = 0.5;
    float rotation_x = -0.9;
    
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