
varying lowp vec4 colorVarying;

uniform lowp vec3 Ka;

void main()
{
    gl_FragColor = colorVarying;// * Ca;
}
