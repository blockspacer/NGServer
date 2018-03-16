//#include <iostream>
//
//#include <GLFW/glfw3.h>
//#include <GLFW/glfw3.h>
//
//using namespace std;
//
//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
//
//const GLuint WIDTH = 800, HEIGHT = 600;
//
//int main()
//{
//    cout << "Starting GLFW context, OpenGL3.3" << endl;
//    glfwInit();
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
//
//    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
//    if(window == NULL)
//    {
//        std::cout << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//        return -1;
//    }
//
//    glViewport(0, 0, WIDTH, HEIGHT);
//
//    while(!glfwWindowShouldClose(window))
//    {
//        glfwPollEvents();
//
//        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT);
//
//        glfwSwapBuffers(window);
//    }
//
//    glfwTerminate();
//
//    return 0;
//}
//
//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
//{
//    std::cout << key << std::endl;
//    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, GL_TRUE);
//}