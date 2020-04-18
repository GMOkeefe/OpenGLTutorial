#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

const unsigned int DEF_WIDTH = 800;
const unsigned int DEF_HEIGHT = 600;

void resize_callback(GLFWwindow *window, int width, int height);

void initGLFW();
bool initWindow(GLFWwindow **window, int width, int height);
bool initGLAD();

void processInput(GLFWwindow *window);

unsigned int TriBuf;
unsigned int shaderProg;
unsigned int vertShader, fragShader;

const char *vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";
const char *fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\0";

void init()
{
	// create triangle vertices
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

	// create VBO
	glGenBuffers(1, &TriBuf);
	glBindBuffer(GL_ARRAY_BUFFER, TriBuf);
	
	// bind vertices to VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// create shaders
	vertShader = glCreateShader(GL_VERTEX_SHADER);
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	// compile shaders
	glShaderSource(vertShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertShader);

	glShaderSource(fragShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragShader);

	// check compile status
	int success;
	char infoLog[512];

	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
			<< infoLog << std::endl;
	}

	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragShader, 412, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
			<< infoLog << std::endl;
	}

	// create shader program
	shaderProg = glCreateProgram();

	// attach shaders
	glAttachShader(shaderProg, vertShader);
	glAttachShader(shaderProg, fragShader);
	glLinkProgram(shaderProg);

	// check link status
	glGetProgramiv(shaderProg, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProg, 512, NULL, infoLog);
		std::cout << "ERROR:SHADER_PROGRAM::LINK_FAILED\n"
			<< infoLog << std::endl;
	}

	// bind program
	glUseProgram(shaderProg);
}

void render()
{
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
	init();
	
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