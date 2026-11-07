#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <GL/glew.h>
#include <glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "Dependencies/stb/stb_image.h"
#include "Utility/Math.h"
#include "Shaders/Shader.h"
#include "Utility/Time.h"
#include "Utility/Physics/CollisionManager.h"
#include "Objects/Cube.h"
#include "Camera.h"
#include "Objects/Plane.h"
#include "Objects/Skybox.h"
#include "Renderer.h"
#include "Player.h"
#include "WorldManager.h"

static int windowWidth, windowHeight;
static bool wasResized = false;
static bool windowWasMoved = false;

void OnResize(GLFWwindow* window, int width, int height)
{
    glViewport(0,0,width, height);
    windowWidth = width;
    windowHeight = height;
    wasResized = true;
}

void OnFocus(GLFWwindow* window, int focused)
{
    if (focused)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}

void OnMove(GLFWwindow* window, int posX, int posY)
{
    windowWasMoved = true;
}

void CameraPosInput(Camera& cam, const double& deltaTime, GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        cam.Position() += cam.Forward() * deltaTime * 5.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cam.Position() -= cam.Forward() * deltaTime * 5.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        cam.Position() += cam.Right() * deltaTime * 5.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        cam.Position() -= cam.Right() * deltaTime * 5.0f;
    }
}

void CameraLookInput(Camera& cam, Math::double2& mousePos, GLFWwindow* window)
{
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    double x1 = x - mousePos.x;
    double y1 = y - mousePos.y;
    if (x1 == 0)
        return;
    x1 *= .5;
    y1 *= .5;

    cam.EulerAngles().y += x1;
    cam.EulerAngles().x += y1;
    Math::Clamp(cam.EulerAngles().x, -80, 80);
    cam.RotateEulerAngles();
    mousePos.x = x;
    mousePos.y = y;
}

int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetBreakAlloc(-1);
    _CrtDumpMemoryLeaks();


    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);//Anti-Aliasing

    GLFWwindow* window = glfwCreateWindow(1920, 1080, "Application", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }


    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        return -1;

    std::cout << glGetString(GL_VERSION) << std::endl;

    glfwSetWindowSizeCallback(window, OnResize);
    glfwSetWindowFocusCallback(window, OnFocus);
    glfwSetWindowPosCallback(window, OnMove);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    glClearDepth(1.0);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE); //MSAA
    glEnable(GL_CULL_FACE); // culls the backside of triangles
    glEnable(GL_FRAMEBUFFER_SRGB); // Gamma correction
    glfwSwapInterval(0); // Vysnc
    
    glClearColor(0, 0, 0, 1);


    Shader shader("Shaders/Basic.shader");
    Shader lightShader("Shaders/Light.shader");
    Shader instanceShader("Shaders/InstancedShader.shader");
    int width, height = 0;
    glfwGetWindowSize(window, &width, &height);
    windowWidth = width;
    windowHeight = height;


    Time time;
    long double frameTimer = 0;
    int frameCount = 0;

    Plane ground(shader);
    ground.SetPosition({ -2,-1, 0});
    ground.GetTransform().Scale(50,1,50);
    //ground.Material().SpecularStrength(10);

    // Instanced cube
    Cube instancedCube(instanceShader);
    instancedCube.SetPosition({ 0.0f,0.0f, 0.0f });
    instancedCube.Color({ .2f, 0.7f, .2f, 1.0f });

    // Creating the model array for instancing
    /*std::vector<Math::mat4f> modelVector;
    std::vector<PhysicsObject> colliderVector;
    float instanceOffsetX = -1.0f, instanceOffsetZ = 1.0f;
    for (size_t z = 0; z < 100; z++)
    {
        for (size_t x = 0; x < 100; x++)
        {
            Transform objectTransform;
            objectTransform.Position({ (float)x + instanceOffsetX, 0.0f, -(float)z + instanceOffsetZ });
            objectTransform.Scale(2.0f, 2.0f, 2.0f);

            modelVector.push_back(objectTransform.GetMatrix().Transposed());

            PhysicsObject objectCollider(&objectTransform);
            objectCollider.GetCollider().SetCenterPos(Math::vector3(instancedCube.GetCenterPosition().x, instancedCube.GetCenterPosition().y, instancedCube.GetCenterPosition().z) + Math::vector3(-objectTransform.Position().x, objectTransform.Position().y, -objectTransform.Position().z));
            colliderVector.push_back(objectCollider);
        }
    }*/

    // Setting the instance model array
    //instancedCube.SetupInstancing(&modelVector[0], modelVector.size() * sizeof(Math::mat4f), modelVector.size());

    glfwSetCursorPos(window, 0, 0);

    //Disabling the cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if(glfwRawMouseMotionSupported())
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    Renderer renderer;
    renderer.SetWindowSize(windowWidth, windowHeight);


    Cube lightSource(lightShader);
    lightSource.SetPosition({ -1.0f,2.0f, -20.0f });
    lightSource.Color({1,1,1,1});
    lightSource.GetTransform().Scale(2.0f, 2.0f, 2.0f);

    Math::vector3 lightPos(lightSource.GetCenterPosition());
    renderer.SetLightPos(lightPos);
    renderer.SetLightColor(lightSource.Color());


    //=========================================================================================================================


    // Creating the player
    Camera cam;
    Player player(&cam, window);
    player.Transform().Position(Math::vector3(1.0f, 3.0f, 1.0f));

    player.SetCameraPosition({0.0f, -2.5f, 0.0f});

    //===== Skybox Loading ====================================================================================================

    Shader skyboxShader("Shaders/Skybox.shader");
    Skybox skybox(skyboxShader);

    //===== Collisions ========================================================================================================

    CollisionManager collisionManager;
    collisionManager.AddDynamic(&player);
    collisionManager.AddDynamic(player.GetgGroundCollider());

    // For testing
    Cube testColCube(instanceShader);
    testColCube.SetPosition({ 0.0f, 1.0f, 0.0f });
    Math::mat4f testColCubeMat = testColCube.GetModelMatrix().Transposed();
    testColCube.SetupInstancing(&testColCubeMat, sizeof(Math::mat4f), 1);

    collisionManager.AddStatic(&testColCube);
    // Adding static instanced cube colliders
    /*for (int i = 0; i < colliderVector.size(); i++)
    {
        collisionManager.AddStatic(&colliderVector[i]);
    }*/

    // Collision thread
    std::thread collisionThread;

    //===== World Management ========================================================================================================

    WorldManager worldManager(instanceShader, player.Transform());

    worldManager.LoadLevel();

    // Pausing
    bool isPaused = false;
    bool pauseWasPressed = false;

    //===== Update Loop =======================================================================================================
    while (!glfwWindowShouldClose(window))
    {
        time.Start();
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        frameTimer += time.DeltaTime();
        if (frameTimer >= 1.0)
        {
            std::cout << frameCount << std::endl;
            frameCount = 0;
            frameTimer = 0;
        }

        if (wasResized)
        {
            renderer.SetWindowSize(windowWidth, windowHeight);
            wasResized = false;
        }

        // Collision callbacks
        if (collisionThread.joinable())
        {
            // Always join the thread before executing collision callbacks
            collisionThread.join();
            collisionManager.HandleCollisionCallbacks();
        }


        // Old rotation code
        //float thetaX = 00.0f;
        //float thetaY = 00.0f;
        //float thetaZ = 00.0f;
        //Math::quaternion rotX(cosf(thetaX / 2), sinf(thetaX / 2), 0, 0);
        //Math::quaternion rotY(cosf(thetaY / 2), 0, sinf(thetaY / 2), 0);
        //Math::quaternion rotZ(cosf(thetaZ / 2), 0, 0, sinf(thetaZ / 2));
        //cube.GetTransform().Rotation() = cube.GetTransform().Rotation() * rotX * rotY * rotZ;

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);// sets cursor position to 0,0 and reveals the cursor
        }

        if (!pauseWasPressed && glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        {
            pauseWasPressed = true;
            isPaused = !isPaused;
        }
        else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE)
        {
            pauseWasPressed = false;
        }
        
        // Checking if window was moved
        if (windowWasMoved)
        {
            // Moving causes delta time to become a huge number
            // Setting delta time to zero on window move to avoid bugs caused by it
            time.SetDeltaTime(0);
            windowWasMoved = false;
        }


        //===== Update calls =========================================================================================
        if (isPaused)
            time.SetDeltaTime(0);

        player.Update(time.DeltaTime());

        worldManager.Update();
        
        collisionManager.SetCollisionChunks(worldManager.GetCollisionChunks());
        collisionThread = std::move(std::thread(CollisionManager::ThreadEntrypoint, &collisionManager));



        //===== Render calls =========================================================================================

        // Old non instanced rendering
        //for (size_t i = 0; i < objects.size(); i++)
        //{
        //    // Currently only drawing the plane
        //    renderer.RenderObject(*objects[i]);
        //}

        //renderer.RenderObject(*objects[2]);

        renderer.SetViewMatrix(Math::RotationMatrix(cam.Rotation()) * Math::TranslationMatrix(cam.Position()));

        renderer.RenderObject(lightSource);
        // New instanced rendering
        //renderer.RenderObjectInstanced(instancedCube);
        renderer.RenderObjectInstanced(testColCube);

        for (auto itr = worldManager.GetRenderChunks().begin(); itr != worldManager.GetRenderChunks().end(); itr++)
        {
            renderer.RenderObjectInstanced(itr->second->cube);
        }
        

        glDepthMask(GL_FALSE);
        renderer.RenderSkybox(skybox, skybox.textureId);
        glDepthMask(GL_TRUE);

        frameCount += 1;
        glfwSwapBuffers(window);
        time.End();
    }

    if (collisionThread.joinable())
    {
        collisionThread.join();
    }

    glfwTerminate();
}