
uniform highp vec3 cameraPos;
uniform samplerCube skybox;

varying highp vec3 normal;
varying highp vec3 position;

void main()
{
    highp vec3 I = normalize(position - cameraPos);
    highp vec3 R = reflect(I, normalize(normal));
    highp vec4 cubeMap = textureCube(skybox, R);
    
    gl_FragColor = cubeMap;
}


//
//uniform mediump mat4 ViewMatrix;
//uniform mediump mat4 ModelMatrix;
//uniform mediump mat4 ProjectionMatrix;
//
//uniform mediump mat3 NormalMatrix;
//
//uniform mediump vec4 LightPos;
//uniform mediump vec4 EyePos;
//
//uniform lowp vec3 Ka;   // ambient material coefficient
//uniform lowp vec3 Kd;   // diffuse material coefficient
//uniform lowp vec3 Ks;   // specular material coefficient
//
//uniform mediump float Ns;   // specular material exponent (shininess)
//
//uniform lowp vec3 Ia;   // ambient light intensity
//uniform lowp vec3 Id;   // diffuse light intensity
//uniform lowp vec3 Is;   // specular light intensity
//
//uniform sampler2D DiffuseMap;
////uniform sampler2D NormalMap;
//
//varying lowp vec4 ambientVarying;
//varying lowp vec4 diffuseVarying;
//varying lowp vec4 specularVarying;
//varying lowp vec4 texCoordVarying;
//
//varying mediump vec4 posVarying;       // pos in world space
//varying mediump vec3 normalVarying;    // normal in world space
//
//varying mediump vec4 pos;
//varying mediump vec3 normal;
//
//void main()
//{
//    
//    mediump vec3 n = normal ;
//    //mediump vec3 n = normalize(texture2D(NormalMap, texCoordVarying.st).rgb * 2.0 - 1.0);
//    mediump vec3 l = normalize(LightPos - pos).xyz;
//    
//    mediump vec3 Ca = Ka * Ia;
//    mediump vec3 Cd = Kd * max(0.0,dot(n,l)) * Id;
//    
//    mediump vec3 Cs = vec3(0.0);
//    if (dot(n,l) > 0.0)
//    {
//        mediump vec3 v = normalize(EyePos - pos).xyz;
//        mediump vec3 r = normalize(l + v);
//        
//        Cs = Ks * pow(dot(n,r),Ns) * Is;
//    }
//    
//    //read color from DiffuseMap
//    mediump vec4 color = texture2D(DiffuseMap, texCoordVarying.st);
//    
//    gl_FragColor = (vec4(clamp(Cd, 0.0, 1.0), 1.0) + vec4(Ca, 1.0)) * color + vec4(clamp(Cs, 0.0, 1.0), 1.0);
//    //gl_FragColor = color;
//    //gl_FragColor = vec4(1,0,0,1);
//}

//varying lowp vec4 colorVarying;
//
//void main()
//{
//    gl_FragColor = colorVarying;
//}
