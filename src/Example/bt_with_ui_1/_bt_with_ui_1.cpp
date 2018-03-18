#include <cassert>

#include <glog/logging.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION

#include "stb/stb_image.h"

const int WIDTH = 800;
const int HEIGHT = 800;

GLFWwindow *window;

// Init glog
void glogInit(char *argv[]) {
    // glog init
    google::InitGoogleLogging(argv[0]);
    google::SetLogDestination(google::GLOG_INFO, "./glog-");
    FLAGS_alsologtostderr = true;
    FLAGS_logbuflevel = -1;
}

// Close glog
void glogClose() {
    google::ShutdownGoogleLogging();
}

// Init Glfw
int glInit() {
    /* Initialize the library */
    if (!glfwInit()) {
        return -1;
    }

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    return 0;
}

// Test png load and draw
GLuint m_texture;

void loadPngTest() {
    int w;
    int h;
    int comp;
    std::string filename = "/home/me/Pictures/standing.png";
    unsigned char *image = stbi_load(filename.c_str(), &w, &h, &comp, STBI_rgb);

    if (image == nullptr)
        throw (std::string("Failed to load texture"));

    glGenTextures(1, &m_texture);

    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if (comp == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    else if (comp == 4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(image);
    LOG(INFO) << w << " " << h;
}

void drawPngTest(GLuint m_texture) {
    int w = 160;
    int h = 80;
    float ww = (float) w / WIDTH * 2;
    float hh = (float) h / WIDTH * 2;

    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, m_texture);

    glBegin(GL_QUADS);

    glColor3ub(255, 255, 255);

    glTexCoord2f(0.0f, 0.0f); // left bottom
    glVertex2f(-ww, 0.0f);

    glTexCoord2f(1.0f, 0.0f); // right bottom
    glVertex2f(0.0f, 0.0f);

    glTexCoord2f(1.0f, -1.0f); // right top
    glVertex2f(0.0f, hh);

    glTexCoord2f(0.0f, -1.0f); // left top
    glVertex2f(-ww, hh);

    glEnd();
}

// Key callback
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
            case GLFW_KEY_SPACE:
                break;
            default:
                break;
        }
    } else if (action == GLFW_RELEASE) {
        switch (key) {
            case GLFW_KEY_SPACE:
                break;
            default:
                break;
        }
    }
}

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
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
    } else if (action == GLFW_RELEASE) {
        switch (button) {
            case GLFW_MOUSE_BUTTON_LEFT:
                LOG(INFO) << "Mosue left button release!";
                break;
            case GLFW_MOUSE_BUTTON_MIDDLE:
                LOG(INFO) << "Mosue middle button release!";
                break;
            case GLFW_MOUSE_BUTTON_RIGHT:
                LOG(INFO) << "Mosue right button release!";
                break;
            default:
                return;
        }
    }
}

void cursorPosCallback(GLFWwindow *window, double x, double y) {
    LOG(INFO) << "Mouse position move to " << int(x) << " " << int(y);
    return;
}

void cursorEnterCallback(GLFWwindow *window, int mods) {
    LOG(INFO) << "Mouse Enter";
}

void scrollCallback(GLFWwindow *window, double x, double y) {
    LOG(INFO) << "Scroll";
}

// main function
int main(int argc, char *argv[]) {
    // Glog init
    glogInit(argv);

    // Gl init
    glInit();

    loadPngTest();

    // Sets the key callback.
    glfwSetKeyCallback(window, keyCallback);

    // Sets the mouse callback.
    // glfwSetMouseButtonCallback(window, mouseButtonCallback);
    // glfwSetCursorPosCallback(window, cursorPosCallback);
    // glfwSetCursorEnterCallback(window, cursorEnterCallback);
    // glfwSetScrollCallback(window, scrollCallback);

    // Sets the file drop callback.
    glfwSetDropCallback(window, nullptr);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        drawPngTest(m_texture);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // Terminates the GLFW library.
    glfwTerminate();

    // Glog close
    glogClose();

    return 0;
}