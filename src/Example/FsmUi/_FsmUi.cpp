#include <GL/gl.h>
#include <png.h>
#include <string>
#include <glog/logging.h>
#include <math.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <thread>

#include "Heroine.h"

const int WIDTH = 800;
const int HEIGHT = 800;

auto hero1 = Heroine();

GLuint png_texture_load(const char *file_name, int *width, int *height) {
    png_byte header[8];

    FILE *fp = fopen(file_name, "rb");
    if (fp == 0) {
        perror(file_name);
        return 0;
    }

    // read the header
    fread(header, 1, 8, fp);

    if (png_sig_cmp(header, 0, 8)) {
        fprintf(stderr, "error: %s is not a PNG.\n", file_name);
        fclose(fp);
        return 0;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fprintf(stderr, "error: png_create_read_struct returned 0.\n");
        fclose(fp);
        return 0;
    }

    // create png info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        fprintf(stderr, "error: png_create_info_struct returned 0.\n");
        png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
        fclose(fp);
        return 0;
    }

    // create png info struct
    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
        fprintf(stderr, "error: png_create_info_struct returned 0.\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
        fclose(fp);
        return 0;
    }

    // the code in this if statement gets called if libpng encounters an error
    if (setjmp(png_jmpbuf(png_ptr))) {
        fprintf(stderr, "error from libpng\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        return 0;
    }

    // init png reading
    png_init_io(png_ptr, fp);

    // let libpng know you already read the first 8 bytes
    png_set_sig_bytes(png_ptr, 8);

    // read all the info up to the image data
    png_read_info(png_ptr, info_ptr);

    // variables to pass to get info
    int bit_depth, color_type;
    png_uint_32 temp_width, temp_height;

    // get info about png
    png_get_IHDR(png_ptr, info_ptr,
                 &temp_width, &temp_height,
                 &bit_depth, &color_type,
                 nullptr, nullptr, nullptr);

    if (width) { *width = temp_width; }
    if (height) { *height = temp_height; }

    //printf("%s: %lux%lu %d\n", file_name, temp_width, temp_height, color_type);

    if (bit_depth != 8) {
        fprintf(stderr, "%s: Unsupported bit depth %d.  Must be 8.\n", file_name, bit_depth);
        return 0;
    }

    GLint format;
    switch (color_type) {
        case PNG_COLOR_TYPE_RGB:
            format = GL_RGB;
            break;
        case PNG_COLOR_TYPE_RGB_ALPHA:
            format = GL_RGBA;
            break;
        default:
            fprintf(stderr, "%s: Unknown libpng color type %d.\n", file_name, color_type);
            return 0;
    }

    // Update the png info struct.
    png_read_update_info(png_ptr, info_ptr);

    // Row size in bytes.
    int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

    // glTexImage2d requires rows to be 4-byte aligned
    rowbytes += 3 - ((rowbytes - 1) % 4);

    // Allocate the image_data as a big block, to be given to opengl
    png_byte *image_data = (png_byte *) malloc(rowbytes * temp_height * sizeof(png_byte) + 15);
    if (image_data == NULL) {
        fprintf(stderr, "error: could not allocate memory for PNG image data\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        return 0;
    }

    // row_pointers is for pointing to image_data for reading the png with libpng
    png_byte **row_pointers = (png_byte **) malloc(temp_height * sizeof(png_byte *));
    if (row_pointers == NULL) {
        fprintf(stderr, "error: could not allocate memory for PNG row pointers\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        free(image_data);
        fclose(fp);
        return 0;
    }

    // set the individual row_pointers to point at the correct offsets of image_data
    for (unsigned int i = 0; i < temp_height; i++) {
        row_pointers[temp_height - 1 - i] = image_data + i * rowbytes;
    }

    // read the png into image_data through row_pointers
    png_read_image(png_ptr, row_pointers);

    // Generate the OpenGL texture object
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, format,
                 temp_width, temp_height,
                 0, format, GL_UNSIGNED_BYTE,
                 image_data);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    free(image_data);
    free(row_pointers);
    fclose(fp);
    return texture;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
            case GLFW_KEY_DOWN:
                LOG(INFO) << "Press Down";
                hero1.handleInput(Input::PRESS_DOWN);
                break;
            case GLFW_KEY_A:
                LOG(INFO) << "Press A";
                hero1.handleInput(Input::FIRE);
                break;
            case GLFW_KEY_SPACE:
                LOG(INFO) << "Press Space";
                hero1.handleInput(Input::PRESS_SPACE);
                break;
            case GLFW_KEY_LEFT:
                LOG(INFO) << "Press Left";
                hero1.handleInput(Input::PRESS_LEFT);
                break;
            case GLFW_KEY_RIGHT:
                LOG(INFO) << "Press Right";
                hero1.handleInput(Input::PRESS_RIGHT);
                break;
            default:
                break;
        }
    } else if (action == GLFW_RELEASE) {
        switch (key) {
            case GLFW_KEY_DOWN:
                LOG(INFO) << "Release Down";
                hero1.handleInput(Input::RELEASE_DOWN);
                break;
            case GLFW_KEY_SPACE:
                LOG(INFO) << "Release Space";
                break;
            case GLFW_KEY_LEFT:
                LOG(INFO) << "Release Left";
                hero1.handleInput(Input::RELEASE_LEFT);
                break;
            case GLFW_KEY_RIGHT:
                LOG(INFO) << "Release Right";
                hero1.handleInput(Input::RELEASE_RIGHT);
            default:
                break;
        }
    }
}

// draw state
void DrawStanding(GLuint &img) {
    if (hero1.state_->name() == "StandingState") {
        glColor3f(1, 1, 1);
    } else {
        glColor3f(0.5, 0.5, 0.5);
    }

    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, img);

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); // left bottom
    glVertex2f(0.2f, 0.0f);

    glTexCoord2f(1.0f, 0.0f); // right bottom
    glVertex2f(0.6f, 0.0f);

    glTexCoord2f(1.0f, 1.0f); // right top
    glVertex2f(0.6f, 0.2f);

    glTexCoord2f(0.0f, 1.0f); // left top
    glVertex2f(0.2f, 0.2f);

    glEnd();
}

void DrawDiving(GLuint &img) {
    if (hero1.state_->name() == "DivingState") {
        glColor3f(1, 1, 1);
    } else {
        glColor3f(0.5, 0.5, 0.5);
    }

    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, img);

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); // left bottom
    glVertex2f(0.2f, 0.3f);

    glTexCoord2f(1.0f, 0.0f); // right bottom
    glVertex2f(0.6f, 0.3f);

    glTexCoord2f(1.0f, 1.0f); // right top
    glVertex2f(0.6f, 0.5f);

    glTexCoord2f(0.0f, 1.0f); // left top
    glVertex2f(0.2f, 0.5f);

    glEnd();
}

void DrawDucking(GLuint &img) {
    if (hero1.state_->name() == "DuckingState") {
        glColor3f(1, 1, 1);
    } else {
        glColor3f(0.5, 0.5, 0.5);
    }

    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, img);

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); // left bottom
    glVertex2f(-0.4f, -0.3f);

    glTexCoord2f(1.0f, 0.0f); // right bottom
    glVertex2f(0.0f, -0.3f);

    glTexCoord2f(1.0f, 1.0f); // right top
    glVertex2f(0.0f, -0.1f);

    glTexCoord2f(0.0f, 1.0f); // left top
    glVertex2f(-0.4f, -0.1f);

    glEnd();
}

void DrawJumping(GLuint &img) {
    if (hero1.state_->name() == "JumpingState") {
        glColor3f(1, 1, 1);
    } else {
        glColor3f(0.5, 0.5, 0.5);
    }

    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, img);

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); // left bottom
    glVertex2f(-0.4f, 0.0f);

    glTexCoord2f(1.0f, 0.0f); // right bottom
    glVertex2f(0.0f, 0.0f);

    glTexCoord2f(1.0f, 1.0f); // right top
    glVertex2f(0.0f, 0.2f);

    glTexCoord2f(0.0f, 1.0f); // left top
    glVertex2f(-0.4f, 0.2f);

    glEnd();
}

void DrawFire(GLuint &img) {
    if (hero1.equipment_->name() == "Fire") {
        glColor3f(1, 1, 1);
    } else {
        glColor3f(0.5, 0.5, 0.5);
    }

    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, img);

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); // left bottom
    glVertex2f(0.2f, -0.3f);

    glTexCoord2f(1.0f, 0.0f); // right bottom
    glVertex2f(0.6f, -0.3f);

    glTexCoord2f(1.0f, 1.0f); // right top
    glVertex2f(0.6f, -0.1f);

    glTexCoord2f(0.0f, 1.0f); // left top
    glVertex2f(0.2f, -0.1f);

    glEnd();
}

void DrawMoving(GLuint &img) {
    if (hero1.state_->name() == "MovingState") {
        glColor3f(1, 1, 1);
    } else {
        glColor3f(0.5, 0.5, 0.5);
    }

    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, img);

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); // left bottom
    glVertex2f(-0.4f, 0.3f);

    glTexCoord2f(1.0f, 0.0f); // right bottom
    glVertex2f(0.0f, 0.3f);

    glTexCoord2f(1.0f, 1.0f); // right top
    glVertex2f(0.0f, 0.5f);

    glTexCoord2f(0.0f, 1.0f); // left top
    glVertex2f(-0.4f, 0.5f);

    glEnd();
}

// update game object
void Update() {
    for (int i = 0; i < 1000; i++) {
        hero1.update();
        usleep(1000 * 50);
    }
}

int main(int argc, char *argv[]) {

    // glog init
    google::InitGoogleLogging(argv[0]);
    google::SetLogDestination(google::GLOG_INFO, "./glog-");
    FLAGS_alsologtostderr = true;
    FLAGS_logbuflevel = -1;

    std::thread t1(Update);

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

    int w1 = 160;
    int h1 = 80;
    auto standing = png_texture_load("/home/me/Pictures/standing.png", &w1, &h1);
    auto diving = png_texture_load("/home/me/Pictures/diving.png", &w1, &h1);
    auto ducking = png_texture_load("/home/me/Pictures/ducking.png", &w1, &h1);
    auto jumping = png_texture_load("/home/me/Pictures/jumping.png", &w1, &h1);
    auto fire = png_texture_load("/home/me/Pictures/fire.png", &w1, &h1);
    auto moving = png_texture_load("/home/me/Pictures/moving.png", &w1, &h1);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {

        DrawStanding(standing);
        DrawDiving(diving);
        DrawDucking(ducking);
        DrawJumping(jumping);
        DrawFire(fire);
        DrawMoving(moving);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

    // close glog
    google::ShutdownGoogleLogging();

    return 0;
}