#include <iostream>
#include "Header_Files/stb_img.h"
#include "glad.c"
#include <GLFW/glfw3.h>
#include <math.h>
#include <filesystem>
#include <fstream>
#include <vector>
#include "Header_Files/glm/glm.hpp"
#include "Header_Files/glm/gtc/matrix_transform.hpp"
#include "Header_Files/glm/gtc/type_ptr.hpp"
// #include "Header_Files/stb_img.h"
#include "texture.cpp"
#include "Camera.cpp"
#include "stb.cpp"
#include "shadderClass.cpp"
#include "VAO.cpp"
#include "EBO.cpp"
#include "VBO.cpp"

// the shaders for the triangle that you will be showing

// float vel = 0.05f, x = 0.0f, y = 0.0f;

/* void move(GLFWwindow *window)
{
	// printf("moving");
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		y += vel;
		printf("%d", 1);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		y -= vel;
		printf("%d", 2);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		x -= vel;
		printf("%d", 3);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		x += vel;
		printf("%d", 4);
	}
} */

const char *vertex_shader_source = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTex;
layout (location = 3) in vec3 aNormal;
out vec3 color;
out vec2 texCoord;
out vec3 Normal;
out vec3 currPos; 
uniform mat4 camMatrix;
uniform mat4 model; 
void main()
{
   
	currPos = vec3(model* vec4( aPos , 1.0f));
	gl_Position = camMatrix * vec4(currPos, 1.0) ;
	color = aColor;
	texCoord = aTex;
	Normal = aNormal;
}
)";
const char *fragment_shader_source = R"(
#version 330 core
out vec4 FragColor;
in vec3 color;
in vec2 texCoord;
in vec3 Normal; 
in vec3 currPos;
uniform sampler2D tex0;
uniform sampler2D tex1;
uniform vec4 lightColor;
uniform vec3 lightPos; 
uniform vec3 camPos; 
void main()
{
 	float ambient = 0.20f; 
	vec3 normal = normalize(Normal); 
	vec3 lightDirection  = normalize(lightPos-currPos);	
	float diffuse = max(dot(normal,lightDirection),0.0f);
	float specularLight = 0.60f; 
	vec3 viewDirection  = normalize( camPos - currPos);
    vec3 reflectionDirection  =reflect( -lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection),0.0f),20);
	float specular = specularLight* specAmount;
	FragColor = texture(tex0, texCoord) * lightColor * (diffuse + ambient)+(texture(tex1, texCoord).r*specular);
}
)";

const char *light_vertex_shader_source = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 model;
uniform mat4 camMatrix;
void main()
{
	gl_Position = camMatrix * model * vec4(aPos, 1.0f);
}
)";
const char *light_fragment_shader_source = R"(
#version 330 core
out vec4 FragColor;
uniform vec4 lightColor;
void main()
{
	FragColor = lightColor;
}
)";

// Vertices coordinates
GLfloat vertices[] =
	{
		//     COORDINATES     /        COLORS          /    TexCoord   /        NORMALS       //
		-0.5f, 0.0f, 0.5f, 0.83f, 0.70f, 0.44f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,	// Bottom side
		-0.5f, 0.0f, -0.5f, 0.83f, 0.70f, 0.44f, 0.0f, 5.0f, 0.0f, -1.0f, 0.0f, // Bottom side
		0.5f, 0.0f, -0.5f, 0.83f, 0.70f, 0.44f, 5.0f, 5.0f, 0.0f, -1.0f, 0.0f,	// Bottom side
		0.5f, 0.0f, 0.5f, 0.83f, 0.70f, 0.44f, 5.0f, 0.0f, 0.0f, -1.0f, 0.0f,	// Bottom side

		-0.5f, 0.0f, 0.5f, 0.83f, 0.70f, 0.44f, 0.0f, 0.0f, -0.8f, 0.5f, 0.0f,	// Left Side
		-0.5f, 0.0f, -0.5f, 0.83f, 0.70f, 0.44f, 5.0f, 0.0f, -0.8f, 0.5f, 0.0f, // Left Side
		0.0f, 0.8f, 0.0f, 0.92f, 0.86f, 0.76f, 2.5f, 5.0f, -0.8f, 0.5f, 0.0f,	// Left Side

		-0.5f, 0.0f, -0.5f, 0.83f, 0.70f, 0.44f, 5.0f, 0.0f, 0.0f, 0.5f, -0.8f, // Non-facing side
		0.5f, 0.0f, -0.5f, 0.83f, 0.70f, 0.44f, 0.0f, 0.0f, 0.0f, 0.5f, -0.8f,	// Non-facing side
		0.0f, 0.8f, 0.0f, 0.92f, 0.86f, 0.76f, 2.5f, 5.0f, 0.0f, 0.5f, -0.8f,	// Non-facing side

		0.5f, 0.0f, -0.5f, 0.83f, 0.70f, 0.44f, 0.0f, 0.0f, 0.8f, 0.5f, 0.0f, // Right side
		0.5f, 0.0f, 0.5f, 0.83f, 0.70f, 0.44f, 5.0f, 0.0f, 0.8f, 0.5f, 0.0f,  // Right side
		0.0f, 0.8f, 0.0f, 0.92f, 0.86f, 0.76f, 2.5f, 5.0f, 0.8f, 0.5f, 0.0f,  // Right side

		0.5f, 0.0f, 0.5f, 0.83f, 0.70f, 0.44f, 5.0f, 0.0f, 0.0f, 0.5f, 0.8f,  // Facing side
		-0.5f, 0.0f, 0.5f, 0.83f, 0.70f, 0.44f, 0.0f, 0.0f, 0.0f, 0.5f, 0.8f, // Facing side
		0.0f, 0.8f, 0.0f, 0.92f, 0.86f, 0.76f, 2.5f, 5.0f, 0.0f, 0.5f, 0.8f	  // Facing side
};
// Indices for vertices order
GLuint indices[] =
	{
		0, 1, 2,	// Bottom side
		0, 2, 3,	// Bottom side
		4, 6, 5,	// Left side
		7, 9, 8,	// Non-facing side
		10, 12, 11, // Right side
		13, 15, 14	// Facing side
};

GLfloat lightVertices[] =
	{ //     COORDINATES     //
		-0.1f, -0.1f, 0.1f,
		-0.1f, -0.1f, -0.1f,
		0.1f, -0.1f, -0.1f,
		0.1f, -0.1f, 0.1f,
		-0.1f, 0.1f, 0.1f,
		-0.1f, 0.1f, -0.1f,
		0.1f, 0.1f, -0.1f,
		0.1f, 0.1f, 0.1f};

GLuint lightIndices[] =
	{
		0, 1, 2,
		0, 2, 3,
		0, 4, 7,
		0, 7, 3,
		3, 7, 6,
		3, 6, 2,
		2, 6, 5,
		2, 5, 1,
		1, 5, 4,
		1, 4, 0,
		4, 5, 6,
		4, 6, 7};

const int height = 800;
const int width = 800;
int main()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	GLFWwindow *window = glfwCreateWindow(800, 800, "prac", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	// Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, 800, 800);

	// Generates Shader object using shaders defualt.vert and default.frag
	Shader shaderProgram(vertex_shader_source, fragment_shader_source);

	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(indices, sizeof(indices));

	// Links VBO to VA11
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void *)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void *)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void *)(6 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void *)(8 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO1.UnBind();
	VBO1.UnBind();
	EBO1.UnBind();

	// light object
	Shader lightShader(light_vertex_shader_source, light_fragment_shader_source);

	// Generates Vertex Array Object and binds it
	VAO lightVAO1;
	lightVAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO lightVBO1(lightVertices, sizeof(lightVertices));
	// Generates Element Buffer Object and links it to indices
	EBO lightEBO1(lightIndices, sizeof(lightIndices));

	// Links VBO to VAO
	lightVAO1.LinkAttrib(lightVBO1, 0, 3, GL_FLOAT, 3 * sizeof(float), (void *)0);
	/* 	lightVAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void *)(3 * sizeof(float)));
		lightVAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void *)(6 * sizeof(float))); */
	// Unbind all to prevent accidentally modifying them
	lightVAO1.UnBind();
	lightVBO1.UnBind();
	lightEBO1.UnBind();

	glm ::vec4 lightColor = glm ::vec4(1.0f, 1.0f, 1.0f, 0.0f);
	// why is this part being use d??
	glm::vec3 lightPos = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	glm::vec3 pyramidPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 pyramidModel = glm::mat4(1.0f);
	pyramidModel = glm::translate(pyramidModel, pyramidPos);

	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	Texture beluga_cat("/home/steel/visual_studio/c++_projects/opengl_projects/flappy-bird/src/texture_wood.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
	beluga_cat.texUnit(shaderProgram, "tex0", 0);
Texture beluga_cat_spec("/home/steel/visual_studio/c++_projects/opengl_projects/flappy-bird/src/texture_wood.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RED, GL_UNSIGNED_BYTE);
	beluga_cat_spec.texUnit(shaderProgram, "tex1", 0);


	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);

	// roattion
	/* float rotation = 0.0f;
	double prevtime = glfwGetTime();*/
	glEnable(GL_DEPTH_TEST);
	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.updateMatrix(45.0f, 0.1f, 100.0f);
		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();
		camera.Matrix(shaderProgram, "camMatrix");
		camera.inputs(window);

		beluga_cat.Bind();
		beluga_cat_spec.Bind();
		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);

		// cube light source
		lightShader.Activate();
		camera.Matrix(lightShader, "camMatrix");
		lightVAO1.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	// Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();
	// glDeleteTextures(1, &texture);
	beluga_cat.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}