#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Program.h"

const unsigned int DEF_WIDTH = 800;
const unsigned int DEF_HEIGHT = 600;

void resize_callback(GLFWwindow *window, int width, int height);

void initGLFW();
bool initWindow(GLFWwindow **window, int width, int height);
bool initGLAD();

void processInput(GLFWwindow *window);

unsigned int TriBuf, EleBuf;
unsigned int TriArr;
std::shared_ptr<Program> shader;

void initProg()
{
	shader = std::make_shared<Program>();
	shader->init("triangle.vert", "triangle.frag");
}

void initGeom()
{
	// create triangle vertices
	float vertices[] = {
		 0.5f,  0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	// create VAO
	glGenVertexArrays(1, &TriArr);
	glBindVertexArray(TriArr);

	// create VBOs
	glGenBuffers(1, &TriBuf);
	glBindBuffer(GL_ARRAY_BUFFER, TriBuf);
	
	glGenBuffers(1, &EleBuf);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EleBuf);
	
	// bind vertices to VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// binding locations
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
}

void render()
{
	// bind stuff
	shader->bind();
	glBindVertexArray(TriArr);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EleBuf);

	// generate and assign color
	float time = glfwGetTime();
	float green = (sin(time) / 2.0f) + 0.5f;
	int colorLoc = glGetUniformLocation(shader->progID, "ourColor");
	glUniform4f(colorLoc, 0.0f, green, 0.0f, 1.0f);

	// draw
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	shader->unbind();
}

int main()
{
	GLFWwindow *window;

	// init GLFW & GLAD
	initGLFW();
	if (!initWindow(&window, 800, 600) || !initGLAD())
	{
		return -1;
	}

	// init GL objects
	initGeom();
	initProg();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void resize_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void initGLFW()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

bool initWindow(GLFWwindow **window, int width, int height)
{
	*window = glfwCreateWindow(DEF_WIDTH, DEF_HEIGHT, "LearnOpenGL", NULL, NULL);

	if (*window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(*window);
	glfwSetFramebufferSizeCallback(*window, resize_callback);

	return true;
}

bool initGLAD()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	return true;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}