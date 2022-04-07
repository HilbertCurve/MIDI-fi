/* date = March 31st 2022 10:52 am */

#ifndef MIDIFI_RENDERER_HPP
#define MIDIFI_RENDERER_HPP

#include <glm/glm.hpp>
#include "Utils.hpp"

namespace MidiFi {
    /*
     * Define this macro if you want finer control over
     * the rendering system. It is used in Renderer.cpp.
     */
#ifdef MIDIFI_RENDERER_INCLUDE_CORE
    ///////////////////////
    // Shaders
    ///////////////////////

    /**
     * A shader is a pair of programs that are executed on the
     * GPU whenever vertex buffers are rendered.
     */
    struct Shader {
        String vertexCode;
        String fragmentCode;

        const char *vertexFP = nullptr;
        const char *fragmentFP = nullptr;

        unsigned int vertexID;
        unsigned int fragmentID;

        unsigned int programID;

        bool isUsed;
    };

    /**
     * Default vertex shader accepting:
     *  vPos: vec3, vColor: vec4, vUV: vec2, vTexID: float
     *
     * Uniforms: none
     */
    extern const char *defaultVertexShader;
    /**
     * Default fragment shader accepting:
     *  fPos: vec4, fColor: vec4, fUV: vec2, fTexID: float
     *
     * Uniforms: sampler2D uTextures[8]
     */
    extern const char *defaultFragmentShader;

    /**
     * Initializes shader object, uploading shader source from
     * `vertPath` and `fragPath` files. If both are `nullptr`, 
     * strings given to the shader object directly are used, and
     * must not be `nullptr` themselves.
     */
    void initShader(Shader &s, const char *vertSource, const char *fragSource);
    /**
     * Deletes shader object, freeing shader sources and ensuring
     * shader program is no longer linked.
     */
    void deleteShader(Shader &s);

    void attachShader(Shader &s);
    void detachShader(Shader &s);

    void shaderUploadMat4(Shader &s, const char *name, const glm::mat4 &data);
    void shaderUploadFloat(Shader &s, const char *name, const float &data);
    void shaderUploadInt(Shader &s, const char *name, const int data);
    void shaderUploadIntV(Shader &s, const char *name, const int *data, int count);
    void shaderUploadFloatV(Shader &s, const char *name, float *arr, int count);
    void shaderUploadVec2V(Shader &s, const char *name, float *arr, int count);
    void shaderUploadVec3V(Shader &s, const char *name, float *arr, int count);
    void shaderUploadVec4V(Shader &s, const char *name, float *arr, int count);

    struct Framebuffer {
        unsigned int fbo;
        unsigned int fbTexHandle;
        bool complete;
        bool isUsed;
    };

    void initFramebuffer(Framebuffer &f);
    void deleteFramebuffer(Framebuffer &f);

    void attachFramebuffer(Framebuffer &f);
    void detachFramebuffer(Framebuffer &f);

    struct Texture {
        unsigned int id;
        const char *sourceFP;
    };

    enum vType {
        SHORT = 0,
        INT,
        LONG,
        FLOAT
    };

    enum vProp {
        POS = 0,
        COLOR,
        TEXID,
        UV,
        OTHER
    };

    struct VertexAttributes {
        int len;
        struct {
            vProp prop;
            vType type;
        } vAttrib;
    };

    struct VertexBuffer {
        void *data;
        VertexAttributes va;
    };
#endif // MIDIFI_RENDERER_INCLUDE_CORE
    void startRenderer();
    void updateRenderer();
}

#endif // MIDIFI_RENDERER_HPP

