
uniform mediump mat4 ViewMatrix;
uniform mediump mat4 ModelMatrix;
uniform mediump mat4 ProjectionMatrix;
uniform mediump mat3 NormalMatrix;

attribute vec4 TexCoord; //i
attribute vec4 Position; //i
attribute vec3 Normal; //i

varying mediump vec3 normal;
varying mediump vec3 position;
varying mediump vec3 cameraVector;

void main()
{
    mediump vec4 worldPosition = ModelMatrix * Position;
    // normal in world space
    normal = normalize(NormalMatrix * Normal);
    
    mediump vec4 cameraPos = ProjectionMatrix * Position;
    
    cameraVector = normalize(cameraPos.xyz - worldPosition.xyz);
    
    position = vec3(ModelMatrix * Position);
    
    //Attention//
    
    //gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(position, 1.0);
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * worldPosition;
    //gl_Position = ViewMatrix * worldPosition;
    
}

//
//// attributes
//attribute vec4 Position;
//attribute vec3 Normal;
//attribute vec4 TexCoord;
//
//// matrices
//uniform mediump mat4 ViewMatrix;
//uniform mediump mat4 ModelMatrix;
//uniform mediump mat4 ProjectionMatrix;
//uniform mediump mat3 NormalMatrix;
//
//// data for fragment shader
//varying vec3 normal;
//varying vec3 cameraPos;
//
//void main(void)
//{
//    //cameraPostion
//    mediump vec4 cameraPosition = ProjectionMatrix * Position;
//    // position in world space
//    vec4 worldPosition = ModelMatrix * vec4(Position, 1);
//    // normal in world space
//    normal = normalize(NormalMatrix * Normal);
//    // direction to camera
//    cameraPos = normalize(cameraPosition.xyz -
//                               worldPosition.xyz);
//    // screen space position of vertex
//    gl_Position = ViewMatrix * worldPosition;
//}
