#include "gl_framework.hpp"
#include "shader_util.hpp"

glm::mat4 model_matrix = glm::mat4(1.0f); //model transformations
glm::mat4 view_matrix = glm::lookAt(glm::vec3(0.0,0.0,-2.0),glm::vec3(0.0,0.0,0.0),glm::vec3(0.0,1.0,0.0));
glm::mat4 ortho_matrix = glm::ortho(-2.0, 2.0, -2.0, 2.0, -20.0, 20.0);


float points[] = {
    -0.25, -0.25, -0.25,  // a
    -0.25,  0.25, -0.25,
     0.25,  0.25, -0.25,
    -0.25, -0.25, -0.25,
     0.25, -0.25, -0.25,
     0.25,  0.25, -0.25,

     0.25, -0.25, -0.25,  // b
     0.25, -0.25,  0.25,
     0.25,  0.25, -0.25,
     0.25,  0.25, -0.25,
     0.25,  0.25,  0.25,
     0.25, -0.25,  0.25,

    -0.25,  0.25,  0.25,  // c
    -0.25, -0.25,  0.25,
     0.25,  0.25,  0.25,
    -0.25, -0.25,  0.25,
     0.25, -0.25,  0.25,
     0.25,  0.25,  0.25,

    -0.25, -0.25,  0.25,  // f
    -0.25,  0.25,  0.25,
    -0.25,  0.25, -0.25,
    -0.25, -0.25,  0.25,
    -0.25, -0.25, -0.25,
    -0.25,  0.25, -0.25,

    -0.25,  0.25,  0.25,  // d
    -0.25,  0.25, -0.25,
     0.25,  0.25,  0.25,
    -0.25,  0.25, -0.25,
     0.25,  0.25, -0.25,
     0.25,  0.25,  0.25,

    -0.25, -0.25, -0.25,  // e
    -0.25, -0.25,  0.25,
     0.25, -0.25,  0.25,
    -0.25, -0.25, -0.25,
     0.25, -0.25, -0.25,
     0.25, -0.25,  0.25
};


float colors[] = {
     0.5, 0.5, 0.5, //a
     0.0, 1.0, 0.0,
     1.0, 1.0, 0.0,
     0.5, 0.5, 0.5,
     1.0, 0.0, 0.0,
     1.0, 1.0, 0.0,
     
     1.0, 0.0, 0.0, //b
     1.0, 0.0, 1.0,
     1.0, 1.0, 0.0,
     1.0, 1.0, 0.0,
     1.0, 1.0, 1.0,
     1.0, 0.0, 1.0,

     0.0, 1.0, 1.0, //c
     0.0, 0.0, 1.0,
     1.0, 1.0, 1.0,
     0.0, 0.0, 1.0,
     1.0, 0.0, 1.0,
     1.0, 1.0, 1.0,

     0.0, 0.0, 1.0, //f
     0.0, 1.0, 1.0,
     0.0, 1.0, 0.0,
     0.0, 0.0, 1.0,
     0.5, 0.5, 0.5,
     0.0, 1.0, 0.0,

     0.0, 1.0, 1.0, //d
     0.0, 1.0, 0.0,
     1.0, 1.0, 1.0,
     0.0, 1.0, 0.0,
     1.0, 1.0, 0.0,
     1.0, 1.0, 1.0,

     0.5, 0.5, 0.5, //e
     0.0, 0.0, 1.0,
     1.0, 0.0, 1.0,
     0.5, 0.5, 0.5,
     1.0, 0.0, 0.0,
     1.0, 0.0, 1.0
};



GLuint shaderProgram;
GLuint vbo, vao;




void initShadersGL(void)
{
  std::string vertex_shader_file("simple_vs.glsl");
  std::string fragment_shader_file("simple_fs.glsl");

  std::vector<GLuint> shaderList;
  shaderList.push_back(glFramework::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList.push_back(glFramework::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

  shaderProgram = glFramework::CreateProgramGL(shaderList);
  
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {

        model_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(-1.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * model_matrix;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {

        model_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * model_matrix;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {

        model_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * model_matrix;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        model_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(-1.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * model_matrix;
    }
}









void initVertexBufferGL(void)
{
  // for a Vertex Array Object (vao)
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // for a Vertex Buffer Object (vbo)
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  // Allocate enough memory for both positions and colors
  glBufferData(GL_ARRAY_BUFFER, sizeof(points)+sizeof(colors), NULL, GL_STATIC_DRAW);

  // Upload the vertex positions
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);

  // Upload the vertex colors with an offset of 9 * sizeof(float)
  glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);

  // Set up position attribute
  GLuint vPosition = glGetAttribLocation(shaderProgram, "vp");
  glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE,0, (void*)0);
  glEnableVertexAttribArray(vPosition);

  // Set up color attribute (offset --> sizeof(points))
  GLuint vColor = glGetAttribLocation(shaderProgram, "color");
  glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE,0, (void*)sizeof(points));
  glEnableVertexAttribArray(vColor);
}


void renderGL(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgram);
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model_matrix));

    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view_matrix));

    GLuint projLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(ortho_matrix));

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
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
  window = glfwCreateWindow(640, 480, "Colored Triangle", NULL, NULL);
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
    processInput(window); 
      // Render here
      renderGL();

      // Swap front and back buffers
      glfwSwapBuffers(window);
      
      // Poll for and process events
      glfwPollEvents();
    }
  
  glfwTerminate();
  return 0;
}

