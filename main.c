#include <stdio.h>
#include <errno.h>

#include "glad/glad.h"

#define RGFW_IMPLEMENTATION
#define RGFW_PRINT_ERRORS
#define RGFW_DEBUG
#include "RGFW.h"

#include "logka.h"

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

char* read_entire_file(const char* filepath) {
    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        errorf("Failed to read entire file %s: %s", filepath, strerror(errno));
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    unsigned long fileSize = ftell(file);
    rewind(file);

    char *buffer = malloc(fileSize + 1);
    if (buffer == NULL) {
        errorf("Failed to allocate %ld bytes of memory for reading file %s", fileSize + 1, filepath);
        fclose(file);
        exit(EXIT_FAILURE);
    }

    size_t bytesRead = fread(buffer, 1, fileSize, file);
    if (bytesRead != fileSize) {
        free(buffer);
        fclose(file);
        errorf("Read less bytes than size of file %s, expected a plain text file.", filepath);
        exit(EXIT_FAILURE);
    }
    buffer[bytesRead] = '\0';

    fclose(file);

    return buffer;
}

unsigned int load_shader(const char *filename, GLenum shader_type) {
    char *shader_text   = read_entire_file(filename);
    unsigned int shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, (const char * const*)&shader_text, NULL);
    glCompileShader(shader);

    GLint success;
    char buffer[4096] = {0};
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, sizeof(buffer), NULL, buffer);
        errorf("Failed to compile shader %s: %s", filename, buffer);
        exit(EXIT_FAILURE);
    }

    free(shader_text);

    return shader;
}

int main(void) {
    RGFW_setHint_OpenGL(RGFW_glMinor, 3);
    RGFW_setHint_OpenGL(RGFW_glMajor, 3);

    RGFW_window* window = RGFW_createWindow(
        "Fraction thing visalizer",
        RGFW_RECT(SCR_WIDTH, SCR_HEIGHT, SCR_WIDTH, SCR_HEIGHT),
        RGFW_windowAllowDND | RGFW_windowCenter | RGFW_windowScaleToMonitor
    );

    if (window == NULL) {
        error("Failed to create RGFW window");
        return -1;
    }

    RGFW_window_makeCurrentContext_OpenGL(window);

    if (!gladLoadGLLoader((GLADloadproc)RGFW_getProcAddress_OpenGL)) {
        error("Failed to initialize GLAD");
        return 1;
    }

    unsigned int vertex_shader   = load_shader("vertex.glsl",   GL_VERTEX_SHADER);
    unsigned int fragment_shader = load_shader("fragment.glsl", GL_FRAGMENT_SHADER);

    unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);


    GLint success;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        char buffer[4096] = {0};
        glGetProgramInfoLog(shader_program, sizeof(buffer), NULL, buffer);
        errorf("Failed to link shader program: %s", buffer);
    }
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    float vertices[] = {
         1.0f,  1.0f, 0.0f,  // top right
         1.0f, -1.0f, 0.0f,  // bottom right
        -1.0f, -1.0f, 0.0f,  // bottom left
        -1.0f,  1.0f, 0.0f   // top left
    };

    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };

    GLint offset_x_shader_location = glGetUniformLocation(shader_program, "offset_x");
    GLint offset_y_shader_location = glGetUniformLocation(shader_program, "offset_y");

    unsigned int VBO, VAO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    int offset_x = 0;
    int offset_y = 0;
    bool mouse_down = false;
    while (!RGFW_isPressed(window, RGFW_escape)) {
        RGFW_event event;

        RGFW_window_checkEvent(window, &event);
        {
            switch (event.type) {
                case RGFW_quit:
                    goto done;

                case RGFW_mouseButtonPressed:  mouse_down = true; break;
                case RGFW_mouseButtonReleased: mouse_down = false;  break;
                case RGFW_mousePosChanged:
                    if (mouse_down) {
                        offset_x -= event.vector.x;
                        offset_y += event.vector.y;
                    }

                    break;
                default: {}
            }
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);

        glUniform1i(offset_x_shader_location, offset_x);
        glUniform1i(offset_y_shader_location, offset_y);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        RGFW_window_swapBuffers_OpenGL(window);
    }

done:

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shader_program);

    RGFW_window_close(window);

    return 0;

}
