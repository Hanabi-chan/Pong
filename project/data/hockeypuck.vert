uniform mediump mat4 ViewMatrix;
uniform mediump mat4 ModelMatrix;
uniform mediump mat4 ProjectionMatrix;

uniform mediump mat3 NormalMatrix;

uniform mediump vec4 LightPos;
uniform mediump vec4 EyePos;

uniform lowp vec3 Ka;   // ambient material coefficient
uniform lowp vec3 Kd;   // diffuse material coefficient
uniform lowp vec3 Ks;   // specular material coefficient

uniform mediump float Ns;   // specular material exponent (shininess)

uniform lowp vec3 Ia;   // ambient light intensity
uniform lowp vec3 Id;   // diffuse light intensity
uniform lowp vec3 Is;   // specular light intensity

attribute vec4 Position;
attribute vec3 Normal;
attribute vec3 Tangent;
attribute vec3 Bitangent;
attribute vec4 TexCoord;

varying lowp vec4 ambientVarying;
varying lowp vec4 diffuseVarying;
varying lowp vec4 specularVarying;
varying lowp vec4 texCoordVarying;

varying mediump vec4 posVarying;       // pos in world space
varying mediump vec3 normalVarying;    // normal in world space

varying mediump vec4 pos;
varying mediump vec3 normal;
varying mediump vec3 cameraVector;
varying mediump vec3 tangentVarying;

void main()
{
    // normal in world space
    normal = normalize(NormalMatrix * Normal);
    
    mediump vec4 cameraPos = ProjectionMatrix * Position;
    
    pos = ModelMatrix * Position;
    
    tangentVarying = Tangent;
    
    cameraVector = normalize(cameraPos.xyz - pos.xyz);
    
    texCoordVarying = TexCoord;
    
    gl_Position = ProjectionMatrix * ViewMatrix * pos;
}
