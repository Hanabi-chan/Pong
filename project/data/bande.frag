
uniform mediump mat4 ViewMatrix;
uniform mediump mat4 ModelViewMatrix;
uniform mediump mat4 ModelMatrix;
uniform mediump mat4 ProjectionMatrix;

uniform mediump mat3 NormalMatrix;

uniform mediump vec4 LightPos;
uniform mediump vec4 EyePos;

uniform sampler2D DiffuseMap;
uniform sampler2D NormalMap;
uniform sampler2D SphereMap;

uniform samplerCube skyboxDiffuse;

varying lowp vec4 texCoordVarying;

varying mediump vec4 posVarying;       // pos in world space
varying mediump vec3 normalVarying;    // normal in world space

varying mediump vec4 pos;
varying mediump vec3 normal;
varying mediump vec3 cameraVector;
varying mediump vec3 tangentVarying;

void main()
{
    /*TBN calculation*/
    
    mediump vec3 t = normalize(vec3(NormalMatrix * tangentVarying));
    mediump vec3 n1 = normalize(vec3(NormalMatrix * normal));
    mediump vec3 b = normalize(vec3(NormalMatrix * cross(t, n1)));
    
    mediump vec3 tOrthogonalized = t-dot(n1,t)*n1;
    mediump vec3 bOrthogonalized = b-dot(n1,b)*n1 - dot(tOrthogonalized,b)*tOrthogonalized;
    mediump mat3 tbn = mat3(tOrthogonalized, bOrthogonalized, n1);
    
    /*IBL*/
    
    //read normal from NormalMap
    mediump vec3 N = normalize(tbn *  texture2D(NormalMap, texCoordVarying.st).rgb * 2.0 - 1.0);
    
    mediump vec3 difLighting = textureCube(skyboxDiffuse, N).rgb;
    
    //read color texture from DiffuseMap
    lowp vec4 color = texture2D(DiffuseMap, vec2(texCoordVarying));

    mediump vec4 iblColor;
    iblColor.xyz = difLighting * 1.5;
    iblColor.a = 1.0;
    
    gl_FragColor = color * iblColor;
}