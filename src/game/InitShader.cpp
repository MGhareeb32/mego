#include "OpenGL.h"

namespace Angel {

std::string readFile(const char* shaderFile) {
    std::ifstream in(shaderFile);
    if (in.fail())
        return "";
    std::stringstream ss;
    std::string line;
    while (getline(in, line))
        ss << line << std::endl;
    ss << '\0';
    in.close();
    return ss.str();
}

// Create a GLSL program object from vertex and fragment shader files
GLuint InitShader(const char* vShaderFile, const char* fShaderFile) {
    struct Shader {
        const char* filename;
        GLenum type;
    } shaders[2] = { { vShaderFile, GL_VERTEX_SHADER },
                     { fShaderFile, GL_FRAGMENT_SHADER } };

    GLuint program = glCreateProgram();
    for (int i = 0; i < 2; ++i) {
        Shader& s = shaders[i];
        std::string source = readFile(s.filename);
        if (!source.size()) {
            std::cerr << "SHADER " << s.filename
                       << "read failed" << std::endl;
            exit(EXIT_FAILURE);
        }

        GLuint shader = glCreateShader(s.type);
        glShaderSource(shader, 1, (const GLchar**) &source, NULL);
        glCompileShader(shader);

        GLint compiled;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            std::cerr << "SHADER "<< s.filename
                      << " compile failed" << std::endl;

            GLint logSize;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
            char* logMsg = new char[logSize];
            glGetShaderInfoLog(shader, logSize, NULL, logMsg);
            std::cerr << logMsg << std::endl;
            delete[] logMsg;

            exit(EXIT_FAILURE);
        }

        glAttachShader(program, shader);
    }

    /* link  and error check */
    glLinkProgram(program);

    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        std::cerr << "SHADER link failed" << std::endl;
        GLint logSize;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
        char* logMsg = new char[logSize];
        glGetProgramInfoLog(program, logSize, NULL, logMsg);
        std::cerr << logMsg << std::endl;
        delete[] logMsg;

        exit( EXIT_FAILURE);
    }

    /* use program object */
    glUseProgram(program);

    return program;
}

}
