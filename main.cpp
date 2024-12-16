#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <objectloader.hpp>
#include <shader.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include <unistd.h>

// PLY object loader
ply_object test("/home/omni/Desktop/me/c_r/artemis/untitled.ply");

// Callback to handle window resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    // Create a window with OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Ply Loader", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Load GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Failed to initialize GLAD\n");
        return -1;
    }

    // Set up viewport and framebuffer callback
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Set background color and enable depth testing
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    // Load the PLY object and shaders
    test.initialize();
    
    shader_program shader;
    shader.createShaderProgram(
        "/home/omni/Desktop/me/c_r/artemis/shaders/basicvertex.glsl",
        "/home/omni/Desktop/me/c_r/artemis/shaders/basicfragment.glsl"
    );
    glUseProgram(shader.getprogramid());



    // Main rendering loop
    while (!glfwWindowShouldClose(window)) {
        // Poll for events
        glfwPollEvents();

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render the PLY object
        test.render();

        // Swap buffers
        glfwSwapBuffers(window);
    }

    // Cleanup and exit
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
