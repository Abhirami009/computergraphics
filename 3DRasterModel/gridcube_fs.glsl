#version 330 core
in vec3 fragColor;
out vec4 frag_color;

void main() {
    frag_color = vec4(fragColor, 1.0);
}

/* void renderGL(GLFWwindow* window)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glUseProgram(shaderProgram);
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 gridModel = glm::mat4(1.0f);

    glm::mat4 gridRotation = glm::mat4(1.0f);
    gridRotation = glm::rotate(gridRotation, glm::radians(rotationAngleX), glm::vec3(0, 1, 0)); // Y-axis
    gridRotation = glm::rotate(gridRotation, glm::radians(rotationAngleY), glm::vec3(1, 0, 0)); // X-axis
    gridRotation = glm::rotate(gridRotation, glm::radians(rotationAngleZ), glm::vec3(0, 0, 1));

    // Handle grid rotation
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        rotationAngleX += 1.0f;  // Rotate around Y-axis
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        rotationAngleX -= 1.0f;  // Rotate around Y-axis
    }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
        rotationAngleY += 1.0f;  // Rotate around X-axis
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        rotationAngleY -= 1.0f;  // Rotate around X-axis
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        rotationAngleZ -= 1.0f;  // Rotate around Z-axis
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        rotationAngleZ += 1.0f;  // Rotate around Z-axis
    }

    // Apply rotation to the grid
    gridModel = glm::rotate(gridModel, glm::radians(rotationAngleX), glm::vec3(0.0f, 1.0f, 0.0f));
    gridModel = glm::rotate(gridModel, glm::radians(rotationAngleY), glm::vec3(1.0f, 0.0f, 0.0f));
    gridModel = glm::rotate(gridModel, glm::radians(rotationAngleZ), glm::vec3(0.0f, 0.0f, 1.0f));

    // Apply the same rotation to the cube's coordinate system
    glm::mat4 cubeRotationMatrix = gridModel;

    // Compute the new position
    glm::vec3 proposedMove = glm::vec3(0.0f);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        float currentTime = glfwGetTime(); 
        if (currentTime - lastMoveTime > moveDelay) {
            proposedMove = glm::vec3(cubeRotationMatrix * glm::vec4(-0.1f, 0.0f, 0.0f, 0.0f));
            lastMoveTime = currentTime;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        float currentTime = glfwGetTime();
        if (currentTime - lastMoveTime > moveDelay) {
            proposedMove = glm::vec3(cubeRotationMatrix * glm::vec4(0.1f, 0.0f, 0.0f, 0.0f));
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
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        float currentTime = glfwGetTime();
        if (currentTime - lastMoveTime > moveDelay) {
            proposedMove = glm::vec3(cubeRotationMatrix * glm::vec4(0.0f, 0.0f, 0.1f, 0.0f));
            lastMoveTime = currentTime;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        float currentTime = glfwGetTime();
        if (currentTime - lastMoveTime > moveDelay) {
            proposedMove = glm::vec3(cubeRotationMatrix * glm::vec4(0.0f, 0.0f, -0.1f, 0.0f));
            lastMoveTime = currentTime;
        }
    }

    // Compute the new world-space position
    glm::vec3 newCubePosition = cubePosition + proposedMove;

    // Clamp the world-space position
    newCubePosition.x = glm::clamp(newCubePosition.x, GRID_MIN_X, GRID_MAX_X);
    newCubePosition.y = glm::clamp(newCubePosition.y, GRID_MIN_Y, GRID_MAX_Y);
    newCubePosition.z = glm::clamp(newCubePosition.z, GRID_MIN_Z, GRID_MAX_Z);

    // Update only if the move is valid
    if (glm::length(newCubePosition - cubePosition) > 0.0001f) {
        cubePosition = newCubePosition;
    }
    glm::mat4 inverseGridRotation = glm::inverse(gridRotation);
   // Calculate grid indices
glm::vec4 cubePositionLocal = inverseGridRotation * glm::vec4(newCubePosition, 1.0f);
int gridX = static_cast<int>((cubePositionLocal.x + 0.5f) * 10);
int gridY = static_cast<int>((cubePositionLocal.y + 0.5f) * 10);
int gridZ = static_cast<int>((cubePositionLocal.z + 0.5f) * 10);

// Clamp grid indices
gridX = glm::clamp(gridX, 0, GRID_SIZE - 1);
gridY = glm::clamp(gridY, 0, GRID_SIZE - 1);
gridZ = glm::clamp(gridZ, 0, GRID_SIZE - 1);

// Key debouncing
static bool isFPressed = false;
static bool isWPressed = false;

if (gridX >= 0 && gridX < GRID_SIZE && gridY >= 0 && gridY < GRID_SIZE && gridZ >= 0 && gridZ < GRID_SIZE) {
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && !isFPressed) {
        std::cout << "Filling cell: (" << gridX << ", " << gridY << ", " << gridZ << ")" << std::endl;
        isFilled[gridX][gridY][gridZ] = true;
        gridColorData[gridX][gridY][gridZ] = cubeColor;
        isFPressed = true;
    } else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE) {
        isFPressed = false;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && !isWPressed) {
        std::cout << "Clearing cell: (" << gridX << ", " << gridY << ", " << gridZ << ")" << std::endl;
        isFilled[gridX][gridY][gridZ] = false;
        isWPressed = true;
    } else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE) {
        isWPressed = false;
    }
}

    // --- CUBE TRANSFORMATION ---
    model = gridModel; 
    model = glm::translate(model, cubePosition);

    // --- GRID TRANSFORMATION ---
    glm::mat4 view_matrix = glm::lookAt(glm::vec3(0.0, 0.0, -2.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 ortho_matrix = glm::ortho(-1.0, 1.0, -1.0, 1.0, -10.0, 10.0);

    auto viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view_matrix));
    auto projLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(ortho_matrix));

    // --- Render Cube ---
    auto modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniform1i(glGetUniformLocation(shaderProgram, "isGrid"), 0);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); // Send cube transformation
    
    glEnable(GL_DEPTH_TEST);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // --- Render Grid ---
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(gridModel)); // Identity matrix for grid
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

    // --- Render Filled Grid Cells ---
    glBindVertexArray(vao);
    for (int x = 0; x < GRID_SIZE; x++) {
        for (int y = 0; y < GRID_SIZE; y++) {
            for (int z = 0; z < GRID_SIZE; z++) {
                if (isFilled[x][y][z]) {
                    glm::mat4 filledModel = glm::translate(gridModel, glm::vec3(x * 0.1f, y * 0.1f, z * 0.1f));
                    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(filledModel));
                    glUniform3fv(glGetUniformLocation(shaderProgram, "gridColor"), 1, glm::value_ptr(gridColorData[x][y][z]));
                    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
                }
            }
        }
    }
}
*/