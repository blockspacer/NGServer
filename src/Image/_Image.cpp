#include <GLFW/glfw3.h>
#include <zconf.h>

const int WIDTH = 480;
const int HEIGHT = 320;

int main(void) {
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    float i = 0.1;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Draw a triangle */
        glBegin(GL_TRIANGLES);

        glColor3f(1.0, 0.0, 0.0);       // Red
        glVertex3f(-0.5, 0, 0.0);        // up point

        glColor3f(0.0, 1.0, 0.0);       // Green
        glVertex3f(-1.0, -1.0, 0.0);    // bottom left

        glColor3f(0.0, 0.0, 1.0);       // Blue
        glVertex3f(0.0, -1.0, 0.0);     // bootom right

        glEnd();

        /* Draw a polygon */
        glBegin(GL_POLYGON);

        glColor3f(0.0, 1.0, 0.0);       // Green

        glVertex3f(0.25, 0.25, 0.0);
        glVertex3f(0.75, 0.25, 0.0);
        glVertex3f(0.75, 0.75, 0.0);
        glVertex3f(0.25, 0.75, 0.0);

        glEnd();
        glLineWidth(2.5);

        glBegin(GL_LINES);


        glColor3f(i, 0.0, 0.0);    // Red
        i=i+0.1;
        if (i > 1) {
            i = 0.1;
        }
        usleep(1000 * 100);

        glVertex3f(-0.5, 0.0, 0.0);
        glVertex3f(i, 0, 0);
//        glVertex3f(0.5, -0.5, 0);
        glEnd();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}