#include <iostream>
#include <glad/glad.h>
#include "glad.c"
#include <GLFW/glfw3.h>
#include <math.h>

// the shaders for the triangle that you will be showing
const char *vertex_shader_source = R"(
#version 330 core
layout(location = 0) in vec2 aPos;
uniform vec2 offset;
void main() {
    gl_Position = vec4(aPos + offset, 0.0, 1.0);
}
)";
const char *fragment_shader_source = R"(
#version 330 core
out vec4 FragColor;
void main() {
    FragColor = vec4(0.8f,0.3f,0.02f,1.0f); 
}
)";

float vel = 0.05f, x = 0.0f, y = 0.0f;

void move(GLFWwindow *window)
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
}
int main(void)
{

    // printf("1\n");
    /* Initialize the GLFW library */
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    //
    GLFWwindow *window; // pointer of the glfw window type to initialise the window with its types and shit
    window = glfwCreateWindow(800, 800, "Flappy Bird", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    // Load OpenGL functions using GLAD
    gladLoadGL();

    // vertices for the triangle
    GLfloat vertices[] = {
        -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
        0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
        0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f,
        -0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,
        0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,
        0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f};

    GLuint indices[] = {
        0, 3, 5};

    // integrating the shaders in the main program

    // compiling the vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertexShader);

    // compling the fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragmentShader);

    // binding both the shaders in same unit shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram); // wrapping shadders to program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    // printf("2\n");

    // sending the data to the CPU in small packages using buffers
    GLuint VAO, VBO, EBO;       // all the vertex buffers
    glGenVertexArrays(1, &VAO); // vertex array object
    glGenBuffers(1, &VBO);      // vertex buffer object
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    // glBindBuffer(GL_ARRAY_BUFFER, VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // you didnt bind ebo to the buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    // printf("3");
    /* Create a windowed mode window and its OpenGL context */

    // Set the initial background color

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        move(window);
        /* Render here */
        // printf("4\n");
        glClearColor(0.25f, 0.5f, 0.75f, 0.78f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        // printf("5\n");
        int offsetlocation = glGetUniformLocation(shaderProgram, "offset");
        printf("%d", offsetlocation);
        glUniform2f(offsetlocation, x, y);
        // the drawing happens here
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // printf("6\n");
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        // printf("7\n");
        /* event lsitening function */
        glfwPollEvents();
    }
    // deleting the arrays and buffers created
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
    ;
}
