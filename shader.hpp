#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "glad/glad.h"
#include <fstream>

class shader_program{
  private:
  std::string loadShaderSource(const std::string& filepath) {
      std::ifstream shaderFile(filepath);
      if (!shaderFile.is_open()) {
          std::cerr << "Error: Could not open shader file: " << filepath << std::endl;
          exit(1);
      }

      std::stringstream shaderStream;
      shaderStream << shaderFile.rdbuf();
      return shaderStream.str();
  }

  GLuint compileShaderFromFile(GLenum shaderType, const std::string& filepath) {
      std::string shaderSource = loadShaderSource(filepath);

      const char* sourceCStr = shaderSource.c_str();

  // Create and compile shader
      GLuint shader = glCreateShader(shaderType);
      glShaderSource(shader, 1, &sourceCStr, nullptr);
      glCompileShader(shader);

  // Check for compilation errors
      GLint success;
      glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
      if (!success) {
          char infoLog[512];
          glGetShaderInfoLog(shader, 512, nullptr, infoLog);
          std::cerr << "Error compiling shader from file (" << filepath << "):\n" << infoLog << std::endl;
          glDeleteShader(shader);
          exit(1);
      }

      return shader;
  }


  GLuint program;
  public:
  void createShaderProgram(const std::string& vertexPath, const std::string& fragmentPath) {
      GLuint vertexShader = compileShaderFromFile(GL_VERTEX_SHADER, vertexPath);
      GLuint fragmentShader = compileShaderFromFile(GL_FRAGMENT_SHADER, fragmentPath);

      program = glCreateProgram();
      glAttachShader(program, vertexShader);
      glAttachShader(program, fragmentShader);
      glLinkProgram(program);

      // Check for linking errors
      GLint success;
      glGetProgramiv(program, GL_LINK_STATUS, &success);
      if (!success) {
          char infoLog[512];
          glGetProgramInfoLog(program, 512, nullptr, infoLog);
          std::cerr << "Error linking program:\n" << infoLog << std::endl;
          glDeleteProgram(program);
          exit(1);
      }

      // Clean up shaders
      glDeleteShader(vertexShader);
      glDeleteShader(fragmentShader);

  }

  GLuint getprogramid(){return program;}

};
