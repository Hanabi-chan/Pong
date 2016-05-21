uniform mediump mat4 ViewMatrix;
uniform mediump mat4 ModelMatrix;
uniform mediump mat4 ProjectionMatrix;
uniform mediump mat3 NormalMatrix;

attribute vec4 Position;
attribute vec3 Normal;

varying mediump vec3 normal;
varying mediump vec3 position;
varying mediump vec4 cameraPos;

void main()
{
    normal = NormalMatrix * Normal;
    position = vec3(ModelMatrix * Position);
    cameraPos = ProjectionMatrix * Position;
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(position, 1.0);
    
}