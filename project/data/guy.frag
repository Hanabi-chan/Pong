
uniform mediump mat4 ViewMatrix;
uniform mediump mat4 ModelMatrix;
uniform mediump mat4 ProjectionMatrix;

uniform mediump mat3 NormalMatrix;

uniform mediump vec4 LightPos;
uniform mediump vec4 EyePos;

uniform lowp vec3 Ka;   // ambient material coefficient
uniform lowp vec3 Kd;   // diffuse material coefficient
uniform lowp vec3 Ks;   // specular material coefficient

uniform mediump float Ns;   // specular material exponent (shininess)

uniform lowp vec3 Ia;   // ambient light intensity
uniform lowp vec3 Id;   // diffuse light intensity
uniform lowp vec3 Is;   // specular light intensity

uniform sampler2D DiffuseMap;
uniform sampler2D SpecularMap;
uniform sampler2D NormalMap;

varying lowp vec4 ambientVarying;
varying lowp vec4 diffuseVarying;
varying lowp vec4 specularVarying;
varying lowp vec4 texCoordVarying;

varying mediump vec4 posVarying;        // pos in world space
varying mediump vec3 normalVarying;     // normal in world space
varying mediump vec3 tangentVarying;    // tangent in world space


void main()
{
    lowp vec4 ambientResult = vec4(Ka * Ia, 1.0);
    
    mediump vec4 pos = posVarying;
    mediump vec3 n = normalize(normalVarying);
    
    //calculate tbn matrix, ensure it's orthonormal
    
    //    mediump vec3 t = tangentVarying;
    //    mediump vec3 b = cross(normalVarying,t);
    //    mediump mat3 tbn = mat3(t,b,normalVarying);
    
    mediump vec3 t = normalize(vec3(ModelMatrix * vec4(tangentVarying,0.0)));
    mediump vec3 n1 = normalize(vec3(ModelMatrix * vec4(normalVarying,0.0)));
    mediump vec3 b = normalize(vec3(ModelMatrix * vec4(cross(t, n1),0.0)));
    
    mediump vec3 tOrthogonalized = t-dot(n1,t)*n1;
    mediump vec3 bOrthogonalized = b-dot(n1,b)*n1 - dot(tOrthogonalized,b)*tOrthogonalized;
    mediump mat3 tbn = mat3(tOrthogonalized, bOrthogonalized, n1);
    
    //read and correctly transform normals from normal map, then use them for lighting
    
    mediump vec3 l = normalize(LightPos - pos).xyz;
    
    lowp float intensity = dot(n, l);
    lowp vec3 diffuse = Kd * clamp(intensity, 0.0, 1.0) * Id;
    lowp vec4 diffuseResult = vec4(clamp(diffuse, 0.0, 1.0), 1.0);
    
    // If vertex is lit, calculate specular term in view space using the Blinn-Phong model
    lowp vec4 specularResult = vec4(0.0);
    if (intensity > 0.0)
    {
        mediump vec3 eyeVec = normalize(EyePos.xyz - pos.xyz);
        mediump vec3 h = normalize(l + eyeVec);
        
        mediump float specIntensity = pow(max(dot(h,n), 0.0), Ns);
        mediump vec3 specular = Ks * clamp(specIntensity, 0.0, 1.0) * Is;
        specularResult = vec4(clamp(specular, 0.0, 1.0), 1.0);
    }
    
    mediump vec4 specularTex = texture2D( SpecularMap, texCoordVarying.st );
    mediump vec3 pixelNormal = tbn * normalize((texture2D( NormalMap, texCoordVarying.st ).xyz * 2.0 - 1.0) * texture2D(DiffuseMap, texCoordVarying.st).xyz);
    lowp vec4 color = normalize(texture2D(DiffuseMap, texCoordVarying.st));

    gl_FragColor = diffuseResult * color + ambientResult * vec4(vec3(0.5) + pixelNormal * 0.5, 1.0) + specularResult*specularTex;
    
}
