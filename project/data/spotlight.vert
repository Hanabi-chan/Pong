
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

uniform mediump vec4 LightPos;
uniform mediump vec4 EyePos;

attribute vec4 Position;
attribute vec3 Normal;
attribute vec4 TexCoord;

varying vec3 normal, lightDir, eyeVec;

void main()
{
    vec4 pos = ModelViewMatrix * Position;  // vertex position in eye coordinates
    normal = normalize(NormalMatrix * Normal.xyz);
    lightDir = normalize(LightPos - pos).xyz;
    eyeVec = normalize(EyePos - pos).xyz;
    
    gl_Position = ProjectionMatrix * pos;
}
