
uniform mediump vec3 cameraPos;
uniform samplerCube skybox;

varying mediump vec3 normal;
varying mediump vec3 position;
varying mediump vec3 cameraVector;

void main()
{
    mediump vec3 N = normalize(normal);
    
    mediump vec3 R1 = normalize(reflect(normalize(-cameraVector), normal));
    
    mediump vec3 difLighting = textureCube(skybox, R1).rgb;
    
    mediump vec4 resultingColor;
    resultingColor.xyz = difLighting * 0.6;
    resultingColor.a = 1.0;
    
    //    mediump vec3 I = normalize(position - cameraPos);
    //    mediump vec3 R = reflect(I, normalize(normal));
    //    mediump vec4 cubeMap = textureCube(skybox, R);
    
    //    gl_FragColor = cubeMap;
    gl_FragColor = resultingColor;
}

//
//// data from vertex shader
//varying vec3 normal;
//varying vec3 cameraPos;
//
//// lighting cube textures
////uniform samplerCube u_ambientTexture;
//uniform samplerCube u_diffuseTexture;
////uniform samplerCube u_specularTexture;
//
//// different lighting components
//const float ambientIntensity = 0.4;
//const float diffuseIntensity = 0.6;
//const float specularIntensity = 0.5;
//
//void main(void)
//{
//    mediump vec3 N = normalize(normal);
//    // view vector reflected with respect to normal
//    mediump vec3 R = normalize(reflect(normalize(-cameraPos),normal));
//    
//    // get colors and intensities for lighting components
//    
//    //vec3 ambLighting = textureCube(u_ambientTexture, N).rgb;
//    mediump vec3 difLighting = textureCube(u_diffuseTexture, N).rgb;
//    //vec3 speLighting = textureCube(u_specularTexture, R).rgb;
//    
//    // combine lighting components
//    mediump vec4 resultingColor;
//    resultingColor.xyz = /*ambLighting **/ 0.4 + difLighting /** 0.6 + speLighting * 0.4*/;
//    resultingColor.a = 1;
//    
//    gl_FragColor resultingColor;
//}