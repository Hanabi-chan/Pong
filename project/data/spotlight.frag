varying mediump vec3 normal, lightDir, eyeVec;

uniform lowp vec3 Ka;   // ambient material coefficient
uniform lowp vec3 Kd;   // diffuse material coefficient

uniform lowp vec3 Ia;   // ambient light intensity
uniform lowp vec3 Ip;   // diffuse light intensity

const mediump float cos_outer_cone_angle = 0.8; // 36 degrees

lowp const float c1 = 0.0;
lowp const float c2 = 0.0;
lowp const float c3 = 1.0;

lowp const float dL = 5.0; // NOT SURE

void main (void)
{
    
    //I = Ia*Ka + fatt*Ip*Kd*(N.L)
    
    //not sure
    mediump fatt = min(1/(c1+c2*dL+c3*pow(dL,2)),1);
    
    mediump I = Ia * Ka + fatt * Ip * Kd * dot(N,L);
    
    
    ///////////////////////////
    
    mediump vec4 final_color =
    (gl_FrontLightModelProduct.sceneColor * gl_FrontMaterial.ambient) +
    (gl_LightSource[0].ambient * gl_FrontMaterial.ambient);
    
    mediump vec3 L = normalize(lightDir);
    mediump vec3 D = normalize(gl_LightSource[0].spotDirection);
    
    mediump float cos_cur_angle = dot(-L, D);
    
    mediump float cos_inner_cone_angle = gl_LightSource[0].spotCosCutoff;
    
    mediump float cos_inner_minus_outer_angle =
    cos_inner_cone_angle - cos_outer_cone_angle;
    
    //****************************************************
    // Don't need dynamic branching at all, precompute
    // falloff(i will call it spot)
    mediump float spot = 0.0;
    spot = clamp((cos_cur_angle - cos_outer_cone_angle) /
                 cos_inner_minus_outer_angle, 0.0, 1.0);
    //****************************************************
    
    mediump vec3 N = normalize(normal);
    
    mediump float lambertTerm = max( dot(N,L), 0.0);
    if(lambertTerm > 0.0)
    {
        final_color += gl_LightSource[0].diffuse *
        gl_FrontMaterial.diffuse *
        lambertTerm * spot;
        
        mediump vec3 E = normalize(eyeVec);
        mediump vec3 R = reflect(-L, N);
        
        mediump float specular = pow( max(dot(R, E), 0.0),
                             gl_FrontMaterial.shininess );
        
        final_color += gl_LightSource[0].specular *
        gl_FrontMaterial.specular *
        specular * spot;
    }
    gl_FragColor = final_color;
}
