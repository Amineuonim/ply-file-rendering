#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <pthread.h>
#include <string>
#include "oneheaders/happly.h"
#include "glad/glad.h"
#include <chrono>



class ply_object{
  private:
// used to store vertices returned by happly.h
  std::vector<std::array<double, 3>> double_vertices;
// used to store indices returned by happly.h
  std::vector<std::vector<size_t>> indices;
// to store the path to the ply file
  std::string file_path;
// used to store vetices in a float type for optimization sake
// set conversion to 0 to use doubles
  bool conversion = true;
  float *float_vertices =nullptr;
// buffers ids
  unsigned int vbo;
  unsigned int ebo;
  unsigned int vao;
  int vertices_size;

  public:

// constructor + destructor
  ply_object(std::string path) : file_path(path){}
  ~ply_object()= default;

// initialize the vertex buffer object
  void initialize_vbo(int number_of_vertices){
    glBindVertexArray(vao);
    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    vertices_size;

// if conversion is set to true we send floats to the gpu
    if(conversion ==true){
    vertices_size = number_of_vertices*sizeof(float)*3;
    glBufferData(GL_ARRAY_BUFFER, vertices_size, float_vertices, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    }

// if conversion is set to false we send the doubles to the gpu
// be warned that this can cause some issues

    if(conversion ==false){
      vertices_size = number_of_vertices *3*sizeof(double);
      glBufferData(GL_ARRAY_BUFFER, vertices_size, double_vertices.data(), GL_DYNAMIC_DRAW);
      glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), (void*)0);
      glEnableVertexAttribArray(0);
    }


    int vbo_size =0;
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &vbo_size);
    if(vbo_size != vertices_size){
      perror("vbo not initialized properly.\n");
      printf("vbo contains %d bytes when it should contain %d bytes.\n",vbo_size,vertices_size);
      exit(1);
    }

    glEnableVertexAttribArray(0);


    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);


  }

// function for debugging since i am too lazy to code the function to set the place correctly in the world
  void scaleVBO(GLuint vbo, size_t numVertices) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Map the buffer to access its data
    float* vboData = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
    if (vboData == nullptr) {
        fprintf(stderr, "Failed to map VBO for scaling.\n");
        return;
    }

    // Scale the VBO contents down by 10
    for (size_t i = 0; i < numVertices * 3; i++) { // Assuming 3 floats per vertex (x, y, z)
        vboData[i] /= 10.0f;
    }

    // Unmap the buffer to apply changes
    if (!glUnmapBuffer(GL_ARRAY_BUFFER)) {
        fprintf(stderr, "Failed to unmap VBO after scaling.\n");
    }

    // Unbind the buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}



// function to load the file content into the variables
  void initialize (){
    happly::PLYData plyIn(file_path);


// Get mesh data from the ply file
    double_vertices = plyIn.getVertexPositions();
    indices = plyIn.getFaceIndices<size_t>();
    int verticesnumber = double_vertices.size();

// convert double vertices into floats if conversion is on
    if(conversion == true){
// memory initialization for the float vertices
      float_vertices = (float*)malloc(sizeof(float)*double_vertices.size()*3);

      for (size_t i = 0; i < double_vertices.size(); i++) {
          float_vertices[i * 3] = static_cast<float>(double_vertices[i][0]);
          float_vertices[i * 3 + 1] = static_cast<float>(double_vertices[i][1]);
          float_vertices[i * 3 + 2] = static_cast<float>(double_vertices[i][2]);
      }


// since we don't need the double vertices anymore cause we're using floats 
// we free the memory assigned to it
      double_vertices = std::vector<std::array<double, 3>>();
    }

    glGenVertexArrays(1,&vao);
    initialize_vbo(verticesnumber);
    scaleVBO(vbo,verticesnumber);

  }


  void render() {
    auto start = std::chrono::high_resolution_clock::now();

    std::printf("rendering started.\n");

    glBindVertexArray(vao);

    glDrawArrays(GL_TRIANGLES, 0, vertices_size/sizeof(float));

    glBindVertexArray(0);


    std::printf("rendering finished.\n");
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Execution time: " << duration.count() << " microseconds" << std::endl;


}

// must be called before initialize function
  void stop_conversion(){conversion == false;};

};
