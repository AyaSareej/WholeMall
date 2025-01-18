#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stb_image.h>
#include "Shader.h"

#include "Trapeozid.h"
#include "Sphere.h"
#include "Cuboid.h"4
#include "Door.h"
#include "WallHole.h"
#include "DoorWall.h"
#include "Closet.h"
#include "Drawer.h"
#include "Lift.h"	
#include "Arch.h"
#include "LiftRoomDoor.h" // elec door
///
#include "ShamCenter.h"
//
#include "TeddyBear.h"
#include "CarToy.h"
///
#include "Cylinder.h"
#include "Camera.h"
#include "TextureLoader.h"
#include "Texture.h"
#include "ToyStore.h"
#include "Table.h"
#include <iostream>
#include <vector>
#include <filesystem>
#include <string>

#include "Model.h"

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

// Camera Speed
int camera_speed = 1;

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
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//Shader
	Shader SimpleShader("simpleVS.vs", "simpleFS.fs");
	Shader SkyboxShader("skybox.vs", "skybox.fs");
	Shader texShader("simpleVS.vs", "texFS.fs");
	///////////////////////////////////


	///
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

	camera.Position = glm::vec3(0.0f, 2000.0f, 0.0f);
	camera.Yaw = 0.0f;
	camera.Pitch = 0.0f;
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

	//ToysStore textures
	//unsigned int t_floorRoomA = loadTexture("resources/ToysStore/floor_roomA.jpg");
	unsigned int t_floorToysStore = loadTexture("resources/ToysStore/ToyFloor.jpg");
	unsigned int t_roof = loadTexture("resources/roof-W.jpg");
	unsigned int t_wallToysStore = loadTexture("resources/ToysStore/ToyWall.jpg");
	unsigned int t_carWheels = loadTexture("resources/ToysStore/wheels.jpg");
	unsigned int t_caroo_fabric = loadTexture("resources/ToysStore/carro-fabric.jpg");
	unsigned int t_pink_fabric = loadTexture("resources/ToysStore/light-pink-fabric.jpg");
	
	
	GLuint teddyTexture = loadTexture("resources/ToysStore/ToyWall2.jpg"); // Assuming you have a texture loading function

	unsigned int d_head = loadTexture("resources/1x/head.png");
	unsigned int t_red = loadTexture("resources/n.png");
	unsigned int t_body = loadTexture("resources/1x/body.png");
	unsigned int t_bell = loadTexture("resources/1x/bell.png");
	unsigned int t_blue = loadTexture("resources/1x/blue.png");
	unsigned int t_white = loadTexture("resources/1x/white.png");
	unsigned int t_nose = loadTexture("resources/1x/nose.png");
	unsigned int t_leg = loadTexture("resources/1x/legt.png");
	unsigned int t_head2 = loadTexture("resources/1x/head2.png");
	
	/////
	unsigned int t_greyIron = loadTexture("resources/street/iron.jpg");
	unsigned int t_WhiteFloor = loadTexture("resources/floors/wf.jpg");
	unsigned int t_glass = loadTexture("resources/walls/glass3.png");

	/////////////  Buildings textures   //////////////
	unsigned int t_building1 = loadTexture("resources/buildings/photo_7_2025-01-16_15-20-24.jpg");
	unsigned int t_building2 = loadTexture("resources/buildings/photo_2_2025-01-16_16-01-39.jpg");
	unsigned int t_building3 = loadTexture("resources/buildings/photo_1_2025-01-16_16-01-39.jpg");
	unsigned int t_building4 = loadTexture("resources/buildings/photo_6_2025-01-16_15-20-24.jpg");
	unsigned int t_building5 = loadTexture("resources/buildings/photo_8_2025-01-16_15-20-24.jpg");
	unsigned int t_building6 = loadTexture("resources/buildings/photo_9_2025-01-16_15-20-24.jpg");
	
	unsigned int t_buildingBlack = loadTexture("resources/buildings/photo_13_2025-01-16_15-47-45.jpg");
	
	unsigned int t_centerBuilding = loadTexture("resources/walls/ceanaterwall.jpg");
	
	unsigned int t_sidewalk1 = loadTexture("resources/street/sidewalk.jpg");
	unsigned int t_sidewalk2 = loadTexture("resources/street/sidewalkcenter.jpg");
	
	unsigned int t_street1 = loadTexture("resources/street/street.jpg");
	unsigned int t_zeft = loadTexture("resources/street/zeft.jpg");



	//unsigned int t_wallRoomA = loadTexture("resources/ToysStore/Wallpaper.png");

	//////////////  Building the outside environment   //////////////



	Cuboid car(10, 5, 5);
	car.SetPosition(0.0f, 25.0f, 10.0f);
	
	Cylinder wheels(2, 2, 36);
	wheels.SetPosition(10, 20, 0);


	
	//the ground GENERATION
	Cuboid terrain(5500.5f, 10.0f, 7000.0f);
	terrain.SetPosition(0, -5, 0);
	terrain.SetTextureRepeat(10.0f, 6.0f);
	for (int i = 0; i < 6; i++) {
			terrain.SetTextureForFace(i, t_zeft);
			car.SetTextureForFace(i, t_red);

	}

	// first group

	Cuboid building(500, 1000, 700);
	building.SetPosition(-3650, 510, 0);
	building.SetRotation(0, 0, 180);

	Cuboid building2(500, 800, 700);
	building2.SetPosition(-3650, 410, 750);	
	building2.SetRotation(0, 0, 180);
	
	Cuboid building10(500, 800, 700);
	building10.SetPosition(-3650, 410, 1500);	
	building10.SetRotation(0, 0, 180);

	Cuboid building12(500, 1200, 1000);
	building12.SetPosition(-3650, 600, 1500 + 900);
	building12.SetRotation(0, 0, 180);

	Cuboid building3(500, 850, 700);
	building3.SetPosition(-3650, 435, -750);
	building3.SetRotation(0, 0, 180);

	Cuboid building8(500, 850, 700);
	building8.SetPosition(-3650, 425, -1500);
	building8.SetRotation(0, 0, 180);	
	
	Cuboid building11(500, 1000, 700);
	building11.SetPosition(-3650, 500, -1500 -750);
	building11.SetRotation(0, 0, 180);


	Cuboid sidewalk2(300, 15, 8000);
	sidewalk2.SetTextureRepeat(2, 20);
	sidewalk2.SetPosition(-3250, 7.5f, 0);

	
	// secound group
	
	Cuboid building7(500, 1300, 900);
	building7.SetPosition(1150, 650, -3750);
	building7.SetRotation(0, 90, 180);

	Cuboid building4(500, 1000, 700);
	building4.SetPosition(300, 510, -3750);
	building4.SetRotation(0, 90, 180);

	Cuboid building5(700, 1300, 800);
	building5.SetPosition(-500, 650, -3850);
	building5.SetRotation(0, 90, 180);

	Cuboid building6(500, 850, 700);
	building6.SetPosition(-1300, 435, -3750);
	building6.SetRotation(0, 90, 180);


	Cuboid sidewalk(300, 15, 5000); // behind the building
	sidewalk.SetTextureRepeat(2, 20);
	sidewalk.SetPosition(2950, -7.5f, 1300);

	Cuboid sidewalk_south2(300, 15, 4300); // near the buildings on left
	sidewalk_south2.SetTextureRepeat(2, 10);
	sidewalk_south2.SetPosition(500, -7.5f, -3350);
	sidewalk_south2.SetRotation(0, 90, 0);

	/// center sidewalks //////////

	Cuboid sidewalk3(500, 15, 5000); // near the center behind
	sidewalk3.SetTextureRepeat(2, 8);
	sidewalk3.SetPosition(1850, -7.5f, 1000);

	Cuboid sidewalk_south(500, 15, 4400); // near the center left
	sidewalk_south.SetTextureRepeat(2, 6);
	sidewalk_south.SetPosition(500, -7.5f, -2250);
	sidewalk_south.SetRotation(0, 90, 0);


	Cuboid sidewalk4(700, 15, 6500); // near the center front
	sidewalk4.SetTextureRepeat(2, 16);
	sidewalk4.SetPosition(-2050, -7.5f, 1000 - 250);
	
	Cuboid sidewalkFB(750, 15, 800); // near the center front bb
	sidewalkFB.SetTextureRepeat(2, 1);
	sidewalkFB.SetPosition(-2025, -7.5f,  -3600);
	
	/////
	/////// Street ///////////
	Cuboid streetL(700,15,8000); // front
	streetL.SetPosition(-2750, -5, -0);
	streetL.SetTextureRepeat(1,2);	
	
	Cuboid streetR(700,15,3000); // behind
	streetR.SetPosition(2450, -5, 0);
	streetR.SetTextureRepeat(1,2);

	Cuboid street_south(700, 15, 5400);
	street_south.SetPosition(275, -5, -2850);
	street_south.SetTextureRepeat(1, 2);
	street_south.SetRotation(0, 90, 0);

	////// Main Sham Center Building ////////////

	Cuboid sham_center(1595, 1000, 2000);
	sham_center.SetPosition(0, 500, 0);
	sham_center.SetTextureRepeat(1, 1);

	ShamCenter sham(3200, 1100, 4000, glm::vec3(0, 0, 0), t_centerBuilding, t_greyIron, t_buildingBlack, t_WhiteFloor,t_glass);
	
	DoorFrame underCenter(glm::vec3(100, -50/2, -800), glm::vec3(3050  , 4000 / 2 ,50), glm::vec3( 300, 4000 / 3 / 2 - 100,50), 1000) ;
	DoorFrame underCenterD(glm::vec3(0, -15, 0), glm::vec3(3440 , 4000 ,20), glm::vec3( 3200 - 300, 4000 - 300,20), 500) ;
	underCenter.Rotate(90, glm::vec3(1,0,0));
	underCenterD.Rotate(90, glm::vec3(1,0,0));

	Cuboid underCenterPart2(3050, 4000/2 - 200, 50);
	underCenterPart2.SetPosition(100,-50/2,1100);
	underCenterPart2.SetRotation(90, 0, 0);
	underCenterPart2.SetTextureRepeat(5, 5);
	//
	for (int i = 0; i < 6; i++) {
		building.SetTextureForFace(i, t_building1);
		building2.SetTextureForFace(i, t_building2);
		building3.SetTextureForFace(i, t_building3);
		building4.SetTextureForFace(i, t_building4);
		building5.SetTextureForFace(i, t_building5);
		building6.SetTextureForFace(i, t_building6);
		building7.SetTextureForFace(i, t_building1);
		building8.SetTextureForFace(i, t_building6);
		building10.SetTextureForFace(i, t_building3);
		building11.SetTextureForFace(i, t_building1);
		building12.SetTextureForFace(i, t_building4);
		underCenter.SetTextureRepeat(5, 10, i);

		sham_center.SetTextureForFace(i, t_centerBuilding);
		underCenter.SetFaceTexture(i, t_WhiteFloor);
		underCenterD.SetFaceTexture(i, t_street1);
		underCenterPart2.SetTextureForFace(i, t_WhiteFloor);

		streetL.SetTextureForFace(i, t_street1);
		streetR.SetTextureForFace(i, t_street1);

		street_south.SetTextureForFace(i, t_street1);
		
		sidewalk.SetTextureForFace(i, t_sidewalk1);
		sidewalk2.SetTextureForFace(i, t_sidewalk1);

		sidewalk4.SetTextureForFace(i, t_sidewalk2);
		sidewalk3.SetTextureForFace(i, t_sidewalk2);
		
		sidewalkFB.SetTextureForFace(i, t_sidewalk1);

		sidewalk_south.SetTextureForFace(i, t_sidewalk2);
		sidewalk_south2.SetTextureForFace(i, t_sidewalk1);

	}	
	

	///////////////////////////////////////////////////
	//rooms
	Cuboid ToysStore(250.0f, 300.0f, 250.0f); //w,h.d
	for (int i = 0; i < 6; i++) {
		if (i == 5) {
			ToysStore.SetTextureForFace(i, t_floorToysStore);
			ToysStore.SetTextureRepeat(2.0f, 2.0f);

		}
		else if (i == 4) {
			ToysStore.SetTextureForFace(i, t_roof);

		}
		else{
			ToysStore.SetTextureForFace(i, t_wallToysStore);
		}
	}	
	CarToy ct(10.0f, 5.0f, 10.0f,2.5f, 1.3f);
	ct.SetBodyTexture(t_red);
	ct.SetWheelTexture(t_carWheels);
	ct.SetPosition(10.0f, 70.0f, 0.0f);

	Sphere sphere(33, 36, 18);
    TeddyBear teddy1(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
	teddy1.SetBodyTexture(t_caroo_fabric);
	teddy1.SetHeadTexture(t_caroo_fabric);
	teddy1.SetArmTexture(t_pink_fabric);
	teddy1.SetLegTexture(t_pink_fabric);
	teddy1.SetEarTexture(t_pink_fabric);

	teddy1.SetPosition(glm::vec3(0.0f, 30.0f, 0.0f));

	////

	// Create a ToyStore instance
	ToyStore myToyStore(500.0f, 300.0f, 400.0f,t_roof, t_wallToysStore, t_floorToysStore);
	myToyStore.SetPosition(-500.0f, 200.0f, 120.0f);
		
	
	ToyStore my2ToyStore(300.0f, 300.0f, 400.0f, t_roof, t_wallToysStore, t_floorToysStore);
	my2ToyStore.SetPosition(700.0f, 150.0f, 500.0f);
	
	Cuboid shelf(4800.0f, 1000.0f, 5000.0f); // Shelf dimensions
	Cuboid shelf2(4800.0f, 1000.0f, 5000.0f); // Shelf dimensions
	for (int i = 0; i < 6; i++) {
		shelf.SetTextureForFace(i, t_outdoor);
		shelf2.SetTextureForFace(i, t_red);

	}
	shelf.SetPosition(0, 0, 0);
	shelf2.SetPosition(0, 0, 0);


	Table table(50, 5, 40, 5, 15);
	table.SetBodyTexture(t_pink_fabric);
	table.SetLegsTexture(t_pink_fabric);
	
	Table table2(50, 5, 40, 5, 15);
	table2.SetBodyTexture(t_pink_fabric);
	table2.SetLegsTexture(t_pink_fabric);
	table2.SetPosition(100, 30, 0);

	// Create toys
	TeddyBear teddy3(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f)); // TeddyBear dimensions
	teddy3.SetHeadTexture(t_caroo_fabric);
	teddy3.SetArmTexture(t_pink_fabric);
	teddy3.SetLegTexture(t_pink_fabric);
	TeddyBear teddy4(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f)); // Another TeddyBear

	Sphere sp(200, 20, 36);
	sp.SetTexture(t_red);

	CarToy car1(10.0f, 5.0f, 10.0f, 2.5f, 1.3f); // Car dimensions
	car1.SetBodyTexture(t_bell);
	car1.SetPosition(2, 40, 3);

	// Add toys to the store with relative positions
	myToyStore.AddTeddyBear(teddy3, glm::vec3(10.0f, 5.0f, 5.0f));
	myToyStore.AddTeddyBear(teddy4, glm::vec3(15.0f, 5.0f, -10.0f));
	myToyStore.AddCarToy(car1, glm::vec3(-10.0f, 100.0f, 0.0f));
	my2ToyStore.AddShelf(shelf, glm::vec3(10.0f, 5.0f, 0.0f)); // Add shelf at relative position
	myToyStore.AddShelf(shelf, glm::vec3(0.0f, 0.0f, 0.0f)); // Add another shelf higher up
	myToyStore.AddSpheres(sp, glm::vec3(0.0f, 0.0f, 0.0f)); // Add another shelf higher up

	my2ToyStore.AddTeddyBear(teddy3, glm::vec3(15.0f, 10.0f, 90.0f));
	my2ToyStore.AddTeddyBear(teddy4, glm::vec3(15.0f, 0.0f, 90.0f));
	my2ToyStore.AddCarToy(car1, glm::vec3(-10.0f, 5.0f, 0.0f));
	my2ToyStore.AddTable(table, glm::vec3(-50.0f, 50.0f, 0.0f));

	my2ToyStore.AddShelf(Cuboid(50, 10, 10), glm::vec3(0, 10, 0));

	//teddy1.SetScale(glm::vec3(2.0f, 2.0f, 2.0f));
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
	Model carModel("resources/3d_models/plant/potted_plant.obj");
	glActiveTexture(GL_TEXTURE0); // Reusing the same texture unit for each model mesh.
	unsigned int image_sampler_loc = glGetUniformLocation(SimpleShader.ID, "image");
	glUniform1i(image_sampler_loc, 0);
	//Model carpet("C:/Aya/Opengl/resources/3d_models/carpet_carpet/scene.gltf");
	////
		/*
	* App State part
	*/

	//AppState appState = {
	//	WallDoor(wall2Width, wall2Height, 10.0f, glm::vec3(2000.0f, 0.0f, 0.0f), doorTexture, bricksTexture, t_wallRoomA),
	//};

	//glfwSetWindowUserPointer(window, &appState);


	///////////////////////////////////////////////////////
	///////////////////// RENDERING LOOP //////////////////

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
		
		double viewX;
		double viewZ;
		glm::vec3 viewPos;

		SimpleShader.Use();


		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();

		SimpleShader.setMat4("projection", projection);
		SimpleShader.setMat4("view", view);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down

		SimpleShader.setMat4("model", model);
		SimpleShader.setInt("texture1", 0);

		// Set the light position uniform
		GLuint lightPosLocation = glGetUniformLocation(SimpleShader.ID, "lightPos");
		glUniform3f(lightPosLocation, lightPosition.x, lightPosition.y, lightPosition.z);

		carModel.Draw(SimpleShader);

		////////////////   TERRAIN   ///////////////
		//glm::mat4 terrainModel = glm::mat4(1.0f);
		//terrainModel = glm::translate(terrainModel, glm::vec3(0.0f, 0.0f, 0.0f)); // Center the cuboid at the origin
		//SimpleShader.setMat4("model", terrainModel);
		//terrain.Draw(SimpleShader);
		SimpleShader.Use();

		////// Main Sham Center Building ////////////
		
		//sham_center.Draw(SimpleShader);
		sham.Draw(SimpleShader);
		underCenter.Draw(SimpleShader);
		underCenterD.Draw(SimpleShader);
		underCenterPart2.Draw(SimpleShader);


		//////////////////     Buildings    ////////////
		building.Draw(SimpleShader);
		building2.Draw(SimpleShader);
		building3.Draw(SimpleShader);
		building4.Draw(SimpleShader);
		building5.Draw(SimpleShader);
		building6.Draw(SimpleShader);
		building7.Draw(SimpleShader);
		building8.Draw(SimpleShader);
		building10.Draw(SimpleShader);
		building11.Draw(SimpleShader);
		building12.Draw(SimpleShader);
		
		// streets
		streetL.Draw(SimpleShader);
		streetR.Draw(SimpleShader);

		street_south.Draw(SimpleShader);

		//// sidewalk ///
		sidewalk.Draw(SimpleShader);
		sidewalk4.Draw(SimpleShader);
		sidewalkFB.Draw(SimpleShader);

		sidewalk2.Draw(SimpleShader);
		sidewalk3.Draw(SimpleShader);

		sidewalk_south.Draw(SimpleShader);
		sidewalk_south2.Draw(SimpleShader);
		//frameKitchenWindow.Rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		//


		// Room A
		glm::mat4 ToysStoreModel = glm::mat4(1.0f);
		SimpleShader.setMat4("model", ToysStoreModel);
		ToysStore.SetPosition(0.0f, 165.0f, 0.0f);
		ToysStore.SetFaceEnabled(0, true); //  front face
		ToysStore.SetFaceEnabled(1, true); //  back face
		ToysStore.SetFaceEnabled(2, false);  //  left face
		ToysStore.SetFaceEnabled(3, true);  //  right face
		ToysStore.SetFaceEnabled(4, true);  //  top face
		ToysStore.SetFaceEnabled(5, true);  //  bottom face
		ToysStore.Draw(SimpleShader);

		

		
	/*	carpet.setPosition(glm::vec3(2.0f, 100.0f, 0.0f));
		carpet.setScale(glm::vec3(1.5f, 1.5f, 1.5f));
		carpet.setRotation(glm::vec3(0.0f, 45.0f, 0.0f));*/
		//carpet.draw(SimpleShader.ID);
		// App states drawing and updating
		//
		//car.Draw(SimpleShader);
	/*	appState.wall.Draw(SimpleShader);
		appState.wall.Update(deltaTime);*/
		teddy1.Draw(SimpleShader);
		glm::mat4 ctModel = glm::mat4(1.0f);
		SimpleShader.setMat4("model", ctModel);
		ct.Draw(SimpleShader);
		//////////////////////  finishing drawing  ////////////////////////////////
		//wheels.Draw(SimpleShader);
		myToyStore.Render(SimpleShader);
		my2ToyStore.Render(SimpleShader);
		table2.Draw(SimpleShader);

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

		///////////////////////////

		//for (unsigned int i = 0; i < carModel.num_meshes; ++i)
		//{
		//	glBindTexture(GL_TEXTURE_2D, carModel.mesh_list[i].tex_handle); // Bind texture for the current mesh.	

		//	glBindVertexArray(carModel.mesh_list[i].VAO);
		//	glDrawElements(GL_TRIANGLES, (GLsizei)carModel.mesh_list[i].vert_indices.size(), GL_UNSIGNED_INT, 0);
		//	glBindVertexArray(0);
		//}
		//for (unsigned int i = 0; i < carModel.num_meshes; ++i)
		//{
		//	glBindTexture(GL_TEXTURE_2D, black_smith.mesh_list[i].tex_handle); // Bind texture for the current mesh.	

		//	glBindVertexArray(black_smith.mesh_list[i].VAO);
		//	glDrawElements(GL_TRIANGLES, (GLsizei)black_smith.mesh_list[i].vert_indices.size(), GL_UNSIGNED_INT, 0);
		//	glBindVertexArray(0);
		//}



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
		camera.ProcessKeyboard(FORWARD, deltaTime * camera_speed);

	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		camera_speed = 3;

	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		camera_speed = 1;

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime * camera_speed);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime * camera_speed);

	
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime * camera_speed);
	
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
		camera.Position = glm::vec3(0.0f, 50.0f, 0.0f);
		camera.Yaw = 0.0f;
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
