
varying lowp vec4 colorVarying;

uniform lowp vec3 Ka;

void main()
{
    lowp vec4 Ca = vec4(Ka, 1.0);
    gl_FragColor = colorVarying * Ca;
}
