#version 120

#extension GL_EXT_gpu_shader4 : enable
#extension GL_ARB_texture_rectangle : enable


varying vec3 v_Normal;
varying vec4 v_Vertex;

uniform sampler2DRect posTex;
uniform sampler2DRect rotTex;
uniform sampler2DRect sizeTex;
uniform sampler2DRect colorTex;
uniform float iCount;

mat3 inverse(mat3 m)
{
    mat3 adj;
    adj[0][0] = + (m[1][1] * m[2][2] - m[2][1] * m[1][2]);
    adj[1][0] = - (m[1][0] * m[2][2] - m[2][0] * m[1][2]);
    adj[2][0] = + (m[1][0] * m[2][1] - m[2][0] * m[1][1]);
    adj[0][1] = - (m[0][1] * m[2][2] - m[2][1] * m[0][2]);
    adj[1][1] = + (m[0][0] * m[2][2] - m[2][0] * m[0][2]);
    adj[2][1] = - (m[0][0] * m[2][1] - m[2][0] * m[0][1]);
    adj[0][2] = + (m[0][1] * m[1][2] - m[1][1] * m[0][2]);
    adj[1][2] = - (m[0][0] * m[1][2] - m[1][0] * m[0][2]);
    adj[2][2] = + (m[0][0] * m[1][1] - m[1][0] * m[0][1]);
    float det = (+ m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1])
                 - m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0])
                 + m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]));
    return adj / det;
}

void main(void)
{
    gl_TexCoord[0] = gl_MultiTexCoord0;
    vec2 st = vec2(mod(gl_InstanceID,int(iCount)),floor(gl_InstanceID/int(iCount)));
    vec3 pos = texture2DRect(posTex,st+vec2(0.5)).rgb;
    vec4 rot = texture2DRect(rotTex,st+vec2(0.5)).rgba;
    vec3 size = texture2DRect(sizeTex,st+vec2(0.5)).rgb;
    vec4 color = texture2DRect(colorTex,st+vec2(0.5)).rgba;
    
    mat4 m = mat4(1-2*pow(rot.y,2)-2*pow(rot.z,2), 2*rot.x*rot.y+2*rot.w*rot.z, 2*rot.x*rot.z-2*rot.w*rot.y, pos.x,
                  2*rot.x*rot.y-2*rot.w*rot.z, 1-2*pow(rot.x,2)-2*pow(rot.z,2), 2*rot.y*rot.z+2*rot.w*rot.x, pos.y,
                  2*rot.x*rot.z+2*rot.w*rot.y, 2*rot.y*rot.z-2*rot.w*rot.x, 1-2*pow(rot.x,2)-2*pow(rot.y,2), pos.z,
                  0,0,0,1);
    mat3 invM = inverse(mat3(m));
    
    vec4 vertex = gl_Vertex * vec4(size,1.0) * m;
    v_Vertex = gl_ModelViewMatrix * vertex;
    v_Normal = normalize(gl_NormalMatrix * mat3(invM) *  gl_Normal);
    
	gl_Position = gl_ModelViewProjectionMatrix * vertex;
    gl_FrontColor = color;
}