#define STB_IMAGE_IMPLEMENTATION
//Shao commented, above line is compulsory
#include <stdio.h>
#include <string.h>
#include <cmath>//Shao commented, not compulsory
#include <vector>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
//#include "CommonValues.h"//Shao commented, not compulsory
#include "Window.h"//Window.h is Waited to be review
#include "Mesh.h"//Mesh.h is Waited to be review
#include "Shader.h"//Shader.h is Waited to be review
#include "Camera.h"//Camere.h is Waited to be review
#include "Texture.h"//Texture.h is Waited to be review
//#include "DirectionalLight.h"//This line is not compulsory 

#include "Material.h"//Waited to be review

#include "Model.h"//Waited to be review

#include "Skybox.h"//Waited to be review

const float toRadians = 3.14159265f / 180.0f;

GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
uniformSpecularIntensity = 0, uniformShininess = 0,
uniformDirectionalLightTransform = 0, uniformOmniLightPos = 0, uniformFarPlane = 0;//Need to be changed

Window mainWindow;
//std::vector<Mesh*> meshList;//Shao removed for making the sphere to be transperant

std::vector<Shader> shaderList;
Shader directionalShadowShader;//Wait to being removed
Shader omniShadowShader;//Wait to being removed

Camera camera;

Material shinyMaterial;
Material dullMaterial;

Model piper_pa18;
Model propeller;

DirectionalLight mainLight;//This line is not compulsory, but affects some below code
PointLight pointLights[MAX_POINT_LIGHTS];//This line is not compulsory but affects some below code
SpotLight spotLights[MAX_SPOT_LIGHTS];//This line is not compulsory but affects some below code

Skybox skybox;

unsigned int pointLightCount = 0;//This line is not compulsory but affects some below code
unsigned int spotLightCount = 0;//This line is not compulsory but affects some below code

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
//GLunit skyboxLocation;//How to solve this redline?

//float z;
glm::vec3 rotation(0.f);
//MyCamera;

// Vertex Shader
static const char* vShader = "Shaders/shader-yuzhou.vert";

// Fragment Shader
static const char* fShader = "Shaders/shader-yuzhou.frag";

//void processInput(GLFWwindow* window) {//Input control
//	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
//		rotation.x= rotation.x + 1;//+=
//	}
//
//	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
//		glfwSetWindowShouldClose(window, true);
//	}
//}//Control K+X

//void processInput() {//Input control
//	if(GLFW_KEY_UP) {
//		rotation.x = rotation.x + 10;//+=
//	}
//	if (GLFW_KEY_DOWN) {
//		rotation.x = rotation.x - 10;//+=
//	}
//	if (GLFW_KEY_LEFT) {
//		rotation.y = rotation.y + 10;//+=
//	}
//	if (GLFW_KEY_RIGHT) {
//		rotation.y = rotation.y - 10;//+=
//	}
//	//if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
//	//	glfwSetWindowShouldClose(window, true);
//	//}
//}

void CreateShaders()
{

	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	directionalShadowShader.CreateFromFiles("Shaders/directional_shadow_map.vert", "Shaders/directional_shadow_map.frag");
	omniShadowShader.CreateFromFiles("Shaders/omni_shadow_map.vert", "Shaders/omni_shadow_map.geom", "Shaders/omni_shadow_map.frag");
	//Above two lines are not compulsory, but if they are removed, it shows small error.
}

void RenderScene()
{
	//glm::mat4 model(1.0f);//Removed 1.0f, no differences
	//Shao edited 02 this Chunk of code for loading the airplane.
	glm::mat4 model1;//model1 for the plane
	glm::mat4 model2;//model2 for the propeller
	//glm::vec3 rotation(0.f);//Refereicing from Youtube
	/*//Init Matrices, Shao referenced from https://youtu.be/uX3Iil0F51U
	glm::vec3 position(0.f);
	glm::vec3 rotation(0.f);
	glm::vec3 scale(1.f);
	//Added up to here, it makes no difference*/

	//Shao removed this chunk of airplane code for making keyboard controling 
	model1 = glm::mat4(1.0f);
	model1 = glm::translate(model1, glm::vec3(0.0f, -2.0f, -5.0f));//The position of the airplane
	//model = glm::translate(model, glm::vec3(x, y, z));
	//model = glm::translate(model, glm::vec3(triOffset, 0.0f, -2.5f));//Added from Udemy-Projection
	model1 = glm::scale(model1, glm::vec3(0.8f, 0.8f, 0.8f));
	model1 = glm::rotate(model1, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));//Shao edited for keyboard controlling rotation
	model1 = glm::rotate(model1, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));//Shao edited for keyboard controlling rotation
	model1 = glm::rotate(model1, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));//Shao edited for keyboard controlling rotation

	//rotation.x++;
	model1 = glm::rotate(model1, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));//Shao once removed static rotation
	//model = glm::rotate(model, -20.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));//Shao once removed static rotation
	//model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));//Shao removed dynamic rotation

	//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(1.0f, 0.0f, 0.0f));//Shao edited
	//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model1));
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	piper_pa18.RenderModel();


	//Loading the Propeller 
	model2 = glm::mat4(1.0f);
	model2 = model2 * model1;
	//glm::mat4(1.0f) = glm::mat4(1.0f) * model;
	//model2 = glm::translate(model2, glm::vec3(0.0f, -1.0f, -7.56f));//Original position
	model2 = glm::translate(model2, glm::vec3(0.0f, 1.26f, 3.15f));
	model2 = glm::rotate(model2, 90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));//Rotate the 3D position of the propeller
	model2 = glm::rotate(model2, 45.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model2 = glm::rotate(model2, 180.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model2 = glm::rotate(model2, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));//Shao edited
	model2 = glm::scale(model2, glm::vec3(3.0f, 3.0f, 3.0f));//Previously was 0.006, so invisible
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model2));
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	propeller.RenderModel();
}

//If directionalShadowMapPass is removed, it shows black screen, since it involves into shaders.
void DirectionalShadowMapPass(DirectionalLight* light)
{
	directionalShadowShader.UseShader();

	glViewport(0, 0, light->getShadowMap()->GetShadowWidth(), light->getShadowMap()->GetShadowHeight());

	light->getShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = directionalShadowShader.GetModelLocation();
	directionalShadowShader.SetDirectionalLightTransform(&light->CalculateLightTransform());

	directionalShadowShader.Validate();

	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//Above function need to be removed, I skip it first.
void OmniShadowMapPass(PointLight* light)
{
	omniShadowShader.UseShader();

	glViewport(0, 0, light->getShadowMap()->GetShadowWidth(), light->getShadowMap()->GetShadowHeight());

	light->getShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = omniShadowShader.GetModelLocation();
	uniformOmniLightPos = omniShadowShader.GetOmniLightPosLocation();
	uniformFarPlane = omniShadowShader.GetFarPlaneLocation();

	glUniform3f(uniformOmniLightPos, light->GetPosition().x, light->GetPosition().y, light->GetPosition().z);
	glUniform1f(uniformFarPlane, light->GetFarPlane());
	omniShadowShader.SetLightMatrices(light->CalculateLightTransform());

	omniShadowShader.Validate();

	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//Above function need to be removed, I skip it first.
void RenderPass(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
	glViewport(0, 0, 1366, 768);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	skybox.DrawSkybox(viewMatrix, projectionMatrix);

	shaderList[0].UseShader();

	uniformModel = shaderList[0].GetModelLocation();
	uniformProjection = shaderList[0].GetProjectionLocation();
	uniformView = shaderList[0].GetViewLocation();
	uniformModel = shaderList[0].GetModelLocation();
	uniformEyePosition = shaderList[0].GetEyePositionLocation();
	uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
	uniformShininess = shaderList[0].GetShininessLocation();

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

	//This chunk of the code is not compulsory for working on airplane
	shaderList[0].SetDirectionalLight(&mainLight);
	shaderList[0].SetPointLights(pointLights, pointLightCount, 3, 0);
	shaderList[0].SetSpotLights(spotLights, spotLightCount, 3 + pointLightCount, pointLightCount);
	shaderList[0].SetDirectionalLightTransform(&mainLight.CalculateLightTransform());

	mainLight.getShadowMap()->Read(GL_TEXTURE2);
	shaderList[0].SetTexture(1);
	shaderList[0].SetDirectionalShadowMap(2);

	glm::vec3 lowerLight = camera.getCameraPosition();
	lowerLight.y -= 0.3f;
	spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

	shaderList[0].Validate();

	RenderScene();
}

//void update(GLFWWindow* window, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale)


int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	//CreateObjects();
	CreateShaders();
	//-45.0f is the camera perspective angle
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 0.5f, 0.5f);
	//Origially, the last 3 parameters are 0.0f, 5.0f, 0.5f  ,Parameters above reduce the sensitivity for controlling
//camera = Camera(glm::vec3(0.0f, -2.0f, -5.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 1.5f, 0.1f);
//Above line of the code is for testing the camera position matching with the airplane's position.                                           

	shinyMaterial = Material(4.0f, 256);
	dullMaterial = Material(0.3f, 4);

	//Shao edited 03 this Chunk of code for loading the airplane.
	piper_pa18 = Model();//Shao eidted 03 for loading sphere.
	piper_pa18.LoadModel("Models/piper_pa18_obj/piper_pa18.obj");


	propeller = Model();//Shao eidted 03 for loading sphere.
	propeller.LoadModel("Models/Pelican_Propeller/propeller.obj");


	mainLight = DirectionalLight(2048, 2048,
		1.0f, 0.53f, 0.3f,
		0.1f, 0.9f,
		-10.0f, -12.0f, 18.5f);

	pointLights[0] = PointLight(1024, 1024,
		0.01f, 100.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f,
		1.0f, 2.0f, 0.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;
	pointLights[1] = PointLight(1024, 1024,
		0.01f, 100.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f,
		-4.0f, 3.0f, 0.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;


	spotLights[0] = SpotLight(1024, 1024,
		0.01f, 100.0f,
		1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;
	spotLights[1] = SpotLight(1024, 1024,
		0.01f, 100.0f,
		1.0f, 1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, -1.5f, 0.0f,
		-100.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);
	//Helped from Zhang Shijun

	//
	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime(); // SDL_GetPerformanceCounter();
		deltaTime = now - lastTime; // (now - lastTime)*1000/SDL_GetPerformanceFrequency();
		lastTime = now;

		// Get + Handle User Input
		glfwPollEvents();

		//Zhang Shijun
		//processInput();

		camera.keyControl(mainWindow.getsKeys(), deltaTime);//Original code
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		if (mainWindow.getsKeys()[GLFW_KEY_UP])
		{
			rotation.x += 10;
		}

		if (mainWindow.getsKeys()[GLFW_KEY_DOWN]) {
			rotation.x -= 10;//+=
		}
		if (mainWindow.getsKeys()[GLFW_KEY_LEFT]) {
			rotation.y += 10;//+=
		}
		if (mainWindow.getsKeys()[GLFW_KEY_RIGHT]) {
			rotation.y -= 10;//+=
		}
		if (mainWindow.getsKeys()[GLFW_KEY_U]) {
			rotation.z += 10;//+=
		}
		if (mainWindow.getsKeys()[GLFW_KEY_O]) {
			rotation.z -= 10;//+=
		}

		if (mainWindow.getsKeys()[GLFW_KEY_L])
		{
			spotLights[0].Toggle();
			mainWindow.getsKeys()[GLFW_KEY_L] = false;
		}
		//updateInput(window, position, rotation, scale);//Shao edited for taking keyboard control of the air plane

		//If this loop is removed, it shows black screen
		DirectionalShadowMapPass(&mainLight);
		for (size_t i = 0; i < pointLightCount; i++)
		{
			OmniShadowMapPass(&pointLights[i]);//Shao reomved for turning off the spot light,remove this line or not, no difference, flashligh is depended line 458.
		}
		for (size_t i = 0; i < spotLightCount; i++)
		{
			OmniShadowMapPass(&spotLights[i]);//Shao reomved for turning off the spot light
		}

		RenderPass(camera.calculateViewMatrix(), projection);

		mainWindow.swapBuffers();
	}

	return 0;
}