#define MIDIFI_RENDERER_INCLUDE_CORE
#include "Renderer.hpp"
#undef MIDIFI_RENDERER_INCLUDE_CORE

#include <GL/gl.h>
#include <glm/gtc/type_ptr.hpp>
#include <cstdio>

#include "Application.hpp"
#include "Utils.hpp"

namespace MidiFi {
    ///////////////////
    // Shaders
    ///////////////////
    const char *defaultVertexShader =
    "#version 330 core\n\t"
    "layout (location=0) in vec3 vPos;\n"
    "layout (location=1) in vec4 vColor;\n"
    "layout (location=2) in vec2 vUV;\n"
    "layout (location=3) in float vTexID;\n"
    "\n"
    "out vec4 fPos;\n"
    "out vec4 fColor;\n"
    "out vec2 fUV;\n"
    "out float fTexID;\n"
    "\n"
    "void main() {\n"
    "    fPos = vec4(vPos, 1.0);\n"
    "    fColor = vColor;\n"
    "    fUV = vUV;\n"
    "    fTexID = vTexID;\n"
    "    gl_Position = vec4(vPos, 1.0);\n"
    "}\n";

    const char *defaultFragmentShader =
    "#version 330 core\n\n"
    "\n"
    "in vec4 fPos;\n"
    "in vec4 fColor;\n"
    "in vec2 fUV;\n"
    "in float fTexID;\n"
    "\n"
    "\n"
    "out vec4 color;\n"
    "\n"
    "void main() {\n"
    "    color = fColor;\n"
    "}\n";

    void initShader(Shader &s, const char *vertPath, const char *fragPath) {
        // load shader code
        if (vertPath && fragPath) {
            // vertex
            FILE *vertFile = fopen(vertPath, "rb");
            m_assert(vertFile, "Could not open file '%s'.", vertPath);

            int vertFileSize = 0;
            fseek(vertFile, 0, SEEK_END);
            vertFileSize = ftell(vertFile);
            rewind(vertFile);

            char vertCode[vertFileSize];
            fread((void *) vertCode, vertFileSize, 1, vertFile);

            s.vertexCode = &vertCode[0];

            // fragment
            FILE *fragFile = fopen(fragPath, "rb");
            m_assert(fragFile, "Could not open file '%s'.", fragPath);

            int fragFileSize = 0;
            fseek(fragFile, 0, SEEK_END);
            fragFileSize = ftell(fragFile);
            rewind(fragFile);

            char fragCode[fragFileSize];
            fread((void *) fragCode, fragFileSize, 1, fragFile);

            s.fragmentCode = &fragCode[0];
        }

        s.vertexID = glCreateShader(GL_VERTEX_SHADER);
        s.fragmentID = glCreateShader(GL_FRAGMENT_SHADER);

        GLint result = GL_FALSE;
        int infoLogLength;

        // compile shaders
        m_log_message("Compiling vertex shader: %s.", vertPath ? vertPath : "<native>");
        glShaderSource(s.vertexID, 1, &s.vertexCode.src, &s.vertexCode.len);
        glCompileShader(s.vertexID);

        glGetShaderiv(s.vertexID, GL_COMPILE_STATUS, &result);
        glGetShaderiv(s.vertexID, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0) {
            char err[infoLogLength + 1];
            glGetShaderInfoLog(s.vertexID, infoLogLength, NULL, &err[0]);
            m_log_warning("Vertex shader compilation failed: %s.", &err[0]);
        }

        m_log_message("Compiling fragment shader: %s.", fragPath ? fragPath : "<native>");
        glShaderSource(s.fragmentID, 1, &s.fragmentCode.src, &s.fragmentCode.len);
        glCompileShader(s.fragmentID);

        glGetShaderiv(s.fragmentID, GL_COMPILE_STATUS, &result);
        glGetShaderiv(s.fragmentID, GL_INFO_LOG_LENGTH, &infoLogLength);

        if (infoLogLength > 0) {
            char err[infoLogLength + 1];
            glGetShaderInfoLog(s.fragmentID, infoLogLength, NULL, &err[0]);
            m_log_warning("Fragment shader compilation failed: %s.", &err[0]);
        }

        // link shaders
        s.programID = glCreateProgram();
        m_log_message("Linking shader program %u.", s.programID);
        glAttachShader(s.programID, s.vertexID);
        glAttachShader(s.programID, s.fragmentID);
        glLinkProgram(s.programID);

        glGetShaderiv(s.programID, GL_LINK_STATUS, &result);
        glGetShaderiv(s.programID, GL_INFO_LOG_LENGTH, &infoLogLength);

        if (infoLogLength > 0) {
            char err[infoLogLength + 1];
            glGetShaderInfoLog(s.fragmentID, infoLogLength, NULL, &err[0]);
            m_log_warning("Shader linking failed: %s.", &err[0]);
        }
    }

    void deleteShader(Shader &s) {
        if (s.isUsed) detachShader(s);

        s.vertexCode = "";
        s.fragmentCode = "";

        s.vertexFP = nullptr;
        s.fragmentFP = nullptr;

        glDeleteShader(s.vertexID);
        glDeleteShader(s.fragmentID);

        glDeleteProgram(s.programID);
    }

    void attachShader(Shader &s) {
        glUseProgram(s.programID);
        s.isUsed = true;
    }

    void detachShader(Shader &s) {
        glUseProgram(0);
        s.isUsed = false;
    }

    void shaderUploadMat4(Shader &s, const char *name, const glm::mat4 &data) {
        GLint varLocation = glGetUniformLocation(s.programID, name);
        if (!s.isUsed) attachShader(s);
        glUniformMatrix4fv(varLocation, 1, GL_FALSE, glm::value_ptr(data));
    }

    void shaderUploatIntV(Shader &s, const char *name, const int *data, int count) {
        GLint varLocation = glGetUniformLocation(s.programID, name);
        if (!s.isUsed) attachShader(s);
        glUniform1iv(varLocation, count, data);
    }
    
    /////////////////////
    // Framebuffers
    /////////////////////

    void initFramebuffer(Framebuffer &f) {
        glGenFramebuffers(1, &f.fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, f.fbo);

        glGenTextures(1, &f.fbTexHandle);
        glBindTexture(GL_TEXTURE_2D, f.fbTexHandle);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, getWindow().width, getWindow().height,
                0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
                GL_TEXTURE_2D, f.fbTexHandle, 0);

        m_assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, 
                "Framebuffer with fbo %d not complete.", f.fbo);
        f.complete = true;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void deleteFamebuffer(Framebuffer &f) {
        if (f.isUsed) detachFramebuffer(f);
        glDeleteFramebuffers(1, &f.fbo);
        glDeleteTextures(1, &f.fbTexHandle);
    }

    void attachFramebuffer(Framebuffer &f) {
        glBindFramebuffer(GL_FRAMEBUFFER, f.fbo);
        f.isUsed = true;
    }

    void detachFramebuffer(Framebuffer &f) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        f.isUsed = false;
    }

    static Shader standard;
    void startRenderer() {
        // initialize default shader
        standard.vertexCode = defaultVertexShader;
        standard.fragmentCode = defaultFragmentShader;
        initShader(standard, 
                nullptr, nullptr);
    }

    void updateRenderer() {
        attachShader(standard);



        detachShader(standard);
    }
}

