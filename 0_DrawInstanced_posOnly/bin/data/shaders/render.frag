#version 120

#extension GL_EXT_gpu_shader4 : require

varying vec3 v_Normal;
varying vec4 v_Vertex;

//マテリアル
uniform vec4 COMMON_Ambient;
uniform vec4 COMMON_Diffuse;
uniform vec4 COMMON_Specular;
uniform float COMMON_Shininess;

//ADS
uniform int ADS_NumLights;

vec4 applyADS(vec4 _ambient, vec4 _diffuse, vec4 _specular,float _shininess, vec4 _vertex, vec3 _normal){
    vec4 _color = vec4(0.0,0.0,0.0,0.0);
    for(int i=0;i<ADS_NumLights;i++){
        vec3 _lightVec = gl_LightSource[i].position.xyz - _vertex.xyz;
        float _dis = length(_lightVec);
        float _attenuation = 1.0 / (gl_LightSource[i].constantAttenuation +
                                   gl_LightSource[i].linearAttenuation * _dis +
                                   gl_LightSource[i].quadraticAttenuation * _dis * _dis);
        vec3 _s = normalize(vec3(gl_LightSource[i].position) - vec3(_vertex));
        vec3 _v = normalize(vec3(-_vertex));
        vec3 _r = reflect(-_s,_normal);
        
        _color += _ambient * gl_LightSource[i].ambient
        +_diffuse*max(dot(_s,_normal),0.0) * gl_LightSource[i].diffuse * _attenuation
        +_specular*pow(max(dot(_r,_v),0.0),_shininess) * gl_LightSource[i].specular * _attenuation;
    }
    return _color;
}

void main(void)
{
    vec4 ADSColor = applyADS(COMMON_Ambient,COMMON_Diffuse,COMMON_Specular,COMMON_Shininess,v_Vertex,v_Normal);
    vec4 finalColor = vec4(ADSColor.rgb,1.0);
    gl_FragColor = finalColor;
}