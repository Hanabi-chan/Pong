
uniform highp vec3 cameraPos;
uniform samplerCube skybox;

varying highp vec3 normal;
varying highp vec3 position;

void main()
{
    highp vec3 I = normalize(position - cameraPos);
    highp vec4 cubeMap = textureCube(skybox, I);
    
    gl_FragColor = cubeMap;
}