
#define ST(x) #x
#define STR(x) ST(x)

static const char *g_ShaderCode =
"uniform float fGlobalTime;"
"void main()"
"{"
	"float PI = 3.14159265;"
	"float x = gl_FragCoord.x / " STR(WIDTH) ";"
	"float y = gl_FragCoord.y / " STR(HEIGHT) ";"
	"float v = sin((x*10.+fGlobalTime));"
	"v+=sin(10.*(x*sin(fGlobalTime/2.)+y*cos(fGlobalTime/3.))+fGlobalTime);"
	"float cx = x + 0.5*sin(fGlobalTime/5.);"
	"float cy = y +.5*cos(fGlobalTime/3.);"
	"v+=sin(sqrt(100.*(cx*cx+cy*cy)+1.)+fGlobalTime);"
	"vec3 col = vec3(1, sin(PI*v), cos(PI*v));"
	"gl_FragColor = vec4(col, 1);"
"}";
