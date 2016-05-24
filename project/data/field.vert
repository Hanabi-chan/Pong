uniform mediump mat4 ViewMatrix;
uniform mediump mat4 ModelViewMatrix;
uniform mediump mat4 ModelMatrix;
uniform mediump mat4 ProjectionMatrix;

uniform mediump mat3 NormalMatrix;

uniform mediump float transparency;
uniform mediump vec3 ambientColor;

attribute vec4 Position;
attribute vec3 Normal;
attribute vec4 TexCoord;

varying lowp vec4 texCoordVarying;

varying mediump vec4 pos;
varying mediump vec3 normal;
varying mediump vec3 cameraVector;

void main()
{
    // normal in world space
    normal = normalize(NormalMatrix * Normal);
    
    mediump vec4 cameraPos = ProjectionMatrix * Position;
    
    pos = ModelMatrix * Position;

    cameraVector = normalize(cameraPos.xyz - pos.xyz);
    
    texCoordVarying = TexCoord;
    
    gl_Position = ProjectionMatrix * ViewMatrix * pos;
}