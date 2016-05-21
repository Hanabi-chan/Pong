uniform mediump vec3 cameraPos;
uniform samplerCube skybox;

varying mediump vec3 normal;
varying mediump vec3 position;

void main()
{
    mediump vec3 I = normalize(position - cameraPos);
    mediump vec3 R = reflect(I, normalize(normal));
    mediump vec4 color = textureCube(skybox, R);
    
    gl_FragColor = vec4(1,1,0,1);
}