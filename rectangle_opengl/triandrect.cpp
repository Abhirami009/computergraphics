#include "gl_framework.hpp"
#include "shader_util.hpp"

float points_t[] = {
	  // Each vertex has position [x, y, z] 
	  0.2f, -0.2f, 0.0f,
	   -0.4f, -0.2f, 0.0f,
	   -0.2f,  0.0f, 0.0f,
  };

float colors_t[] = {
      1.0, 0.0, 0.0, // for the red color of the vertex
      0.0, 1.0, 0.0, // for the green color
      0.0, 0.0, 1.0 // for the blue color
};

 float points_r[] ={
    0.0f, 0.3f, 0.0f,
    0.5f, 0.3f, 0.0f,
    0.5f, 0.3f, 0.0f,
    0.5f, 0.0f, 0.0f,
    0.5f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 0.3f, 0.0f
 };

 float colors_r[] = {
    1.0, 0.0, 0.0, // for the red color of the vertex
      0.0, 1.0, 0.0, // for the green color
      0.0, 1.0, 0.0,
      0.0, 0.0, 1.0,
      0.0, 0.0, 1.0,
      1.0, 1.0, 0.0,
      1.0, 1.0, 0.0,
      1.0, 0.0, 0.0
 };

GLuint shaderProgram;
GLuint vbor, vbot, vaor, vaot;
// GLuint vbo, vao;

void initShadersGL(void)
{
  std::string vertex_shader_file("simple_vs.glsl");
  std::string fragment_shader_file("simple_fs.glsl");

  std::vector<GLuint> shaderList;
  shaderList.push_back(glFramework::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList.push_back(glFramework::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

  shaderProgram = glFramework::CreateProgramGL(shaderList);
  
}

void initVertexBufferGL(void)
{
  // for a Vertex Array Object (vao)
  glGenVertexArrays(1, &vaot);
  glBindVertexArray(vaot);

  // for a Vertex Buffer Object (vbo)
  glGenBuffers(1, &vbot);
  glBindBuffer(GL_ARRAY_BUFFER, vbot);

  // Allocate enough memory for both positions and colors
  glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), NULL, GL_STATIC_DRAW);

  // Upload the vertex positions
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points_t), points_t);

  // Upload the vertex colors with an offset of 9 * sizeof(float)
  glBufferSubData(GL_ARRAY_BUFFER, sizeof(points_t), sizeof(colors_t), colors_t);

  // Set up position attribute
  GLuint vPosition = glGetAttribLocation(shaderProgram, "vp");
  glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE,0, (void*)0);
  glEnableVertexAttribArray(vPosition);

  // Set up color attribute (offset --> sizeof(points))
  GLuint vColor = glGetAttribLocation(shaderProgram, "color");
  glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE,0, (void*)sizeof(points_t));
  glEnableVertexAttribArray(vColor);
}

void initVertexBufferforRect(void) {
    glGenVertexArrays(1, &vaor);
    glBindVertexArray(vaor);

    glGenBuffers(1, &vbor);
    glBindBuffer(GL_ARRAY_BUFFER, vbor);

    glBufferData(GL_ARRAY_BUFFER, 48 * sizeof(float), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points_r), points_r);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points_r), sizeof(colors_r), colors_r);

    GLuint vPosition = glGetAttribLocation(shaderProgram, "vp");
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(vPosition);

    GLuint vColor = glGetAttribLocation(shaderProgram, "color");
    glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, (void*)sizeof(points_r));
    glEnableVertexAttribArray(vColor);
}



void renderGL(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(shaderProgram);

  glBindVertexArray (vaot);
  glDrawArrays(GL_TRIANGLES, 0, 3);

  glBindVertexArray (vaor);
  glDrawArrays(GL_LINES, 0, 8);

  // Draw points 0-3 from the currently bound VAO with current in-use shader
  glPointSize(5.0);
  // glDrawArrays(GL_POINTS, 0, 6);
  //glDrawArrays(GL_LINES, 0, 6);
  // glDrawArrays(GL_TRIANGLES, 0, 3);
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
  initShadersGL();
  initVertexBufferGL();
  initVertexBufferforRect();

  // Loop until the user closes the window
  while (glfwWindowShouldClose(window) == 0)
    {
       
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

