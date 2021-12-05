#include<iostream>
#include <GL/glew.h>
#include<GLFW/glfw3.h>
#include <GL/freeglut.h>
#include "Shader.h"
#include"Material.h"
#include<glm/glm.hpp>
#include<glm/gtx/euler_angles.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include<glm/gtc/type_ptr.hpp>
#include"Camera.h"
#include"LightDirectional.h"
#include"LightPoint.h"
#include"LightSpot.h"
#include"Mesh.h"
#include"Model.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
using namespace std;


#pragma region Camera Declare

//Instantiate Camera Class，实例化一个照相机

Camera MyCamera(glm::vec3(0, 0.0f, 500.0f), glm::radians(0.0f), glm::radians(180.0f), glm::vec3(0, 1.0f, 0));
Camera MyCamera1(glm::vec3(0, 0.0f, 500.0f), glm::radians(0.0f), glm::radians(180.0f), glm::vec3(0, 1.0f, 0));
Camera MyCamera2(glm::vec3(0, 0.0f, 1000.0f), glm::radians(0.0f), glm::radians(180.0f), glm::vec3(0, 1.0f, 0));
#pragma endregion

#pragma region Light Declare
//平行光
LightDirectional MyLight(glm::vec3(50.0, 50.0f, -500.0f), //光源位置
	glm::vec3(glm::radians(0.0f), glm::radians(180.0f), glm::radians(180.0f)),//光源偏转角
	glm::vec3(1.0, 1.0f, 1.0f));//光源颜色

//point light
LightPoint MyLight0(glm::vec3(0.0f, -10.0f, -50.0f), //position
	glm::vec3(glm::radians(45.0f), glm::radians(45.0f), 0),//angle
	glm::vec3(1000.0f, 1000.0f, 1000.0f));//color

//spot light
LightSpot MyLightSpot(glm::vec3(0.0f, 5.0f, 0.0f), //position
	glm::vec3(glm::radians(90.0f), 0, 0),
	100.0f*glm::normalize(glm::vec3(55.0, 192.0f, 103.0f)));//color

#pragma endregion

#pragma region Input Declare

float LastX;
float LastY;
bool firstMouse = true;
float SpecularPower = 32.0f;
float F = 0.5;
float yaw = 0.0f;
float pitch = 90.0f;//当pitch是90度，改变yaw和roll效果是一样的，只会沿着一个方向转动
float roll = 0.0f;
float x = 0.0f, y = 0.0f, z=0.0f;
float w=0.0f,wx = 0.0,wy=90.0f,wz=0.0f;
int key = 0;
glm::vec3 RotationAxis;
glm::vec3 modelposition = glm::vec3(0, 800.0f, 0.0f);
bool flag = false;
void processInput(GLFWwindow* window) {//Input control
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
		MyCamera = MyCamera1;
	}
	else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		MyCamera = MyCamera2;
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		MyCamera.speedZ = 1.5f;
		modelposition.z += MyCamera.speedZ;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		MyCamera.speedZ = -1.5f;
		modelposition.z += MyCamera.speedZ;
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		MyCamera.speedX = -1.5f;
		modelposition.x += MyCamera.speedX;
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		MyCamera.speedX = 1.5f;
		modelposition.x += MyCamera.speedX;
	}
	else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		MyCamera.speedY = 1.5f;
		modelposition.y += MyCamera.speedY;
	}
	else if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
		MyCamera.speedY = -1.5f;
		modelposition.y += MyCamera.speedY;
	}
	else if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
		F -= 0.01;
		
	}
	else if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
		F += 0.01;
		flag = true;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		yaw -= 1.0f;
		flag = true;
	}
	else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		yaw += 1.0f;
		flag = true;
	}
	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		pitch += 1.0f;
		flag = true;
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		pitch -= 1.0f;
		flag = true;

	}
	else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		roll -= 1.0f;
		flag = true;

	}
	else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		roll += 1.0f;
		flag = true;
	}
	else
	{
		MyCamera.speedZ = 0;
		MyCamera.speedX = 0;
		MyCamera.speedY = 0;
		flag = false;
	}
}


void mouse_callback(GLFWwindow* window, double xPos, double yPos) {//Use mouse to change view angle
	if (firstMouse == true) {
		LastX = xPos;
		LastY = yPos;
		firstMouse = false;
	}
	float deltaX, deltaY;
	deltaX = xPos - LastX;
	deltaY = yPos - LastY;
	LastX = xPos;
	LastY = yPos;
	MyCamera.ProcessMouseMovement(-deltaX, -deltaY);
}

#pragma endregion

//Load Image
unsigned int LoadImageToGPU(const char* filename, GLint internalFormate, GLenum format, int texSlot) {
	unsigned int TexBuffer;
	glGenTextures(1, &TexBuffer);//产生一个VAO并且把ID赋给上一行的VAO
	glActiveTexture(GL_TEXTURE0 + texSlot);//开启textureBuffer中的几号位置
	glBindTexture(GL_TEXTURE_2D, TexBuffer);//把纹理buffer绑到操作位置上	
	int width, height, nrChannel;
	stbi_set_flip_vertically_on_load(true);//翻转纹理贴图
	unsigned char *data = stbi_load(filename, &width, &height, &nrChannel, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormate, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "Load image failed." << endl;
	}
	stbi_image_free(data);
	return TexBuffer;
}

//load CubeMap
unsigned int loadCubemap(vector<std::string> faces,int texSlot)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0 + texSlot);//开启textureBuffer中的几号位置
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	stbi_set_flip_vertically_on_load(false);//翻转纹理贴图
	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{	
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
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
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}


int main(int argc, char* argv[]) {

	std::string exePath = argv[0];
	//std::cout << exePath.substr(0,exePath.find_last_of('\\'))+"\\model\\nanosuit.obj"<< std::endl;

#pragma region Open a Window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Open GLFW Window开窗
	GLFWwindow* MyWindow = glfwCreateWindow(800, 600, "My First Window", NULL, NULL);
	if (MyWindow == NULL) {
		printf("Open window failed.");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(MyWindow);//将当前window作为主要上下文（用这个window）
	glfwSetInputMode(MyWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//关掉窗口中的鼠标
	glfwSetCursorPosCallback(MyWindow, mouse_callback);

	//Init GLEW
	glewExperimental = true;//实验性的功能设置为true，固定写法
	if (glewInit() != GLEW_OK) {
		printf("Init GLEW failed.");
		glfwTerminate();
		return -1;
	}
	glViewport(0, 0, 800, 600);//设置视角大小，OpenGL预设逆时针画点为正面，顺时针为反面

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);//开启Z-Buffer功能

#pragma endregion

#pragma region Init Shader Program
	Shader* MyShader = new Shader("vertexSource.vert", "FragmentSource.frag");//new a shader object
	Shader* MyShader1 = new Shader("vertexSource.vert", "Lab2Shader.frag");//new a shader object
	Shader* MyShader2 = new Shader("vertexSource.vert", "skybox.frag");//new a shader object
	Shader* MyShader3 = new Shader("vertexSource.vert", "Lab2Shader.frag");//new a shader object

	
	//材质球
#pragma region Init Material
	Material* MyMaterial = new Material(MyShader,
		LoadImageToGPU("ShipTex.jpg", GL_RGB, GL_RGB, 1),//diffuse，材质漫反射读图
		LoadImageToGPU("container2_specular.png", GL_RGBA, GL_RGBA, 2),//specular，镜面反射贴图
		glm::vec3(1.0f, 1.0f, 1.0f),//ambient
		32.0f);//shininess


	Material* MyMaterial1 = new Material(MyShader1,
		LoadImageToGPU("Sauce.png", GL_RGB, GL_RGB, 3),//diffuse，材质漫反射读图
		LoadImageToGPU("container2_specular.png", GL_RGBA, GL_RGBA, 2),//specular，镜面反射贴图
		glm::vec3(1.0f, 1.0f, 1.0f),//ambient
		32.0f);//shininess

	//cubemap
	vector<std::string> faces
	{
		"right.jpg",
		"left.jpg",
		"top.jpg",
		"bottom.jpg",
		"front.jpg",
		"back.jpg"
	};
	unsigned int cubemapTexture = loadCubemap(faces,4);

#pragma endregion

#pragma region Init and load Models to VAO, VBO

	Model model0(exePath.substr(0, exePath.find_last_of('\\')) + "\\model1\\small_transport_unit_04_03_high.obj");//Ship
	Model model1(exePath.substr(0, exePath.find_last_of('\\')) + "\\model4\\Sauce.obj");
	Model model2(exePath.substr(0, exePath.find_last_of('\\')) + "\\model2\\Mercury1K.obj");//球
	//Model model2(exePath.substr(0, exePath.find_last_of('\\')) + "\\model5\\Peter_LOD_0_GV2.max");//球

	
#pragma endregion

#pragma region Prepare MVP matrices	

//张角，画面尺寸，近平面远平面
//glm::mat4 trans;//设置transform，位移 旋转 缩放顺序来操作，避免互相影响

	glm::mat4 modelMat;//模型位置 飞船
	glm::mat4 modelMat1;//模型位置 Phong
	glm::mat4 modelMat2;//模型位置 BlinnPhong
	glm::mat4 modelMat3;//模型位置 

	glm::mat4 viewMat;//视角位置
	glm::mat4 projMat;//投影位置
	glm::quat t1 = glm::angleAxis(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::quat t2 = glm::angleAxis(glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::quat t3 = glm::angleAxis(glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::quat temp = t1 * t2*t3;
	//glm::quat temp;
	projMat = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 50000.0f);


	pitch = 90;//Now pitch is 90

	
#pragma endregion
	while (!glfwWindowShouldClose(MyWindow)) {//渲染循环

		viewMat = MyCamera.GetViewMatrix(MyCamera.Position);//视角变换
		//调用前面输入控制函数
		processInput(MyWindow);
		//清空&设置背景颜色
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);//RGB的值，不透明度1.0
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除颜色和深度

//SkyBox 第一个渲染
#pragma region Shader2 set and use, Draw Box

				//天空盒顶点
		float skyboxVertices[] = {
			// positions          
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
		};//

		MyShader2->use();
		modelMat2 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 200.0f, 0.0f));
		modelMat2 = glm::scale(modelMat2, glm::vec3(3000.0f, 3000.0f, 3000.0f));
		//modelMat2 = glm::rotate(modelMat1, -0.5f*(float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		//Vertex Shader
		glm::mat4 view = glm::mat4(glm::mat3(MyCamera.GetViewMatrix(MyCamera.Position)));
		glUniformMatrix4fv(glGetUniformLocation(MyShader2->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat2));//给uniform变量赋予位移矩阵
		glUniformMatrix4fv(glGetUniformLocation(MyShader2->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));//给uniform变量赋予视角矩阵
		glUniformMatrix4fv(glGetUniformLocation(MyShader2->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));//给uniform变量赋予投影矩阵
		//Fragment Shader							
		glUniform1i(glGetUniformLocation(MyShader2->ID, "cubemap"), 4);
		//glDepthMask(GL_FALSE);
		// ... 设置观察和投影矩阵
		unsigned int skyboxVBO, skyboxVAO;
		glGenVertexArrays(1, &skyboxVAO);
		glBindVertexArray(skyboxVAO);

		glGenBuffers(1, &skyboxVBO);
		glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(skyboxVAO);

		glDrawArrays(GL_TRIANGLES, 0, 36);
		//glBindVertexArray(0);

#pragma endregion 


		//Space Ship
	#pragma region Shader1 set and use, Draw model1
				
				MyShader1->use();
				modelMat1 = glm::scale(modelMat1, glm::vec3(1.0f, 1.0f, 1.0f));
				modelMat1 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

				//Euler angle
				glm::mat4 EulerMatrix = glm::yawPitchRoll(glm::radians(yaw), glm::radians(pitch), glm::radians(roll));
				//modelMat1 *= EulerMatrix;

				
				//quat
				glm::quat MyQ1 = glm::angleAxis(glm::radians(yaw), glm::vec3(1.0f, 0.0f, 0.0f));
				glm::quat MyQ2 = glm::angleAxis(glm::radians(pitch), glm::vec3(0.0f, 1.0f, 0.0f));
				glm::quat MyQ3 = glm::angleAxis(glm::radians(roll), glm::vec3(0.0f, 0.0f, 1.0f));
				glm::quat MyQ = MyQ1*MyQ2*MyQ3;
				MyQ = glm::normalize(MyQ);
				if (flag) {
					temp *= MyQ;//first,temp=quat(1,0,0,0);
				}				
				temp = glm::normalize(temp);
				glm::mat4 RotationMatrix = glm::toMat4(temp);
				modelMat1 *= RotationMatrix;

			
			
				//Vertex Shader
				glUniformMatrix4fv(glGetUniformLocation(MyShader1->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat1));//给uniform变量赋予位移矩阵
				glUniformMatrix4fv(glGetUniformLocation(MyShader1->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));//给uniform变量赋予视角矩阵
				glUniformMatrix4fv(glGetUniformLocation(MyShader1->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));//给uniform变量赋予投影矩阵
				//Fragment Shader				
				glUniform3f(glGetUniformLocation(MyShader1->ID, "CameraPos"), MyCamera.Position.x, MyCamera.Position.y, MyCamera.Position.z);//视角位置（影响镜面反射）
				glUniform1f(glGetUniformLocation(MyShader1->ID, "Material.Eta"), 0.8);
				glUniform1f(glGetUniformLocation(MyShader1->ID, "Material.ReflectionFactor"), 0.8);
				glUniform1i(glGetUniformLocation(MyShader1->ID, "cubemap"), 4);

				glUniform1f(glGetUniformLocation(MyShader1->ID, "fresnelBias"), F);
				glUniform1f(glGetUniformLocation(MyShader1->ID, "fresnelScale"), 2);
				glUniform1f(glGetUniformLocation(MyShader1->ID, "fresnelPower"), 5.0);

				model0.Draw(MyShader1);
	#pragma endregion 


				//propeller
#pragma region Shader3 set and use, Draw model1

				MyShader3->use();
				modelMat2 = glm::translate(modelMat1, glm::vec3(-65.0f, 0.0f, 0.0f));
				modelMat2 = glm::rotate(modelMat2, 4*(float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));
				modelMat2 = glm::scale(modelMat2, glm::vec3(10.0f, 100.0f, 500.0f));

				
				//Vertex Shader
				glUniformMatrix4fv(glGetUniformLocation(MyShader3->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat2));//给uniform变量赋予位移矩阵
				glUniformMatrix4fv(glGetUniformLocation(MyShader3->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));//给uniform变量赋予视角矩阵
				glUniformMatrix4fv(glGetUniformLocation(MyShader3->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));//给uniform变量赋予投影矩阵
				//Fragment Shader				
				glUniform3f(glGetUniformLocation(MyShader3->ID, "CameraPos"), MyCamera.Position.x, MyCamera.Position.y, MyCamera.Position.z);//视角位置（影响镜面反射）
				glUniform1f(glGetUniformLocation(MyShader3->ID, "Material.Eta"), 0.8);
				glUniform1f(glGetUniformLocation(MyShader3->ID, "Material.ReflectionFactor"), 0.8);
				glUniform1i(glGetUniformLocation(MyShader3->ID, "cubemap"), 4);

				glUniform1f(glGetUniformLocation(MyShader3->ID, "fresnelBias"), F);
				glUniform1f(glGetUniformLocation(MyShader3->ID, "fresnelScale"), 2);
				glUniform1f(glGetUniformLocation(MyShader3->ID, "fresnelPower"), 5.0);

				model2.Draw(MyShader3);
#pragma endregion 
		

		//Clean up, prepare for next render loop
		glfwSwapBuffers(MyWindow);//刷新
		glfwPollEvents();//获取按键
		MyCamera.UpdateCameraPosition();

	}
	//Exit program
	glfwTerminate();
	return 0;
}
















