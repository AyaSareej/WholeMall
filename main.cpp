#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include <stb_image.h>
#include "Shader.h"
#include "Sphere.h"
#include "Cuboid.h"
#include "Door.h"
#include "Model.h"
#include "WallHole.h"
#include "DoorWall.h"
#include "Closet.h"
#include "Drawer.h"
#include "Lift.h"	
#include "Arch.h"
#include "LiftRoomDoor.h"

#include "Cylinder.h"
#include "Camera.h"
#include "TextureLoader.h"
//#include "Model.h"

#include <iostream>
#include <vector>
#include <filesystem>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
unsigned int loadTexture(char const* path);
unsigned int loadCubemap(std::vector<std::string> faces);
void GetDesktopResolution(float& horizontal, float& vertical) //to get the full screen correctly
{
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	horizontal = desktop.right;
	vertical = desktop.bottom;
}
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool onRotate = false;
bool onFreeCam = true;
float SCREEN_WIDTH = 800, SCREEN_HEIGHT = 600;

glm::vec3 point = glm::vec3(0.0f, 0.0f, 0.0f);
GLfloat lastX = (GLfloat)(SCREEN_WIDTH / 2.0);
GLfloat lastY = (GLfloat)(SCREEN_HEIGHT / 2.0);
glm::vec3 DoPositions[15];

bool keys[1024];
GLfloat SceneRotateY = 0.0f;
GLfloat SceneRotateX = 0.0f;
bool onPlanet = false;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

bool firstMouse = true;
GLfloat xoff = 0.0f, yoff = 0.0f;

//struct AppState {
//	WallDoor wall; 
//
//};



void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = (GLfloat)xpos;
		lastY = (GLfloat)ypos;
		firstMouse = false;
	}

	GLfloat xoffset = (GLfloat)(xpos - lastX);
	GLfloat yoffset = (GLfloat)(lastY - ypos);
	xoff = xoffset;
	yoff = yoff;

	lastX = (GLfloat)xpos;
	lastY = (GLfloat)ypos;
	if (onRotate)
	{
		SceneRotateY += yoffset * 0.1f;
		SceneRotateX += xoffset * 0.1f;
	}
	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (yoffset == 1)
		camera.ProcessKeyboard(SCROLL_FORWARD, deltaTime);
	else
	{
		camera.ProcessKeyboard(SCROLL_BACKWARD, deltaTime);
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (action == GLFW_PRESS) {
		//AppState* appState = static_cast<AppState*>(glfwGetWindowUserPointer(window));
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		ypos = SCREEN_HEIGHT - ypos; // Invert Y coordinate

		glm::mat4 viewProjectionMatrix = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 10000.0f) * camera.GetViewMatrix();

		// Handle the click
		//appState->closetA.OnMouseClick(xpos, ypos, viewProjectionMatrix, glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
	}
}





int main() {
	GetDesktopResolution(SCREEN_WIDTH, SCREEN_HEIGHT);
	camera.LookAtPos = point;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	//window createing
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Doraimon", glfwGetPrimaryMonitor(), NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	/////////////////////////////

	//LOAD GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	///////////////////////////////

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Shader
	Shader SimpleShader("simpleVS.vs", "simpleFS.fs");
	Shader SkyboxShader("skybox.vs", "skybox.fs");
	Shader texShader("simpleVS.vs", "texFS.fs");
	///////////////////////////////////

	float cube[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	float skyboxVertices[] = {
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	// SKYBOX GENERATION
	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	///////////////////////////////////////////////

	//////////////////////////////////////////////////
	std::vector<std::string> faces
	{
		"resources/skybox/sky/right.bmp",
		"resources/skybox/sky/left.bmp",
		"resources/skybox/sky/top.bmp",
		"resources/skybox/sky/bottom.bmp",
		"resources/skybox/sky/front.bmp",
		"resources/skybox/sky/back.bmp",
	};

	unsigned int cubemapTexture = loadCubemap(faces);
	GLfloat camX = 10.0f;
	GLfloat camZ = 10.0f;

	camera.Position = glm::vec3(200.0f, 50.0f, 0.0f);
	camera.Yaw = 180.0f;
	camera.Pitch = -10.0f;
	camera.ProcessMouseMovement(xoff, yoff);
	camera.FreeCam = false;
	onFreeCam = true;
	glm::mat4 view;

	// Light position
	glm::vec3 lightPosition(0.0f, 1500.0f, 500.0f); // Set initial light position
	

	///////////load images///////////////

	GLuint t_outdoor = loadTexture("resources/pavement_outdoor/pavement_outdoor.jpg");
	GLuint  bricksTexture = loadTexture("resources/red_brick/red_brick.jpg");
	unsigned int doorTexture = loadTexture("resources/Door_01_Orange.png");

	//roomA textures
	unsigned int t_floorRoomA = loadTexture("resources/roomA/floor_roomA.jpg");
	unsigned int t_wallRoomA = loadTexture("resources/roomA/Wallpaper.png");

	////////////////////////////
	
	//the ground GENERATION
	Cuboid terrain(300.5f, 2.0f, 300.0f);
	terrain.SetTextureRepeat(3.0f, 3.0f);


	//rooms
/*	Cuboid roomA(2490.0f, 1490.0f, 1490.0f);
	for (int i = 0; i < 6; i++) {
		if (i == 5) {
			roomA.SetTextureForFace(i, t_floorRoomA);
			roomA.SetTextureRepeat(3.0f, 3.0f);

		}
		else{
			roomA.SetTextureForFace(i, t_wallRoomA);
		}
	}*/	
	

	// ground
	//DoorFrame frameRoomA(
	//	glm::vec3(750.0f, 755.0f, -720.0f),  // Position (center of the frame)
	//	glm::vec3(2490.0f, 1490.0f, 10.0f),    // Outer dimensions
	//	glm::vec3(700.0f, 700.0f, 10.0f),     // Hole dimensions
	//	0.0f
	//	//t_wallRoomA                        // Texture ID
	//);
	//DoorFrame frame(
	//	glm::vec3(750.0f, 750.0f, -750.0f),  // Position (center of the frame)
	//	glm::vec3(2510.0f, 1500.0f, 5.0f),    // Outer dimensions
	//	glm::vec3(700.0f, 700.0f, 10.0f),     // Hole dimensions
	//	0.0f
	//	//bricksTexture                        // Texture ID
	//);
	

	/*
	* Models
	*/
	
	//Model carpet("C:/Aya/Opengl/resources/3d_models/carpet_carpet/scene.gltf");
	////
		/*
	* App State part
	*/

	//AppState appState = {
	//	WallDoor(wall2Width, wall2Height, 10.0f, glm::vec3(2000.0f, 0.0f, 0.0f), doorTexture, bricksTexture, t_wallRoomA),
	//};

	//glfwSetWindowUserPointer(window, &appState);



	///////////////////// RENDERING LOOP /////////////

	while (!glfwWindowShouldClose(window))
	{

		GLfloat currentFrame = (GLfloat)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		if (camera.FreeCam) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		processInput(window); // input

		if (!onFreeCam)
		{
			SceneRotateY = 0.0f;
			SceneRotateX = 0.0f;
		}
		//		if (camera.FreeCam || PlanetView > 0)

		if (camera.FreeCam)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		// render
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model = glm::mat4(1.0f);

		double viewX;
		double viewZ;
		glm::vec3 viewPos;

		SimpleShader.Use();
		// Set the light position uniform
		GLuint lightPosLocation = glGetUniformLocation(SimpleShader.ID, "lightPos");
		glUniform3f(lightPosLocation, lightPosition.x, lightPosition.y, lightPosition.z);

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 10000.0f);
		SimpleShader.setMat4("model", model);
		SimpleShader.setMat4("view", view);
		SimpleShader.setMat4("projection", projection);


		////////////////   TERRAIN   ///////////////
		glm::mat4 terrainModel = glm::mat4(1.0f);
		terrainModel = glm::translate(terrainModel, glm::vec3(0.0f, 0.0f, 0.0f)); // Center the cuboid at the origin
		SimpleShader.setMat4("model", terrainModel);
		terrain.Draw(SimpleShader);
		SimpleShader.Use();
		//////////////////////////////


		//frameKitchenWindow.Rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		//


		/// Room A
		//glm::mat4 roomAModel = glm::mat4(1.0f);
		//SimpleShader.setMat4("model", roomAModel);
		//roomA.SetPosition(750.0f, 755.0f, 0.0f);
		//roomA.SetFaceEnabled(0, true); //  front face
		//roomA.SetFaceEnabled(1, false); //  back face
		//roomA.SetFaceEnabled(2, false);  //  left face
		//roomA.SetFaceEnabled(3, false);  //  right face
		//roomA.SetFaceEnabled(4, true);  //  top face
		//roomA.SetFaceEnabled(5, true);  //  bottom face
		//roomA.Draw(SimpleShader);

		//
		// App states drawing and updating
		//

	/*	appState.wall.Draw(SimpleShader);
		appState.wall.Update(deltaTime);*/
	
	

		
		//////////////////////  finishing drawing  ////////////////////////////////


		/* DRAW SKYBOX */
		glDepthFunc(GL_LEQUAL);
		SkyboxShader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		SkyboxShader.setMat4("view", view);
		SkyboxShader.setMat4("projection", projection);
		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);
		/* DRAW SKYBOX */

		// TRACKING 
		view = camera.GetViewMatrix();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	//AppState* appState = static_cast<AppState*>(glfwGetWindowUserPointer(window));

	///////// start free camera ///////////////////
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		camera.FreeCam = true;

	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);

	
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		std::cout << "Key Pressed L: " << std::endl;

		//appState->lift.HandleInput(GLFW_KEY_L);
	}

	
	if (glfwGetKey(window, GLFW_KEY_SLASH) == GLFW_PRESS) {
		//appState->doorLiftA.HandleInput(GLFW_KEY_SLASH);
		//appState->doorLiftKitchen.HandleInput(GLFW_KEY_SLASH);
	}


	/////////// stop free camera /////////
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
	{
		camera.FreeCam = false;
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		onFreeCam = true;
		camera.FreeCam = false;
		camera.Position = glm::vec3(200.0f, 50.0f, 0.0f);
		camera.Yaw = 180.0f;
		camera.Pitch = 0.0f;
		camera.GetViewMatrix();
		camera.ProcessMouseMovement(xoff, yoff);
	}


}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

unsigned int loadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;

	for (unsigned int i = 0; i < 6; i++)
	{

		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);

		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	return textureID;
}
