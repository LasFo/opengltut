#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class Shader {
public:
  // the program ID
  unsigned int ID;

  Shader(const char *vertexPath, const char *fragmentPath) : Shader(vertexPath, fragmentPath, nullptr) {
  }
  // constructor reads and builds the shader
  Shader(const char *vertexPath, const char *fragmentPath, const char *geoPath) : ID(0) {
    // 1. retrieve the vertex/fragment source code from filePath

    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    std::ifstream vShaderFile(vertexPath);
    if (!vShaderFile.good()) {
      fprintf(stderr, "vertex shader file '%s' does not exist\n", vertexPath);
      return;
    }
    std::ifstream fShaderFile(fragmentPath);
    if (!fShaderFile.good()) {
      fprintf(stderr, "fragment shader file '%s' does not exist\n", fragmentPath);
      return;
    }
    std::ifstream gShaderFile(geoPath);
    if (geoPath != nullptr && !gShaderFile.good()) {
      fprintf(stderr, "geometry shader file '%s' does not exist\n", geoPath);
      return;
    }

    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    if (geoPath != nullptr) {
      gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    }
    try {
      std::stringstream vShaderStream, fShaderStream, geometryShader;
      // read file's buffer contents into streams
      vShaderStream << vShaderFile.rdbuf();
      fShaderStream << fShaderFile.rdbuf();
      // close file handlers
      vShaderFile.close();
      fShaderFile.close();
      // convert stream into string
      vertexCode = vShaderStream.str();
      fragmentCode = fShaderStream.str();
      if (geoPath != nullptr) {
        geometryShader << gShaderFile.rdbuf();
        gShaderFile.close();
        geometryCode = geometryShader.str();
      }
    } catch (std::ifstream::failure e) {
      std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
      return;
    }
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    const char *gShaderCode = geometryCode.c_str();
    // 2. compile shaders
    unsigned int vertex, fragment, geometry;
    int success;
    char infoLog[512];

    // vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(vertex, 512, NULL, infoLog);
      fprintf(stderr, "failed to compile vertex shader: %s\n", infoLog);
      return;
    };

    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(fragment, 512, NULL, infoLog);
      fprintf(stderr, "failed to compile fragment shader: %s\n", infoLog);
      return;
    };

    if (geoPath != nullptr) {
      // geometry Shader
      geometry = glCreateShader(GL_GEOMETRY_SHADER);
      glShaderSource(geometry, 1, &gShaderCode, NULL);
      glCompileShader(geometry);
      // print compile errors if any
      glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
      if (!success) {
        glGetShaderInfoLog(geometry, 512, NULL, infoLog);
        fprintf(stderr, "failed to compile geometry shader: %s\n", infoLog);
        return;
      };
    }

    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    if (geoPath != nullptr)
      glAttachShader(ID, geometry);
    glLinkProgram(ID);
    // print linking errors if any
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(ID, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                << infoLog << std::endl;
      ID = 0;
    }

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
  }
  // use/activate the shader
  void use() {
    glUseProgram(ID);
  }
  // utility uniform functions
  void setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
  }
  void setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
  }
  void setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
  }
  void setMat4(const std::string &name, glm::mat4 value) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
  }
  void setVec2(const std::string &name, float x, float y) const {
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
  }
  void setVec2(const std::string &name, const glm::vec2 v) const {
    glUniform2f(glGetUniformLocation(ID, name.c_str()), v.x, v.y);
  }
  void setVec3(const std::string &name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
  }
  void setVec3(const std::string &name, const glm::vec3 vec) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), vec.x, vec.y, vec.z);
  }

  bool valid() {
    return ID != 0;
  }
};

#endif
