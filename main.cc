#include <iostream>

#include <glad/glad.h>
// GLFW needs to be imported after glad
#include <GLFW/glfw3.h>

const char *vertexShaderSource = R"(#version 460 core
layout (location = 0) in vec3 aPos;

void main() {
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)";

const char *fragmentShaderSourceOrange = R"(#version 460 core
out vec4 FragColor;

void main() {
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
)";

const char *fragmentShaderSourceBlue = R"(#version 460 core
out vec4 FragColor;

void main() {
    FragColor = vec4(0.2f, 0.0f, 0.8f, 1.0f);
}
)";

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glViewport(0, 0, 800, 600);

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // Set up vertex shader
  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cerr << "vertex shader compilation failed:\n"
              << infoLog << std::endl;
    return 1;
  }

  // Set up fragment shader orange
  unsigned int fragmentShaderOrange;
  fragmentShaderOrange = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShaderOrange, 1, &fragmentShaderSourceOrange, NULL);
  glCompileShader(fragmentShaderOrange);
  glGetShaderiv(fragmentShaderOrange, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShaderOrange, 512, NULL, infoLog);
    std::cerr << "fragment shader compilation failed:\n"
              << infoLog << std::endl;
    return 1;
  }

  unsigned int shaderProgramOrange;
  shaderProgramOrange = glCreateProgram();
  glAttachShader(shaderProgramOrange, vertexShader);
  glAttachShader(shaderProgramOrange, fragmentShaderOrange);
  glLinkProgram(shaderProgramOrange);
  glGetProgramiv(shaderProgramOrange, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgramOrange, 512, NULL, infoLog);
    std::cerr << "linking shader program failed:\n"
              << infoLog << std::endl;
    return 1;
  }
  glDeleteShader(fragmentShaderOrange);

  // Set up fragment shader orange
  unsigned int fragmentShaderBlue;
  fragmentShaderBlue = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShaderBlue, 1, &fragmentShaderSourceBlue, NULL);
  glCompileShader(fragmentShaderBlue);
  glGetShaderiv(fragmentShaderBlue, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShaderBlue, 512, NULL, infoLog);
    std::cerr << "fragment shader compilation failed:\n"
              << infoLog << std::endl;
    return 1;
  }

  unsigned int shaderProgramBlue;
  shaderProgramBlue = glCreateProgram();
  glAttachShader(shaderProgramBlue, vertexShader);
  glAttachShader(shaderProgramBlue, fragmentShaderBlue);
  glLinkProgram(shaderProgramBlue);
  glGetProgramiv(shaderProgramBlue, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgramBlue, 512, NULL, infoLog);
    std::cerr << "linking shader program failed:\n"
              << infoLog << std::endl;
    return 1;
  }
  glDeleteShader(fragmentShaderBlue);
  glDeleteShader(vertexShader);

  // Setup vertex buffer
  float vertices[] = {
      0.0f, 0.0f, 0.0f,
      0.5f, -0.5f, 0.0f,
      -0.5f, -0.5f, 0.0f};
  unsigned int indices[] = {
      0, 1, 3,
      1, 2, 3};
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  unsigned int EBO;
  glGenBuffers(1, &EBO);

  // Set up vertex array object
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  float vertices1[] = {
      0.0f, 0.0f, 0.0f,
      -0.5f, 0.5f, 0.0f,
      0.5f, 0.5f, 0.0f};
  unsigned int VBO1;
  glGenBuffers(1, &VBO1);
  unsigned int VAO1;
  glGenVertexArrays(1, &VAO1);
  glBindVertexArray(VAO1);
  glBindBuffer(GL_ARRAY_BUFFER, VBO1);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);


  // to fill...
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  // or not to fill?
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  // fill!
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  bool toggle = false;

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgramOrange);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glUseProgram(shaderProgramBlue);
    glBindVertexArray(VAO1);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    //    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
