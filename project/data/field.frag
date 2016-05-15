
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
uniform mediump float Ni;   // optical density (1.0 means light does not bend while passing through object)
uniform mediump float transparency;
uniform mediump vec3 ambientColor;

uniform lowp vec3 Ia;   // ambient light intensity
uniform lowp vec3 Id;   // diffuse light intensity
uniform lowp vec3 Is;   // specular light intensity

uniform sampler2D DiffuseMap;

varying lowp vec4 ambientVarying;
varying lowp vec4 diffuseVarying;
varying lowp vec4 specularVarying;
varying lowp vec4 texCoordVarying;

varying mediump vec4 posVarying;       // pos in world space
varying mediump vec3 normalVarying;    // normal in world space

varying mediump vec4 pos;
varying mediump vec3 normal;

void main()
{
    
    mediump vec3 n = normal ;
    mediump vec3 l = normalize(LightPos - pos).xyz;
    
    mediump vec3 Ca = Ka * Ia;
    mediump vec3 Cd = Kd * max(0.0,dot(n,l)) * Id;
    
    mediump vec3 Cs = vec3(0.0);
    if (dot(n,l) > 0.0)
    {
        mediump vec3 v = normalize(EyePos - pos).xyz;
        mediump vec3 r = normalize(l + v);
        
        Cs = Ks * pow(dot(n,r),Ni) * Is;
    }
    
    //read color from DiffuseMap
    lowp vec4 color = texture2D(DiffuseMap, vec2(texCoordVarying));
    
    gl_FragColor = (vec4(clamp(Cd, 0.0, 1.0), 1.0) + vec4(Ca, 1.0)) * color + vec4(clamp(Cs, 0.0, 1.0), transparency);
}