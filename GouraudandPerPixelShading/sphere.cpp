#include "gl_framework.hpp"
#include "shader_util.hpp"

// glm::mat4 model_matrix = glm::mat4(1.0f); //model transformations
// glm::mat4 view_matrix = glm::lookAt(glm::vec3(0.0,0.0,-2.0),glm::vec3(0.0,0.0,0.0),glm::vec3(0.0,1.0,0.0));
// glm::mat4 ortho_matrix = glm::ortho(-2.0, 2.0, -2.0, 2.0, -20.0, 20.0);

float rot_angle_x = 0.0f;
float rot_angle_y = 0.0f;
float rot_angle_z = 0.0f;

float radius = 10;
int sectorCount = 20;
int stackCount = 30;

std::vector<float> vertices_global;

std::vector<float> generateSphere(float radius, int sectorCount, int stackCount) {
  std::vector<float> vertices;
  for (int i = 0; i <= stackCount; ++i) {
      float stackAngle = glm::pi<float>() * float(i) / float(stackCount);
      float xy = radius * sin(stackAngle);
      float z = radius * cos(stackAngle);

      for (int j = 0; j <= sectorCount; ++j) {
          float sectorAngle = 2 * glm::pi<float>() * float(j) / float(sectorCount);
          float x = xy * cos(sectorAngle);
          float y = xy * sin(sectorAngle);

          // Add vertex coordinates (x, y, z) and color (r, g, b)
          vertices.push_back(x);
          vertices.push_back(y);
          vertices.push_back(z);

          // Simple coloring based on vertex position
          float red = (x + radius) / (2 * radius);
          float green = (y + radius) / (2 * radius);
          float blue = (z + radius) / (2 * radius);

          vertices.push_back(red);
          vertices.push_back(green);
          vertices.push_back(blue);
      }
  }
  return vertices;
}


GLuint shaderProgram;
GLuint vbo, vao, ebo;




void initShadersGL(void)
{
  std::string vertex_shader_file("simple_vs.glsl");
  std::string fragment_shader_file("simple_fs.glsl");

  std::vector<GLuint> shaderList;
  shaderList.push_back(glFramework::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList.push_back(glFramework::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

  shaderProgram = glFramework::CreateProgramGL(shaderList);
  
}
//A,D (AROUND X) W,S (Y) Z,C (Z)
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {

        // model_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(-1.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * model_matrix;
        rot_angle_x -= 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {

        // model_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * model_matrix;
        rot_angle_x += 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {

        // model_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * model_matrix;
        rot_angle_y += 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {

        // model_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(-1.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * model_matrix;
        rot_angle_y -= 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {

      // model_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * model_matrix;
      rot_angle_z += 1.0f;
  }
  if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {

      // model_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(-1.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * model_matrix;
      rot_angle_z -= 1.0f;
  }
    
}

// void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
//   if (key == GLFW_KEY_I && action == GLFW_PRESS) {
//         int vertexIndex;
//         float r, g, b;

//         std::cout << "Enter vertex number (0-7): ";
//         std::cin >> vertexIndex;

//         if (vertexIndex < 0 || vertexIndex >= 8) {
//             std::cout << "Invalid vertex index! Choose between 0-7.\n";
//             return;
//         }

//         std::cout << "Enter new RGB values (0.0 - 1.0): ";
//         std::cin >> r >> g >> b;

//         if (r < 0.0f || r > 1.0f || g < 0.0f || g > 1.0f || b < 0.0f || b > 1.0f) {
//             std::cout << "Invalid color values! Use values between 0.0 and 1.0.\n";
//             return;
//         }

//         // Update the color array
      

//         // Push the updated colors to GPU
//         glBindBuffer(GL_ARRAY_BUFFER, vbo);
//         glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);
//         std::cout << "Color updated for vertex " << vertexIndex << "!\n";
//     }
// }











void initVertexBufferGL(void)
{
  vertices_global = generateSphere(radius, sectorCount, stackCount);
  glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices_global.size() * sizeof(float), &vertices_global[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);




}


void renderGL(GLFWwindow* window) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    processInput(window); 

    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
        GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
        GLint projectionLoc = glGetUniformLocation(shaderProgram, "projection");

    glm::mat4 view = glm::lookAt(glm::vec3(3.0f, 3.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    // Set up the model matrix (for rotating the sphere)
    glm::mat4 model = glm::mat4(1.0f);

    view = glm::rotate(view, glm::radians(rot_angle_x), glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::rotate(view, glm::radians(rot_angle_y), glm::vec3(0.0f, 1.0f, 0.0f));
    view = glm::rotate(view, glm::radians(rot_angle_z), glm::vec3(0.0f, 0.0f, 1.0f));



    glUseProgram(shaderProgram);
   

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Draw the sphere
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices_global.size() / 6);

}




int main(int argc, char** argv)
{
  //! The pointer to the GLFW window
  GLFWwindow* window;

  //! Setting up the GLFW Error callback
  glfwSetErrorCallback(glFramework::error_callback);

  //! Initialize GLFW
  if (!glfwInit())
    return -1;

  //We want OpenGL 4.0
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  //This is for MacOSX - can be omitted otherwise
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
  //We don't want the old OpenGL 
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

  //! Create a windowed mode window and its OpenGL context
  window = glfwCreateWindow(640, 480, "Rotating view", NULL, NULL);
  if (!window)
    {
      glfwTerminate();
      return -1;
    }
  
  //! Make the window's context current 
  glfwMakeContextCurrent(window);

  //Initialize GLEW
  //Turn this on to get Shader based OpenGL
  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (GLEW_OK != err)
    {
      //Problem: glewInit failed, something is seriously wrong.
      std::cerr<<"GLEW Init Failed : %s"<<std::endl;
    }

  //Print and see what context got enabled
  std::cout<<"Vendor: "<<glGetString (GL_VENDOR)<<std::endl;
  std::cout<<"Renderer: "<<glGetString (GL_RENDERER)<<std::endl;
  std::cout<<"Version: "<<glGetString (GL_VERSION)<<std::endl;
  std::cout<<"GLSL Version: "<<glGetString (GL_SHADING_LANGUAGE_VERSION)<<std::endl;

  //Keyboard Callback
  glfwSetKeyCallback(window, glFramework::key_callback);
  //Framebuffer resize callback
  glfwSetFramebufferSizeCallback(window, glFramework::framebuffer_size_callback);

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  //Initialize GL state
  glFramework::initGL();
  glEnable(GL_DEPTH_TEST);

  initShadersGL();
  initVertexBufferGL();

  // Loop until the user closes the window
  while (glfwWindowShouldClose(window) == 0)
    {
    //  processInput(window); 
      // Render here
      renderGL(window);

      // Swap front and back buffers
      glfwSwapBuffers(window);
      
      // Poll for and process events
      glfwPollEvents();
    }
  
  glfwTerminate();
  return 0;
}

