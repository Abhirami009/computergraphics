#include "gl_framework.hpp"
#include "shader_util.hpp"

// Variables for rotations
float rot_angle_x = 0.0f;
float rot_angle_y = 0.0f;
float rot_angle_z = 0.0f;


float radius = 5;
int sectorCount = 100;
int stackCount = 100;

std::vector<float> vertices_global;

// Function to generate sphere vertices (position and color)
std::vector<unsigned int> indices_global;

// Function to generate sphere vertices (position and color) with indices
void generateSphere(float radius, int sectorCount, int stackCount) {
    vertices_global.clear();
    indices_global.clear();

    for (int i = 0; i <= stackCount; ++i) {
        float stackAngle = glm::pi<float>() * float(i) / float(stackCount);
        float xy = radius * sin(stackAngle);
        float z = radius * cos(stackAngle);

        for (int j = 0; j <= sectorCount; ++j) {
            float sectorAngle = 2 * glm::pi<float>() * float(j) / float(sectorCount);
            float x = xy * cos(sectorAngle);
            float y = xy * sin(sectorAngle);

            // Add vertex coordinates (x, y, z) and color (r, g, b)
            vertices_global.push_back(x);
            vertices_global.push_back(y);
            vertices_global.push_back(z);

            float red = fabs(sin(sectorAngle));
            float green = fabs(cos(stackAngle));
            float blue = fabs(sin(stackAngle) * cos(sectorAngle));

            vertices_global.push_back(red);
            vertices_global.push_back(green);
            vertices_global.push_back(blue);
        }
    }

    for (int i = 0; i < stackCount; ++i) {
        int k1 = i * (sectorCount + 1);
        int k2 = k1 + sectorCount + 1;
        
        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            indices_global.push_back(k1);
            indices_global.push_back(k2);
            indices_global.push_back(k1 + 1);

            indices_global.push_back(k1 + 1);
            indices_global.push_back(k2);
            indices_global.push_back(k2 + 1);
        }
    }
}



// Global shader program and buffer objects
GLuint shaderProgram;
GLuint vbo, vao, ebo;

// Function to initialize shaders
void initShadersGL(void) {
    std::string vertex_shader_file("simple_vs.glsl");
    std::string fragment_shader_file("simple_fs.glsl");

    std::vector<GLuint> shaderList;
    shaderList.push_back(glFramework::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
    shaderList.push_back(glFramework::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

    shaderProgram = glFramework::CreateProgramGL(shaderList);
}

// Function to process user input for rotation
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        rot_angle_x -= 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        rot_angle_x += 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        rot_angle_y += 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        rot_angle_y -= 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        rot_angle_z += 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        rot_angle_z -= 1.0f;
    }
}

// Function to initialize vertex buffer
void initVertexBufferGL(void) {
    generateSphere(radius, sectorCount, stackCount);
  
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices_global.size() * sizeof(float), &vertices_global[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_global.size() * sizeof(unsigned int), &indices_global[0], GL_STATIC_DRAW);
}

// Function to render the scene
void renderGL(GLFWwindow* window) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE); // Enable back-face culling

    processInput(window); 

    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLint projectionLoc = glGetUniformLocation(shaderProgram, "projection");

    // Set up the view matrix (camera)
    glm::mat4 view = glm::lookAt(glm::vec3(15.0f, 15.0f, 15.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(rot_angle_x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rot_angle_y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rot_angle_z), glm::vec3(0.0f, 0.0f, 1.0f));

    glUseProgram(shaderProgram);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices_global.size(), GL_UNSIGNED_INT, 0);
}

// Main function
int main(int argc, char** argv)
{
    GLFWwindow* window;

    glfwSetErrorCallback(glFramework::error_callback);

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

    window = glfwCreateWindow(640, 480, "Rotating Sphere", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
  
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cerr << "GLEW Init Failed" << std::endl;
        return -1;
    }

    glfwSetKeyCallback(window, glFramework::key_callback);
    glfwSetFramebufferSizeCallback(window, glFramework::framebuffer_size_callback);

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glFramework::initGL();
    glEnable(GL_DEPTH_TEST);

    initShadersGL();
    initVertexBufferGL();

    while (!glfwWindowShouldClose(window)) {
        renderGL(window);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
