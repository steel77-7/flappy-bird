#include <iostream>
#include "glad.c"
#include <GLFW/glfw3.h>
#include <math.h>
#include "VAO.cpp"
#include "EBO.cpp"
#include "VBO.cpp"
#include "shadderClass.cpp"

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

    Shader shaderProgram("default.vert", "default.frag");
    VAO VAO1;
    VAO1.Bind();

    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    VAO1.LinkVBO(VBO1, 0);
    VAO1.UnBind();
    VBO1.UnBind();
    EBO1.UnBind();
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // move(window);
        /* Render here */
        // printf("4\n");
        glClearColor(0.25f, 0.5f, 0.75f, 0.78f);
        glClear(GL_COLOR_BUFFER_BIT);
        shaderProgram.Activate();
        VAO1.Bind();

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
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    shaderProgram.Delete();
    glfwTerminate();
    return 0;
    ;
}
