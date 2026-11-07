#pragma once
#include <glfw3.h>
#include "Camera.h"
#include "Objects/Transform.h"
#include "Objects/PhysicsObject.h"
#include "Utility/Physics/Collider.h"

class Player : public PhysicsObject
{
private:

	GLFWwindow* window;
	Camera* playerCam;
	Transform playerTransform;
	PhysicsObject groundCollider;

	Math::vector3 camPosition;
	Math::double2 prevMousePos;
	Math::double2 mouseSens;

	Math::vector3 PlayerCenterPos;
	Math::vector3 playerVelocity;

	float walkSpeed;
	float sprintSpeed;
	float moveSpeed;

	double deltaTime;

	bool isGrounded = false;
	bool pressedSpace = false;
	PhysicsObject* groundObject;

	void UpdateLook();
	void UpdatePosition();
	void UpdateGravity();

public:
	Player(Camera* _cam, GLFWwindow* _window);
	~Player();


	void OnCollisionEnter(Math::vector3 mtv, PhysicsObject* other) override;

	void OnCollision(Math::vector3 mtv, PhysicsObject* other) override;

	void OnCollisionExit(PhysicsObject* other) override;

	void UpdateCollider();

	void Update(const double& _deltaTime);

	Transform& Transform();

	void SetCameraPosition(const Math::vector3& pos);

	PhysicsObject* GetgGroundCollider()
	{
		return &groundCollider;
	}
};

