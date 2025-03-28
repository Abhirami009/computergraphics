#include "gl_framework.hpp"
#include "shader_util.hpp"

float rot_angle_x = 0.0f;
float rot_angle_y = 0.0f;
float rot_angle_z = 0.0f;


  // Positions for a cube
  float points[] = {
    // Front face
    -0.95f, -0.95f, -0.95f,  0.0f, 0.0f,
     0.95f, -0.95f, -0.95f,  1.0f, 0.0f, 
     0.95f,  0.95f, -0.95f,  1.0f, 1.0f, 
    -0.95f,  0.95f, -0.95f,  0.0f, 1.0f, 

    // Back face
    -0.95f, -0.95f,  0.95f,  1.0f, 0.0f,
     0.95f, -0.95f,  0.95f,  0.0f, 0.0f,
     0.95f,  0.95f,  0.95f,  0.0f, 1.0f,
    -0.95f,  0.95f,  0.95f,  1.0f, 1.0f,

    // Left face
    -0.95f, -0.95f,  0.95f,  0.0f, 0.0f,
    -0.95f, -0.95f, -0.95f,  1.0f, 0.0f,
    -0.95f,  0.95f, -0.95f,  1.0f, 1.0f,
    -0.95f,  0.95f,  0.95f,  0.0f, 1.0f,

    // Right face
     0.95f, -0.95f, -0.95f,  0.0f, 0.0f,
     0.95f, -0.95f,  0.95f,  1.0f, 0.0f,
     0.95f,  0.95f,  0.95f,  1.0f, 1.0f,
     0.95f,  0.95f, -0.95f,  0.0f, 1.0f,

    // Top face
    -0.95f,  0.95f, -0.95f,  0.0f, 0.0f,
     0.95f,  0.95f, -0.95f,  1.0f, 0.0f,
     0.95f,  0.95f,  0.95f,  1.0f, 1.0f,
    -0.95f,  0.95f,  0.95f,  0.0f, 1.0f,

    // Bottom face
    -0.95f, -0.95f, -0.95f,  0.0f, 0.0f,
     0.95f, -0.95f, -0.95f,  1.0f, 0.0f,
     0.95f, -0.95f,  0.95f,  1.0f, 1.0f,
    -0.95f, -0.95f,  0.95f,  0.0f, 1.0f
};


unsigned int indices[] = {
  // Front face
  0, 1, 2,  2, 3, 0,

  // Back face
  4, 5, 6,  6, 7, 4,

  // Left face
  8, 9, 10,  10, 11, 8,

  // Right face
  12, 13, 14,  14, 15, 12,

  // Top face
  16, 17, 18,  18, 19, 16,

  // Bottom face
  20, 21, 22,  22, 23, 20
};





GLuint shaderProgram;
GLuint vbo, vao, ebo, texVBO, texture;




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
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {

        // model_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(-1.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * model_matrix;
        rot_angle_x -= 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {

        // model_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * model_matrix;
        rot_angle_x += 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {

        // model_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * model_matrix;
        rot_angle_y += 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {

        // model_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(-1.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * model_matrix;
        rot_angle_y -= 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {

      // model_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * model_matrix;
      rot_angle_z += 1.0f;
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {

      // model_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(-1.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * model_matrix;
      rot_angle_z -= 1.0f;
  }
    
}












GLuint loadBMP(const char* filename) {
  std::ifstream file(filename, std::ios::binary);
  if (!file) {
      std::cerr << "Failed to open BMP file: " << filename << std::endl;
      return 0;
  }

  unsigned char header[54];
  file.read(reinterpret_cast<char*>(header), 54);
  if (header[0] != 'B' || header[1] != 'M') {
      std::cerr << "Not a valid BMP file." << std::endl;
      return 0;
  }

  int dataPos = *reinterpret_cast<int*>(&header[0x0A]);
  int width = *reinterpret_cast<int*>(&header[0x12]);
  int height = *reinterpret_cast<int*>(&header[0x16]);
  int imageSize = *reinterpret_cast<int*>(&header[0x22]);
  if (imageSize == 0) imageSize = width * height * 3;
  if (dataPos == 0) dataPos = 54;

  unsigned char* data = new unsigned char[imageSize];
  file.seekg(dataPos);
  file.read(reinterpret_cast<char*>(data), imageSize);
  file.close();

  GLuint textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);

  glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  delete[] data;
  return textureID;
}


void initVertexBufferGL(void) {
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Create VBO (for both positions & texture coordinates)
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

  // Create EBO
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // Enable vertex position attribute
  GLuint vPosition = glGetAttribLocation(shaderProgram, "vp");
  glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(vPosition);

  // Enable texture coordinate attribute
  GLuint texCoordLoc = glGetAttribLocation(shaderProgram, "texCoord");
  glVertexAttribPointer(texCoordLoc, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(texCoordLoc);

  // Load texture
  texture = loadBMP("texture2.bmp");
}



void renderGL(GLFWwindow* window) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  processInput(window);
  
  glm::mat4 model_matrix = glm::mat4(1.0f);
  glm::mat4 view_matrix = glm::lookAt(glm::vec3(0.0,0.0,-2.0), glm::vec3(0.0,0.0,0.0), glm::vec3(0.0,1.0,0.0));
  glm::mat4 ortho_matrix = glm::ortho(-2.0, 2.0, -2.0, 2.0, -20.0, 20.0);
  
  model_matrix = glm::rotate(model_matrix, glm::radians(rot_angle_x), glm::vec3(1.0f, 0.0f, 0.0f));
  model_matrix = glm::rotate(model_matrix, glm::radians(rot_angle_y), glm::vec3(0.0f, 1.0f, 0.0f));
  model_matrix = glm::rotate(model_matrix, glm::radians(rot_angle_z), glm::vec3(0.0f, 0.0f, 1.0f));
  
  glUseProgram(shaderProgram);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
  GLuint texLoc = glGetUniformLocation(shaderProgram, "texture1");
  glUniform1i(texLoc, 0);
  
  GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model_matrix));
  
  GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view_matrix));
  
  GLuint projLoc = glGetUniformLocation(shaderProgram, "projection");
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(ortho_matrix));
  
  
  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
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





