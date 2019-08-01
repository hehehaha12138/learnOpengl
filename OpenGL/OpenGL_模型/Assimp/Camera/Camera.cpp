
#include "pch.h"
#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <GL\glew.h>
#include <GL\glfw3.h>
#include <GL\freeglut.h>
#include "shader.h"
#include "camera.h"
#include "model.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#pragma comment(lib,"glew32.lib")

unsigned int loadTexture(const char *path);

//计时
static GLfloat time = 0.01f;
static GLfloat lastFrame = 0.01f;
static GLfloat deltaTime = 0.00f;

//光标位置设置
GLfloat lastMousePosX = 400.0f;
GLfloat lastMousePosY = 300.0f;

//初次进入
GLboolean first = true;
GLint cnt = 0;

//按键flag
int pressFlag = 0;

//摄像机 
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

//光源位置
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

//贴图
int width, height, nrChannels;
GLuint diffuseMap;
GLuint specularMap;
GLuint ambientMap;

//方块的位置数据
glm::vec3 cubePositions[] = {
  glm::vec3(0.0f,  0.0f,  0.0f),
  glm::vec3(2.0f,  5.0f, -15.0f),
  glm::vec3(-1.5f, -2.2f, -2.5f),
  glm::vec3(-3.8f, -2.0f, -12.3f),
  glm::vec3(2.4f, -0.4f, -3.5f),
  glm::vec3(-1.7f,  3.0f, -7.5f),
  glm::vec3(1.3f, -2.0f, -2.5f),
  glm::vec3(1.5f,  2.0f, -2.5f),
  glm::vec3(1.5f,  0.2f, -1.5f),
  glm::vec3(-1.3f,  1.0f, -1.5f)
};

//载入的模型
Model ourModel;

//点光源位置数据
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};

//初始化函数
void init() {
	ourModel = Model("nanosuit/nanosuit.obj");
}

void display() {
	//清除缓存
	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT| GL_STENCIL_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	//着色器
	Shader modelShader("multiLightVertex.vs", "multiLightFragment.fs");
	modelShader.use();

	//Uniform值
	modelShader.setVec3("viewPos", camera.Position);

	//点光源数组参数
	modelShader.setVec3("pointLights[0].position", pointLightPositions[0]);
	modelShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
	modelShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
	modelShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
	modelShader.setFloat("pointLights[0].constant", 1.0f);
	modelShader.setFloat("pointLights[0].linear", 0.09);
	modelShader.setFloat("pointLights[0].quadratic", 0.032);

	modelShader.setVec3("pointLights[1].position", pointLightPositions[1]);
	modelShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
	modelShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
	modelShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
	modelShader.setFloat("pointLights[1].constant", 1.0f);
	modelShader.setFloat("pointLights[1].linear", 0.09);
	modelShader.setFloat("pointLights[1].quadratic", 0.032);

	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)800.0f / (float)600.0f, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();
	modelShader.setMat4("projection", projection);
	modelShader.setMat4("view", view);

	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
	modelShader.setMat4("model", model);
	ourModel.Draw(modelShader);

	//关闭模板写入
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);
	glDisable(GL_DEPTH_TEST);

	//绘制边框
	Shader stencilShader("stencilVertex.vs", "stencilFragment.fs");
	stencilShader.use();
	stencilShader.setMat4("view", view);
	stencilShader.setMat4("projection", projection);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
	model = glm::scale(model, glm::vec3(0.202f, 0.202f, 0.202f));	// it's a bit too big for our scene, so scale it down
	//model = glm::scale(model, glm::vec3(0.0f, -0.1f, 0.0f));
	stencilShader.setMat4("model", model);
	ourModel.Draw(stencilShader);
	glStencilMask(0xFF);
	glEnable(GL_DEPTH_TEST);

	glutSwapBuffers();
	deltaTime = time - lastFrame;
	lastFrame = time;

	//帧数计数器
	cnt++;

	//释放申请的内存
	modelShader.close();
	stencilShader.close();
}

void idle() {
	display();
}

void processNormalKeys(unsigned char key, int x, int y) {
	float cameraSpeed = 10 * deltaTime;
	if (key == 119) {
		pressFlag = 119;
		camera.ProcessKeyboard(FORWARD, deltaTime);
		//cameraPos += cameraSpeed * cameraFront;
	}
	else if (key == 115) {
		//Press_S
		pressFlag = 115;
		camera.ProcessKeyboard(BACKWARD, deltaTime);
		//cameraPos -= cameraSpeed * cameraFront;
	}
	else if (key == 100) {
		//Press_D
		pressFlag = 100;
		camera.ProcessKeyboard(RIGHT, deltaTime);
		//cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp))*cameraSpeed;
	}
	else if (key == 97) {
		pressFlag = 97;
		camera.ProcessKeyboard(LEFT, deltaTime);
		//cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp))*cameraSpeed;
	}
	else {
		pressFlag = 0;
	}
	display();
}

void timerProc(int id) {
	time += 0.01f;
	glutTimerFunc(1, timerProc, 1);
}

//重设光标
void resetCursor(int sx, int sy) {

	if (first) {
		first = false;
	}
	else {
		//俯仰角,偏航角偏移量
		float xoffset = sx - lastMousePosX;
		float yoffset = lastMousePosY - sy;
		
		camera.ProcessMouseMovement(xoffset, yoffset);
		glutWarpPointer(400.0f, 300.0f);
	}
	
}



int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(800, 600);
	glutCreateWindow("纹理");

	GLuint result = glewInit();
	if (result != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(result));
		return 1;
	}
	init();
	glutDisplayFunc(display);
	glutSetCursor(GLUT_CURSOR_NONE);
	glutWarpPointer(400.0f,300.0f);
	glutTimerFunc(10, timerProc, 1);
	glutIdleFunc(idle);
	glutKeyboardFunc(processNormalKeys);
	glutPassiveMotionFunc(resetCursor);
	glutMainLoop();
	return 0;
}

//贴图加载函数
unsigned int loadTexture(char const * path) {
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
