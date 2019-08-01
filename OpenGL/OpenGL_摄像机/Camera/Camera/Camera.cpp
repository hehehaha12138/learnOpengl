
#include "pch.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <GL\glew.h>
#include <GL\glfw3.h>
#include <GL\freeglut.h>
#include "shader.h"
#include "camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#pragma comment(lib,"glew32.lib")

//顶点数据
float vertices[] = {
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

GLuint index[] = {
	0,1,3,
	1,2,3
};

//顶点缓冲区
GLuint VBO;

//数组缓冲
GLuint VAO;

//索引缓冲对象
GLuint EBO;

//纹理
int width, height, nrChannels;
GLuint texture;
GLuint texture_2;

GLfloat visibility;

//旋转数值
GLfloat degree = 0.0f;
GLfloat magnitude = 1.0f;

//模型矩阵
glm::mat4 model = glm::mat4(1.0f);

//观察矩阵
glm::mat4 view = glm::mat4(1.0f);

//投影矩阵
glm::mat4 projection = glm::mat4(1.0f);

//计时
static GLfloat time = 0.01f;
static GLfloat lastFrame = 0.01f;
static GLfloat deltaTime = 0.00f;

//摄像机自定义
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

//光标位置设置
GLfloat lastMousePosX = 400.0f;
GLfloat lastMousePosY = 300.0f;

//俯仰角和偏航角
GLfloat pitch = 0.0f;
GLfloat yaw = 0.0f;

//初次进入
GLboolean first = true;

//按键flag
GLint pressFlag = 0;

//摄像机 
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

//初始化函数
void init() {
	//初始化矩阵
	//model = glm::rotate(model, time*glm::radians(-55.0f), time * glm::vec3(1.0f, 0.0f, 0.0f));

	//开启Z-test
	//glEnable(GL_DEPTH_TEST);

	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	//初始化缓冲
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);

	//载入纹理
	unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	//设置过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	glGenTextures(1, &texture_2);
	glBindTexture(GL_TEXTURE_2D, texture_2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	stbi_image_free(data);
}

void display() {

	//清除缓存
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(0.0, 0.0, 1);

	//使用Shader
	Shader sp_1("vertex.vs", "fragment.fs");
	sp_1.use();

	//旋转摄像头
	/*float radius = 10.0f;
	float camX = sin(time)*radius;
	float camZ = cos(time)*radius;
	view = glm::lookAt(glm::vec3(camX, 0.0, camZ), 
			glm::vec3(0.0, 0.0, 0.0), 
			glm::vec3(0.0, 1.0, 0.0));*/
	//根据deltaTime来进行移动
	float cameraSpeed = 10 * deltaTime;
	if (pressFlag == 119) {
		cameraPos += cameraSpeed * cameraFront;
	}
	else if (pressFlag == 115) {
		cameraPos -= cameraSpeed * cameraFront;
	}
	else if (pressFlag == 100) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp))*cameraSpeed;
	}
	else if (pressFlag == 97) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp))*cameraSpeed;
	}
	pressFlag = 0;
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);


	//设置变换矩阵
	int modelLoc = glGetUniformLocation(sp_1.ID, "view");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(view));

	modelLoc = glGetUniformLocation(sp_1.ID, "projection");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(projection));

	//设置贴图2可见度
	sp_1.setInt("texture1", 0);
	sp_1.setInt("texture2", 1);
	sp_1.setFloat("visibiliy", visibility);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture_2);
	glBindVertexArray(VAO);

	for (unsigned int i = 0; i < 10; i++)
	{
		// calculate the model matrix for each object and pass it to shader before drawing
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[i]);
		float angle = 20.0f * i;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		model = glm::rotate(model, time * glm::radians(50.0f), glm::vec3(1.0f, 0.3f, 0.5f));

		modelLoc = glGetUniformLocation(sp_1.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glFlush();
	glutSwapBuffers();
	sp_1.close();
	deltaTime = time - lastFrame;
	lastFrame = time;
}

void idle() {
	display();
}

void processNormalKeys(unsigned char key, int x, int y) {
	float cameraSpeed = 10 * deltaTime;
	if (key == 119) {
		pressFlag = 119;
		//cameraPos += cameraSpeed * cameraFront;
	}
	else if (key == 115) {
		//Press_S
		pressFlag = 115;
		//cameraPos -= cameraSpeed * cameraFront;
	}
	else if (key == 100) {
		//Press_D
		pressFlag = 100;
		//cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp))*cameraSpeed;
	}
	else if (key == 97) {
		pressFlag = 97;
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



		//鼠标灵敏度
		float sensitivity = 0.05f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		//设置yaw和pitch
		yaw += xoffset;
		pitch += yoffset;

		//设置旋转限制
		if (pitch > 89.0f) {
			pitch = 89.0f;
		}

		if (pitch < -89.0f) {
			pitch = -89.0f;
		}

		//计算摄像机朝向(CameraFront向量)
		glm::vec3 front;
		front.z = -cos(glm::radians(pitch)) * cos(glm::radians(yaw));
		front.y = sin(glm::radians(pitch));
		front.x = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
		cameraFront = glm::normalize(front);

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
