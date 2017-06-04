#ifndef GUARD_INCLUDE_GL_UTIL_H
#define GUARD_INCLUDE_GL_UTIL_H

#ifdef GL_UTIL_STATIC
    #define GL_UTIL_DEF static
#else
    #define GL_UTIL_DEF extern
#endif

#include <glad/glad.h>

namespace util {

GL_UTIL_DEF bool buildShader(GLuint shaderId, char const *source);
GL_UTIL_DEF bool linkProgram(GLuint programId);
GL_UTIL_DEF void logGlError();

} // end namespace util

#endif // GUARD_INCLUDE_GL_UTIL_H

#ifdef GL_UTIL_IMPLEMENTATION

namespace util {

#include <glad/glad.h>
#include <SDL_log.h>

GL_UTIL_DEF bool buildShader(GLuint shaderId, char const *source)
{
    glShaderSource(shaderId, 1, &source, nullptr);
    glCompileShader(shaderId);

    GLint didCompile;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &didCompile);
    if (didCompile != GL_TRUE) {
        GLsizei loglen;
        GLchar  msg[1024];
        glGetShaderInfoLog(shaderId, 1024, &loglen, msg);
        msg[loglen] = '\0';
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s\n", msg);
        return false;
    }

    return true;
}

GL_UTIL_DEF bool linkProgram(GLuint programId)
{
    glLinkProgram(programId);

    GLint didLink;
    glGetProgramiv(programId, GL_LINK_STATUS, &didLink);
    if (didLink != GL_TRUE) {
        GLsizei loglen;
        GLchar  msg[1024];
        glGetProgramInfoLog(programId, 1024, &loglen, msg);
        msg[loglen] = '\0';
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s\n", msg);
        return false;
    }

    return true;
}

void logGlError()
{
    switch (glGetError()) {
    case GL_NO_ERROR:
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "OpenGL -- no error\n");
        break;
    case GL_INVALID_ENUM:
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "OpenGL -- invalid enum\n");
        break;
    case GL_INVALID_VALUE:
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "OpenGL -- invalid value\n");
        break;
    case GL_INVALID_OPERATION:
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "OpenGL -- invalid operation\n");
        break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "OpenGL -- invalid framebuffer operation\n");
        break;
    case GL_OUT_OF_MEMORY:
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "OpenGL -- out of memory\n");
        break;
    default:
        break;
    }
}

} // end namespace util

#endif GL_UTIL_IMPLEMENTATION
