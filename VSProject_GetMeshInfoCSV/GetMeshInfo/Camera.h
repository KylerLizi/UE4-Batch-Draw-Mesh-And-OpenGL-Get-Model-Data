#ifndef CAMERA_H
#define CAMERA_H

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "vector"

enum CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

//default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 15.0f;
const float SENSITIVITY = 0.1f;
const float FOV = 45.0f;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera{
	public:
		//Camera Attributes
		glm::vec3 Position;
		glm::vec3 Front;
		glm::vec3 Up;
		glm::vec3 Right;
		glm::vec3 WorldUp;
		//Euler Angles
		float Yaw;
		float Pitch;
		//Camera options
		float MovementSpeed;
		float MouseSensitivity;
		float Fov;

		//Constructor with vectors
		Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH)
			: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Fov(FOV) 
		{
			Position = position;
			WorldUp = up;
			Up = up;
			Yaw = yaw;
			Pitch = pitch;
			UpdateCameraVectors();
		}
		//Constructor with scalar values
		Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
			:Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Fov(FOV)
		{
			Position = glm::vec3(posX, posY, posZ);
			WorldUp = glm::vec3(upX, upY, upZ);
			Yaw = yaw;
			Pitch = pitch;
			UpdateCameraVectors();
		}

		//Returns the view matrix calculated using Euler Angles and the LookAt Matrix
		glm::mat4 GetViewMatrix()
		{
			return glm::lookAt(Position, Position + Front, Up);
		}

		//Processes input received from any keyboard-like input system.Accepts input parameter in the form of camera defined ENUM
		void ProcessKeyboard(CameraMovement direction, float deltaTime)
		{
			float velocity = MovementSpeed * deltaTime;
			if (direction == FORWARD)
				Position += Front * velocity;
			if (direction == BACKWARD)
				Position -= Front * velocity;
			if (direction == RIGHT)
				Position += cross(Front,Up) * velocity;
			if (direction == LEFT)
				Position -= cross(Front, Up)* velocity;
			if (direction == UP)
				Position += Up * velocity;
			if (direction == DOWN)
				Position -= Up * velocity;
		}

		//Process input received from a mouse input system.Excepts the offset value in both the x and y direction.
		void ProcessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true)
		{
			xOffset *= MouseSensitivity;
			yOffset *= MouseSensitivity;

			Yaw += xOffset;
			Pitch += yOffset;

			//Make sure that when pitch is out of bounds, screen doesn't get flipped
			if (constrainPitch)
			{
				if (Pitch > 89.0f)
					Pitch = 89.0f;
				if (Pitch < -89.0f)
					Pitch = -89.0f;
			}

			//Update Front,Right and Up Vectors using the updated Euler angles
			UpdateCameraVectors();
		}

		//Process input received from a mouse scroll-wheel event.Only requires input on the vertical wheel-axis
		void ProcessMouseScroll(float yOffset)
		{
			if (Fov >= 1.0f && Fov <= 45.0f)
				Fov -= yOffset;
			if (Fov >= 45.0f)
				Fov = 45.0f;
			if (Fov <= 1.0f)
				Fov = 1.0f;
		}
private:
	//Calculates the front vector from the Camera's Euler Angles
	void UpdateCameraVectors()
	{
		//Calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(Pitch)) * cos(glm::radians(Yaw));
		front.y = sin(glm::radians(Pitch));
		front.z = cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
		Front = glm::normalize(front);
		//Also re-calculate the Right and Up
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}
	
};
#endif