// CoreGameEngine.cpp : Defines the entry point for the console application.
//


#if __APPLE__
#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <string.h>

#else
#include "../Dependencies/glew/glew.h"
#include "../Dependencies/freeglut/freeglut.h"
#include <string>
#endif

#include <iostream>
#include <fstream>
#include "math_3d.h"

using namespace std;

GLuint VBO;

static const char* pVS = "                                                    \n\
#version 330                                                                  \n\
\n\
layout (location = 0) in vec3 Position;                                       \n\
\n\
void main()                                                                   \n\
{                                                                             \n\
gl_Position = vec4(0.5 * Position.x, 0.5 * Position.y, Position.z, 1.0);  \n\
}";

static const char* pFS = "                                                    \n\
#version 330                                                                  \n\
\n\
out vec4 FragColor;                                                           \n\
\n\
void main()                                                                   \n\
{                                                                             \n\
FragColor = vec4(1.0, 0.0, 0.0, 1.0);                                     \n\
}";


// Get file length
unsigned long GetFileLength()
{
    return 1;
}

// Load Shader
static string LoadShader(string filename){
    
    std::ifstream file;
    file.open(filename, ios::in);
    string line;
    if (!file) return "error";
    
    string shader;
    while (std::getline(file, line)){
        shader += line;
        shader += "/n";
    }
    
    return shader;
}



static void CreateVertexBuffer()
{
    Vector3f Vertices[3];
    Vertices[0] = Vector3f(-1.0f, -1.0f, 0.0f);
    Vertices[1] = Vector3f(1.0f, -1.0f, 0.0f);
    Vertices[2] = Vector3f(0.0f, 1.0f, 0.0f);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}

static void AddShader(GLuint shaderProgram, const char *pShaderText, GLenum shaderType){
    GLuint shaderObj = glCreateShader(shaderType);
    
    if (shaderObj == 0){
        fprintf(stderr, "Error creating shader of type: %d\n", shaderType);
        exit(0);
    }
    
    // Create and Compile Shader
    const GLchar *p[1];
    p[0] = pShaderText;
    
    GLint Lengths[1];
    Lengths[0] = (GLint) strlen(pShaderText);
    
    glShaderSource(shaderObj, 1, p, Lengths);
    glCompileShader(shaderObj);
    
    // Check shader
    GLint success;
    glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &success);
    if (!success){
        GLchar log[1024];
        glGetShaderInfoLog(shaderObj, sizeof(log), NULL, log);
        fprintf(stderr, "Error compiling shader of type %d: %s\n", shaderType, log);
        exit(1);
    }
    
    // Attach shader to program
    glAttachShader(shaderProgram, shaderObj);
}

static void CompileShader() {
    GLuint shaderProgram = glCreateProgram();
    
    if (shaderProgram == 0){
        fprintf(stderr, "Error creating shader program\n");
        exit(1);
    }
    
    // Add shaders to program
    string vertexShader = LoadShader("simpleshader.vert");
    string fragmentShader = LoadShader("simpleshader.frag");
    AddShader(shaderProgram, pVS, GL_VERTEX_SHADER);
    AddShader(shaderProgram, pFS, GL_FRAGMENT_SHADER);
    
    GLint success = 0;
    GLchar ErrorLog[1024] = {0};
    
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (success == 0) {
        glGetProgramInfoLog(shaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
        exit(1);
    }
    
    glValidateProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
        exit(1);
    }
    
    
    // Send prgram into opengl
    glUseProgram(shaderProgram);
}


#if __APPLE__
const int WIDTH = 800, HEIGHT = 600;
int main(int argc, char** argv)
{
    // GLFW (OSX doesn't support oGL 3.3 on GLUT)
    glfwInit();
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "GameEngine", nullptr, nullptr);
    int screenWidth, screenHeight;
    glfwGetWindowSize(window, &screenWidth, &screenHeight);
    
    if (window == nullptr) {
        fprintf(stderr, "Failed to create window");
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    
    fprintf(stderr, "GLSL V: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
    
    GLenum res = glewInit();
    if (res != GLEW_OK)
    {
        std::cout << "Error: %s\n" << glewGetErrorString(res) << std::endl;
        return -1;
    }
    
    glViewport(0, 0, screenWidth, screenHeight);
    
    // Create the vertexs
    CreateVertexBuffer();
    
    // Create and compile shaders
    CompileShader();
    
    // Main Game Loop
    while (!glfwWindowShouldClose(window)){
        // Loop for input events
        glfwPollEvents();
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glDisableVertexAttribArray(0);
        
        glfwSwapBuffers(window);
    }
    
    glfwTerminate();
    

    
    return 0;
}

#else

void renderScene(void)
{
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    
    glDisableVertexAttribArray(0);
    
    glutSwapBuffers();
}

static void InitializeGlutCallbacks()
{
    glutDisplayFunc(renderScene);
}

int main(int argc, char** argv)
{
    // Init glut
    glutInit(&argc, argv);

    // Configure Display

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    
    glutInitWindowSize(1024, 768);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("GameEngine");

    fprintf(stderr, "GLSL V: %s", glGetString(GL_VERSION));
    
    // register callbacks
    InitializeGlutCallbacks();

    GLenum res = glewInit();
    if (res != GLEW_OK)
    {
        std::cout << "Error: %s\n" << glewGetErrorString(res) << std::endl;
        return 1;
    }
    
    // Clear screen
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    
    // Create the vertexs
    CreateVertexBuffer();

    // Create and compile shaders
    CompileShader();

    glutMainLoop();
    return 0;
}

#endif
