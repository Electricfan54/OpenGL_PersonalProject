#include "Player.h"

Player::Player(Camera* _cam, GLFWwindow* _window)
    : playerCam(_cam), window(_window), deltaTime(0), prevMousePos({ 0,0 }), PlayerCenterPos(-0.125f, 1.25f, -0.125f),
    mouseSens({ .5, .5 }), walkSpeed(5.0f), sprintSpeed(10.0f), moveSpeed(0), groundObject(nullptr)
{
    moveSpeed = walkSpeed;

    collider.SetTransform(&playerTransform);
    collider.SetExtents(Math::vector3(0.5f, 1.25, .5f));
    collider.SetCenterPos(playerTransform.Position() + PlayerCenterPos);

    groundCollider.GetCollider().SetTransform(&playerTransform);
    groundCollider.GetCollider().SetExtents(Math::vector3(0.4f, .01f, 0.4f));
    groundCollider.GetCollider().SetCenterPos(Math::vector3(PlayerCenterPos.x, 0, PlayerCenterPos.z));
    groundCollider.SetReceiveCollisionEvents(true);
    groundCollider.BindOnCollisionEnter(this);
    groundCollider.BindOnCollision(this);
    groundCollider.BindOnCollisionExit(this);


    receiveCollisionEvents = true;
}

Player::~Player()
{
}

void Player::UpdateLook()
{
    double x, y;
    glfwGetCursorPos(window, &x, &y);

    double x1 = x - prevMousePos.x;
    double y1 = y - prevMousePos.y;

    x1 *= mouseSens.x;
    y1 *= mouseSens.y;

    playerTransform.RotateEuler(0, x1, 0);
    //playerCam->EulerAngles().y += x1;
    playerCam->EulerAngles().x += y1;
    Math::Clamp(playerCam->EulerAngles().x, -80, 80);

    playerCam->RotateEulerAngles();

    playerCam->Rotation(playerCam->Rotation() * playerTransform.Rotation());

    prevMousePos.x = x;
    prevMousePos.y = y;
}

void Player::UpdatePosition()
{
    playerVelocity.x = 0;
    playerVelocity.z = 0;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        playerVelocity += playerCam->Forward() * walkSpeed;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        playerVelocity -= playerCam->Forward() * walkSpeed;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        playerVelocity += playerCam->Right() * walkSpeed;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        playerVelocity -= playerCam->Right() * walkSpeed;
    }

    UpdateGravity();

    playerTransform.Position() += playerVelocity * deltaTime;
    playerCam->Position() = Math::vector3(playerTransform.Position().x + camPosition.x, -playerTransform.Position().y + camPosition.y, playerTransform.Position().z + camPosition.z);

    UpdateCollider();
}

void Player::UpdateGravity()
{
    // Checking for jump input
    bool input = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
    if (!isGrounded)
        playerVelocity.y -= 5.0f * deltaTime;
    else if (!pressedSpace && input && isGrounded)
    {
        playerVelocity.y += 5.0f;
        isGrounded = false;
        // For giving the space bar on press functionalty
        pressedSpace = true;
    }
    else if(!input)
    {
        if (pressedSpace == true)
            pressedSpace = false;
        playerVelocity.y = 0;
    }
}

void Player::OnCollisionEnter(Math::vector3 mtv, PhysicsObject* other)
{
    if (other == nullptr)
    {
        isGrounded = true;
        return;
    }

    PhysicsObject::OnCollisionEnter(mtv, other);
    playerTransform.Position() += mtv;
    UpdateCollider();

    // Checking if colliding with an object below the player
    /*if (mtv.x == 0 && mtv.z == 0 && mtv.y > 0)
    {
        if (!isGrounded)
            //isGrounded = true;
        // Not a vector which may cause bugs
        // Setting the ground object for checking if the player is still grounded in the OnCollisionExit code
        groundObject = other;
    }*/
}

void Player::OnCollision(Math::vector3 mtv, PhysicsObject* other)
{
    if (other == nullptr)
    {
        isGrounded = true;
        return;
    }

    PhysicsObject::OnCollision(mtv, other);
    playerTransform.Position() += mtv;
    UpdateCollider();

    // Checking if colliding with an object below the player
     /*if (mtv.x == 0 && mtv.z == 0 && mtv.y > 0)
     {
         if (!isGrounded)
            isGrounded = true;
         // Not a vector which may cause bugs
         // Setting the ground object for checking if the player is still grounded in the OnCollisionExit code
         groundObject = other;
     }*/
}

void Player::OnCollisionExit(PhysicsObject* other)
{
    if (other == nullptr)
    {
        isGrounded = false;
        return;
    }

    PhysicsObject::OnCollisionExit(other);
    /*if (collidingObjects.size() == 0)
        isGrounded = false;
    else if (groundObject != nullptr && groundObject == other)
    {
        isGrounded = false;
        groundObject = nullptr;
    }*/
}

void Player::UpdateCollider()
{
    collider.SetCenterPos(playerTransform.Position() + PlayerCenterPos);
    //collider.UpdatePosition(playerTransform);
    collider.UpdateCollider(playerTransform);

    groundCollider.GetCollider().SetCenterPos(playerTransform.Position());
    groundCollider.GetCollider().UpdateCollider(playerTransform);
}

void Player::Update(const double& _deltaTime)
{
    deltaTime = _deltaTime;
    UpdateLook();
    UpdatePosition();
}

Transform& Player::Transform()
{
    return playerTransform;
}

void Player::SetCameraPosition(const Math::vector3& pos)
{
    camPosition = pos;
}
