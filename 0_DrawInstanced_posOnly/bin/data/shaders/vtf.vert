#version 120

#extension GL_EXT_gpu_shader4 : enable
#extension GL_ARB_texture_rectangle : enable


varying vec3 v_Normal;
varying vec4 v_Vertex;

uniform sampler2DRect posTex;
uniform float iCount;

void main(void)
{
    gl_TexCoord[0] = gl_MultiTexCoord0;
    vec2 st = vec2(mod(gl_InstanceID,int(iCount)),floor(gl_InstanceID/int(iCount)));
    vec3 pos = texture2DRect(posTex,st+vec2(0.5)).rgb;
    mat4 m = mat4(1, 0, 0, pos.x,
                  0, 1, 0, pos.y,
                  0, 0, 1, pos.z,
                  0, 0, 0, 1);
    vec4 vertex = gl_Vertex * m;
    v_Vertex = gl_ModelViewMatrix * vertex;
    v_Normal = normalize(gl_NormalMatrix * gl_Normal);
    
	gl_Position = gl_ModelViewProjectionMatrix * vertex;
    gl_FrontColor = vec4(1.0,1.0,1.0,1.0);
}