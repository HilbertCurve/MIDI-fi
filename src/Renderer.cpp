#define MIDIFI_RENDERER_INCLUDE_CORE
#include "Renderer.hpp"
#undef MIDIFI_RENDERER_INCLUDE_CORE

#include <GL/gl.h>
#include <cstdio>
#include "Utils.hpp"

namespace MidiFi {
    ///////////////////
    // Shaders
    ///////////////////
    String defaultVertexShader = "\n"
    "#version 330 core\n"
    "layout (location=0) in vec3 vPos;\n"
    "layout (location=1) in vec4 vColor;\n"
    "layout (location=2) in vec2 vUV;\n"
    "layout (location=3) in float vTexID\n"
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

    String defaultFragmentShader = "\n"
    "#version 330 core\n"
    "\n"
    "in vec4 fPos;\n"
    "in vec4 fColor;\n"
    "in vec2 fUV;\n"
    "in float fTexID;\n"
    "\n"
    "uniform sampler2D uTextures[8];\n"
    "\n"
    "out vec4 color;\n"
    "\n"
    "void main() {\n"
    "    if (fTexID > 0.0) {\n"
    "        int id = int(fTexID);\n"
    "        color= fColor * texture(uTextures[id - 1], fUV);\n"
    "    } else {\n"
    "        color = fColor;\n"
    "    }\n"
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

    void initRenderer() {

    }
}

