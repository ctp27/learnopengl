//
//  main.cpp
//  OpenGLCherno
//
//  Created by Clinton Pinto on 8/10/21.
//
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filePath)
{
    std::ifstream stream(filePath);
    
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };
    
    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }
    
    return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    // Creates a shader program and assigns an id to it
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    
    // Sets the source code to the shader. Specify the id obtained while creating it.
    // Clears all previously set shader data
    glShaderSource(id, 1, &src, nullptr);
    
    // Compiles the source code set in the specified shader.
    glCompileShader(id);
    
    // TODO: Error handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << message << std:: endl;
        std::cout << message << std:: endl;
        glDeleteShader(id);
        return 0;
    }
    
    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    
    glLinkProgram(program);
    glValidateProgram(program);
    
    // Deletes intermediate obj c++ files for the shaders since
    // they are now attached to the program. Do this to clean up.
    glDeleteShader(vs);
    glDeleteShader(fs);
    
    return program;
}

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
    
    float positions[6] = {
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f
    };
    
    // Stores the id of the buffer.
    unsigned int buffer;
    unsigned int vao;
    
    // Required for mac os to draw a triangle
    glGenVertexArrays(1, &vao);
    
    // Generate a buffer where 1 is count and a pointer to an integer which stores
    // the id of the created buffer.
    glGenBuffers(1, &buffer);
    
    // Required for mac os to draw a triangle
    glBindVertexArray(vao);
    
    // Use this function to select the buffer created in the previous step.
    // Binding is selecting. We want an array buffer, and pass in the ID of the buffer.
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    
    // Specify the data for the buffer. and how it will be accessed.
    // See documentation for function docs.gl
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
        
//    TODO::Figure out how to use relative paths in Xcode
    ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");

    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    
    //    Clean up by removing the shader program
    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}
