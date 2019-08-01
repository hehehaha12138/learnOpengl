
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
#include <vector>
#include <map>

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
GLuint grassTexture;

//透明方块排序
std::map<float, glm::vec3> stored;

//方块的位置数据
float cubeVertices[] = {
	// Back face
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
	// Front face
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
	// Left face
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
	// Right face
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
	// Bottom face
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
	// Top face
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left        
};

//平面的位置数据
float planeVertices[] = {
	// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
	-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
	-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
	5.0f, -0.5f,  5.0f,  2.0f, 0.0f,

	 5.0f, -0.5f, -5.0f,  2.0f, 2.0f,
	-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
	5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
};

//小草的位置数据
float transparentVertices[] = {
	0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
	0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
	1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

	0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
	1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
	1.0f,  0.5f,  0.0f,  1.0f,  0.0f
};

vector<glm::vec3> vegetation
{
	glm::vec3(-1.5f, 0.0f, -0.48f),
	glm::vec3(1.5f, 0.0f, 0.51f),
	glm::vec3(0.0f, 0.0f, 0.7f),
	glm::vec3(-0.3f, 0.0f, -2.3f),
	glm::vec3(0.5f, 0.0f, -0.6f)
};

//方块缓冲
GLuint cubeVAO, cubeVBO;

//平面缓冲
GLuint planeVAO, planeVBO;

//草缓冲
GLuint grassVAO, grassVBO;

//初始化函数
void init() {
	//方块VAO设定
	glGenVertexArrays(1,&cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(cubeVertices),&cubeVertices,GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);

	//平面VAO设定
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);

	//草VAO设定
	glGenVertexArrays(1, &grassVAO);
	glGenBuffers(1,&grassVBO);
	glBindVertexArray(grassVAO);
	glBindBuffer(GL_ARRAY_BUFFER,grassVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), &transparentVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);

	cubeTexture = loadTexture("marble.jpg");
	planeTexture = loadTexture("metal.png");
	grassTexture = loadTexture("blending_transparent_window.png");
}

void display() {
	//清除缓存
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//z_test设置
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//启用面剔除
	glEnable(GL_CULL_FACE);

	//着色器
	Shader zTestShader("zTestVertex.vs", "zTestFragment.fs");
	zTestShader.use();
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)800.0f / (float)600.0f, 0.1f, 100.0f);
	zTestShader.setMat4("view", view);
	zTestShader.setMat4("projection", projection);
	
	glActiveTexture(GL_TEXTURE0);

	// floor
	glBindVertexArray(planeVAO);
	glBindTexture(GL_TEXTURE_2D, planeTexture);
	zTestShader.setMat4("model", glm::mat4(1.0f));
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	// cubes
	glBindVertexArray(cubeVAO);
	glBindTexture(GL_TEXTURE_2D, cubeTexture);
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
	zTestShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
	zTestShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDisable(GL_CULL_FACE);

	//透明着色器
	Shader alphaShader("alphaVertex.vs", "alphaFragment.fs");
	alphaShader.use();
	alphaShader.setMat4("view",view);
	alphaShader.setMat4("projection", projection);
	//grass
	glBindVertexArray(grassVAO);
	glBindTexture(GL_TEXTURE_2D,grassTexture);
	for (unsigned int i = 0; i < vegetation.size(); i++) {
		float distance = glm::length(camera.Position - vegetation[i]);
		stored[distance] = vegetation[i];
	}
	for (std::map<float, glm::vec3>::reverse_iterator it = stored.rbegin(); it != stored.rend();++it) {
		model = glm::mat4(1.0f);
		model = glm::translate(model, it->second);
		alphaShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	

	glutSwapBuffers();
	deltaTime = time - lastFrame;
	lastFrame = time;
	

	//帧数计数器
	cnt++;

	//释放申请的内存
	zTestShader.close();
	alphaShader.close();
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

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
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


