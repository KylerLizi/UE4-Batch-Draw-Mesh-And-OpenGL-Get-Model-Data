#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Config/stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Camera.h"
#include "Shader.h"
#include "Model.h"
#include "iostream"

/* show mode*/
#define SHOW_WIREFRAME

//settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//delta time
float deltaTime;//储存了渲染上一帧所用的时间
float lastTime = 0.0f;

//camera rotation and zoom
Camera ourCamera(glm::vec3(40.0f, 41.0f, 40.0f),glm::vec3(0.0f,1.0f,0.0f),-130.0f,-30.0f);
bool firstMove = true;
float lastX = SCR_WIDTH / 2;
float lastY = SCR_HEIGHT / 2;

//lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void Mouse_Callback(GLFWwindow* window, double xPos, double yPos);
void Scroll_Callback(GLFWwindow* window, double xOffset, double yOffset);

int main()
{
	// glfw initialize and configure
	//----------------------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//主版本号(Major)=3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//次版本号(Minor)=3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//GLFW使用核心模式
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);//mac端设置

	//glfw window creation
	//----------------------------------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);//★

	//register glfw
	//---------------------------------------------------
	//register callback func for window scale changed
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//register callback func for mouse move
	glfwSetCursorPosCallback(window, Mouse_Callback);
	//register callback func for mouse scroll
	glfwSetScrollCallback(window, Scroll_Callback);

	//cursor hide and doesn't escape window, suitable for fpp
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	//glad: load all OpenGL function pointers
	//----------------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//open z-buffer
	glEnable(GL_DEPTH_TEST);

	//build and compile our shader program
	//----------------------------------------------
	Shader objectShader("ShaderSource/ModelLoad.vs", "ShaderSource/ModelLoad.fs");

	//load models
	//----------------------------------------------
	//const char* path = "Model/SimpleMesh.obj";
	const char* path = "Model/Teapot.obj";
	Model ourModel(path);
	
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
	};

	glm::vec3 pointLightColors[] = {
		glm::vec3(1.0f, 0.6f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
	};

#ifdef SHOW_WIREFRAME
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

	//render loop
	//----------------------------------------------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// -----------------------------
		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		// input
		// -----------------------------
		processInput(window);

		// render
		// -----------------------------
		glClearColor(0.156f, 0.156f, 0.156f, 1.0f);
		//glClearColor(0.63f, 0.63f, 0.64f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		objectShader.use();

		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;
		//draw model
		//----------------------------------
		view = ourCamera.GetViewMatrix();
		objectShader.setMat4("view", view);
		projection = glm::perspective(glm::radians(ourCamera.Fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 300.0f);
		objectShader.setMat4("projection", projection);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		objectShader.setMat4("model", model);
		ourModel.Draw(objectShader);

		//glfw: swap buffers and poll IO events(keys pressed/released, mouse moved etc.)
		// ------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);//前两个参数控制窗口左下角的位置
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	//press ESC key to close window
	//---------------------------------------------------
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	//press WASD to move camera position
	float cameraSpeed = 3.f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W))
		ourCamera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S))
		ourCamera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A))
		ourCamera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D))
		ourCamera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q))
		ourCamera.ProcessKeyboard(DOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E))
		ourCamera.ProcessKeyboard(UP, deltaTime);
}

void Mouse_Callback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMove)
	{
		lastX = xPos;
		lastY = yPos;
		firstMove = false;
	}

	//calculate offset
	float xOffset = xPos - lastX;
	float yOffset = lastY - yPos;
	lastX = xPos;
	lastY = yPos;

	ourCamera.ProcessMouseMovement(xOffset, yOffset);
}

void Scroll_Callback(GLFWwindow* window, double xOffset, double yOffset)
{
	ourCamera.ProcessMouseScroll(yOffset);
}