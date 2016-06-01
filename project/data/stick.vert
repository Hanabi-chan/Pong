//
//uniform mat4 ModelViewMatrix;
//uniform mat3 NormalMatrix;
//uniform mat4 ViewMatrix;
//uniform mat4 ProjectionMatrix;
//
//attribute vec4 Position;
//attribute vec3 Normal;
//attribute vec4 TexCoord;
//
//varying vec4 colorVarying;
//
//uniform vec4 color;
//
//void main()
//{
//    vec4 pos = ModelViewMatrix * Position;  // vertex position in eye coordinates
//    vec3 normal = normalize(NormalMatrix * Normal.xyz);
//    
//    colorVarying = vec4(vec3(0.5) + normal * 0.5, 1.0);
//    //colorVarying = color;
//    
//    gl_Position = ProjectionMatrix * pos;
//}


uniform mediump mat4 ViewMatrix;
uniform mediump mat4 ModelViewMatrix;
uniform mediump mat4 ModelMatrix;
uniform mediump mat4 ProjectionMatrix;

uniform mediump mat3 NormalMatrix;

uniform mediump vec4 LightPos;
uniform mediump vec4 EyePos;

uniform lowp vec3 Ka;   // ambient material coefficient
uniform lowp vec3 Kd;   // diffuse material coefficient
uniform lowp vec3 Ks;   // specular material coefficient

uniform mediump float Ns;   // specular material exponent (shininess)
uniform mediump float Ni;   // optical density (1.0 means light does not bend while passing through object)
uniform mediump float transparency;
uniform mediump vec3 ambientColor;

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

