
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
GLuint cubeTexture;
GLuint planeTexture;

//模型
Model nano;

//点的位置数据
float points[] = {
	-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // 左上
	 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // 右上
	 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // 右下
	-0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // 左下
};


//方块缓冲
GLuint cubeVAO, cubeVBO;

//初始化函数
void init() {
	nano = Model("nanosuit/nanosuit.obj");
}

void display() {
	//清除缓存
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//z_test设置
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//着色器
	Shader geoShader("gVertex.vs", "gFragment.fs", "gGeometry.gs");
	Shader normalShader("gNormalVertex.vs","gNormalFragment.fs");
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 1.0f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();;
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));

	//绘制模型
	normalShader.use();
	normalShader.setMat4("projection", projection);
	normalShader.setMat4("view", view);
	normalShader.setMat4("model", model);
	nano.Draw(normalShader);

	//绘制法向量
	geoShader.use();
	geoShader.setMat4("projection", projection);
	geoShader.setMat4("view", view);
	geoShader.setMat4("model", model);
	//geoShader.setFloat("time", time);
	nano.Draw(geoShader);
	
	glutSwapBuffers();
	deltaTime = time - lastFrame;
	lastFrame = time;

	//帧数计数器
	cnt++;

	//释放申请的内存
	geoShader.close();
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

