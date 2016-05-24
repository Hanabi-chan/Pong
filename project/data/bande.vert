
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

void main()
{
    
    // normal in world space
    normal = normalize(NormalMatrix * Normal);
    
    mediump vec4 cameraPos = ProjectionMatrix * Position;
    
    pos = ModelMatrix * Position;
    
    cameraVector = normalize(cameraPos.xyz - pos.xyz);
    
    texCoordVarying = TexCoord;
    
    gl_Position = ProjectionMatrix * ViewMatrix * pos;

//    pos = ModelMatrix * Position;
//    normal = normalize(NormalMatrix * Normal);
//    mediump vec4 cameraPos = ProjectionMatrix * Position;
//    texCoordVarying = TexCoord;
//    
//    ambientVarying = vec4(ambientColor, 1.0); //vec4(Ka * Ia, 1.0);
//    
//    //calculate diffuse lighting
//    diffuseVarying = vec4(0.0);
//    mediump vec3 n = normal ;
//    mediump vec3 l = normalize(LightPos - pos).xyz;
//    
//    lowp float intensity = dot(n,l);
//    lowp vec3 diffuse = Kd * clamp(intensity, 0.0, 1.0) * Id;
//    diffuseVarying = vec4(clamp(diffuse, 0.0, 1.0), 1.0);
//    
//    //If vertex is lit, calculate specular term in view space using the Blinn-Phong model
//    specularVarying = vec4(0.0);
//    if (intensity > 0.0)
//    {
//        mediump vec3 eyeVec = normalize(EyePos - pos).xyz;
//        mediump vec3 h = normalize(l+ eyeVec);
//        
//        mediump vec3 specular = Ks * pow(dot(n,h),Ns) * Is;
//        specularVarying = vec4(clamp(specular, 0.0, 1.0), 1.0);
//    }
//    
//    gl_Position = ProjectionMatrix * ViewMatrix * pos;
}