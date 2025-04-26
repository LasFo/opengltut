#include <cstdio>
#include <ctime>
#include <iostream>
#include <map>
#include <string>
// #include <thread>

#include <glad/glad.h>
// GLFW needs to be imported after glad
#include <GLFW/glfw3.h>

#include "camera.h"
#include "model.h"
#include "shader.h"
#include "third_party/stb/stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
unsigned int WIDTH = 1272;
unsigned int HEIGHT = 2090;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

float ambientStrength = 0.3f;
float diffuseStrength = 0.8f;

bool stopLight = true;

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    WIDTH = width;
    HEIGHT = height;
  glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);
  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
    return;
  }
  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;
  camera.ProcessMouseMovement(xoffset, yoffset);
  lastX = xpos;
  lastY = ypos;
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    ambientStrength += 0.01f;
  }
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    ambientStrength -= 0.01f;
  }
  if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
    diffuseStrength += 0.01f;
  }
  if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
    diffuseStrength -= 0.01f;
  }
  if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
    stopLight = false;
  }
  if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
    stopLight = true;
  }
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessKeyboard(RIGHT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    camera.ProcessKeyboard(UP, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
    camera.ProcessKeyboard(DOWN, deltaTime);
}

unsigned int loadTexture(char const *path, GLint mode) {
  unsigned int textureID;
  glGenTextures(1, &textureID);

  int width, height, nrComponents;
  unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
  if (data) {
    GLenum format = GL_RED;
    if (nrComponents == 1)
      format = GL_RED;
    else if (nrComponents == 3)
      format = GL_RGB;
    else if (nrComponents == 4)
      format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
  } else {
    std::cout << "Texture failed to load at path: " << path << std::endl;
    stbi_image_free(data);
  }

  return textureID;
}

int main() {
  // glfw: initialize and configure
  // ------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // glfw window creation
  // --------------------
  GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  // tell GLFW to capture our mouse
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // configure global opengl state
  // -----------------------------
  glEnable(GL_DEPTH_TEST);

  // build and compile shaders
  // -------------------------
  Shader shader("shader.vert", "shader.frag");
  Shader sceneShader("shader.vs", "shader.fs");

  unsigned int framebuffer;
  glGenFramebuffers(1, &framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

  unsigned int textureColorbuffer;
  glGenTextures(1, &textureColorbuffer);
  glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    fprintf(stderr, "textureColorbuffer not complete\n");
    return 0;
  }

  unsigned int rbo;
  glGenRenderbuffers(1, &rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    fprintf(stderr, "renderbuffer not complete\n");
    return 0;
  }

  float quadVertices[] = {
      // positions   // texCoords
      -1.0f, 1.0f, 0.0f, 1.0f,
      -1.0f, -1.0f, 0.0f, 0.0f,
      1.0f, -1.0f, 1.0f, 0.0f,

      -1.0f, 1.0f, 0.0f, 1.0f,
      1.0f, -1.0f, 1.0f, 0.0f,
      1.0f, 1.0f, 1.0f, 1.0f};
  unsigned int quadVAO, quadVBO;
  glGenVertexArrays(1, &quadVAO);
  glGenBuffers(1, &quadVBO);
  glBindVertexArray(quadVAO);
  glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
  glBindVertexArray(0);


  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  float cubeVertices[] = {
      // Back face
      0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  // bottom-right
      0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   // top-right
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // Bottom-left
      -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,  // top-left
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // bottom-left
      0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   // top-right
      // Front face
      0.5f, 0.5f, 0.5f, 1.0f, 1.0f,   // top-right
      0.5f, -0.5f, 0.5f, 1.0f, 0.0f,  // bottom-right
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-left
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-left
      -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,  // top-left
      0.5f, 0.5f, 0.5f, 1.0f, 1.0f,   // top-right
      // Left face
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-left
      -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,  // top-left
      -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,   // top-right
      -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,   // top-right
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  // bottom-right
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-left
                                       // Right face
      0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   // top-right
      0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  // bottom-right
      0.5f, 0.5f, 0.5f, 1.0f, 0.0f,    // top-left
      0.5f, -0.5f, 0.5f, 0.0f, 0.0f,   // bottom-left
      0.5f, 0.5f, 0.5f, 1.0f, 0.0f,    // top-left
      0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  // bottom-right
      // Bottom face
      0.5f, -0.5f, 0.5f, 1.0f, 0.0f,   // bottom-left
      0.5f, -0.5f, -0.5f, 1.0f, 1.0f,  // top-left
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // top-right
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // top-right
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  // bottom-right
      0.5f, -0.5f, 0.5f, 1.0f, 0.0f,   // bottom-left
      // Top face
      0.5f, 0.5f, -0.5f, 1.0f, 1.0f,  // top-right
      0.5f, 0.5f, 0.5f, 1.0f, 0.0f,   // bottom-right
      -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // top-left
      -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // top-left
      0.5f, 0.5f, 0.5f, 1.0f, 0.0f,   // bottom-right
      -0.5f, 0.5f, 0.5f, 0.0f, 0.0f   // bottom-left
  };
  float planeVertices[] = {
      5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
      -5.0f, -0.5f, 5.0f, 0.0f, 0.0f,
      -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,

      5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
      -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,
      5.0f, -0.5f, -5.0f, 2.0f, 2.0f};
  // cube VAO
  unsigned int cubeVAO, cubeVBO;
  glGenVertexArrays(1, &cubeVAO);
  glGenBuffers(1, &cubeVBO);
  glBindVertexArray(cubeVAO);
  glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
  glBindVertexArray(0);
  // plane VAO
  unsigned int planeVAO, planeVBO;
  glGenVertexArrays(1, &planeVAO);
  glGenBuffers(1, &planeVBO);
  glBindVertexArray(planeVAO);
  glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
  glBindVertexArray(0);

  // load textures
  // -------------
  stbi_set_flip_vertically_on_load(true);
  unsigned int cubeTexture = loadTexture("resources/textures/container.jpg", GL_REPEAT);
  unsigned int floorTexture = loadTexture("resources/textures/metal.png", GL_REPEAT);
  // shader configuration
  // --------------------
  shader.use();
  shader.setInt("texture1", 0);

  std::vector<glm::vec3> windows;
  windows.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
  windows.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
  windows.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
  windows.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
  windows.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

  // render loop
  // -----------
  while (!glfwWindowShouldClose(window)) {
    // per-frame time logic
    // --------------------
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // input
    // -----
    processInput(window);

    // first pass: render scene to framebuffer
    // ------
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);


    // set uniforms
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    sceneShader.use();
    sceneShader.setMat4("view", view);
    sceneShader.setMat4("projection", projection);

    glBindVertexArray(planeVAO);
    glBindTexture(GL_TEXTURE_2D, floorTexture);
    sceneShader.setMat4("model", glm::mat4(1.0f));
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    // 1st. render pass, draw objects as normal, writing to the stencil buffer
    // --------------------------------------------------------------------
    // cubes
    glBindVertexArray(cubeVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cubeTexture);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
    sceneShader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
    sceneShader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // second pass: render texture to screne
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    shader.use();

    glBindVertexArray(quadVAO);
    glDisable(GL_DEPTH_TEST);
    model = glm::mat4(1.0);
    shader.setMat4("model", model);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    model = glm::mat4(1.0);
    model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
    shader.setMat4("model", model);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  glDeleteVertexArrays(1, &cubeVAO);
  glDeleteVertexArrays(1, &planeVAO);
  glDeleteBuffers(1, &cubeVBO);
  glDeleteBuffers(1, &planeVBO);
  glDeleteFramebuffers(1, &framebuffer);

  glfwTerminate();
  return 0;
}
