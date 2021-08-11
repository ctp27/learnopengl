//
//  main.cpp
//  OpenGLCherno
//
//  Created by Clinton Pinto on 8/10/21.
//
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main(int argc, const char * argv[]) {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR,  3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR,  2);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    
    std::cout << glGetString(GL_VERSION) << std::endl;
    
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error intializing glew" << std::endl;
    }
    
    static const GLfloat red[] = {1.0f, 0.0f, 0.0f, 1.0f};
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        glClearBufferfv(GL_COLOR, 0, red);
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
