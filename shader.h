
#define ST(x) #x
#define STR(x) ST(x)

static const char *g_ShaderCode =
"uniform float t;"
"void main()"
"{"
"vec2 d = gl_FragCoord / vec2(" STR(WIDTH) ", " STR(HEIGHT) ");"
"float v = sin((d.x*10. + t*.5));"
"v += sin(10.*(d.x*sin(t / 4.) + d.y*cos(t / 5.)));"
"vec2 c = vec2(d.x + 0.5*sin(t / 5.), d.y + .5*cos(t / 3.));"
"v += sin(sqrt(100.*(c.x*c.x + c.y*c.y) + 1.));"
"gl_FragColor = vec4(cos(v*1.5)*0.8, sin(3.*v + 3.), sin(3.*v), 1);"
"}";
