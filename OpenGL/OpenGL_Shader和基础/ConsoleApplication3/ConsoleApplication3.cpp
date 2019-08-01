// ConsoleApplication3.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//


#include "pch.h"
#include <iostream>
#include <GL\glew.h>
#include <GL\glfw3.h>
#include <GL\freeglut.h>
#include "shader.h"

#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#pragma comment(lib,"glew32.lib")

//顶点数据
GLfloat vertex[] = { 
	 -0.5f, 0.0f, 0.1f,  
	0.0f, 0.0f, 0.1f,  
	0.0f, 0.5f, 0.1f, 
};

GLfloat vertex_2[] = {
	 0.5f, 0.0f, 0.0f,   // 右上角
	0.0f, 0.0f, 0.0f,  // 右下角
	0.0f, -0.5f, 0.0f, // 左下角
};

GLfloat vertex_3[] = {
	 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
	-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
	 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
};


//绘制索引
GLuint index[] = {
	0,1,3,
	1,2,3
};

//顶点缓冲区
GLuint VBO;
GLuint VBO_2;

//索引缓冲对象
GLuint EBO;



//顶点着色器
GLuint vertexShader;
GLuint fragmentShader;
GLuint fragmentShader_2;
GLuint fragmentShader_3;
GLuint vertexShader_1;


//着色器程序
unsigned int shaderProgram;

//着色器程序_2
unsigned int shaderProgram_2;

//着色器程序_3
unsigned int shaderProgram_3;

unsigned int shaderProgram_4;

//顶点着色器源码
const char* vertexShaderSource = "#version 330 core\n"
						        "layout(location = 0) in vec3 aPos;\n"
								"out vec4 vertexColor;\n"
								"void main()\n"
								"{\n"
								"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
								"	vertexColor = vec4(0.5,0.0,0.0,1.0);\n"
								"}";


const char* vertexShaderSource_1 = "#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(aPos, 1.0);\n"
"	 ourColor = aColor;\n"
"}";

//片段着色器源码(接受顶点着色器的颜色)
const char* fragmentShaderSource = "#version 330 core\n"
								  "in vec3 ourColor;\n"
								  "out vec4 FragColor;\n"
	                              "void main()\n"
	                              "{\n"
                                  "	FragColor = vec4(ourColor, 1.0f);\n"
                                  "} ";

//片段着色器源码2(自己决定颜色)
const char* fragmentShaderSource_2 = "#version 330 core\n"
									"in vec4 vertexColor;\n"
									"out vec4 FragColor;\n"
									"void main()\n"
									"{\n"	
										"FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
									"} ";

//片段着色器源码3(uniform变量决定颜色)
const char* fragmentShaderSource_3 = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 ourColor;\n"
"void main()\n"
"{\n"
"	FragColor = ourColor;\n"
"}\n";

								
//着色器编译信息
int  success;
char infoLog[512];

//创建VAO
GLuint VAO;
GLuint VAO_2;



void init()
{
	glClearColor(1, 1, 1, 0.0);
	//编译着色器源码
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	vertexShader_1 = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader_1, 1, &vertexShaderSource_1, NULL);
	glCompileShader(vertexShader_1);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//片段着色器的创建
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	
	//片段着色器_2
	fragmentShader_2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader_2, 1, &fragmentShaderSource_2, NULL);
	glCompileShader(fragmentShader_2);

	//片段着色器_3
	fragmentShader_3 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader_3, 1, &fragmentShaderSource_3, NULL);
	glCompileShader(fragmentShader_3);

	//设置着色器程序
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//设置着色器程序2
	shaderProgram_2 = glCreateProgram();
	glAttachShader(shaderProgram_2, vertexShader);
	glAttachShader(shaderProgram_2, fragmentShader_2);
	glLinkProgram(shaderProgram_2);

	//设置着色器程序3
	shaderProgram_3 = glCreateProgram();
	glAttachShader(shaderProgram_3, vertexShader);
	glAttachShader(shaderProgram_3, fragmentShader_3);
	glLinkProgram(shaderProgram_3);

	shaderProgram_4 = glCreateProgram();
	glAttachShader(shaderProgram_4, vertexShader_1);
	glAttachShader(shaderProgram_4, fragmentShader);
	glLinkProgram(shaderProgram_4);

	//删除着色器引用
	glDeleteShader(vertexShader);
	glDeleteShader(vertexShader_1);
	glDeleteShader(fragmentShader);
	glDeleteShader(fragmentShader_2);
	glDeleteShader(fragmentShader_3);


	//设置VAO_2
	glGenVertexArrays(1, &VAO_2);

	//设置VBO_2
	glGenBuffers(1, &VBO_2);
	glBindVertexArray(VAO_2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_3), vertex_3, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 1);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//激活着色器程序
	//时间信息
	/*GLfloat timeValue = glutGet(GLUT_ELAPSED_TIME);
	float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
	glUseProgram(shaderProgram_3);
	int vertexColorLocation = glGetUniformLocation(shaderProgram_3, "ourColor");
	glUniform4f(vertexColorLocation, 0.0f,greenValue, 0.0f, 1.0f);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);*/
	//glDisableVertexAttribArray(0);
	//glDeleteProgram(shaderProgram);

	//glUniform4f(vertexColorLocation, greenValue, 0.0f, 0.0f, 1.0f);
	Shader sp_1("Vertex_1.vs", "fragment_1.fs");
	sp_1.use();
	//glUseProgram(shaderProgram_4);
	glBindVertexArray(VAO_2);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	//glDisableVertexAttribArray(0);
	
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);
	//glBindVertexArray(0);
	glFlush();
	glutSwapBuffers();	
}

void Idle()
{
	Sleep(60);
	display();
}

int main(int argc, char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(400, 300);
	glutCreateWindow("顶点");

	
	GLuint result = glewInit();
	if (result != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(result));
		return 1;
	}
	init();
	glutDisplayFunc(display);
	glutIdleFunc(Idle);
	glutMainLoop();
	return 0;
}