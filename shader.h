
#define ST(x) #x
#define STR(x) ST(x)

static const char *g_ShaderCode =
/*"void main()"
"{"
	"vec4 x=(gl_FragCoord/" STR(HALF_HEIGHT) "-vec4(" STR(RATIO) ",1,1,1))*gl_TexCoord[0].z+gl_TexCoord[0],y=x,z=y;"

	"int w=256;"

	"while(--w>0&&y.x<5)"
	"{"
		"y=x*x;"
		"x.y*=x.x*2;"
		"x.x=y.x-y.y;"
		"x+=z;"
		"y.x+=y.y;"
	"}"

	"gl_FragColor=sin(w/18.+log(y.x)/28+vec4(2,3.5,5,5))/2+.5;"
"}";*/
"	void main()"
"{"
"	vec2 uv = gl_FragCoord.xy / vec2(" STR(WIDTH) "," STR(HEIGHT) ").xy;"
"gl_FragColor = vec4(uv, 0.5 + 0.5*sin(5), 1.0);"
"}";
