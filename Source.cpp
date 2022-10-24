/*
AUTHOR: ALEXANDREA TEIGELER
DATE  : 10/02/2022
SOURCE: https://github.com/SNHU-CS/CS-330/blob/master/module03/tut_03_03.cpp
ABOUT : THIS PROGRAM WILL CREATE A STILL LIFE 3D SCENE
        FIXME: THE Q E INPUT BUTTONS HAVE BEEN COMMENTED OUT OF THE CAMERA.H FILE DUE TO AN ERROR I CAN NOT FIX
        THE ERROR CAUSES THE CAMERA TO INFINATELY CALL THE UP COMMAND.

        COMPLETE: PEAR, APPLE, VASE, CHALLICE, FLOWER, TABLE

        

        KNOWN ISSUES: CAN NOT RENDER 2 OBJECTS
                      LIGHTING REMOVES TEXTURES
*/

// OPEN GL
#include <GLEW/glew.h>  
#include <GLFW/glfw3.h>  
// MATH
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// IMAGE RENDERING
#include <SOIL2/SOIL2.h>
// CAMERA
#include <learnOpengl/camera.h>
// BASIC FUNCTIONALITY
#include <iostream>
#include <cstdlib>

using namespace std;

// Shader program Macro //
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version " core \n" #Source
#endif

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


namespace
{
    const char* const WINDOW_TITLE = "Alexandrea Teigeler PROJECT";
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;

    // FOR DRAWING PRIMITIVES
    int width, height;


    struct PearMesh
    {
        GLuint pear_vao;         //vertex array object
        GLuint pear_vbo;         //vertex buffer object
        GLuint pear_num_vertices;    //Number of indices 
    };

    struct AppleMesh
    {
        GLuint apple_vao;
        GLuint apple_vbo;
        GLuint apple_num_vertices;
    };

    struct VaseMesh
    {
        GLuint vase_vao;
        GLuint vase_vbo;
        GLuint vase_num_vertices;
    };

    struct ChaliceMesh
    {
        GLuint chalice_vao;
        GLuint chalice_vbo;
        GLuint chalice_num_vertices;
    };

    struct PlaneMesh
    {
        GLuint plane_vao;
        GLuint plane_vbo;
        GLuint plane_num_vertices;
    };

    struct StemMesh
    {
        GLuint stem_vao;
        GLuint stem_vbo;
        GLuint stem_num_vertices;
    };

    struct PetalMesh
    {
        GLuint petal_vao;
        GLuint petal_vbo;
        GLuint petal_num_vertices;
    };

    // WINDOW //
    GLFWwindow* window = nullptr;
    // MESH //
    PearMesh pear_mesh;
    AppleMesh apple_mesh;
    VaseMesh vase_mesh;
    ChaliceMesh chalice_mesh;
    PlaneMesh plane_mesh;
    StemMesh stem_mesh;
    PetalMesh petal_mesh;
    // Shader //
    GLuint shader;
    GLuint light_shader;
    // TEXTURE ID //
    GLuint pear_texture_id;
    GLuint vase_texture_id;
    GLuint challice_texture_id;
    GLuint petal_texture_id;
    GLuint stem_texture_id;
    GLuint apple_texture_id;
    GLuint table_texture_id;
    glm::vec2 gUVScale(5.0f, 5.0f);
    GLint gTexWrapMode = GL_REPEAT;

    // CAMERA //
    Camera gCamera(glm::vec3(-10.0f, 20.0f, 20.0f));
    float gLastX = WINDOW_WIDTH / 2.0f;
    float gLastY = WINDOW_HEIGHT / 2.0f;
    bool gFirstMouse = true;

    // TIMING //
    float gDeltaTime = 0.0f;
    float gLastFrame = 0.0f;


    // SCENE LOCATION //
    glm::vec3 objectPos(0.0f, 0.0f, 0.0f);
    glm::vec3 objectScale(2.0f);
    glm::vec3 objectColor(0.0f, 1.0f, 0.0f);
    //LIGHT //
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos(5.0f, 10.0f, 3.0f);
    glm::vec2 lightScale(0.3f);

    bool lamp_orbit = true;
}

/* User-defined Function prototypes to:
 * initialize the program, set the window size,
 * redraw graphics on the window when resized,
 * and render graphics on the screen
 */

 // USER DEFINED FUNCTIONS //
bool UInitialize(int, char* [], GLFWwindow** window);
void UResizeWindow(GLFWwindow* window, int width, int height);
void UProcessInput(GLFWwindow* window);

void UMousePositionCallback(GLFWwindow* window, double xpos, double ypos);
void UMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void UMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

void UPearRender(PearMesh& mesh);
void UAppleRender(AppleMesh& mesh);
void UVaseRender(VaseMesh& mesh);
void UChaliceRender(ChaliceMesh& mesh);
void UPlaneRender(PlaneMesh& mesh);
void UPlaneRender(PlaneMesh& mesh, float t_x_value, float t_y_value, float t_z_value);
void UPlaneRender(PlaneMesh& mesh, float t_x_value, float t_y_value, float t_z_value, float r_x_value, float r_y_value, float r_z_value);
void UStemRender(StemMesh& mesh);
void UStemRender(StemMesh& mesh, float r_x_value, float r_y_value, float r_z_value);
void UPetalRender(PetalMesh& mesh, float t_x_value, float t_y_value, float t_z_value, float r_x_value, float r_y_value, float r_z_value);
void ULightRender();

void UCreatePear(PearMesh& mesh);
void UCreateApple(AppleMesh& mesh);
void UCreateVase(VaseMesh& mesh);
void UCreateChalice(ChaliceMesh& mesh);
void UCreatePlane(PlaneMesh& mesh);
void UCreateStem(StemMesh& mesh);
void UCreatePetal(PetalMesh& mesh);

void UDestroyMesh(PlaneMesh& mesh);
void UDestroyMesh(ChaliceMesh& mesh);
void UDestroyMesh(AppleMesh& mesh);
void UDestroyMesh(StemMesh& mesh);
void UDestroyMesh(PetalMesh& mesh);
void UDestroyMesh(VaseMesh& mesh);
void UDestroyMesh(PearMesh& mesh);

bool UCreateTexture(const char* filename, GLuint& textureId);
void UDestroyTexture(GLuint textureId);

bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId);
void UDestroyShaderProgram(GLuint programId);

/*Vertex Shader Source Code*/
const GLchar* vertexShaderSource = GLSL(440,

    layout(location = 0) in vec3 position; // VAP position 0 for vertex position data
layout(location = 1) in vec3 normal; // VAP position 1 for normals
layout(location = 2) in vec2 textureCoordinate;

out vec3 vertexNormal; // For outgoing normals to fragment shader
out vec3 vertexFragmentPos; // For outgoing color / pixels to fragment shader
out vec2 vertexTextureCoordinate;

//Uniform / Global variables for the  transform matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f); // Transforms vertices into clip coordinates

    vertexFragmentPos = vec3(model * vec4(position, 1.0f)); // Gets fragment / pixel position in world space only (exclude view and projection)

    vertexNormal = mat3(transpose(inverse(model))) * normal; // get normal vectors in world space only and exclude normal translation properties
    vertexTextureCoordinate = textureCoordinate;
}
);


/*Fragment Shader Source Code*/
const GLchar* fragmentShaderSource = GLSL(440,

    in vec3 vertexNormal; // For incoming normals
in vec3 vertexFragmentPos; // For incoming fragment position
in vec2 vertexTextureCoordinate;

out vec4 fragmentColor; // For outgoing cube color to the GPU

// Uniform / Global variables for object color, light color, light position, and camera/view position
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPosition;
uniform sampler2D uTexture; // Useful when working with multiple textures
uniform vec2 uvScale;

void main()
{
    /*Phong lighting model calculations to generate ambient, diffuse, and specular components*/

    //Calculate Ambient lighting*/
    float ambientStrength = 0.5f; // Set ambient or global lighting strength
    vec3 ambient = ambientStrength * lightColor; // Generate ambient light color

    //Calculate Diffuse lighting*/
    vec3 norm = normalize(vertexNormal); // Normalize vectors to 1 unit
    vec3 lightDirection = normalize(lightPos - vertexFragmentPos); // Calculate distance (light direction) between light source and fragments/pixels on cube
    float impact = max(dot(norm, lightDirection), 0.0);// Calculate diffuse impact by generating dot product of normal and light
    vec3 diffuse = impact * lightColor; // Generate diffuse light color

    //Calculate Specular lighting*/
    float specularIntensity = 0.8f; // Set specular light strength
    float highlightSize = 16.0f; // Set specular highlight size
    vec3 viewDir = normalize(viewPosition - vertexFragmentPos); // Calculate view direction
    vec3 reflectDir = reflect(-lightDirection, norm);// Calculate reflection vector
    //Calculate specular component
    float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);
    vec3 specular = specularIntensity * specularComponent * lightColor;

    // Texture holds the color to be used for all three components
    vec4 textureColor = texture(uTexture, vertexTextureCoordinate * uvScale);

    // Calculate phong result
    vec3 phong = (ambient + diffuse + specular) * textureColor.xyz;

    fragmentColor = vec4(phong, 1.0); // Send lighting results to GPU
}
);


/* Lamp Shader Source Code*/
const GLchar* lampVertexShaderSource = GLSL(440,

    layout(location = 0) in vec3 position; // VAP position 0 for vertex position data

        //Uniform / Global variables for the  transform matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f); // Transforms vertices into clip coordinates
}
);


/* Fragment Shader Source Code*/
const GLchar* lampFragmentShaderSource = GLSL(440,

    out vec4 fragmentColor; // For outgoing lamp color (smaller cube) to the GPU

void main()
{
    fragmentColor = vec4(1.0f); // Set color to white (1.0f,1.0f,1.0f) with alpha 1.0
}
);

// FLIP IMAGE TO LOAD CORRECTLY
void flipImageVertically(unsigned char* image, int width, int height, int channels)
{
    for (int j = 0; j < height / 2; ++j)
    {
        int index1 = j * width * channels;
        int index2 = (height - 1 - j) * width * channels;

        for (int i = width * channels; i > 0; --i)
        {
            unsigned char tmp = image[index1];
            image[index1] = image[index2];
            image[index2] = tmp;
            ++index1;
            ++index2;
        }
    }
}


int main(int argc, char* argv[])
{
    if (!UInitialize(argc, argv, &window))
        return EXIT_FAILURE;

    // CREATE MESH
    UCreatePear(pear_mesh);
    UCreateApple(apple_mesh);
    UCreateVase(vase_mesh);
    UCreateChalice(chalice_mesh);
    UCreateStem(stem_mesh);
    UCreatePetal(petal_mesh);
    UCreatePlane(plane_mesh);

    // CREATE SHADER
    if (!UCreateShaderProgram(vertexShaderSource, fragmentShaderSource, shader))
        return EXIT_FAILURE;

    if (!UCreateShaderProgram(lampVertexShaderSource, lampFragmentShaderSource, light_shader))
        return EXIT_FAILURE;

    // PREPARE TEXTURE
    const char* pearfile = "C:/Users/disle/source/repos/Textures/pear.jpg";
    if (!UCreateTexture(pearfile, pear_texture_id))
    {
        cout << "Failed to load texture " << pearfile << endl;
        return EXIT_FAILURE;
    }

    const char* vasefile = "C:/Users/disle/source/repos/Textures/vase.jpg";
    if (!UCreateTexture(vasefile, vase_texture_id))
    {
        cout << "Failed to load texture " << vasefile << endl;
        return EXIT_FAILURE;
    }

    const char* challicefile = "C:/Users/disle/source/repos/Textures/glass.jpg";
    if (!UCreateTexture(challicefile, challice_texture_id))
    {
        cout << "Failed to load texture " << challicefile << endl;
        return EXIT_FAILURE;
    }

    const char* petalfile = "C:/Users/disle/source/repos/Textures/petal.jpg";
    if (!UCreateTexture(petalfile, petal_texture_id))
    {
        cout << "Failed to load texture " << petalfile << endl;
        return EXIT_FAILURE;
    }

    const char* stemfile = "C:/Users/disle/source/repos/Textures/stem.jpeg";
    if (!UCreateTexture(stemfile, stem_texture_id))
    {
        cout << "Failed to load texture " << stemfile << endl;
        return EXIT_FAILURE;
    }

    const char* applefile = "C:/Users/disle/source/repos/Textures/apple.jpg";
    if (!UCreateTexture(applefile, apple_texture_id))
    {
        cout << "Failed to load texture " << applefile << endl;
        return EXIT_FAILURE;
    }

    const char* tablefile = "C:/Users/disle/source/repos/Textures/table.jpg";
    if (!UCreateTexture(tablefile, table_texture_id))
    {
        cout << "Failed to load texture " << tablefile << endl;
        return EXIT_FAILURE;
    }
    glUseProgram(shader);
    glUniform1i(glGetUniformLocation(shader, "uTexture"), 0);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // LOOP FOR RENDERING
    while (!glfwWindowShouldClose(window))
    {
        // TIMING
        float currentFrame = glfwGetTime();
        gDeltaTime = currentFrame - gLastFrame;
        gLastFrame = currentFrame;
        //double time = glfwGetTime();

        // INPUT
        UProcessInput(window);

        //RENDER
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // CREATE TABLE TOP
        UPlaneRender(plane_mesh, 5.0f, 0.0f, 25.0f);

        // CREATE BACKDROP -- FIX ME -- ORIENTATION ROTATION NOT WORKING AS INTENDED?
        //UPlaneRender(plane_mesh, 0.0f, 0.0f, -25.0f, 5.0f, 1.0f, 0.0f);

        //CREATE VASE & FLOWERS
        UVaseRender(vase_mesh);
        // FLOWERS
        UStemRender(stem_mesh); // BASE LINE
        UPetalRender(petal_mesh, 18.0f, 4.0f, -5.0f, 0.3f, 1.0f, 2.0f);
        UPetalRender(petal_mesh, 12.0f, 1.0f, -10.0f, 6.0f, 4.0f, 4.0f);

        UStemRender(stem_mesh, -0.30f, 1.0f, 0.0f); 
        UPetalRender(petal_mesh, 16.0f, 4.0f, -10.0f, 0.3f, 4.0f, 9.0f);
        //UStemRender(flower_mesh, -0.30f, 1.0f, 0.20f);
        UStemRender(stem_mesh, -0.30f, 1.0f, -0.20f);
        
        ULightRender();
        
        // RENDER FRUIT
        UPearRender(pear_mesh);
        UAppleRender(apple_mesh);

        // RENDER CHALICE
        UChaliceRender(chalice_mesh);

        glUseProgram(0);
        glBindVertexArray(0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // GARBAGE CLEAN UP
    UDestroyMesh(pear_mesh);
    UDestroyMesh(apple_mesh);
    UDestroyMesh(stem_mesh);
    UDestroyMesh(vase_mesh);
    UDestroyMesh(plane_mesh);

    UDestroyTexture(pear_texture_id);
    UDestroyTexture(table_texture_id);
    UDestroyTexture(apple_texture_id);
    UDestroyTexture(vase_texture_id);
    UDestroyTexture(challice_texture_id);
    UDestroyTexture(stem_texture_id);
    UDestroyTexture(petal_texture_id);

    UDestroyShaderProgram(shader);
    UDestroyShaderProgram(light_shader);
    exit(EXIT_SUCCESS);
}


bool UInitialize(int argc, char* argv[], GLFWwindow** window)
{
    // GLFW: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // GLFW: window creation
    // ---------------------
    * window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (*window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(*window);
    glfwSetFramebufferSizeCallback(*window, UResizeWindow);
    glfwSetCursorPosCallback(*window, UMousePositionCallback);
    glfwSetScrollCallback(*window, UMouseScrollCallback);
    glfwSetMouseButtonCallback(*window, UMouseButtonCallback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // GLEW: initialize
    // ----------------
    // Note: if using GLEW version 1.13 or earlier
    glewExperimental = GL_TRUE;
    GLenum GlewInitResult = glewInit();

    if (GLEW_OK != GlewInitResult)
    {
        std::cerr << glewGetErrorString(GlewInitResult) << std::endl;
        return false;
    }

    // Displays GPU OpenGL version
    cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << endl;

    return true;
}


// PROCESS INPUT
void UProcessInput(GLFWwindow* window)
{
    static const float cameraSpeed = 2.5f;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    //WASD
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        gCamera.ProcessKeyboard(FORWARD, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        gCamera.ProcessKeyboard(BACKWARD, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        gCamera.ProcessKeyboard(LEFT, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        gCamera.ProcessKeyboard(RIGHT, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        gCamera.ProcessKeyboard(UP, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        gCamera.ProcessKeyboard(DOWN, gDeltaTime);

}


// RESIZE WINDOW
void UResizeWindow(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


//FOR MOUSE MOVEMENT
void UMousePositionCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (gFirstMouse)
    {
        gLastX = xpos;
        gLastY = ypos;
        gFirstMouse = false;
    }

    float xoffset = xpos - gLastX;
    float yoffset = gLastY - ypos;

    gLastX = xpos;
    gLastY = ypos;

    gCamera.ProcessMouseMovement(xoffset, yoffset);
}


// MOUSE SCROLL WHEEL
void UMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    gCamera.ProcessMouseScroll(yoffset);
}

// MOUSE BUTTONS
void UMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    switch (button)
    {
    case GLFW_MOUSE_BUTTON_LEFT:
    {
        if (action == GLFW_PRESS)
            cout << "Left mouse button pressed" << endl;
        else
            cout << "Left mouse button released" << endl;
    }
    break;

    case GLFW_MOUSE_BUTTON_MIDDLE:
    {
        if (action == GLFW_PRESS)
            cout << "Middle mouse button pressed" << endl;
        else
            cout << "Middle mouse button released" << endl;
    }
    break;

    case GLFW_MOUSE_BUTTON_RIGHT:
    {
        if (action == GLFW_PRESS)
            cout << "Right mouse button pressed" << endl;
        else
            cout << "Right mouse button released" << endl;
    }
    break;

    default:
        cout << "Unhandled mouse button event" << endl;
        break;
    }
}

// RENDER PEAR
void UPearRender(PearMesh& mesh)
{
    glEnable(GL_DEPTH_TEST);

    // VAO
    glBindVertexArray(mesh.pear_vao);
    // SHADER
    glUseProgram(shader);

    // SCALE
    glm::mat4 scale = glm::scale(glm::vec3(0.60f, 0.60f, 0.60f));
    // ROTATION
    glm::mat4 rotation = glm::rotate(45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    // MOVE TO ORIGIN
    glm::mat4 translation = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));
    // MODEL MATRIX
    glm::mat4 model = translation * rotation * scale;

    // CAMERA VIEW
    glm::mat4 view = gCamera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(gCamera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);

    // SEND TO SHADER
    GLint modelLoc = glGetUniformLocation(shader, "model");
    GLint viewLoc = glGetUniformLocation(shader, "view");
    GLint projLoc = glGetUniformLocation(shader, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // LIGHT //
    GLint objectColorLoc = glGetUniformLocation(shader, "objectColor");
    GLint lightColorLoc = glGetUniformLocation(shader, "lightColor");
    GLint lightPositionLoc = glGetUniformLocation(shader, "lightPos");
    GLint viewPositionLoc = glGetUniformLocation(shader, "viewPosition");

    // Pass color, light, and camera data to the Scene Shader program's corresponding uniforms
    glUniform3f(objectColorLoc, objectColor.r, objectColor.g, objectColor.b);
    glUniform3f(lightColorLoc, lightColor.r, lightColor.g, lightColor.b);
    glUniform3f(lightPositionLoc, lightPos.x, lightPos.y, lightPos.z);
    const glm::vec3 cameraPosition = gCamera.Position;
    glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);


    GLuint UVScaleLoc = glGetUniformLocation(shader, "uvScale");
    glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));

    // BIND TEXTURES
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, pear_texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // DRAW TRIANGLES
    glDrawArrays(GL_TRIANGLES, 0, mesh.pear_num_vertices);
}

// RENDER APPLE
void UAppleRender(AppleMesh& mesh)
{
    glEnable(GL_DEPTH_TEST);
    // VAO
    glBindVertexArray(mesh.apple_vao);
    // SHADER
    glUseProgram(shader);

    // SCALE
    glm::mat4 scale = glm::scale(glm::vec3(0.5f, 0.5f, 0.5f));
    // ROTATION
    glm::mat4 rotation = glm::rotate(45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    // MOVE TO ORIGIN
    glm::mat4 translation = glm::translate(glm::vec3(4.5f, 0.0f, -1.0f));
    // MODEL MATRIX
    glm::mat4 model = translation * rotation * scale;

    // CAMERA VIEW
    glm::mat4 view = gCamera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(gCamera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);


    // SEND TO SHADER
    GLint modelLoc = glGetUniformLocation(shader, "model");
    GLint viewLoc = glGetUniformLocation(shader, "view");
    GLint projLoc = glGetUniformLocation(shader, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // LIGHT //
    GLint objectColorLoc = glGetUniformLocation(shader, "objectColor");
    GLint lightColorLoc = glGetUniformLocation(shader, "lightColor");
    GLint lightPositionLoc = glGetUniformLocation(shader, "lightPos");
    GLint viewPositionLoc = glGetUniformLocation(shader, "viewPosition");

    // Pass color, light, and camera data to the Scene Shader program's corresponding uniforms
    glUniform3f(objectColorLoc, objectColor.r, objectColor.g, objectColor.b);
    glUniform3f(lightColorLoc, lightColor.r, lightColor.g, lightColor.b);
    glUniform3f(lightPositionLoc, lightPos.x, lightPos.y, lightPos.z);
    const glm::vec3 cameraPosition = gCamera.Position;
    glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

    GLuint UVScaleLoc = glGetUniformLocation(shader, "uvScale");
    glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));

    // BIND TEXTURES
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, apple_texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    

    // DRAW TRIANGLES
    glDrawArrays(GL_TRIANGLES, 0, mesh.apple_num_vertices);
}

// RENDER VASE
void UVaseRender(VaseMesh& mesh)
{
    glEnable(GL_DEPTH_TEST);
    // VAO
    glBindVertexArray(mesh.vase_vao);
    // SHADER
    glUseProgram(shader);

    // SCALE
    glm::mat4 scale = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
    // ROTATION
    glm::mat4 rotation = glm::rotate(45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    // MOVE TO ORIGIN
    glm::mat4 translation = glm::translate(glm::vec3(8.0f, 0.0f, -5.0f));
    // MODEL MATRIX
    glm::mat4 model = translation * rotation * scale;

    // CAMERA VIEW
    glm::mat4 view = gCamera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(gCamera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);

    // SEND TO SHADER
    GLint modelLoc = glGetUniformLocation(shader, "model");
    GLint viewLoc = glGetUniformLocation(shader, "view");
    GLint projLoc = glGetUniformLocation(shader, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // LIGHT //
    GLint objectColorLoc = glGetUniformLocation(shader, "objectColor");
    GLint lightColorLoc = glGetUniformLocation(shader, "lightColor");
    GLint lightPositionLoc = glGetUniformLocation(shader, "lightPos");
    GLint viewPositionLoc = glGetUniformLocation(shader, "viewPosition");

    // Pass color, light, and camera data to the Scene Shader program's corresponding uniforms
    glUniform3f(objectColorLoc, objectColor.r, objectColor.g, objectColor.b);
    glUniform3f(lightColorLoc, lightColor.r, lightColor.g, lightColor.b);
    glUniform3f(lightPositionLoc, lightPos.x, lightPos.y, lightPos.z);
    const glm::vec3 cameraPosition = gCamera.Position;
    glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);


    GLint UVScaleLoc = glGetUniformLocation(shader, "uvScale");
    glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));
    // BIND TEXTURES
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, vase_texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // DRAW TRIANGLES
    glDrawArrays(GL_TRIANGLES, 0, mesh.vase_num_vertices);
}

// RENDER CHALICE
void UChaliceRender(ChaliceMesh& mesh)
{
    glEnable(GL_DEPTH_TEST);

    glBindVertexArray(mesh.chalice_vao);
    // SHADER
    glUseProgram(shader);
    // SCALE
    glm::mat4 scale = glm::scale(glm::vec3(0.5f, 0.5f, 0.5f));
    // ROTATION
    glm::mat4 rotation = glm::rotate(45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    // MOVE TO ORIGIN
    glm::mat4 translation = glm::translate(glm::vec3(10.0f, 2.0f, 2.0f));
    // MODEL MATRIX
    glm::mat4 model = translation * rotation * scale;

    // CAMERA VIEW
    glm::mat4 view = gCamera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(gCamera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);


    // SEND TO SHADER
    GLint modelLoc = glGetUniformLocation(shader, "model");
    GLint viewLoc = glGetUniformLocation(shader, "view");
    GLint projLoc = glGetUniformLocation(shader, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // LIGHT //
    GLint objectColorLoc = glGetUniformLocation(shader, "objectColor");
    GLint lightColorLoc = glGetUniformLocation(shader, "lightColor");
    GLint lightPositionLoc = glGetUniformLocation(shader, "lightPos");
    GLint viewPositionLoc = glGetUniformLocation(shader, "viewPosition");

    // Pass color, light, and camera data to the Scene Shader program's corresponding uniforms
    glUniform3f(objectColorLoc, objectColor.r, objectColor.g, objectColor.b);
    glUniform3f(lightColorLoc, lightColor.r, lightColor.g, lightColor.b);
    glUniform3f(lightPositionLoc, lightPos.x, lightPos.y, lightPos.z);
    const glm::vec3 cameraPosition = gCamera.Position;
    glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

    GLint UVScaleLoc = glGetUniformLocation(shader, "uvScale");
    glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));

    // BIND TEXTURES
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, challice_texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    

    // DRAW TRIANGLES
    glDrawArrays(GL_TRIANGLES, 0, mesh.chalice_num_vertices);
}

// RENDER PLANE (TABLE)
void UPlaneRender(PlaneMesh& mesh)
{
    glEnable(GL_DEPTH_TEST);

    glBindVertexArray(mesh.plane_vao);
    // SHADER
    glUseProgram(shader);

    // SCALE
    glm::mat4 scale = glm::scale(glm::vec3(10.0f, 10.0f, 10.0f));
    // ROTATION
    glm::mat4 rotation = glm::rotate(45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    // MOVE TO ORIGIN
    glm::mat4 translation = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));
    // MODEL MATRIX
    glm::mat4 model = translation * rotation * scale;

    // CAMERA VIEW
    glm::mat4 view = gCamera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(gCamera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);

    // SEND TO SHADER
    GLint modelLoc = glGetUniformLocation(shader, "model");
    GLint viewLoc = glGetUniformLocation(shader, "view");
    GLint projLoc = glGetUniformLocation(shader, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // LIGHT //
    GLint objectColorLoc = glGetUniformLocation(shader, "objectColor");
    GLint lightColorLoc = glGetUniformLocation(shader, "lightColor");
    GLint lightPositionLoc = glGetUniformLocation(shader, "lightPos");
    GLint viewPositionLoc = glGetUniformLocation(shader, "viewPosition");

    // Pass color, light, and camera data to the Scene Shader program's corresponding uniforms
    glUniform3f(objectColorLoc, objectColor.r, objectColor.g, objectColor.b);
    glUniform3f(lightColorLoc, lightColor.r, lightColor.g, lightColor.b);
    glUniform3f(lightPositionLoc, lightPos.x, lightPos.y, lightPos.z);
    const glm::vec3 cameraPosition = gCamera.Position;
    glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);


    GLint UVScaleLoc = glGetUniformLocation(shader, "uvScale");
    glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));

    // BIND TEXTURES
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, table_texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    

    // DRAW TRIANGLES
    glDrawArrays(GL_TRIANGLES, 0, mesh.plane_num_vertices);

}


void UPlaneRender(PlaneMesh& mesh, float t_x_value, float t_y_value, float t_z_value)
{
    glEnable(GL_DEPTH_TEST);

    glBindVertexArray(mesh.plane_vao);
    // SHADER
    glUseProgram(shader);

    // SCALE
    glm::mat4 scale = glm::scale(glm::vec3(10.0f, 10.0f, 10.0f));
    // ROTATION
    glm::mat4 rotation = glm::rotate(45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    // MOVE TO ORIGIN
    glm::mat4 translation = glm::translate(glm::vec3(t_x_value, t_y_value, t_z_value));
    // MODEL MATRIX
    glm::mat4 model = translation * rotation * scale;

    // CAMERA VIEW
    glm::mat4 view = gCamera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(gCamera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);


    // SEND TO SHADER
    GLint modelLoc = glGetUniformLocation(shader, "model");
    GLint viewLoc = glGetUniformLocation(shader, "view");
    GLint projLoc = glGetUniformLocation(shader, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    
    // LIGHT //
    GLint objectColorLoc = glGetUniformLocation(shader, "objectColor");
    GLint lightColorLoc = glGetUniformLocation(shader, "lightColor");
    GLint lightPositionLoc = glGetUniformLocation(shader, "lightPos");
    GLint viewPositionLoc = glGetUniformLocation(shader, "viewPosition");

    // Pass color, light, and camera data to the Scene Shader program's corresponding uniforms
    glUniform3f(objectColorLoc, objectColor.r, objectColor.g, objectColor.b);
    glUniform3f(lightColorLoc, lightColor.r, lightColor.g, lightColor.b);
    glUniform3f(lightPositionLoc, lightPos.x, lightPos.y, lightPos.z);
    const glm::vec3 cameraPosition = gCamera.Position;
    glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

    GLint UVScaleLoc = glGetUniformLocation(shader, "uvScale");
    glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));

    // BIND TEXTURES
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, table_texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // DRAW TRIANGLES
    glDrawArrays(GL_TRIANGLES, 0, mesh.plane_num_vertices);

}

// FIX ME -- USED FOR BACKDROP -- DOESN'T WORK AS INTENDED -- ROATION ORIENTATION NOT RIGHT?
void UPlaneRender(PlaneMesh& mesh, float t_x_value, float t_y_value, float t_z_value, float r_x_value, float r_y_value, float r_z_value)
{
    glEnable(GL_DEPTH_TEST);
    
    glBindVertexArray(mesh.plane_vao);
    // SHADER
    glUseProgram(shader);
    // SCALE
    glm::mat4 scale = glm::scale(glm::vec3(10.0f, 10.0f, 10.0f));
    // ROTATION
    glm::mat4 rotation = glm::rotate(10.0f, glm::vec3(r_x_value, r_y_value, r_z_value));
    // MOVE TO ORIGIN
    glm::mat4 translation = glm::translate(glm::vec3(t_x_value, t_y_value, t_z_value));
    // MODEL MATRIX
    glm::mat4 model = translation * rotation * scale;

    // CAMERA VIEW
    glm::mat4 view = gCamera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(gCamera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);

    // SEND TO SHADER
    GLint modelLoc = glGetUniformLocation(shader, "model");
    GLint viewLoc = glGetUniformLocation(shader, "view");
    GLint projLoc = glGetUniformLocation(shader, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // LIGHT //
    GLint objectColorLoc = glGetUniformLocation(shader, "objectColor");
    GLint lightColorLoc = glGetUniformLocation(shader, "lightColor");
    GLint lightPositionLoc = glGetUniformLocation(shader, "lightPos");
    GLint viewPositionLoc = glGetUniformLocation(shader, "viewPosition");

    // Pass color, light, and camera data to the Scene Shader program's corresponding uniforms
    glUniform3f(objectColorLoc, objectColor.r, objectColor.g, objectColor.b);
    glUniform3f(lightColorLoc, lightColor.r, lightColor.g, lightColor.b);
    glUniform3f(lightPositionLoc, lightPos.x, lightPos.y, lightPos.z);
    const glm::vec3 cameraPosition = gCamera.Position;
    glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

    GLint UVScaleLoc = glGetUniformLocation(shader, "uvScale");
    glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));

    // BIND TEXTURES
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, pear_texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // DRAW TRIANGLES
    glDrawArrays(GL_TRIANGLES, 0, mesh.plane_num_vertices);

}
// RENDER FLOWER
void UStemRender(StemMesh& mesh)
{
    glEnable(GL_DEPTH_TEST);

    glBindVertexArray(mesh.stem_vao);
    // SHADER
    glUseProgram(shader);
    // SCALE
    glm::mat4 scale = glm::scale(glm::vec3(0.5f, 0.85f, 0.5f));
    // ROTATION
    glm::mat4 rotation = glm::rotate(45.0f, glm::vec3(0.30f, 1.0f, 0.0f));
    // MOVE TO ORIGIN
    glm::mat4 translation = glm::translate(glm::vec3(8.0f, 0.0f, -5.0f));
    // MODEL MATRIX
    glm::mat4 model = translation * rotation * scale;

    // CAMERA VIEW
    glm::mat4 view = gCamera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(gCamera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);

    // SEND TO SHADER
    GLint modelLoc = glGetUniformLocation(shader, "model");
    GLint viewLoc = glGetUniformLocation(shader, "view");
    GLint projLoc = glGetUniformLocation(shader, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // LIGHT //
    GLint objectColorLoc = glGetUniformLocation(shader, "objectColor");
    GLint lightColorLoc = glGetUniformLocation(shader, "lightColor");
    GLint lightPositionLoc = glGetUniformLocation(shader, "lightPos");
    GLint viewPositionLoc = glGetUniformLocation(shader, "viewPosition");

    // Pass color, light, and camera data to the Scene Shader program's corresponding uniforms
    glUniform3f(objectColorLoc, objectColor.r, objectColor.g, objectColor.b);
    glUniform3f(lightColorLoc, lightColor.r, lightColor.g, lightColor.b);
    glUniform3f(lightPositionLoc, lightPos.x, lightPos.y, lightPos.z);
    const glm::vec3 cameraPosition = gCamera.Position;
    glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

    GLint UVScaleLoc = glGetUniformLocation(shader, "uvScale");
    glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));

    // BIND TEXTURES
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, stem_texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // DRAW TRIANGLES
    glDrawArrays(GL_TRIANGLES, 0, mesh.stem_num_vertices);


}

void UStemRender(StemMesh& mesh, float r_x_value, float r_y_value, float r_z_value)
{
    glEnable(GL_DEPTH_TEST);

    glBindVertexArray(mesh.stem_vao);
    // SHADER
    glUseProgram(shader);
    // SCALE
    glm::mat4 scale = glm::scale(glm::vec3(0.5f, 0.85f, 0.5f));
    // ROTATION
    glm::mat4 rotation = glm::rotate(45.0f, glm::vec3(r_x_value, r_y_value, r_z_value));
    // MOVE TO ORIGIN
    glm::mat4 translation = glm::translate(glm::vec3(8.0f, 0.0f, -5.0f));
    // MODEL MATRIX
    glm::mat4 model = translation * rotation * scale;

    // CAMERA VIEW
    glm::mat4 view = gCamera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(gCamera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);

    // SEND TO SHADER
    GLint modelLoc = glGetUniformLocation(shader, "model");
    GLint viewLoc = glGetUniformLocation(shader, "view");
    GLint projLoc = glGetUniformLocation(shader, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // LIGHT //
    GLint objectColorLoc = glGetUniformLocation(shader, "objectColor");
    GLint lightColorLoc = glGetUniformLocation(shader, "lightColor");
    GLint lightPositionLoc = glGetUniformLocation(shader, "lightPos");
    GLint viewPositionLoc = glGetUniformLocation(shader, "viewPosition");

    // Pass color, light, and camera data to the Scene Shader program's corresponding uniforms
    glUniform3f(objectColorLoc, objectColor.r, objectColor.g, objectColor.b);
    glUniform3f(lightColorLoc, lightColor.r, lightColor.g, lightColor.b);
    glUniform3f(lightPositionLoc, lightPos.x, lightPos.y, lightPos.z);
    const glm::vec3 cameraPosition = gCamera.Position;
    glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

    GLint UVScaleLoc = glGetUniformLocation(shader, "uvScale");
    glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));

    // BIND TEXTURES
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, stem_texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // DRAW TRIANGLES
    glDrawArrays(GL_TRIANGLES, 0, mesh.stem_num_vertices);

}

void UPetalRender(PetalMesh& mesh, float t_x_value, float t_y_value, float t_z_value, float r_x_value, float r_y_value, float r_z_value)
{
    glEnable(GL_DEPTH_TEST);

    glBindVertexArray(mesh.petal_vao);
    // SHADER
    glUseProgram(shader);
    // SCALE
    glm::mat4 scale = glm::scale(glm::vec3(0.5f, 0.85f, 0.5f));
    // ROTATION
    glm::mat4 rotation = glm::rotate(45.0f, glm::vec3(r_x_value, r_y_value, r_z_value));
    // MOVE TO ORIGIN
    glm::mat4 translation = glm::translate(glm::vec3(t_x_value, t_y_value, t_z_value));
    // MODEL MATRIX
    glm::mat4 model = translation * rotation * scale;

    // CAMERA VIEW
    glm::mat4 view = gCamera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(gCamera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);


    // SEND TO SHADER
    GLint modelLoc = glGetUniformLocation(shader, "model");
    GLint viewLoc = glGetUniformLocation(shader, "view");
    GLint projLoc = glGetUniformLocation(shader, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // LIGHT //
    GLint objectColorLoc = glGetUniformLocation(shader, "objectColor");
    GLint lightColorLoc = glGetUniformLocation(shader, "lightColor");
    GLint lightPositionLoc = glGetUniformLocation(shader, "lightPos");
    GLint viewPositionLoc = glGetUniformLocation(shader, "viewPosition");

    // Pass color, light, and camera data to the Scene Shader program's corresponding uniforms
    glUniform3f(objectColorLoc, objectColor.r, objectColor.g, objectColor.b);
    glUniform3f(lightColorLoc, lightColor.r, lightColor.g, lightColor.b);
    glUniform3f(lightPositionLoc, lightPos.x, lightPos.y, lightPos.z);
    const glm::vec3 cameraPosition = gCamera.Position;
    glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

    GLint UVScaleLoc = glGetUniformLocation(shader, "uvScale");
    glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));

    // BIND TEXTURES
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, petal_texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // DRAW TRIANGLES
    glDrawArrays(GL_TRIANGLES, 0, mesh.petal_num_vertices);

}

void ULightRender()
{
    
}


// HOLDS PEAR VERTS
void UCreatePear(PearMesh& mesh)
{
    // Vertex data
    GLfloat verts[] = {
        // BOTTOM HALF //
        //Positions         // NEGATIVE Y NORMAL   //Texture Coordinates
        0.0f, 0.0f,  0.0f,   0.0f, -1.0f, 0.0f,     0.0f, 0.0f,
       -2.0f, 1.0f,  2.0f,   0.0f, -1.0f, 0.0f,     1.0f, 0.0f,
       -2.0f, 1.0f, -2.0f,   0.0f, -1.0f, 0.0f,     0.0f, 1.0f,

        0.0f, 0.0f,  0.0f,   0.0f, -1.0f, 0.0f,     0.0f, 0.0f,
       -2.0f, 1.0f, -2.0f,   0.0f, -1.0f, 0.0f,     1.0f, 0.0f,
        2.0f, 1.0f, -2.0f,   0.0f, -1.0f, 0.0f,     0.0f, 1.0f,

        0.0f, 0.0f,  0.0f,   0.0f, -1.0f, 0.0f,     0.0f, 0.0f,
        2.0f, 1.0f, -2.0f,   0.0f, -1.0f, 0.0f,     1.0f, 0.0f,
        2.0f, 1.0f,  2.0f,   0.0f, -1.0f, 0.0f,     0.0f, 1.0f,

        0.0f, 0.0f,  0.0f,   0.0f, -1.0f, 0.0f,     0.0f, 0.0f,
        2.0f, 1.0f,  2.0f,   0.0f, -1.0f, 0.0f,     1.0f, 0.0f,
       -2.0f, 1.0f,  2.0f,   0.0f, -1.0f, 0.0f,     0.0f, 1.0f,

       -2.0f, 1.0f, -2.0f,   0.0f, -1.0f, 0.0f,     0.0f, 0.0f,
       -3.0f, 2.0f,  0.0f,   0.0f, -1.0f, 0.0f,     1.0f, 0.0f,
       -2.0f, 1.0f,  2.0f,   0.0f, -1.0f, 0.0f,     0.0f, 1.0f,

       -2.0f, 1.0f, -2.0f,   0.0f, -1.0f, 0.0f,     0.0f, 0.0f,
        0.0f, 2.0f, -3.0f,   0.0f, -1.0f, 0.0f,     1.0f, 0.0f,
        2.0f, 1.0f, -2.0f,   0.0f, -1.0f, 0.0f,     0.0f, 1.0f,

        2.0f, 1.0f, -2.0f,   0.0f, -1.0f, 0.0f,     0.0f, 0.0f,
        3.0f, 2.0f,  0.0f,   0.0f, -1.0f, 0.0f,     1.0f, 0.0f,
        2.0f, 1.0f,  2.0f,   0.0f, -1.0f, 0.0f,     0.0f, 1.0f,

        2.0f, 1.0f,  2.0f,   0.0f, -1.0f, 0.0f,     0.0f, 0.0f,
        0.0f, 2.0f,  3.0f,   0.0f, -1.0f, 0.0f,     1.0f, 0.0f,
       -2.0f, 1.0f,  2.0f,   0.0f, -1.0f, 0.0f,     0.0f, 1.0f,

       -2.0f, 1.0f,  2.0f,   -1.0f, 0.0f,  0.0f,     0.0f, 0.0f,
       -3.0f, 2.0f,  0.0f,   -1.0f, 0.0f,  0.0f,     1.0f, 0.0f,
       -3.0f, 3.0f,  3.0f,   -1.0f, 0.0f,  0.0f,     0.0f, 1.0f,

       -2.0f, 1.0f,  2.0f,   -1.0f, 0.0f,  0.0f,     0.0f, 0.0f,
       -3.0f, 3.0f,  3.0f,   -1.0f, 0.0f,  0.0f,     1.0f, 0.0f,
        0.0f, 2.0f,  3.0f,   -1.0f, 0.0f,  0.0f,     0.0f, 1.0f,

       -3.0f, 2.0f,  0.0f,   -1.0f, 0.0f,  0.0f,     0.0f, 0.0f,
       -3.0f, 3.0f, -3.0f,   -1.0f, 0.0f,  0.0f,     1.0f, 0.0f,
       -2.0f, 1.0f, -2.0f,   -1.0f, 0.0f,  0.0f,     0.0f, 1.0f,

       -2.0f, 1.0f, -2.0f,   -1.0f, 0.0f,  0.0f,     0.0f, 0.0f,
       -3.0f, 3.0f, -3.0f,   -1.0f, 0.0f,  0.0f,     1.0f, 0.0f,
        0.0f, 2.0f, -3.0f,   -1.0f, 0.0f,  0.0f,     0.0f, 1.0f,
        //POSITIVE X NORMAL
    0.0f, 2.0f, -3.0f,   1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
    3.0f, 3.0f, -3.0f,   1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
    2.0f, 1.0f, -2.0f,   1.0f,  0.0f,  0.0f,    0.0f, 1.0f,

    3.0f, 2.0f,  0.0f,   1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
    2.0f, 1.0f, -2.0f,   1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
    3.0f, 3.0f, -3.0f,   1.0f,  0.0f,  0.0f,    0.0f, 1.0f,

    3.0f, 2.0f,  0.0f,   1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
    3.0f, 3.0f,  3.0f,   1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
    2.0f, 1.0f,  2.0f,   1.0f,  0.0f,  0.0f,    0.0f, 1.0f,

    0.0f, 2.0f,  3.0f,   1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
    2.0f, 1.0f,  2.0f,   1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
    3.0f, 3.0f,  3.0f,   1.0f,  0.0f,  0.0f,    0.0f, 1.0f,

    -3.0f, 3.0f, -3.0f,   1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
     0.0f, 3.0f, -3.0f,   1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
     0.0f, 2.0f, -3.0f,   1.0f,  0.0f,  0.0f,    0.0f, 1.0f,

     0.0f, 2.0f, -3.0f,   -1.0f, 0.0f,  0.0f,    0.0f, 0.0f,
     0.0f, 3.0f, -3.0f,   -1.0f, 0.0f,  0.0f,    1.0f, 0.0f,
     3.0f, 3.0f, -3.0f,   -1.0f, 0.0f,  0.0f,    0.0f, 1.0f,

     3.0f, 3.0f, -3.0f,   -1.0f, 0.0f,  0.0f,    0.0f, 0.0f,
     3.0f, 3.0f,  0.0f,   -1.0f, 0.0f,  0.0f,    1.0f, 0.0f,
     3.0f, 2.0f,  0.0f,   -1.0f, 0.0f,  0.0f,    0.0f, 1.0f,

     3.0f, 2.0f,  0.0f,   -1.0f, 0.0f,  0.0f,    0.0f, 0.0f,
     3.0f, 3.0f,  0.0f,   -1.0f, 0.0f,  0.0f,    1.0f, 0.0f,
     3.0f, 3.0f,  3.0f,   -1.0f, 0.0f,  0.0f,    0.0f, 1.0f,

     3.0f, 3.0f,  3.0f,   -1.0f, 0.0f,  0.0f,    0.0f, 0.0f,
     0.0f, 3.0f,  3.0f,   -1.0f, 0.0f,  0.0f,    1.0f, 0.0f,
     0.0f, 2.0f,  3.0f,   -1.0f, 0.0f,  0.0f,    0.0f, 1.0f,

     0.0f, 2.0f,  3.0f,   -1.0f, 0.0f,  0.0f,    0.0f, 0.0f,
     0.0f, 3.0f,  3.0f,   -1.0f, 0.0f,  0.0f,    1.0f, 0.0f,
    -3.0f, 3.0f,  3.0f,   -1.0f, 0.0f,  0.0f,    0.0f, 1.0f,

    -3.0f, 3.0f,  3.0f,   -1.0f, 0.0f,  0.0f,    0.0f, 0.0f,
    -3.0f, 3.0f,  0.0f,   -1.0f, 0.0f,  0.0f,    1.0f, 0.0f,
    -3.0f, 2.0f,  0.0f,   -1.0f, 0.0f,  0.0f,    0.0f, 1.0f,

    -3.0f, 2.0f,  0.0f,   -1.0f, 0.0f,  0.0f,    0.0f, 0.0f,
    -3.0f, 3.0f,  0.0f,   -1.0f, 0.0f,  0.0f,    1.0f, 0.0f,
    -3.0f, 3.0f, -3.0f,   -1.0f, 0.0f,  0.0f,    0.0f, 1.0f,

    -3.0f, 3.0f, -3.0f,    1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
     0.0f, 3.0f, -3.0f,    1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
     0.0f, 5.0f, -3.0f,    1.0f,  0.0f,  0.0f,    0.0f, 1.0f,

    -3.0f, 3.0f, -3.0f,   1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
    -3.0f, 5.0f, -3.0f,   1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
     0.0f, 5.0f, -3.0f,   1.0f,  0.0f,  0.0f,    0.0f, 1.0f,

     0.0f, 3.0f, -3.0f,   1.0f, 0.0f,   0.0f,    0.0f, 0.0f,
     0.0f, 5.0f, -3.0f,   1.0f, 0.0f,   0.0f,    1.0f, 0.0f,
     3.0f, 3.0f, -3.0f,   1.0f, 0.0f,   0.0f,    0.0f, 1.0f,

     3.0f, 3.0f, -3.0f,   1.0f, 0.0f,   0.0f,    0.0f, 0.0f,
     0.0f, 5.0f, -3.0f,   1.0f, 0.0f,   0.0f,    1.0f, 0.0f,
     3.0f, 5.0f, -3.0f,   1.0f, 0.0f,   0.0f,    0.0f, 1.0f,

     3.0f, 3.0f, -3.0f,   1.0f, 0.0f,   0.0f,    0.0f, 0.0f,
     3.0f, 3.0f,  0.0f,   1.0f, 0.0f,   0.0f,    1.0f, 0.0f,
     3.0f, 5.0f,  0.0f,   1.0f, 0.0f,   0.0f,    0.0f, 1.0f,

     3.0f, 3.0f, -3.0f,   1.0f, 0.0f,   0.0f,    0.0f, 0.0f,
     3.0f, 5.0f, -3.0f,   1.0f, 0.0f,   0.0f,    1.0f, 0.0f,
     3.0f, 5.0f,  0.0f,   1.0f, 0.0f,   0.0f,    0.0f, 1.0f,

     3.0f, 3.0f,  0.0f,   1.0f, 0.0f,   0.0f,    0.0f, 0.0f,
     3.0f, 5.0f,  0.0f,   1.0f, 0.0f,   0.0f,    1.0f, 0.0f,
     3.0f, 3.0f,  3.0f,   1.0f, 0.0f,   0.0f,    0.0f, 1.0f,

     3.0f, 3.0f,  3.0f,   1.0f, 0.0f,   0.0f,    0.0f, 0.0f,
     3.0f, 5.0f,  0.0f,   1.0f, 0.0f,   0.0f,    1.0f, 0.0f,
     3.0f, 5.0f,  3.0f,   1.0f, 0.0f,   0.0f,    0.0f, 1.0f,

     3.0f, 3.0f,  3.0f,   1.0f, 0.0f,   0.0f,    0.0f, 0.0f,
     0.0f, 3.0f,  3.0f,   1.0f, 0.0f,   0.0f,    1.0f, 0.0f,
     0.0f, 5.0f,  3.0f,   1.0f, 0.0f,   0.0f,    0.0f, 1.0f,

     3.0f, 3.0f,  3.0f,   1.0f, 0.0f,   0.0f,    0.0f, 0.0f,
     3.0f, 5.0f,  3.0f,   1.0f, 0.0f,   0.0f,    1.0f, 0.0f,
     0.0f, 5.0f,  3.0f,   1.0f, 0.0f,   0.0f,    0.0f, 1.0f,

     0.0f, 5.0f,  3.0f,  -1.0f, 0.0f,   0.0f,    0.0f, 0.0f,
    -3.0f, 3.0f,  3.0f,  -1.0f, 0.0f,   0.0f,    1.0f, 0.0f,
     0.0f, 3.0f,  3.0f,  -1.0f, 0.0f,   0.0f,    0.0f, 1.0f,

     0.0f, 5.0f,  3.0f,  -1.0f, 0.0f,   0.0f,    0.0f, 0.0f,
    -3.0f, 5.0f,  3.0f,  -1.0f, 0.0f,   0.0f,    1.0f, 0.0f,
    -3.0f, 3.0f,  3.0f,  -1.0f, 0.0f,   0.0f,    0.0f, 1.0f,

    -3.0f, 3.0f,  3.0f,  -1.0f, 0.0f,   0.0f,    0.0f, 0.0f,
    -3.0f, 3.0f,  0.0f,  -1.0f, 0.0f,   0.0f,    1.0f, 0.0f,
    -3.0f, 5.0f,  3.0f,  -1.0f, 0.0f,   0.0f,    0.0f, 1.0f,

    -3.0f, 5.0f,  3.0f,  -1.0f, 0.0f,   0.0f,    0.0f, 0.0f,
    -3.0f, 3.0f,  0.0f,  -1.0f, 0.0f,   0.0f,    1.0f, 0.0f,
    -3.0f, 5.0f,  0.0f,  -1.0f, 0.0f,   0.0f,    0.0f, 1.0f,

    -3.0f, 3.0f,  0.0f,  -1.0f, 0.0f,   0.0f,    0.0f, 0.0f,
    -3.0f, 5.0f,  0.0f,  -1.0f, 0.0f,   0.0f,    1.0f, 0.0f,
    -3.0f, 3.0f, -3.0f,  -1.0f, 0.0f,   0.0f,    0.0f, 1.0f,

    -3.0f, 5.0f,  0.0f,  -1.0f, 0.0f,   0.0f,    0.0f, 0.0f,
    -3.0f, 3.0f, -3.0f,  -1.0f, 0.0f,   0.0f,    1.0f, 0.0f,
    -3.0f, 5.0f, -3.0f,  -1.0f, 0.0f,   0.0f,    0.0f, 1.0f,

    // TOP HALF //      //POSITIVE Y NORMAL //
    0.0f, 10.0f,  0.0f,   0.0f, 1.0f, 0.0f,    0.0f, 0.0f,
    -1.0f,  9.0f,  1.0f,   0.0f, 1.0f, 0.0f,    1.0f, 0.0f,
    -1.0f,  9.0f, -1.0f,   0.0f, 1.0f, 0.0f,    0.0f, 1.0f,

     0.0f, 10.0f,  0.0f,   0.0f, 1.0f, 0.0f,    0.0f, 0.0f,
    -1.0f,  9.0f, -1.0f,   0.0f, 1.0f, 0.0f,    1.0f, 0.0f,
     1.0f,  9.0f, -1.0f,   0.0f, 1.0f, 0.0f,    0.0f, 1.0f,

     0.0f, 10.0f,  0.0f,   0.0f, 1.0f, 0.0f,    0.0f, 0.0f,
     1.0f,  9.0f, -1.0f,   0.0f, 1.0f, 0.0f,    1.0f, 0.0f,
     1.0f,  9.0f,  1.0f,   0.0f, 1.0f, 0.0f,    0.0f, 1.0f,

     0.0f, 10.0f,  0.0f,   0.0f, 1.0f, 0.0f,    0.0f, 0.0f,
     1.0f,  9.0f,  1.0f,   0.0f, 1.0f, 0.0f,    1.0f, 0.0f,
    -1.0f,  9.0f,  1.0f,   0.0f, 1.0f, 0.0f,    0.0f, 1.0f,

    -1.0f,  9.0f,  1.0f,  -1.0f, 0.0f, 0.0f,    0.0f, 0.0f,
    -1.0f,  9.0f, -1.0f,  -1.0f, 0.0f, 0.0f,    1.0f, 0.0f,
    -1.0f,  8.0f,  1.0f,  -1.0f, 0.0f, 0.0f,    0.0f, 1.0f,

    -1.0f,  9.0f, -1.0f,  -1.0f, 0.0f, 0.0f,    0.0f, 0.0f,
    -1.0f,  8.0f,  1.0f,  -1.0f, 0.0f, 0.0f,    1.0f, 0.0f,
    -1.0f,  8.0f, -1.0f,  -1.0f, 0.0f, 0.0f,    0.0f, 1.0f,

    -1.0f,  9.0f, -1.0f,   0.0f, 0.0f, -1.0f,   0.0f, 0.0f,
    -1.0f,  8.0f, -1.0f,   0.0f, 0.0f, -1.0f,    1.0f, 0.0f,
     1.0f,  9.0f, -1.0f,   0.0f, 0.0f, -1.0f,    0.0f, 1.0f,

     1.0f,  9.0f, -1.0f,   0.0f, 0.0f, -1.0f,    0.0f, 0.0f,
    -1.0f,  8.0f, -1.0f,   0.0f, 0.0f, -1.0f,    1.0f, 0.0f,
     1.0f,  8.0f, -1.0f,   0.0f, 0.0f, -1.0f,    0.0f, 1.0f,

     1.0f,  9.0f, -1.0f,   1.0f, 0.0f, 0.0f,    0.0f, 0.0f,
     1.0f,  8.0f, -1.0f,   1.0f, 0.0f, 0.0f,    1.0f, 0.0f,
     1.0f,  9.0f,  1.0f,   1.0f, 0.0f, 0.0f,    0.0f, 1.0f,

     1.0f,  9.0f,  1.0f,   1.0f, 0.0f, 0.0f,    0.0f, 0.0f,
     1.0f,  8.0f, -1.0f,   1.0f, 0.0f, 0.0f,    1.0f, 0.0f,
     1.0f,  8.0f,  1.0f,   1.0f, 0.0f, 0.0f,    0.0f, 1.0f,

     1.0f,  9.0f,  1.0f,   0.0f, 0.0f, 1.0f,    0.0f, 0.0f,
     1.0f,  8.0f,  1.0f,   0.0f, 0.0f, 1.0f,    1.0f, 0.0f,
    -1.0f,  9.0f,  1.0f,   0.0f, 0.0f, 1.0f,    0.0f, 1.0f,

    -1.0f,  9.0f,  1.0f,   0.0f, 0.0f, 1.0f,    0.0f, 0.0f,
     1.0f,  8.0f,  1.0f,   0.0f, 0.0f, 1.0f,    1.0f, 0.0f,
    -1.0f,  8.0f,  1.0f,   0.0f, 0.0f, 1.0f,    0.0f, 1.0f,

    // MIDDLE //        //POSITIVE Z NORMAL//
    -3.0f,  5.0f,  3.0f,   0.0f, 0.0f, 1.0f,    0.0f, 0.0f,
    -3.0f,  5.0f,  0.0f,   0.0f, 0.0f, 1.0f,    1.0f, 0.0f,
    -2.0f,  6.0f,  2.0f,   0.0f, 0.0f, 1.0f,    0.0f, 1.0f,

    -2.0f,  6.0f,  2.0f,   0.0f, 0.0f, 1.0f,    0.0f, 0.0f,
    -3.0f,  5.0f,  0.0f,   0.0f, 0.0f, 1.0f,    1.0f, 0.0f,
    -2.0f,  6.0f,  0.0f,   0.0f, 0.0f, 1.0f,    0.0f, 1.0f,

    -3.0f,  5.0f,  0.0f,   0.0f, 0.0f, 1.0f,    0.0f, 0.0f,
    -3.0f,  5.0f, -3.0f,   0.0f, 0.0f, 1.0f,    1.0f, 0.0f,
    -2.0f,  6.0f, -2.0f,   0.0f, 0.0f, 1.0f,    0.0f, 1.0f,

    -2.0f,  6.0f,  0.0f,   0.0f, 0.0f, 1.0f,    0.0f, 0.0f,
    -3.0f,  5.0f,  0.0f,   0.0f, 0.0f, 1.0f,    1.0f, 0.0f,
    -2.0f,  6.0f, -2.0f,   0.0f, 0.0f, 1.0f,    0.0f, 1.0f,

    -2.0f,  6.0f, -2.0f,   0.0f, 0.0f, 1.0f,    0.0f, 0.0f,
    -3.0f,  5.0f, -3.0f,   0.0f, 0.0f, 1.0f,    1.0f, 0.0f,
     0.0f,  5.0f, -3.0f,   0.0f, 0.0f, 1.0f,    0.0f, 1.0f,

    -2.0f,  6.0f, -2.0f,   0.0f, 0.0f, 1.0f,    0.0f, 0.0f,
     0.0f,  5.0f, -3.0f,   0.0f, 0.0f, 1.0f,    1.0f, 0.0f,
     0.0f,  6.0f, -2.0f,   0.0f, 0.0f, 1.0f,    0.0f, 1.0f,

     0.0f,  5.0f, -3.0f,   0.0f, 0.0f, 1.0f,    0.0f, 0.0f,
     3.0f,  5.0f, -3.0f,   0.0f, 0.0f, 1.0f,    1.0f, 0.0f,
     2.0f,  6.0f, -2.0f,   0.0f, 0.0f, 1.0f,    0.0f, 1.0f,

     0.0f,  6.0f, -2.0f,   0.0f, 0.0f, 1.0f,    0.0f, 0.0f,
     0.0f,  5.0f, -3.0f,   0.0f, 0.0f, 1.0f,    1.0f, 0.0f,
     2.0f,  6.0f, -2.0f,   0.0f, 0.0f, 1.0f,    0.0f, 1.0f,

     3.0f,  5.0f, -3.0f,   0.0f, 0.0f, 1.0f,    0.0f, 0.0f,
     3.0f,  5.0f,  0.0f,   0.0f, 0.0f, 1.0f,    1.0f, 0.0f,
     2.0f,  6.0f, -2.0f,   0.0f, 0.0f, 1.0f,    0.0f, 1.0f,

     3.0f,  5.0f,  0.0f,   0.0f, 0.0f, 1.0f,    0.0f, 0.0f,
     2.0f,  6.0f,  0.0f,   0.0f, 0.0f, 1.0f,    1.0f, 0.0f,
     2.0f,  6.0f, -2.0f,   0.0f, 0.0f, 1.0f,    0.0f, 1.0f,

     3.0f,  5.0f,  0.0f,   0.0f, 0.0f, 1.0f,    0.0f, 0.0f,
     3.0f,  5.0f,  3.0f,   0.0f, 0.0f, 1.0f,    1.0f, 0.0f,
     2.0f,  6.0f,  2.0f,   0.0f, 0.0f, 1.0f,    0.0f, 1.0f,

     3.0f,  5.0f,  0.0f,   0.0f, 0.0f, 1.0f,    0.0f, 0.0f,
     2.0f,  6.0f,  2.0f,   0.0f, 0.0f, 1.0f,    1.0f, 0.0f,
     2.0f,  6.0f,  0.0f,   0.0f, 0.0f, 1.0f,    0.0f, 1.0f,

     3.0f,  5.0f,  3.0f,   0.0f, 0.0f, 1.0f,    0.0f, 0.0f,
     0.0f,  5.0f,  3.0f,   0.0f, 0.0f, 1.0f,    1.0f, 0.0f,
     2.0f,  6.0f,  2.0f,   0.0f, 0.0f, 1.0f,    0.0f, 1.0f,

     2.0f,  6.0f,  2.0f,   0.0f, 0.0f, 1.0f,    0.0f, 0.0f,
     0.0f,  5.0f,  3.0f,   0.0f, 0.0f, 1.0f,    1.0f, 0.0f,
     0.0f,  6.0f,  2.0f,   0.0f, 0.0f, 1.0f,    0.0f, 1.0f,
     // NEGATIVE Z NORMAL
    0.0f,  5.0f,  3.0f,   0.0f, 0.0f, -1.0f,   0.0f, 0.0f,
    -3.0f,  5.0f,  3.0f,   0.0f, 0.0f, -1.0f,   1.0f, 0.0f,
    -2.0f,  6.0f,  2.0f,   0.0f, 0.0f, -1.0f,   0.0f, 1.0f,

     0.0f,  5.0f,  3.0f,   0.0f, 0.0f, -1.0f,   0.0f, 0.0f,
    -2.0f,  6.0f,  2.0f,   0.0f, 0.0f, -1.0f,   1.0f, 0.0f,
     0.0f,  6.0f,  2.0f,   0.0f, 0.0f, -1.0f,   0.0f, 1.0f,

     0.0f,  6.0f,  2.0f,   0.0f, 0.0f, -1.0f,   0.0f, 0.0f,
    -2.0f,  6.0f,  2.0f,   0.0f, 0.0f, -1.0f,   1.0f, 0.0f,
    -1.0f,  7.0f,  1.0f,   0.0f, 0.0f, -1.0f,   0.0f, 1.0f,

    -1.0f,  7.0f,  1.0f,   0.0f, 0.0f, -1.0f,   0.0f, 0.0f,
    -2.0f,  6.0f,  2.0f,   0.0f, 0.0f, -1.0f,   1.0f, 0.0f,
    -2.0f,  6.0f,  0.0f,   0.0f, 0.0f, -1.0f,   0.0f, 1.0f,

    -1.0f,  7.0f,  1.0f,   0.0f, 0.0f, -1.0f,   0.0f, 0.0f,
    -2.0f,  6.0f,  0.0f,   0.0f, 0.0f, -1.0f,   1.0f, 0.0f,
    -1.0f,  7.0f, -1.0f,   0.0f, 0.0f, -1.0f,   0.0f, 1.0f,

    -2.0f,  6.0f,  0.0f,   0.0f, 0.0f, -1.0f,   0.0f, 0.0f,
    -2.0f,  6.0f, -2.0f,   0.0f, 0.0f, -1.0f,   1.0f, 0.0f,
    -1.0f,  7.0f, -1.0f,   0.0f, 0.0f, -1.0f,   0.0f, 1.0f,

    -1.0f,  7.0f, -1.0f,   0.0f, 0.0f, -1.0f,   0.0f, 0.0f,
    -2.0f,  6.0f, -2.0f,   0.0f, 0.0f, -1.0f,   1.0f, 0.0f,
     0.0f,  6.0f, -2.0f,   0.0f, 0.0f, -1.0f,   0.0f, 1.0f,

    -1.0f,  7.0f, -1.0f,   0.0f, 0.0f, -1.0f,   0.0f, 0.0f,
     0.0f,  6.0f, -2.0f,   0.0f, 0.0f, -1.0f,   1.0f, 0.0f,
     1.0f,  7.0f, -1.0f,   0.0f, 0.0f, -1.0f,   0.0f, 1.0f,

     1.0f,  7.0f, -1.0f,   0.0f, 0.0f, -1.0f,   0.0f, 0.0f,
     0.0f,  6.0f, -2.0f,   0.0f, 0.0f, -1.0f,   1.0f, 0.0f,
     2.0f,  6.0f, -2.0f,   0.0f, 0.0f, -1.0f,   0.0f, 1.0f,

     1.0f,  7.0f, -1.0f,   0.0f, 0.0f, -1.0f,   0.0f, 0.0f,
     2.0f,  6.0f, -2.0f,   0.0f, 0.0f, -1.0f,   1.0f, 0.0f,
     2.0f,  6.0f,  0.0f,   0.0f, 0.0f, -1.0f,   0.0f, 1.0f,

     1.0f,  7.0f, -1.0f,   0.0f, 0.0f, -1.0f,   0.0f, 0.0f,
     2.0f,  6.0f,  0.0f,   0.0f, 0.0f, -1.0f,   1.0f, 0.0f,
     1.0f,  7.0f,  1.0f,   0.0f, 0.0f, -1.0f,   0.0f, 1.0f,

     2.0f,  6.0f,  0.0f,   0.0f, 0.0f, -1.0f,   0.0f, 0.0f,
     2.0f,  6.0f,  2.0f,   0.0f, 0.0f, -1.0f,   1.0f, 0.0f,
     1.0f,  7.0f,  1.0f,   0.0f, 0.0f, -1.0f,   0.0f, 1.0f,

     2.0f,  6.0f,  2.0f,   0.0f, 0.0f, -1.0f,   0.0f, 0.0f,
     0.0f,  6.0f,  2.0f,   0.0f, 0.0f, -1.0f,   1.0f, 0.0f,
     1.0f,  7.0f,  1.0f,   0.0f, 0.0f, -1.0f,   0.0f, 1.0f,

     1.0f,  7.0f,  1.0f,   0.0f, 0.0f, -1.0f,   0.0f, 0.0f,
     0.0f,  6.0f,  2.0f,   0.0f, 0.0f, -1.0f,   1.0f, 0.0f,
    -1.0f,  7.0f,  1.0f,   0.0f, 0.0f, -1.0f,   0.0f, 1.0f,

    -1.0f,  7.0f,  1.0f,  -1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
    -1.0f,  7.0f, -1.0f,  -1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
    -1.0f,  8.0f, -1.0f,  -1.0f, 0.0f, 0.0f,   0.0f, 1.0f,

    -1.0f,  7.0f,  1.0f,  -1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
    -1.0f,  8.0f,  1.0f,  -1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
    -1.0f,  8.0f, -1.0f,  -1.0f, 0.0f, 0.0f,   0.0f, 1.0f,

    -1.0f,  7.0f, -1.0f,   0.0f, 0.0f, -1.0f,   0.0f, 0.0f,
     1.0f,  7.0f, -1.0f,   0.0f, 0.0f, -1.0f,   1.0f, 0.0f,
     1.0f,  8.0f, -1.0f,   0.0f, 0.0f, -1.0f,   0.0f, 1.0f,

    -1.0f,  7.0f, -1.0f,   0.0f, 0.0f, -1.0f,   0.0f, 0.0f,
    -1.0f,  8.0f, -1.0f,   0.0f, 0.0f, -1.0f,   1.0f, 0.0f,
     1.0f,  8.0f, -1.0f,   0.0f, 0.0f, -1.0f,   0.0f, 1.0f,

     1.0f,  7.0f, -1.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
     1.0f,  7.0f,  1.0f,   0.0f, 0.0f, 0.0f,   1.0f, 0.0f,
     1.0f,  8.0f,  1.0f,   0.0f, 0.0f, 0.0f,   0.0f, 1.0f,

     1.0f,  7.0f, -1.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
     1.0f,  8.0f, -1.0f,   0.0f, 0.0f, 0.0f,   1.0f, 0.0f,
     1.0f,  8.0f,  1.0f,   0.0f, 0.0f, 0.0f,   0.0f, 1.0f,

     1.0f,  7.0f,  1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
    -1.0f,  7.0f,  1.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
    -1.0f,  8.0f,  1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,

     1.0f,  7.0f,  1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
     1.0f,  8.0f,  1.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
    -1.0f,  8.0f,  1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,

    };

    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;

    mesh.pear_num_vertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));

    glGenVertexArrays(1, &mesh.pear_vao);
    glBindVertexArray(mesh.pear_vao);

    // CREATE AND ACTIVATE VBO
    glGenBuffers(1, &mesh.pear_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.pear_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    // STRIDES
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);

    // VERRTEX ATTRIBUTE POINTERS
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex + floatsPerNormal));
    glEnableVertexAttribArray(2);
}

// HOLDS APPLE VERTS
void UCreateApple(AppleMesh& mesh)
{
    GLfloat verts[] = {
        // BOTTOM HALF //
        //Positions         // NEGATIVE Y NORMAL   //Texture Coordinates
        0.0f, 0.0f, 0.0f,   0.0f, -1.0f,  0.0f,    0.0f, 0.0f,
       -3.0f, 0.0f, 0.0f,   0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
       -2.0f, 0.0f, -2.0f,  0.0f, -1.0f,  0.0f,    0.0f, 1.0f,

       0.0f, 0.0f, 0.0f,    0.0f, -1.0f,  0.0f,    0.0f, 0.0f,
      -2.0f, 0.0f, -2.0f,   0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
       0.0f, 0.0f, -3.0f,   0.0f, -1.0f,  0.0f,    0.0f, 1.0f,

       0.0f, 0.0f, 0.0f,    0.0f, -1.0f,  0.0f,    0.0f, 0.0f,
       0.0f, 0.0f, -3.0f,   0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
       2.0f, 0.0f, -2.0f,   0.0f, -1.0f,  0.0f,    0.0f, 1.0f,

       0.0f, 0.0f, 0.0f,    0.0f, -1.0f,  0.0f,    0.0f, 0.0f,
       2.0f, 0.0f, -2.0f,   0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
       3.0f, 0.0f, 0.0f,    0.0f, -1.0f,  0.0f,    0.0f, 1.0f,

       0.0f, 0.0f, 0.0f,    0.0f, -1.0f,  0.0f,    0.0f, 0.0f,
       3.0f, 0.0f, 0.0f,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
       2.0f, 0.0f, 2.0f,    0.0f, -1.0f,  0.0f,    0.0f, 1.0f,

       0.0f, 0.0f, 0.0f,    0.0f, -1.0f,  0.0f,    0.0f, 0.0f,
       2.0f, 0.0f, 2.0f,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
       0.0f, 0.0f, 3.0f,    0.0f, -1.0f,  0.0f,    0.0f, 1.0f,

       0.0f, 0.0f, 0.0f,    0.0f, -1.0f,  0.0f,    0.0f, 0.0f,
       0.0f, 0.0f, 3.0f,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
      -2.0f, 0.0f, 2.0f,    0.0f, -1.0f,  0.0f,    0.0f, 1.0f,

       0.0f, 0.0f, 0.0f,    0.0f, -1.0f,  0.0f,    0.0f, 0.0f,
      -2.0f, 0.0f, 2.0f,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
      -3.0f, 0.0f, 0.0f,    0.0f, -1.0f,  0.0f,    0.0f, 1.0f,

      // 2ND LAYER OF BOTTOM -- Y = 2
      //Positions         // NORMAL            //Texture Coordinates
      -3.0f, 0.0f, 0.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
      -4.0f, 2.0f, 0.0f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
      -2.0f, 0.0f,-2.0f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

      -4.0f, 2.0f, 0.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
      -3.0f, 2.0f, -3.0f, 0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
      -2.0f, 0.0f, -2.0f, 0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

      -2.0f, 0.0f, -2.0f, 0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
      -3.0f, 2.0f, -3.0f, 0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
       0.0f, 2.0f, -4.0f, 0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

       -2.0f, 0.0f, -2.0f, 0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        0.0f, 2.0f, -4.0f, 0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        0.0f, 0.0f, -3.0f, 0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

        0.0f, 0.0f, -3.0f, 0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        0.0f, 2.0f, -4.0f, 0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        2.0f, 0.0f, -2.0f, 0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

        0.0f, 2.0f, -4.0f, 0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        3.0f, 2.0f, -3.0f, 0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        2.0f, 0.0f, -2.0f, 0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

        2.0f, 0.0f, -2.0f, 0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        3.0f, 2.0f, -3.0f, 0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        4.0f, 2.0f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

        2.0f, 0.0f, -2.0f, 0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        4.0f, 2.0f,  0.0f, 0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        3.0f, 0.0f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

        3.0f, 0.0f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
        4.0f, 2.0f,  0.0f, 0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
        2.0f, 0.0f,  2.0f, 0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

        4.0f, 2.0f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
        3.0f, 2.0f,  3.0f, 0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
        2.0f, 0.0f,  2.0f, 0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

        2.0f, 0.0f,  2.0f, 0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
        3.0f, 2.0f,  3.0f, 0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
        0.0f, 2.0f,  4.0f, 0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

        2.0f, 0.0f,  2.0f, 0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
        0.0f, 2.0f,  4.0f, 0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
        0.0f, 0.0f,  3.0f, 0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

        0.0f, 0.0f,  3.0f, 0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
        0.0f, 2.0f,  4.0f, 0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
       -2.0f, 0.0f,  2.0f, 0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

        0.0f, 2.0f,  4.0f, 0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
       -3.0f, 2.0f,  3.0f, 0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
       -2.0f, 0.0f,  2.0f, 0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

       -2.0f, 0.0f,  2.0f, 0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
       -3.0f, 2.0f,  3.0f, 0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
       -4.0f, 2.0f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

       -2.0f, 0.0f,  2.0f, 0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
       -4.0f, 2.0f,  0.0f, 0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
       -3.0f, 0.0f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

       // MIDDLE LAYER
       //Positions         // NORMAL        //Texture Coordinates
             -3.0f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
             -4.0f, 2.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
             -2.0f, 4.0f, -2.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

             -4.0f, 2.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
             -3.0f, 2.0f, -3.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
             -2.0f, 4.0f, -2.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

             -2.0f, 4.0f, -2.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
             -3.0f, 2.0f, -3.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
             0.0f, 2.0f, -4.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

             -2.0f, 4.0f, -2.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
             0.0f, 2.0f, -4.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
             0.0f, 4.0f, -3.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

             0.0f, 4.0f, -3.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
             0.0f, 2.0f, -4.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
             2.0f, 4.0f, -2.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

             0.0f, 2.0f, -4.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
             3.0f, 2.0f, -3.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
             2.0f, 4.0f, -2.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

             2.0f, 4.0f, -2.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
             3.0f, 2.0f, -3.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
             4.0f, 2.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

             2.0f, 4.0f, -2.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
             4.0f, 2.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
             3.0f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

             3.0f, 4.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
             4.0f, 2.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
             2.0f, 4.0f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

             4.0f, 2.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
             3.0f, 2.0f, 3.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
             2.0f, 4.0f, 2.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

             2.0f, 4.0f, 2.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
             3.0f, 2.0f, 3.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
             0.0f, 2.0f, 4.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

             2.0f, 4.0f, 2.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
             0.0f, 2.0f, 4.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
             0.0f, 4.0f, 3.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

             0.0f, 4.0f, 3.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
             0.0f, 2.0f, 4.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
             -2.0f, 4.0f, 2.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

             0.0f, 2.0f, 4.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
             -3.0f, 2.0f, 3.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
             -2.0f, 4.0f, 2.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

             -2.0f, 4.0f, 2.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
             -3.0f, 2.0f, 3.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
             -4.0f, 2.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

             -2.0f, 4.0f, 2.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
             -4.0f, 2.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
             -3.0f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
        // TOP
            0.0f, 4.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            -3.0f, 4.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            -2.0f, 4.0f, -2.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

            0.0f, 4.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            -2.0f, 4.0f, -2.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 4.0f, -3.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

            0.0f, 4.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 4.0f, -3.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            2.0f, 4.0f, -2.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

            0.0f, 4.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            2.0f, 4.0f, -2.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            3.0f, 4.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

            0.0f, 4.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            3.0f, 4.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            2.0f, 4.0f, 2.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

            0.0f, 4.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            2.0f, 4.0f, 2.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 4.0f, 3.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

            0.0f, 4.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 4.0f, 3.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            -2.0f, 4.0f, 2.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

            0.0f, 4.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            -2.0f, 4.0f, 2.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            -3.0f, 4.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
         
    };

    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;

    mesh.apple_num_vertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));

    glGenVertexArrays(1, &mesh.apple_vao);
    glBindVertexArray(mesh.apple_vao);

    // CREATE AND ACTIVATE VBO
    glGenBuffers(1, &mesh.apple_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.apple_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    // STRIDES
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);

    // VERRTEX ATTRIBUTE POINTERS
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex + floatsPerNormal));
    glEnableVertexAttribArray(2);
}

// HOLDS VASE VERTS
void UCreateVase(VaseMesh& mesh)
{
    // Vertex data
    GLfloat verts[] = {
        // BOTTOM OF VASE
        //Positions         // NEGATIVE Y NORMAL   //Texture Coordinates
        0.0f, 0.0f, 0.0f,   0.0f, -1.0f,  0.0f,    0.0f, 0.0f,
       -3.0f, 0.0f, 0.0f,   0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
       -2.0f, 0.0f, -2.0f,  0.0f, -1.0f,  0.0f,    0.0f, 1.0f,

       0.0f, 0.0f, 0.0f,    0.0f, -1.0f,  0.0f,    0.0f, 0.0f,
      -2.0f, 0.0f, -2.0f,   0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
       0.0f, 0.0f, -3.0f,   0.0f, -1.0f,  0.0f,    0.0f, 1.0f,

       0.0f, 0.0f, 0.0f,    0.0f, -1.0f,  0.0f,    0.0f, 0.0f,
       0.0f, 0.0f, -3.0f,   0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
       2.0f, 0.0f, -2.0f,   0.0f, -1.0f,  0.0f,    0.0f, 1.0f,

       0.0f, 0.0f, 0.0f,    0.0f, -1.0f,  0.0f,    0.0f, 0.0f,
       2.0f, 0.0f, -2.0f,   0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
       3.0f, 0.0f, 0.0f,    0.0f, -1.0f,  0.0f,    0.0f, 1.0f,

       0.0f, 0.0f, 0.0f,    0.0f, -1.0f,  0.0f,    0.0f, 0.0f,
       3.0f, 0.0f, 0.0f,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
       2.0f, 0.0f, 2.0f,    0.0f, -1.0f,  0.0f,    0.0f, 1.0f,

       0.0f, 0.0f, 0.0f,    0.0f, -1.0f,  0.0f,    0.0f, 0.0f,
       2.0f, 0.0f, 2.0f,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
       0.0f, 0.0f, 3.0f,    0.0f, -1.0f,  0.0f,    0.0f, 1.0f,

       0.0f, 0.0f, 0.0f,    0.0f, -1.0f,  0.0f,    0.0f, 0.0f,
       0.0f, 0.0f, 3.0f,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
      -2.0f, 0.0f, 2.0f,    0.0f, -1.0f,  0.0f,    0.0f, 1.0f,

       0.0f, 0.0f, 0.0f,    0.0f, -1.0f,  0.0f,    0.0f, 0.0f,
      -2.0f, 0.0f, 2.0f,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
      -3.0f, 0.0f, 0.0f,    0.0f, -1.0f,  0.0f,    0.0f, 1.0f,

      // 2ND LAYER OF BOTTOM -- Y = 2
      //Positions         // NORMAL            //Texture Coordinates
      -3.0f, 0.0f, 0.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
      -4.0f, 2.0f, 0.0f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
      -2.0f, 0.0f,-2.0f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

      -4.0f, 2.0f, 0.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
      -3.0f, 2.0f, -3.0f, 0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
      -2.0f, 0.0f, -2.0f, 0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

      -2.0f, 0.0f, -2.0f, 0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
      -3.0f, 2.0f, -3.0f, 0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
       0.0f, 2.0f, -4.0f, 0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

       -2.0f, 0.0f, -2.0f, 0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        0.0f, 2.0f, -4.0f, 0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        0.0f, 0.0f, -3.0f, 0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

        0.0f, 0.0f, -3.0f, 0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        0.0f, 2.0f, -4.0f, 0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        2.0f, 0.0f, -2.0f, 0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

        0.0f, 2.0f, -4.0f, 0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        3.0f, 2.0f, -3.0f, 0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        2.0f, 0.0f, -2.0f, 0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

        2.0f, 0.0f, -2.0f, 0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        3.0f, 2.0f, -3.0f, 0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        4.0f, 2.0f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

        2.0f, 0.0f, -2.0f, 0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        4.0f, 2.0f,  0.0f, 0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        3.0f, 0.0f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

        3.0f, 0.0f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
        4.0f, 2.0f,  0.0f, 0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
        2.0f, 0.0f,  2.0f, 0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

        4.0f, 2.0f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
        3.0f, 2.0f,  3.0f, 0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
        2.0f, 0.0f,  2.0f, 0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

        2.0f, 0.0f,  2.0f, 0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
        3.0f, 2.0f,  3.0f, 0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
        0.0f, 2.0f,  4.0f, 0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

        2.0f, 0.0f,  2.0f, 0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
        0.0f, 2.0f,  4.0f, 0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
        0.0f, 0.0f,  3.0f, 0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

        0.0f, 0.0f,  3.0f, 0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
        0.0f, 2.0f,  4.0f, 0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
       -2.0f, 0.0f,  2.0f, 0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

        0.0f, 2.0f,  4.0f, 0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
       -3.0f, 2.0f,  3.0f, 0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
       -2.0f, 0.0f,  2.0f, 0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

       -2.0f, 0.0f,  2.0f, 0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
       -3.0f, 2.0f,  3.0f, 0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
       -4.0f, 2.0f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

       -2.0f, 0.0f,  2.0f, 0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
       -4.0f, 2.0f,  0.0f, 0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
       -3.0f, 0.0f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

      // MIDDLE LAYER
      //Positions         // NEGATIVE Y NORMAL   //Texture Coordinates
            -3.0f, 7.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
            -4.0f, 2.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
            -2.0f, 7.5f, -2.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

            -4.0f, 2.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
            -3.0f, 2.0f, -3.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
            -2.0f, 7.5f, -2.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

            -2.0f, 7.5f, -2.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
            -3.0f, 2.0f, -3.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
            0.0f, 2.0f, -4.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

            -2.0f, 7.5f, -2.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
            0.0f, 2.0f, -4.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
            0.0f, 7.5f, -3.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

            0.0f, 7.5f, -3.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
            0.0f, 2.0f, -4.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
            2.0f, 7.5f, -2.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

            0.0f, 2.0f, -4.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
            3.0f, 2.0f, -3.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
            2.0f, 7.5f, -2.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

            2.0f, 7.5f, -2.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
            3.0f, 2.0f, -3.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
            4.0f, 2.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

            2.0f, 7.5f, -2.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
            4.0f, 2.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
            3.0f, 7.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

            3.0f, 7.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            4.0f, 2.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            2.0f, 7.5f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

            4.0f, 2.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            3.0f, 2.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            2.0f, 7.5f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

            2.0f, 7.5f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            3.0f, 2.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            0.0f, 2.0f, 4.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

            2.0f, 7.5f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 2.0f, 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            0.0f, 7.5f, 3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

            0.0f, 7.5f, 3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 2.0f, 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            -2.0f, 7.5f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

            0.0f, 2.0f, 4.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            -3.0f, 2.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            -2.0f, 7.5f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

            -2.0f, 7.5f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            -3.0f, 2.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            -4.0f, 2.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

            -2.0f, 7.5f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            -4.0f, 2.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            -3.0f, 7.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 
    };

    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;

    mesh.vase_num_vertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));

    glGenVertexArrays(1, &mesh.vase_vao);
    glBindVertexArray(mesh.vase_vao);

    // CREATE AND ACTIVATE VBO
    glGenBuffers(1, &mesh.vase_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vase_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    // STRIDES
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);

    // VERRTEX ATTRIBUTE POINTERS
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex + floatsPerNormal));
    glEnableVertexAttribArray(2);
}

//HOLDS CHALICE VERTS
void UCreateChalice(ChaliceMesh& mesh)
{

    GLfloat verts[] = {
        // BOTTOM OF CHALLICE CUP -- NOT STEM
        //Positions         // NEGATIVE Y NORMAL   //Texture Coordinates
        0.0f, 0.0f, 0.0f,   0.0f, -1.0f,  0.0f,    0.0f, 0.0f,
       -3.0f, 0.0f, 0.0f,   0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
       -2.0f, 0.0f, -2.0f,  0.0f, -1.0f,  0.0f,    0.0f, 1.0f,

       0.0f, 0.0f, 0.0f,    0.0f, -1.0f,  0.0f,    0.0f, 0.0f,
      -2.0f, 0.0f, -2.0f,   0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
       0.0f, 0.0f, -3.0f,   0.0f, -1.0f,  0.0f,    0.0f, 1.0f,

       0.0f, 0.0f, 0.0f,    0.0f, -1.0f,  0.0f,    0.0f, 0.0f,
       0.0f, 0.0f, -3.0f,   0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
       2.0f, 0.0f, -2.0f,   0.0f, -1.0f,  0.0f,    0.0f, 1.0f,

       0.0f, 0.0f, 0.0f,    0.0f, -1.0f,  0.0f,    0.0f, 0.0f,
       2.0f, 0.0f, -2.0f,   0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
       3.0f, 0.0f, 0.0f,    0.0f, -1.0f,  0.0f,    0.0f, 1.0f,

       0.0f, 0.0f, 0.0f,    0.0f, -1.0f,  0.0f,    0.0f, 0.0f,
       3.0f, 0.0f, 0.0f,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
       2.0f, 0.0f, 2.0f,    0.0f, -1.0f,  0.0f,    0.0f, 1.0f,

       0.0f, 0.0f, 0.0f,    0.0f, -1.0f,  0.0f,    0.0f, 0.0f,
       2.0f, 0.0f, 2.0f,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
       0.0f, 0.0f, 3.0f,    0.0f, -1.0f,  0.0f,    0.0f, 1.0f,

       0.0f, 0.0f, 0.0f,    0.0f, -1.0f,  0.0f,    0.0f, 0.0f,
       0.0f, 0.0f, 3.0f,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
      -2.0f, 0.0f, 2.0f,    0.0f, -1.0f,  0.0f,    0.0f, 1.0f,

       0.0f, 0.0f, 0.0f,    0.0f, -1.0f,  0.0f,    0.0f, 0.0f,
      -2.0f, 0.0f, 2.0f,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
      -3.0f, 0.0f, 0.0f,    0.0f, -1.0f,  0.0f,    0.0f, 1.0f,

      // 2ND LAYER OF BOTTOM -- Y = 2
      //Positions         // NORMAL            //Texture Coordinates
      -3.0f, 0.0f, 0.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
      -4.0f, 2.0f, 0.0f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
      -2.0f, 0.0f,-2.0f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

      -4.0f, 2.0f, 0.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
      -3.0f, 2.0f, -3.0f, 0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
      -2.0f, 0.0f, -2.0f, 0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

      -2.0f, 0.0f, -2.0f, 0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
      -3.0f, 2.0f, -3.0f, 0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
       0.0f, 2.0f, -4.0f, 0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

       -2.0f, 0.0f, -2.0f, 0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        0.0f, 2.0f, -4.0f, 0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        0.0f, 0.0f, -3.0f, 0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

        0.0f, 0.0f, -3.0f, 0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        0.0f, 2.0f, -4.0f, 0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        2.0f, 0.0f, -2.0f, 0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

        0.0f, 2.0f, -4.0f, 0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        3.0f, 2.0f, -3.0f, 0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        2.0f, 0.0f, -2.0f, 0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

        2.0f, 0.0f, -2.0f, 0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        3.0f, 2.0f, -3.0f, 0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        4.0f, 2.0f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

        2.0f, 0.0f, -2.0f, 0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        4.0f, 2.0f,  0.0f, 0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        3.0f, 0.0f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

        3.0f, 0.0f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
        4.0f, 2.0f,  0.0f, 0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
        2.0f, 0.0f,  2.0f, 0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

        4.0f, 2.0f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
        3.0f, 2.0f,  3.0f, 0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
        2.0f, 0.0f,  2.0f, 0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

        2.0f, 0.0f,  2.0f, 0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
        3.0f, 2.0f,  3.0f, 0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
        0.0f, 2.0f,  4.0f, 0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

        2.0f, 0.0f,  2.0f, 0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
        0.0f, 2.0f,  4.0f, 0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
        0.0f, 0.0f,  3.0f, 0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

        0.0f, 0.0f,  3.0f, 0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
        0.0f, 2.0f,  4.0f, 0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
       -2.0f, 0.0f,  2.0f, 0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

        0.0f, 2.0f,  4.0f, 0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
       -3.0f, 2.0f,  3.0f, 0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
       -2.0f, 0.0f,  2.0f, 0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

       -2.0f, 0.0f,  2.0f, 0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
       -3.0f, 2.0f,  3.0f, 0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
       -4.0f, 2.0f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

       -2.0f, 0.0f,  2.0f, 0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
       -4.0f, 2.0f,  0.0f, 0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
       -3.0f, 0.0f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

       // MIDDLE LAYER
       //Positions         // NORMAL        //Texture Coordinates
             -3.0f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
             -4.0f, 2.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
             -2.0f, 4.0f, -2.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

             -4.0f, 2.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
             -3.0f, 2.0f, -3.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
             -2.0f, 4.0f, -2.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

             -2.0f, 4.0f, -2.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
             -3.0f, 2.0f, -3.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
             0.0f, 2.0f, -4.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

             -2.0f, 4.0f, -2.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
             0.0f, 2.0f, -4.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
             0.0f, 4.0f, -3.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

             0.0f, 4.0f, -3.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
             0.0f, 2.0f, -4.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
             2.0f, 4.0f, -2.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

             0.0f, 2.0f, -4.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
             3.0f, 2.0f, -3.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
             2.0f, 4.0f, -2.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

             2.0f, 4.0f, -2.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
             3.0f, 2.0f, -3.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
             4.0f, 2.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

             2.0f, 4.0f, -2.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
             4.0f, 2.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
             3.0f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

             3.0f, 4.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
             4.0f, 2.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
             2.0f, 4.0f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

             4.0f, 2.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
             3.0f, 2.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
             2.0f, 4.0f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

             2.0f, 4.0f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
             3.0f, 2.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
             0.0f, 2.0f, 4.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

             2.0f, 4.0f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
             0.0f, 2.0f, 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
             0.0f, 4.0f, 3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

             0.0f, 4.0f, 3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
             0.0f, 2.0f, 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
             -2.0f, 4.0f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

             0.0f, 2.0f, 4.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
             -3.0f, 2.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
             -2.0f, 4.0f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

             -2.0f, 4.0f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
             -3.0f, 2.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
             -4.0f, 2.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

             -2.0f, 4.0f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
             -4.0f, 2.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
             -3.0f, 4.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

        // CHALLICE STEM
        //Positions         // NEGATIVE Y NORMAL   //Texture Coordinates
            0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            -1.0f, -3.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

            0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            -1.0f, -3.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, -3.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            //STEM -- LEFT SIDE -- BACK
            //Positions           // NORMALs         //Texture Coordinates
            -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
            0.0f, -3.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

            -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
            -1.0f, -3.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
            0.0f, -3.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
            //STEM -- RIGHT SIDE -- BACK
            //Positions           // NORMALs         //Texture Coordinates
            0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
            1.0f, -3.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

            0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
            0.0f, -3.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
            1.0f, -3.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
            //STEM -- RIGHT SIDE -- FRONT
            //Positions           // NORMALs         //Texture Coordinates
            1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            0.0f, -3.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

            1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            1.0f, -3.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            0.0f, -3.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

        // BOTTOM FLAT 
        //Positions           // NORMALs         //Texture Coordinates
                 // PETAL BASE 1 OUT OF 4
                 -1.0f, -3.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                 -2.0f, -4.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                 -3.0f, -4.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                 -1.0f, -3.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                 -3.0f, -4.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                 -3.0f, -4.0f, -2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                 -1.0f, -3.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                 -3.0f, -4.0f, -2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                 -2.0f, -4.0f, -3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                 -1.0f, -3.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                 -2.0f, -4.0f, -3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                 0.0f, -3.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                 0.0f, -3.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                 -2.0f, -4.0f, -3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                 -1.0f, -4.0f, -3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                 0.0f, -3.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                 -1.0f, -4.0f, -3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                 0.0f, -4.0f, -2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                 // PETAL BASE 2 OUT OF 4 (BASE LAYER Y = 15)
                 //Positions           // NORMALs         //Texture Coordinates
                 1.0f, -3.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                 2.0f, -4.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                 3.0f, -4.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                 1.0f, -3.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                 3.0f, -4.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                 3.0f, -4.0f, -2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                 1.0f, -3.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                 3.0f, -4.0f, -2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                 2.0f, -4.0f, -3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                 1.0f, -3.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                 2.0f, -4.0f, -3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                 0.0f, -3.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                 0.0f, -3.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                 2.0f, -4.0f, -3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                 1.0f, -4.0f, -3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                 0.0f, -3.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                 1.0f, -4.0f, -3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                 0.0f, -4.0f, -2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                 // PETAL BASE 3 OUT OF 4
                 //Positions           // NORMALs         //Texture Coordinates
                 1.0f, -3.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                 2.0f, -4.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                 3.0f, -4.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                 1.0f, -3.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                 3.0f, -4.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                 3.0f, -4.0f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                 1.0f, -3.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                 3.0f, -4.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                 2.0f, -4.0f, 3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                 1.0f, -3.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                 2.0f, -4.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                 0.0f, -3.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                 0.0f, -3.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                 2.0f, -4.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                 1.0f, -4.0f, 3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                 0.0f, -3.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                 1.0f, -4.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                 0.0f, -4.0f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                 // PETAL BASE 4 OUT OF 4
                 //Positions           // NORMALs         //Texture Coordinates
                 -1.0f, -3.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                 -2.0f, -4.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                 -3.0f, -4.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                 -1.0f, -3.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                 -3.0f, -4.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                 -3.0f, -4.0f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                 -1.0f, -3.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                 -3.0f, -4.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                 -2.0f, -4.0f, 3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                 -1.0f, -3.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                 -2.0f, -4.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                 0.0f, -3.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                 0.0f, -3.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                 -2.0f, -4.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                 -1.0f, -4.0f, 3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                 0.0f, -3.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                 -1.0f, -4.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                 0.0f, -4.0f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    };
    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;

    mesh.chalice_num_vertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));

    glGenVertexArrays(1, &mesh.chalice_vao);
    glBindVertexArray(mesh.chalice_vao);

    // CREATE AND ACTIVATE VBO
    glGenBuffers(1, &mesh.chalice_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.chalice_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    // STRIDES
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);

    // VERRTEX ATTRIBUTE POINTERS
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex + floatsPerNormal));
    glEnableVertexAttribArray(2);
}

// HOLDS PLANE (TABLE) VERTS
void UCreatePlane(PlaneMesh& mesh)
{
    // Vertex data
    GLfloat verts[] = {
        //Positions        // NORMALs            //Texture Coordinates
       0.0f, 0.0f, 0.0f,   0.0f,  1.0f,  0.0f,   0.0f, 0.0f,
       0.0f, 0.0f, -5.0f,  0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
       5.0f, 0.0f, 0.0f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f,

       5.0f, 0.0f, 0.0f,   0.0f,  1.0f,  0.0f,   0.0f, 0.0f,
       0.0f, 0.0f, -5.0f,  0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
       5.0f, 0.0f, -5.0f,  0.0f,  1.0f,  0.0f,   0.0f, 1.0f,
    };
    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;

    mesh.plane_num_vertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));

    glGenVertexArrays(1, &mesh.plane_vao);
    glBindVertexArray(mesh.plane_vao);

    // CREATE AND ACTIVATE VBO
    glGenBuffers(1, &mesh.plane_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.plane_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    // STRIDES
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);

    // VERRTEX ATTRIBUTE POINTERS
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex + floatsPerNormal));
    glEnableVertexAttribArray(2);
}

// HOLDS FLOWER VERTS
void UCreateStem(StemMesh& mesh)
{
    GLfloat verts[] = {
        // STEM -- LEFT SIDE -- FRONT
        //Positions           // NORMALs         //Texture Coordinates
        0.0f, 0.0f, 1.0f,   -1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
       -1.0f, 0.0f, 0.0f,   -1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
       -1.0f, 15.0f, 0.0f,  -1.0f, 0.0f, 0.0f,   0.0f, 1.0f,

       0.0f, 0.0f, 1.0f,   -1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
      -1.0f, 15.0f, 0.0f,  -1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
       0.0f, 15.0f, 1.0f,  -1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
       //STEM -- LEFT SIDE -- BACK
       //Positions           // NORMALs         //Texture Coordinates
       -1.0f, 0.0f, 0.0f,     0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        0.0f, 0.0f, -1.0f,    0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        0.0f, 15.0f, -1.0f,   0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

        -1.0f, 0.0f, 0.0f,     0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        -1.0f, 15.0f, 0.0f,    0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.0f, 15.0f, -1.0f,   0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
         //STEM -- RIGHT SIDE -- BACK
         //Positions           // NORMALs         //Texture Coordinates
         0.0f, 0.0f, -1.0f,    0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         1.0f, 0.0f, 0.0f,     0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         1.0f, 15.0f, 0.0f,    0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

         0.0f, 0.0f, -1.0f,    0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.0f, 15.0f, -1.0f,   0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         1.0f, 15.0f, 0.0f,    0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
         //STEM -- RIGHT SIDE -- FRONT
         //Positions           // NORMALs         //Texture Coordinates
         1.0f, 0.0f, 0.0f,     0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
         0.0f, 0.0f, 1.0f,     0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
         0.0f, 15.0f, 1.0f,    0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

         1.0f, 0.0f, 0.0f,     0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
         1.0f, 15.0f, 0.0f,    0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
         0.0f, 15.0f, 1.0f,    0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
         //FLOWER -- STOMA -- TOP
         //Positions           // NORMALs         //Texture Coordinates
         -1.0f, 15.0f, 0.0f,   0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
          0.0f, 15.0f, -1.0f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
          0.0f, 16.0f, 0.0f,   0.0f,  1.0f,  0.0f,  0.0f, 1.0f,

          0.0f, 15.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
          1.0f, 15.0f, 0.0f,   0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
          0.0f, 16.0f, 0.0f,   0.0f,  1.0f,  0.0f,  0.0f, 0.0f,

          1.0f, 15.0f, 0.0f,   0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
          0.0f, 15.0f, 1.0f,   0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
          0.0f, 16.0f, 0.0f,   0.0f,  1.0f,  0.0f,  0.0f, 0.0f,

          0.0f, 15.0f, 1.0f,   0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
         -1.0f, 15.0f, 0.0f,   0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
          0.0f, 16.0f, 0.0f,   0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         //LEAVES Y = 8 -- LEFT -- FRONT
         //Positions           // NORMALs         //Texture Coordinates
         -1.0f, 12.0f, 0.0f,    0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
         -4.0f, 12.0f, -1.0f,   0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         -2.0f, 12.0f, -1.0f,   0.0f,  1.0f,  0.0f,  0.0f, 1.0f,

         -1.0f, 12.0f, 0.0f,    0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
         -5.0f, 12.0f, 0.0f,    0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         -4.0f, 12.0f, -1.0f,   0.0f,  1.0f,  0.0f,  0.0f, 1.0f,

         -1.0f, 12.0f, 0.0f,    0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
         -5.0f, 12.0f, 0.0f,    0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         -4.0f, 12.0f, 1.0f,    0.0f,  1.0f,  0.0f,  0.0f, 1.0f,

         -1.0f, 12.0f, 0.0f,    0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
         -4.0f, 12.0f, 1.0f,    0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         -2.0f, 12.0f, 1.0f,    0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         // LEAVES Y = 8 -- RIGHT -- FRONT
         //Positions           // NORMALs         //Texture Coordinates
         1.0f, 12.0f, 0.0f,    0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
         2.0f, 12.0f, -1.0f,   0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         4.0f, 12.0f, -1.0f,   0.0f,  1.0f,  0.0f,  0.0f, 1.0f,

         1.0f, 12.0f, 0.0f,    0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
         4.0f, 12.0f, -1.0f,   0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         5.0f, 12.0f, 0.0f,    0.0f,  1.0f,  0.0f,  0.0f, 1.0f,

         1.0f, 12.0f, 0.0f,    0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
         4.0f, 12.0f, 1.0f,    0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         5.0f, 12.0f, 0.0f,    0.0f,  1.0f,  0.0f,  0.0f, 1.0f,

         1.0f, 12.0f, 0.0f,    0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
         2.0f, 12.0f, 1.0f,    0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         4.0f, 12.0f, 1.0f,    0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
    };
    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;

    mesh.stem_num_vertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));

    glGenVertexArrays(1, &mesh.stem_vao);
    glBindVertexArray(mesh.stem_vao);

    // CREATE AND ACTIVATE VBO
    glGenBuffers(1, &mesh.stem_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.stem_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    // STRIDES
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);

    // VERRTEX ATTRIBUTE POINTERS
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex + floatsPerNormal));
    glEnableVertexAttribArray(2);
}

void UCreatePetal(PetalMesh& mesh)
{
    GLfloat verts[] =
    {
        //FLOWER -- STOMA -- TOP
         //Positions           // NORMALs         //Texture Coordinates
         -1.0f, 15.0f, 0.0f,   0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
          0.0f, 15.0f, -1.0f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
          0.0f, 16.0f, 0.0f,   0.0f,  1.0f,  0.0f,  0.0f, 1.0f,

          0.0f, 15.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
          1.0f, 15.0f, 0.0f,   0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
          0.0f, 16.0f, 0.0f,   0.0f,  1.0f,  0.0f,  0.0f, 0.0f,

          1.0f, 15.0f, 0.0f,   0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
          0.0f, 15.0f, 1.0f,   0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
          0.0f, 16.0f, 0.0f,   0.0f,  1.0f,  0.0f,  0.0f, 0.0f,

          0.0f, 15.0f, 1.0f,   0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
         -1.0f, 15.0f, 0.0f,   0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
          0.0f, 16.0f, 0.0f,   0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        // FLOWER -- PETAL 1 OUT OF 4 (BASE LAYER Y = 15)
         //Positions           // NORMALs         //Texture Coordinates
         -1.0f, 15.0f, 0.0f,   0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
         -2.0f, 15.0f, 0.0f,   0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
         -3.0f, 15.0f,-1.0f,   0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

         -1.0f, 15.0f, 0.0f,   0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
         -3.0f, 15.0f,-1.0f,   0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
         -3.0f, 15.0f,-2.0f,   0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

         -1.0f, 15.0f, 0.0f,   0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
         -3.0f, 15.0f,-2.0f,   0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
         -2.0f, 15.0f,-3.0f,   0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

         -1.0f, 15.0f, 0.0f,   0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
         -2.0f, 15.0f,-3.0f,   0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
          0.0f, 15.0f,-1.0f,   0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

          0.0f, 15.0f,-1.0f,   0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
         -2.0f, 15.0f,-3.0f,   0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
         -1.0f, 15.0f,-3.0f,   0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

         0.0f, 15.0f,-1.0f,    0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
        -1.0f, 15.0f,-3.0f,    0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
         0.0f, 15.0f,-2.0f,    0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
         // FLOWER -- PETAL 2 OUT OF 4 (BASE LAYER Y = 15)
         //Positions           // NORMALs         //Texture Coordinates
         1.0f, 15.0f, 0.0f,   0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
         2.0f, 15.0f, 0.0f,   0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
         3.0f, 15.0f,-1.0f,   0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

         1.0f, 15.0f, 0.0f,   0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
         3.0f, 15.0f,-1.0f,   0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
         3.0f, 15.0f,-2.0f,   0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

         1.0f, 15.0f, 0.0f,   0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
         3.0f, 15.0f,-2.0f,   0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
         2.0f, 15.0f,-3.0f,   0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

         1.0f, 15.0f, 0.0f,   0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
         2.0f, 15.0f,-3.0f,   0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
         0.0f, 15.0f,-1.0f,   0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

         0.0f, 15.0f,-1.0f,   0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
         2.0f, 15.0f,-3.0f,   0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
         1.0f, 15.0f,-3.0f,   0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

         0.0f, 15.0f,-1.0f,    0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
         1.0f, 15.0f,-3.0f,    0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
         0.0f, 15.0f,-2.0f,    0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
         // FLOWER -- PETAL 3 OUT OF 4 (BASE LAYER Y = 15)
         //Positions           // NORMALs         //Texture Coordinates
           1.0f, 15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
           2.0f, 15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
           3.0f, 15.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

           1.0f, 15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
           3.0f, 15.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
           3.0f, 15.0f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

           1.0f, 15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
           3.0f, 15.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
           2.0f, 15.0f, 3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

           1.0f, 15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
           2.0f, 15.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
           0.0f, 15.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

           0.0f, 15.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
           2.0f, 15.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
           1.0f, 15.0f, 3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

           0.0f, 15.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
           1.0f, 15.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
           0.0f, 15.0f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
           // FLOWER -- PETAL 4 OUT OF 4 (BASE LAYER Y = 15)
           //Positions           // NORMALs         //Texture Coordinates
             -1.0f, 15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
             -2.0f, 15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
             -3.0f, 15.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

             -1.0f, 15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
             -3.0f, 15.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
             -3.0f, 15.0f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

             -1.0f, 15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
             -3.0f, 15.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
             -2.0f, 15.0f, 3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

             -1.0f, 15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
             -2.0f, 15.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
              0.0f, 15.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

              0.0f, 15.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
             -2.0f, 15.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
             -1.0f, 15.0f, 3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

              0.0f, 15.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
             -1.0f, 15.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
              0.0f, 15.0f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
              //FLOWER -- PETALS, 2ND LAYER (ONLY Y'S ARE ELEVATED Y = 16)
              // FLOWER -- PETAL 1 OUT OF 4
              //Positions           // NORMALs         //Texture Coordinates
                  -1.0f, 15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                  -2.0f, 16.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                  -3.0f, 16.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                  -1.0f, 15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                  -3.0f, 16.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                  -3.0f, 16.0f, -2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                  -1.0f, 15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                  -3.0f, 16.0f, -2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                  -2.0f, 16.0f, -3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                  -1.0f, 15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                  -2.0f, 16.0f, -3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                  0.0f, 15.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                  0.0f, 15.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                  -2.0f, 16.0f, -3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                  -1.0f, 16.0f, -3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                  0.0f, 15.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                  -1.0f, 16.0f, -3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                  0.0f, 16.0f, -2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                  // FLOWER -- PETAL 2 OUT OF 4
                  //Positions           // NORMALs         //Texture Coordinates
                  1.0f, 15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                  2.0f, 16.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                  3.0f, 16.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                  1.0f, 15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                  3.0f, 16.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                  3.0f, 16.0f, -2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                  1.0f, 15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                  3.0f, 16.0f, -2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                  2.0f, 16.0f, -3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                  1.0f, 15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                  2.0f, 16.0f, -3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                  0.0f, 15.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                  0.0f, 15.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                  2.0f, 16.0f, -3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                  1.0f, 16.0f, -3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                  0.0f, 15.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                  1.0f, 16.0f, -3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                  0.0f, 16.0f, -2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                  // FLOWER -- PETAL 3 OUT OF 4
                  //Positions           // NORMALs         //Texture Coordinates
                  1.0f, 15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                  2.0f, 16.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                  3.0f, 16.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                  1.0f, 15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                  3.0f, 16.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                  3.0f, 16.0f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                  1.0f, 15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                  3.0f, 16.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                  2.0f, 16.0f, 3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                  1.0f, 15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                  2.0f, 16.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                  0.0f, 15.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                  0.0f, 15.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                  2.0f, 16.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                  1.0f, 16.0f, 3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                  0.0f, 15.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                  1.0f, 16.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                  0.0f, 16.0f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                  // FLOWER -- PETAL 4 OUT OF 4
                  //Positions           // NORMALs         //Texture Coordinates
                  -1.0f, 15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                  -2.0f, 16.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                  -3.0f, 16.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                  -1.0f, 15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                  -3.0f, 16.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                  -3.0f, 16.0f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                  -1.0f, 15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                  -3.0f, 16.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                  -2.0f, 16.0f, 3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                  -1.0f, 15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                  -2.0f, 16.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                  0.0f, 15.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                  0.0f, 15.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                  -2.0f, 16.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                  -1.0f, 16.0f, 3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                  0.0f, 15.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                  -1.0f, 16.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                  0.0f, 16.0f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                  //FLOWER -- PETALS, 3RD LAYER (ONLY Y'S ARE ELEVATED Y = 17)
                  //Positions           // NORMALs         //Texture Coordinates
                 // FLOWER -- PETAL 1 OUT OF 4
                  //Positions           // NORMALs         //Texture Coordinates
                          -1.0f, 15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                          -2.0f, 17.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                          -3.0f, 17.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                          -1.0f, 15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                          -3.0f, 17.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                          -3.0f, 17.0f, -2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                          -1.0f, 15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                          -3.0f, 17.0f, -2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                          -2.0f, 17.0f, -3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                          -1.0f, 15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                          -2.0f, 17.0f, -3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                          0.0f, 15.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                          0.0f, 15.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                          -2.0f, 17.0f, -3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                          -1.0f, 17.0f, -3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                          0.0f, 15.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                          -1.0f, 17.0f, -3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                          0.0f, 17.0f, -2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                          // FLOWER -- PETAL 2 OUT OF 4
                          //Positions           // NORMALs         //Texture Coordinates
                          1.0f, 15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                          2.0f, 17.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                          3.0f, 17.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                          1.0f, 15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                          3.0f, 17.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                          3.0f, 17.0f, -2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                          1.0f, 15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                          3.0f, 17.0f, -2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                          2.0f, 17.0f, -3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                          1.0f, 15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                          2.0f, 17.0f, -3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                          0.0f, 15.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                          0.0f, 15.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                          2.0f, 17.0f, -3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                          1.0f, 17.0f, -3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                          0.0f, 15.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                          1.0f, 17.0f, -3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                          0.0f, 17.0f, -2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                          // FLOWER -- PETAL 3 OUT OF 4
                          //Positions           // NORMALs         //Texture Coordinates
                          1.0f, 15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                          2.0f, 17.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                          3.0f, 17.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                          1.0f, 15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                          3.0f, 17.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                          3.0f, 17.0f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                          1.0f, 15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                          3.0f, 17.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                          2.0f, 17.0f, 3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                          1.0f, 15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                          2.0f, 17.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                          0.0f, 15.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                          0.0f, 15.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                          2.0f, 17.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                          1.0f, 17.0f, 3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                          0.0f, 15.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                          1.0f, 17.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                          0.0f, 17.0f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                          // FLOWER -- PETAL 4 OUT OF 4
                          //Positions           // NORMALs         //Texture Coordinates
                          -1.0f, 15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                          -2.0f, 17.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                          -3.0f, 17.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                          -1.0f, 15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                          -3.0f, 17.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                          -3.0f, 17.0f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                          -1.0f, 15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                          -3.0f, 17.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                          -2.0f, 17.0f, 3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                          -1.0f, 15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                          -2.0f, 17.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                          0.0f, 15.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                          0.0f, 15.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                          -2.0f, 17.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                          -1.0f, 17.0f, 3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                          0.0f, 15.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                          -1.0f, 17.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                          0.0f, 17.0f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    };

    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;

    mesh.petal_num_vertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));

    glGenVertexArrays(1, &mesh.petal_vao);
    glBindVertexArray(mesh.petal_vao);

    // CREATE AND ACTIVATE VBO
    glGenBuffers(1, &mesh.petal_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.petal_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    // STRIDES
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);

    // VERRTEX ATTRIBUTE POINTERS
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float)* floatsPerVertex));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float)* floatsPerVertex + floatsPerNormal));
    glEnableVertexAttribArray(2);
}

void UDestroyMesh(PearMesh& mesh)
{
    glDeleteVertexArrays(1, &mesh.pear_vao);
    glDeleteBuffers(1, &mesh.pear_vbo);
}

void UDestroyMesh(AppleMesh& mesh)
{
    glDeleteVertexArrays(1, &mesh.apple_vao);
    glDeleteBuffers(1, &mesh.apple_vbo);
}

void UDestroyMesh(StemMesh& mesh)
{
    glDeleteVertexArrays(1, &mesh.stem_vao);
    glDeleteBuffers(1, &mesh.stem_vbo);
}

void UDestroyMesh(PetalMesh& mesh)
{
    glDeleteVertexArrays(1, &mesh.petal_vao);
    glDeleteBuffers(1, &mesh.petal_vbo);
}

void UDestroyMesh(ChaliceMesh& mesh)
{
    glDeleteVertexArrays(1, &mesh.chalice_vao);
    glDeleteBuffers(1, &mesh.chalice_vbo);
}
void UDestroyMesh(VaseMesh& mesh)
{
    glDeleteVertexArrays(1, &mesh.vase_vao);
    glDeleteBuffers(1, &mesh.vase_vbo);
}

void UDestroyMesh(PlaneMesh& mesh)
{
    glDeleteVertexArrays(1, &mesh.plane_vao);
    glDeleteBuffers(1, &mesh.plane_vbo);
}

// GENERATE TEXTURE
bool UCreateTexture(const char* filename, GLuint& textureId)
{
    int width, height, channels;
    unsigned char* image = stbi_load(filename, &width, &height, &channels, 0);
    if (image)
    {
        flipImageVertically(image, width, height, channels);

        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);

        // TEXTURE WRAPPING
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // TEXTURE FILTERING
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (channels == 3)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        else if (channels == 4)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        else
        {
            cout << "Not implemented to handle image with " << channels << " channels" << endl;
            return false;
        }

        glGenerateMipmap(GL_TEXTURE_2D);

        // CLEAN UP
        stbi_image_free(image);
        glBindTexture(GL_TEXTURE_2D, 0);

        return true;
    }

    // ERROR HANDLING
    return false;
}

void UDestroyTexture(GLuint textureId)
{
    glGenTextures(1, &textureId);
}

bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId)
{

    int success = 0;
    char infoLog[512];

    programId = glCreateProgram();

    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShaderId, 1, &vtxShaderSource, NULL);
    glShaderSource(fragmentShaderId, 1, &fragShaderSource, NULL);


    glCompileShader(vertexShaderId);
    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;

        return false;
    }
    glCompileShader(fragmentShaderId);

    glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderId, sizeof(infoLog), NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;

        return false;
    }
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);

    glLinkProgram(programId);
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(programId, sizeof(infoLog), NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;

        return false;
    }

    glUseProgram(programId);

    return true;
}


void UDestroyShaderProgram(GLuint programId)
{
    glDeleteProgram(programId);
}