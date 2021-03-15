#define GLEW_STATIC
#include <iostream>
#include <cmath>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader1.h"
#include "Camera.h"


// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void do_movement();
void renderQuad();
unsigned int loadTexture(char const* path);
unsigned int loadCubemap(std::vector <std::string> faces);

const GLuint WIDTH = 800, HEIGHT = 600;
float heightScale = 0.1;
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool    keys[1024];
glm::vec3 lightPos1(0.0f, 3.0f, 0.0f);
glm::vec3 lightPos2(3.5f, 3.0f, 2.5f);
GLfloat deltaTime = 0.0f;	
GLfloat lastFrame = 0.0f;  	

int main()
{
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Vitushkin305", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glewExperimental = GL_TRUE;
    glewInit();
    glViewport(0, 0, WIDTH, HEIGHT);
    glEnable(GL_DEPTH_TEST);


    // Shaders
    std::string path1 = "C:\\Denis\\dp\\dp_project\\Project1\\";
    Shader lightingShader(path1 + "lighting.vs", path1 + "lighting.fs");
    Shader lampShader(path1 + "lamp.vs", path1 + "lamp.fs");
    Shader texsh(path1 + "texsh.vs", path1 + "texsh.fs");
    Shader skyboxShader(path1 + "skybox.vs", path1 + "skybox.fs");
    Shader cubeShader(path1 + "cubesh.vs", path1 + "cubesh.fs");
    Shader Normalmap(path1 + "NormalMapping.vs", path1 + "NormalMapping.fs");
    Shader Parallax(path1 + "Parallax.vs", path1 + "Parallax.fs");


    //vertex data
    GLfloat bigwall[] = {
        -10.0f, -1.5f, -0.1f,  0.0f,  0.0f, -1.0f,
         10.0f, -1.5f, -0.1f,  0.0f,  0.0f, -1.0f,
         10.0f,  1.5f, -0.1f,  0.0f,  0.0f, -1.0f,
         10.0f,  1.5f, -0.1f,  0.0f,  0.0f, -1.0f,
        -10.0f,  1.5f, -0.1f,  0.0f,  0.0f, -1.0f,
        -10.0f, -1.5f, -0.1f,  0.0f,  0.0f, -1.0f,

        -10.0f, -1.5f,  0.1f,  0.0f,  0.0f,  1.0f,
         10.0f, -1.5f,  0.1f,  0.0f,  0.0f,  1.0f,
         10.0f,  1.5f,  0.1f,  0.0f,  0.0f,  1.0f,
         10.0f,  1.5f,  0.1f,  0.0f,  0.0f,  1.0f,
        -10.0f,  1.5f,  0.1f,  0.0f,  0.0f,  1.0f,
        -10.0f, -1.5f,  0.1f,  0.0f,  0.0f,  1.0f,

        -10.0f,  1.5f,  0.1f, -1.0f,  0.0f,  0.0f,
        -10.0f,  1.5f, -0.1f, -1.0f,  0.0f,  0.0f,
        -10.0f, -1.5f, -0.1f, -1.0f,  0.0f,  0.0f,
        -10.0f, -1.5f, -0.1f, -1.0f,  0.0f,  0.0f,
        -10.0f, -1.5f,  0.1f, -1.0f,  0.0f,  0.0f,
        -10.0f,  1.5f,  0.1f, -1.0f,  0.0f,  0.0f,

         10.0f,  1.5f,  0.1f,  1.0f,  0.0f,  0.0f,
         10.0f,  1.5f, -0.1f,  1.0f,  0.0f,  0.0f,
         10.0f, -1.5f, -0.1f,  1.0f,  0.0f,  0.0f,
         10.0f, -1.5f, -0.1f,  1.0f,  0.0f,  0.0f,
         10.0f, -1.5f,  0.1f,  1.0f,  0.0f,  0.0f,
         10.0f,  1.5f,  0.1f,  1.0f,  0.0f,  0.0f,

        -10.0f, -1.5f, -0.1f,  0.0f, -1.0f,  0.0f,
         10.0f, -1.5f, -0.1f,  0.0f, -1.0f,  0.0f,
         10.0f, -1.5f,  0.1f,  0.0f, -1.0f,  0.0f,
         10.0f, -1.5f,  0.1f,  0.0f, -1.0f,  0.0f,
        -10.0f, -1.5f,  0.1f,  0.0f, -1.0f,  0.0f,
        -10.0f, -1.5f, -0.1f,  0.0f, -1.0f,  0.0f,

        -10.0f,  1.5f, -0.1f,  0.0f,  1.0f,  0.0f,
         10.0f,  1.5f, -0.1f,  0.0f,  1.0f,  0.0f,
         10.0f,  1.5f,  0.1f,  0.0f,  1.0f,  0.0f,
         10.0f,  1.5f,  0.1f,  0.0f,  1.0f,  0.0f,
        -10.0f,  1.5f,  0.1f,  0.0f,  1.0f,  0.0f,
        -10.0f,  1.5f, -0.1f,  0.0f,  1.0f,  0.0f
    };
    GLfloat vertices1[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
    float texcube[] = {
                            
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };
    float floorVertices[] = {
         10.0f, -0.5f,  20.0f,  0.0f, 1.0f, 0.0f,   10.0f,  0.0f,
        -10.0f, -0.5f,  20.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -10.0f, -0.5f, -20.0f,  0.0f, 1.0f, 0.0f,   0.0f,  10.0f,

         10.0f, -0.5f,  20.0f,  0.0f, 1.0f, 0.0f,   10.0f,  0.0f,
        -10.0f, -0.5f, -20.0f,  0.0f, 1.0f, 0.0f,   0.0f,  10.0f,
         10.0f, -0.5f, -20.0f,  0.0f, 1.0f, 0.0f,   10.0f,  10.0f
    };
    float skyboxVertices[] = {     
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    float cubeVertices[] = {
              
        1.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         2.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         2.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         2.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        1.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        1.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        1.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         2.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         2.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         2.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        1.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        1.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        1.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        1.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        1.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        1.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        1.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        1.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         2.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         2.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         2.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         2.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         2.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         2.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        1.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         2.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         2.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         2.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        1.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        1.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        1.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         2.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         2.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         2.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        1.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        1.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    //bigwall
    GLuint bigwallVBO, bigwallVAO;
    glGenVertexArrays(1, &bigwallVAO);
    glGenBuffers(1, &bigwallVBO);
    glBindVertexArray(bigwallVAO);
    glBindBuffer(GL_ARRAY_BUFFER, bigwallVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bigwall), bigwall, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));


    //lamp
    GLuint lampVBO, lampVAO;
    glGenVertexArrays(1, &lampVAO);
    glGenBuffers(1, &lampVBO);
    glBindVertexArray(lampVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lampVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

    //texcube
    GLuint texcubeVBO, texcubeVAO;
    glGenVertexArrays(1, &texcubeVAO);
    glGenBuffers(1, &texcubeVBO);
    glBindVertexArray(texcubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, texcubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texcube), texcube, GL_STATIC_DRAW);
    glBindVertexArray(texcubeVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //floor
    GLuint floorVAO, floorVBO;
    glGenVertexArrays(1, &floorVAO);
    glGenBuffers(1, &floorVBO);
    glBindVertexArray(floorVAO);
    glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), &floorVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    //skybox
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


    //mirror
    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    //skybox textures
    std::vector<std::string> faces
    {
        "C:\\Denis\\dp\\dp_project\\Project1\\right.jpg",
        "C:\\Denis\\dp\\dp_project\\Project1\\left.jpg",
        "C:\\Denis\\dp\\dp_project\\Project1\\top.jpg",
        "C:\\Denis\\dp\\dp_project\\Project1\\bottom.jpg",
        "C:\\Denis\\dp\\dp_project\\Project1\\front.jpg",
        "C:\\Denis\\dp\\dp_project\\Project1\\back.jpg",
    };
    
    //textures
    unsigned int cubemapTexture = loadCubemap(faces);
    unsigned int diffuseMap = loadTexture("C:\\Denis\\dp\\dp_project\\Project1\\container2.png");
    unsigned int floor = loadTexture("C:\\Denis\\dp\\dp_project\\Project1\\floor.png");
    unsigned int diffuseMap2 = loadTexture("C:\\Denis\\dp\\dp_project\\Project1\\brickwall_diff.png");
    unsigned int normalMap2 = loadTexture("C:\\Denis\\dp\\dp_project\\Project1\\brickwall_normal.png");
    unsigned int highMap3 = loadTexture("C:\\Denis\\dp\\dp_project\\Project1\\brickwall_high.png");

    //activating shaders
    texsh.Use();
    texsh.setInt("material.diffuse", 0);
    cubeShader.Use();
    cubeShader.setInt("skybox", 0);
    skyboxShader.Use();
    skyboxShader.setInt("skybox", 0);
    Normalmap.Use();
    Normalmap.setInt("diffuseMap", 0);
    Normalmap.setInt("normalMap", 1);
    Parallax.Use();
    Parallax.setInt("diffuseMap", 0);
    Parallax.setInt("normalMap", 1);
    Parallax.setInt("depthMap", 2);
    //game loop
    while (!glfwWindowShouldClose(window))
    {

        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();
        do_movement();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 projection = glm::perspective(camera.Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
        view = camera.GetViewMatrix();

        //Parallax map effect----------------
        Parallax.Use();
        Parallax.setMat4("projection", projection);
        Parallax.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0f));
        model = glm::rotate(model, -45.0f, glm::vec3(0.0, 1.0, 0.0));
        Parallax.setMat4("model", model);
        Parallax.setVec3("viewPos", camera.Position);
        Parallax.setVec3("lightPos", lightPos1);
        Parallax.setFloat("heightScale", heightScale);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap2);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, normalMap2);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, highMap3);
        renderQuad();
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos1);
        model = glm::scale(model, glm::vec3(0.1f));
        Parallax.setMat4("model", model);
        renderQuad();


        //Normal map effect-----------------------
        Normalmap.Use();
        Normalmap.setMat4("projection", projection);
        Normalmap.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-2.0f, 0.0f, 1.0f));
        model = glm::rotate(model, 45.0f, glm::vec3(0.0, 1.0, 0.0));
        Normalmap.setMat4("model", model);
        Normalmap.setVec3("viewPos", camera.Position);
        Normalmap.setVec3("lightPos", lightPos1);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap2);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, normalMap2);
        renderQuad();

        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos1);
        model = glm::scale(model, glm::vec3(0.1f));
        Normalmap.setMat4("model", model);
        renderQuad();
        //----------------------------------------

        //Texture effect-------------------------
        texsh.Use();
        texsh.setVec3("light.position", lightPos1);
        texsh.setVec3("viewPos", camera.Position);
        texsh.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
        texsh.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
        texsh.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        texsh.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
        texsh.setFloat("material.shininess", 64.0f);
        texsh.setMat4("projection", projection);
        texsh.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        texsh.setMat4("model", model);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glBindVertexArray(texcubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //-----------------------------------------

        //floor-----------------------------------
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
        texsh.setMat4("model", model);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, floor);
        glBindVertexArray(floorVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //----------------------------------------

        lightingShader.Use();
        lightingShader.setVec3("light.position", lightPos1);
        lightingShader.setVec3("light.ambient", 0.3f, 0.3f, 0.3f);
        lightingShader.setVec3("light.diffuse", 0.3f, 0.3f, 0.3f);
        lightingShader.setVec3("light.specular", 0.3f, 0.3f, 0.3f);
        lightingShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
        lightingShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
        lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
        lightingShader.setFloat("material.shininess", 16.0f);
        lightingShader.setMat4("view", view);
        lightingShader.setMat4("projection", projection);

        //building walls--------------------------------
        glBindVertexArray(bigwallVAO);
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -10.0f));
        lightingShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        // bigwall2
        glBindVertexArray(bigwallVAO);
        model = glm::mat4();
        model = glm::rotate(model, 90.0f, glm::vec3(0.0, 1.0, 0.0));
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 10.0f));
        lightingShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        // bigwall3
        glBindVertexArray(bigwallVAO);
        model = glm::mat4();
        model = glm::rotate(model, 90.0f, glm::vec3(0.0, 1.0, 0.0));
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -10.0f));
        lightingShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        //---------------------------------------------

        //mirror effect-------------------------------
        cubeShader.Use();
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(-2.0f, 0.0f, 7.0f));
        cubeShader.setMat4("model", model);
        cubeShader.setMat4("view", view);
        cubeShader.setMat4("projection", projection);
        cubeShader.setVec3("cameraPos", camera.Position);
        glBindVertexArray(cubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        //lamp-----------------------------------------
        lampShader.Use();
        lampShader.setMat4("view", view);
        lampShader.setMat4("projection", projection);
        glBindVertexArray(lampVAO);
        model = glm::mat4();
        model = glm::translate(model, lightPos1);
        model = glm::scale(model, glm::vec3(0.4f));
        lampShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);



        //skybox effect------------------------------------
        glDepthFunc(GL_LEQUAL);  
        skyboxShader.Use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix())); 
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); 
        glBindVertexArray(lampVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //------------------------------------------------
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}

//keyboard----------------------------
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
}
//-movement-------------------------------
void do_movement()
{
    // Camera controls
    if (keys[GLFW_KEY_W])
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S])
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A])
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D])
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = SOIL_load_image(path, &width, &height, &nrComponents, 0);
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

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        SOIL_free_image_data(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        SOIL_free_image_data(data);
    }

    return textureID;


}

unsigned int loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = SOIL_load_image(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            SOIL_free_image_data(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            SOIL_free_image_data(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}
unsigned int quadVAO = 0;
unsigned int quadVBO;

void renderQuad()
{
    if (quadVAO == 0)
    {
        // positions
        glm::vec3 pos1(-1.0f, 1.0f, 0.0f);
        glm::vec3 pos2(-1.0f, -1.0f, 0.0f);
        glm::vec3 pos3(1.0f, -1.0f, 0.0f);
        glm::vec3 pos4(1.0f, 1.0f, 0.0f);
        // texture coordinates
        glm::vec2 uv1(0.0f, 1.0f);
        glm::vec2 uv2(0.0f, 0.0f);
        glm::vec2 uv3(1.0f, 0.0f);
        glm::vec2 uv4(1.0f, 1.0f);
        // normal vector
        glm::vec3 nm(0.0f, 0.0f, 1.0f);

        // calculate tangent/bitangent vectors of both triangles
        glm::vec3 tangent1, bitangent1;
        glm::vec3 tangent2, bitangent2;
        // triangle 1
        // ----------
        glm::vec3 edge1 = pos2 - pos1;
        glm::vec3 edge2 = pos3 - pos1;
        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

        bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

        // triangle 2
        // ----------
        edge1 = pos3 - pos1;
        edge2 = pos4 - pos1;
        deltaUV1 = uv3 - uv1;
        deltaUV2 = uv4 - uv1;

        f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);


        bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);


        float quadVertices[] = {
            // positions            // normal         // texcoords  // tangent                          // bitangent
            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
            pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
            pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
        };
        // configure plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}