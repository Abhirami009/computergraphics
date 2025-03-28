#include "gl_framework.hpp"
#include "shader_util.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

// vertices for smaller cube
float points[] = {
	-0.1f, -0.1f, -0.1f,  // A - 0
	 -0.1f, -0.1f,  0.0f,  // B - 1
	 -0.1f,  0.0f,  0.0f,  // C - 2
	 -0.1f,  0.0f, -0.1f,  // D - 3
	  0.0f, -0.1f, -0.1f,  // E - 4
	  0.0f, -0.1f,  0.0f,  // F - 5
	  0.0f,  0.0f,  0.0f,  // G - 6
	  0.0f,  0.0f, -0.1f   // H - 7
 };

 struct cubes{
    glm::vec3 pos;
    glm::vec3 color;
    };

    std::vector <cubes> copied_cubes;

// make an array to store the grid points
std::vector<float> gridpoints;
glm::vec3 gridColor(0.0f, 0.0f, 0.0f); // Black grid color

float start_point = -0.5;
// draw a 10x10x10 grid
void make_grid_indices() {
  float start_point = -0.5f; 
  float step = 0.1f;

  for (int j = 0; j <= 10; j++) {
      for (int k = 0; k <= 10; k++) {
          gridpoints.push_back(start_point);
          gridpoints.push_back(start_point + j * step);
          gridpoints.push_back(start_point + k * step);

          gridpoints.push_back(start_point + 1.0f); // x2
          gridpoints.push_back(start_point + j * step); // y2
          gridpoints.push_back(start_point + k * step); // z2
      }
  }

  // Lines parallel to Y-axis
  for (int i = 0; i <= 10; i++) {
      for (int k = 0; k <= 10; k++) {
          gridpoints.push_back(start_point + i * step); 
          gridpoints.push_back(start_point);
          gridpoints.push_back(start_point + k * step);

          gridpoints.push_back(start_point + i * step);
          gridpoints.push_back(start_point + 1.0f);
          gridpoints.push_back(start_point + k * step);
      }
  }

 
  for (int i = 0; i <= 10; i++) {
      for (int j = 0; j <= 10; j++) {
          gridpoints.push_back(start_point + i * step);
          gridpoints.push_back(start_point + j * step);
          gridpoints.push_back(start_point);

          gridpoints.push_back(start_point + i * step);
          gridpoints.push_back(start_point + j * step);
          gridpoints.push_back(start_point + 1.0f);
      }
  }
}



// start with a red colored cube
float colors[] = {
  1.0f, 0.0f, 0.0f,
  1.0f, 0.0f, 0.0f,
  1.0f, 0.0f, 0.0f,
  1.0f, 0.0f, 0.0f,
  1.0f, 0.0f, 0.0f,
  1.0f, 0.0f, 0.0f,
  1.0f, 0.0f, 0.0f,
  1.0f, 0.0f, 0.0f,
};

int indices[] = {
  0, 1, 2,  2, 3, 0,  // Left
    4, 5, 6,  6, 7, 4,  // Right
    0, 4, 7,  7, 3, 0,  // Back
    1, 5, 6,  6, 2, 1,  // Front
    3, 7, 6,  6, 2, 3,  // Top
    0, 4, 5,  5, 1, 0   // Bottom
};


GLuint shaderProgram;
GLuint vbo, vao, ebo;
GLuint gridvbo, gridvao;


void initShadersGL(void)
{
  std::string vertex_shader_file("gridcube_vs.glsl");
  std::string fragment_shader_file("gridcube_fs.glsl");

  std::vector<GLuint> shaderList;
  shaderList.push_back(glFramework::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList.push_back(glFramework::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

  shaderProgram = glFramework::CreateProgramGL(shaderList);
  
}

//initialise vertexBuffer to draw gridlines and a solid cube whose vertices are stored in points[]
void initVertexBufferGL(void)
{
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
   glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), NULL, GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
  glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);
  
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  
  // Initialize the vertex position attribute from the vertex shader
  GLuint vPosition = glGetAttribLocation(shaderProgram, "vp");
  glEnableVertexAttribArray(vPosition);
  glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
  
  // Initialize the color attribute from the vertex shader
  GLuint vColor = glGetAttribLocation(shaderProgram, "vColor");
  glEnableVertexAttribArray(vColor);
  glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(points)));
  
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // ----- GRID BUFFER SETUP -----
  glGenVertexArrays(1, &gridvao);
  glBindVertexArray(gridvao);

  glGenBuffers(1, &gridvbo);
  glBindBuffer(GL_ARRAY_BUFFER, gridvbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * gridpoints.size(), gridpoints.data(), GL_STATIC_DRAW);

  // Gridlines use only position, no colors
  GLuint gridPosition = glGetAttribLocation(shaderProgram, "vp");
  glEnableVertexAttribArray(gridPosition);
  glVertexAttribPointer(gridPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}


float rotationAngleX = 0.0f;
float rotationAngleY = 0.0f;
float rotationAngleZ = 0.0f;


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        } else if (key == GLFW_KEY_C) {
            int node;
            float r, g, b;
            std::cout << "Enter RGB values (0-1): ";
            std::cin >> r >> g >> b;
			for (node = 0 ; node < 8; node++){
            // update the color array when c is pressed
				colors[node * 3] = r;
				colors[node * 3 + 1] = g;
				colors[node * 3 + 2] = b;
			}

            // reupload color data to buffer
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
    }
}

float moveDelay = 0.2f;  // delay in seconds btwn moves to see movements distinctly
static float lastMoveTime = 0.0f;
glm::vec3 cubePosition = glm::vec3(0.0f, 0.0f, 0.0f);

const float EPSILON = 1e-5f; // error correction

bool isApproximatelyEqual(const glm::vec3& a, const glm::vec3& b, float epsilon) {
    return glm::length(a - b) < epsilon;
}

void renderGL(GLFWwindow* window)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

  glUseProgram(shaderProgram);
  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 gridModel = glm::mat4(1.0f);

  if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
      rotationAngleX += 1.0f;  // left to right
  }
  if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
      rotationAngleX -= 1.0f;  // right to left
  }
  if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
      rotationAngleY += 1.0f;  // top to bott
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
      rotationAngleY -= 1.0f;  // bot to top
  }
  
  // rotation to the grid
  gridModel = glm::rotate(gridModel, glm::radians(rotationAngleX), glm::vec3(0.0f, 1.0f, 0.0f));
  gridModel = glm::rotate(gridModel, glm::radians(rotationAngleY), glm::vec3(1.0f, 0.0f, 0.0f));
  
  // rotation to the cube's coordinate system
  glm::mat4 cubeRotationMatrix = gridModel;
  // new position
glm::vec3 proposedMove = glm::vec3(0.0f);
if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
    float currentTime = glfwGetTime(); 
    if (currentTime - lastMoveTime > moveDelay) {
        proposedMove = glm::vec3(cubeRotationMatrix * glm::vec4(0.1f, 0.0f, 0.0f, 0.0f));
        lastMoveTime = currentTime;
    }
}
if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    float currentTime = glfwGetTime();
    if (currentTime - lastMoveTime > moveDelay) {
        proposedMove = glm::vec3(cubeRotationMatrix * glm::vec4(-0.1f, 0.0f, 0.0f, 0.0f));
        lastMoveTime = currentTime;
    }
}
if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    float currentTime = glfwGetTime();
    if (currentTime - lastMoveTime > moveDelay) {
        proposedMove = glm::vec3(cubeRotationMatrix * glm::vec4(0.0f, -0.1f, 0.0f, 0.0f));
        lastMoveTime = currentTime;
    }
}
if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    float currentTime = glfwGetTime();
    if (currentTime - lastMoveTime > moveDelay) {
        proposedMove = glm::vec3(cubeRotationMatrix * glm::vec4(0.0f, 0.1f, 0.0f, 0.0f));
        lastMoveTime = currentTime;
    }
}
if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
    float currentTime = glfwGetTime();
    if (currentTime - lastMoveTime > moveDelay) {
        proposedMove = glm::vec3(cubeRotationMatrix * glm::vec4(0.0f, 0.0f, 0.1f, 0.0f));
        lastMoveTime = currentTime;
    }
}
if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
    float currentTime = glfwGetTime();
    if (currentTime - lastMoveTime > moveDelay) {
        proposedMove = glm::vec3(cubeRotationMatrix * glm::vec4(0.0f, 0.0f, -0.1f, 0.0f));
        lastMoveTime = currentTime;
    }
}

// new world-space position
glm::vec3 newCubePosition = cubePosition + proposedMove;

//ensure that cube doesnt go out of boundary
newCubePosition.x = glm::clamp(newCubePosition.x, -0.4f, 0.5f);
newCubePosition.y = glm::clamp(newCubePosition.y, -0.4f, 0.5f);
newCubePosition.z = glm::clamp(newCubePosition.z, -0.4f, 0.5f);

// move is valid
if (glm::length(newCubePosition - cubePosition) > 0.0001f) {
    cubePosition = newCubePosition;
}

if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS){
	struct cubes cube;
			cube.pos = cubePosition;
            for (auto &c : copied_cubes){
                if (isApproximatelyEqual(c.pos, cubePosition, EPSILON)){
                    c.color = glm::vec3(colors[0],colors[1], colors[2]);
                }
            }
			cube.color = glm::vec3(colors[0],colors[1], colors[2]);
			copied_cubes.push_back(cube);
	}

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		copied_cubes.erase(
			std::remove_if(copied_cubes.begin(), copied_cubes.end(),
						   [&](const cubes& c) { 
							   return isApproximatelyEqual(c.pos, cubePosition, EPSILON); 
						   }),
			copied_cubes.end());
	}

  
  // --- CUBE TRANSFORMATION ---
  model = gridModel; 
  model = glm::translate(model, cubePosition);

  // --- GRID TRANSFORMATION  ---
  
  glm::mat4 view_matrix = glm::lookAt(glm::vec3(0.0, 0.0, -2.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
  glm::mat4 ortho_matrix = glm::ortho(-1.0, 1.0, -1.0, 1.0, -10.0, 10.0);

  auto viewLoc = glGetUniformLocation(shaderProgram, "view");
  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view_matrix));
  auto projLoc = glGetUniformLocation(shaderProgram, "projection");
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(ortho_matrix));

  // --- Render Cube ---
  auto modelLoc = glGetUniformLocation(shaderProgram, "model");
  glUniform1i(glGetUniformLocation(shaderProgram, "isGrid"), 0);
  glUniform1i(glGetUniformLocation(shaderProgram, "isCube"), 1);
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); 
  
  glEnable(GL_DEPTH_TEST);
  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);


  glBindVertexArray(vao);
for (auto &cc : copied_cubes) {
	glm::mat4 model_c = gridModel;
	model_c = glm::translate(model_c, cc.pos);
    auto modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniform1i(glGetUniformLocation(shaderProgram, "isGrid"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "isCube"), 0);
	glUniform3fv(glGetUniformLocation(shaderProgram, "col"), 1, glm::value_ptr(cc.color));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model_c));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
glBindVertexArray(0);


  // --- Render Grid ---
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(gridModel)); 
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glLineWidth(1.0f);
  glBindVertexArray(gridvao);
  glUniform1i(glGetUniformLocation(shaderProgram, "isGrid"), 1);
glUniform3fv(glGetUniformLocation(shaderProgram, "gridColor"), 1, glm::value_ptr(gridColor));
  glDrawArrays(GL_LINES, 0, gridpoints.size() / 3);
  glBindVertexArray(0);
  glDisable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);


}


int main(int argc, char** argv)
{
  GLFWwindow* window;
  make_grid_indices();
  glfwSetErrorCallback(glFramework::error_callback);
  if (!glfwInit())
    return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

  
  window = glfwCreateWindow(800, 800, "Assignment1", NULL, NULL);
  if (!window)
    {
      glfwTerminate();
      return -1;
    }
  
  glfwMakeContextCurrent(window);

  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (GLEW_OK != err)
    {
      std::cerr<<"GLEW Init Failed : %s"<<std::endl;
    }

  
  std::cout<<"Vendor: "<<glGetString (GL_VENDOR)<<std::endl;
  std::cout<<"Renderer: "<<glGetString (GL_RENDERER)<<std::endl;
  std::cout<<"Version: "<<glGetString (GL_VERSION)<<std::endl;
  std::cout<<"GLSL Version: "<<glGetString (GL_SHADING_LANGUAGE_VERSION)<<std::endl;
  
  
  glfwSetKeyCallback(window,key_callback);
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