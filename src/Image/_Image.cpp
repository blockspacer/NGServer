#include <GLFW/glfw3.h>
#include <zconf.h>
#include <glog/logging.h>

#include <math.h>

const int WIDTH = 720;
const int HEIGHT = 720;

static void error_callback(int error, const char *description) {

}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
            case GLFW_KEY_SPACE:
                LOG(INFO) << "Press Space";
                break;
            default:
                break;
        }
    } else if (action == GLFW_RELEASE) {
        switch (key) {
            case GLFW_KEY_SPACE:
                LOG(INFO) << "Release Space";
                break;
            default:
                break;
        }
    }
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (button) {
            case GLFW_MOUSE_BUTTON_LEFT:
                LOG(INFO) << "Mosue left button clicked!";
                break;
            case GLFW_MOUSE_BUTTON_MIDDLE:
                LOG(INFO) << "Mosue middle button clicked!";
                break;
            case GLFW_MOUSE_BUTTON_RIGHT:
                LOG(INFO) << "Mosue right button clicked!";
                break;
            default:
                return;
        }
    }
}

void DrawTriangle() {
    glBegin(GL_TRIANGLES);

    glColor3f(1.0, 0.0, 0.0);       // Red
    glVertex3f(-0.75, -0.5, 0.0);        // up point

    glColor3f(0.0, 1.0, 0.0);       // Green
    glVertex3f(-1.0, -1.0, 0.0);    // bottom left

    glColor3f(0.0, 0.0, 1.0);       // Blue
    glVertex3f(-0.5, -1, 0.0);     // bootom right

    glEnd();
}

void DrawPolygon() {
    glBegin(GL_POLYGON);

    glColor3f(0.0, 1.0, 0.0);       // Green

    glVertex3f(0.25, 0.25, 0.0);
    glVertex3f(0.75, 0.25, 0.0);
    glVertex3f(0.75, 0.75, 0.0);
    glVertex3f(0.25, 0.75, 0.0);

    glEnd();
}

void DrawLine() {
    glLineWidth(2.5);

    glBegin(GL_LINES);

    glColor3f(0.5, 0.0, 0.0);    // Red

    glVertex3f(-0.5, 0.0, 0.0);
    glVertex3f(0.5, 0, 0);

    glEnd();
}

void DrawCircle() {
    int n = 120;
    float PI = 3.1415926f;
    float R = 0.2f;

    glColor3f(1, 0.0, 0.0);    // Red

    glBegin(GL_POLYGON);

    for (int i = 0; i < n; i++) {
        glVertex2f(-0.5 + R * cos(2 * PI * i / n), 0.5 + R * sin(2 * PI * i / n));
    }
    glEnd();

}

int main(int argc, char *argv[]) {
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

    glfwSetKeyCallback(window, key_callback);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {

        DrawTriangle();

        DrawPolygon();

        DrawLine();

        DrawCircle();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}