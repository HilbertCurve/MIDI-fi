#version 430 core

#ifdef GL_ES
 precision mediump float;
#endif

in vec4 fPos;
in vec4 fColor;
in vec2 fTexCoords;
in float fTexID;

uniform sampler2D uTextures[16];
uniform float uTime;

out vec4 color;

float dist(vec3 a, vec3 b)
{
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return sqrt(dx*dx + dy*dy);
}

void main()
{
    if (fTexID > 0.0) 
    {
        vec3 point = vec3(0.0, 0.0, 0.0);
        int id = int(fTexID);
        float vignetteConst = dist(point, fPos.xyz) + (sin(2*uTime) * sin(2*uTime) / 10.0) + 0.2;
        
        if (id == 0)
        {
            color = fColor;
        }
        else
        {
            color = fColor * texture(uTextures[id - 1], fTexCoords);// - vignetteConst;
        }
    } 
    else
    {
        color = fColor;
    }
}
