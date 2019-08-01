
#include "pch.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <GL\glew.h>
#include <GL\glfw3.h>
#include <GL\freeglut.h>
#include "shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#pragma comment(lib,"glew32.lib")

//顶点数据
float vertices[] = {
	//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
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

//变化矩阵
glm::mat4 trans = glm::mat4(1.0f);

//旋转数值
GLfloat degree = 0.0f;
GLfloat magnitude = 1.0f;

//模型矩阵

//初始化函数
void init() {
	//初始化缓冲
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	
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
	
	glGenTextures(1,&texture_2);
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
	//trans = glm::rotate(trans, glm::radians(degree), glm::vec3(0.0, 0.0, 1.0));
	trans = glm::scale(trans, glm::vec3(magnitude, magnitude, 1.0f));
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 1);
	Shader sp_1("vertex_2.vs", "fragment.fs");
	sp_1.use();
	sp_1.setInt("texture1", 0);
	sp_1.setInt("texture2", 1);
	sp_1.setFloat("visibiliy",visibility);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture_2);
	unsigned int transformLoc = glGetUniformLocation(sp_1.ID, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glFlush();
	glutSwapBuffers();
}

void idle() {
	display();
}

void processNormalKeys(unsigned char key, int x, int y) {
	cout << "a" << endl;
	if (key == 119) {
		degree = 0.0f;
		magnitude = 1.0f;
		if (visibility >= 1.0f) {
			visibility = 1.0f;
		}
		else {
			visibility += 0.05;
		}
	}
	else if (key == 115) {
		degree = 0.0f;
		magnitude = 1.0f;
		if (visibility <= 0.0f) {
			visibility = 0.0f;
		}
		else {
			visibility -= 0.05;
		}
	}
	else if (key == 100) {
		//D_顺时针旋转
		//degree = -1.0f;
		magnitude = 1.1f;
	}
	else if (key == 97) {
		//A_逆时针旋转
		magnitude = 0.9f;
	}
	display();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(400, 300);
	glutCreateWindow("纹理");

	GLuint result = glewInit();
	if (result != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(result));
		return 1;
	}
	init();
	glutDisplayFunc(display);
	//glutIdleFunc(idle);
	glutKeyboardFunc(processNormalKeys);
	glutMainLoop();
	return 0;
}
