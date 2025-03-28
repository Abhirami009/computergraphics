#include "gl_framework.hpp"
#include "shader_util.hpp"

// glm::mat4 model_matrix = glm::mat4(1.0f); //model transformations
// glm::mat4 view_matrix = glm::lookAt(glm::vec3(0.0,0.0,-2.0),glm::vec3(0.0,0.0,0.0),glm::vec3(0.0,1.0,0.0));
// glm::mat4 ortho_matrix = glm::ortho(-2.0, 2.0, -2.0, 2.0, -20.0, 20.0);

float rot_angle_x = 0.0f;
float rot_angle_y = 0.0f;
float rot_angle_z = 0.0f;


float points[] = {
-0.25, -0.25, -0.25, //0
-0.25,  0.25, -0.25, //1
 0.25,  0.25, -0.25, //2
 0.25, -0.25, -0.25, //3
 0.25, -0.25,  0.25, //4
 0.25,  0.25,  0.25, //5
-0.25,  0.25,  0.25, //6
-0.25, -0.25,  0.25  //7
};


float colors[] = {
     0.5, 0.5, 0.5, 
     0.0, 1.0, 0.0, 
     1.0, 1.0, 0.0, 
     1.0, 0.0, 0.0, 
     1.0, 0.0, 1.0,
     1.0, 1.0, 1.0,
     0.0, 1.0, 1.0, 
     0.0, 0.0, 1.0,
};

int indices[] ={
  0,1,2,
  0,3,2,
  3,4,2,
  2,5,4,
  6,7,5,
  7,4,5,
  7,6,1,
  7,0,1,
  6,1,5,
  1,2,5,
  0,7,4,
  0,3,4
};



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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_I && action == GLFW_PRESS) {
        int vertexIndex;
        float r, g, b;

        std::cout << "Enter vertex number (0-7): ";
        std::cin >> vertexIndex;

        if (vertexIndex < 0 || vertexIndex >= 8) {
            std::cout << "Invalid vertex index! Choose between 0-7.\n";
            return;
        }

        std::cout << "Enter new RGB values (0.0 - 1.0): ";
        std::cin >> r >> g >> b;

        if (r < 0.0f || r > 1.0f || g < 0.0f || g > 1.0f || b < 0.0f || b > 1.0f) {
            std::cout << "Invalid color values! Use values between 0.0 and 1.0.\n";
            return;
        }

        // Update the color array
        colors[vertexIndex * 3] = r;
        colors[vertexIndex * 3 + 1] = g;
        colors[vertexIndex * 3 + 2] = b;

        // Push the updated colors to GPU
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);
        std::cout << "Color updated for vertex " << vertexIndex << "!\n";
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

  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);



  // Allocate enough memory for both positions and colors
  glBufferData(GL_ARRAY_BUFFER, sizeof(points)+sizeof(colors), NULL, GL_STATIC_DRAW);

  // Upload the vertex positions
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);

  // Upload the vertex colors with an offset of 9 * sizeof(float)
  glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);

  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // Set up position attribute
  GLuint vPosition = glGetAttribLocation(shaderProgram, "vp");
  glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE,0, (void*)0);
  glEnableVertexAttribArray(vPosition);

  // Set up color attribute (offset --> sizeof(points))
  GLuint vColor = glGetAttribLocation(shaderProgram, "color");
  glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE,0, (void*)sizeof(points));
  glEnableVertexAttribArray(vColor);

}


void renderGL(GLFWwindow* window) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    processInput(window); 

    glm::mat4 model_matrix = glm::mat4(1.0f); //model transformations
    glm::mat4 view_matrix = glm::lookAt(glm::vec3(0.0,0.0,-2.0),glm::vec3(0.0,0.0,0.0),glm::vec3(0.0,1.0,0.0));
    glm::mat4 ortho_matrix = glm::ortho(-2.0, 2.0, -2.0, 2.0, -20.0, 20.0);

    view_matrix = glm::rotate(view_matrix, glm::radians(rot_angle_x), glm::vec3(1.0f, 0.0f, 0.0f));
    view_matrix = glm::rotate(view_matrix, glm::radians(rot_angle_y), glm::vec3(0.0f, 1.0f, 0.0f));
    view_matrix = glm::rotate(view_matrix, glm::radians(rot_angle_z), glm::vec3(0.0f, 0.0f, 1.0f));



    glUseProgram(shaderProgram);
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
  glfwSetKeyCallback(window, key_callback);
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

