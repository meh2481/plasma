#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <GL/gl.h>
#include "glext.h"

#define WIDTH       1920
#define HEIGHT      1080
#define HALF_HEIGHT 540
#define RATIO       16./9

#include "shader.h"

const static PIXELFORMATDESCRIPTOR pfd =
{
	0, 0, PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};


// Take away prolog and epilog, then put a minial prolog back manually with assembly below. The function never returns so no epilog is necessary.
__declspec(naked) void winmain()
{
	// Prolog
	__asm enter 0x10, 0;
	__asm pushad;

	{ // Extra scope to make compiler accept the __decalspec(naked) with local variables
		
		// Create window and initialize OpenGL
		HWND hwnd = CreateWindow("edit", 0, WS_POPUP | WS_VISIBLE | WS_MAXIMIZE, 0, 0, 0, 0, 0, 0, 0, 0);

		HDC hdc = GetDC(hwnd);

		SetPixelFormat(hdc, ChoosePixelFormat(hdc, &pfd), &pfd);
		wglMakeCurrent(hdc, wglCreateContext(hdc));

		ShowCursor(FALSE);

		// Create shader
		GLuint prog = ((PFNGLCREATEPROGRAMPROC) wglGetProcAddress("glCreateProgram"))();
		GLuint shader = ((PFNGLCREATESHADERPROC) wglGetProcAddress("glCreateShader"))(GL_FRAGMENT_SHADER);
		((PFNGLSHADERSOURCEPROC) wglGetProcAddress("glShaderSource"))(shader, 1, &g_ShaderCode, 0);
		((PFNGLCOMPILESHADERPROC) wglGetProcAddress("glCompileShader"))(shader);
		((PFNGLATTACHSHADERPROC) wglGetProcAddress("glAttachShader"))(prog, shader);
		((PFNGLLINKPROGRAMPROC) wglGetProcAddress("glLinkProgram"))(prog);
		((PFNGLUSEPROGRAMPROC) wglGetProcAddress("glUseProgram"))(prog);

		GLint t = ((PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation"))(prog, "t");
		float fTicks = 0.0f;
		DWORD lastTicks = GetTickCount();

		for (;;)
		{
			DWORD ticks = GetTickCount();
			float diff = ticks - lastTicks;
			fTicks += diff / 1000.0f;
			if(t != -1)
				((PFNGLUNIFORM1FPROC)wglGetProcAddress("glUniform1f"))(t, fTicks);

			glRects(-1, -1, 1, 1);
			SwapBuffers(hdc);

			if (GetAsyncKeyState(VK_ESCAPE))
				break;

			lastTicks = ticks;

			static MSG dummy;
			PeekMessageA(&dummy, 0, 0, 0, 1); // Remove all Windows messages to prevent "Program not responding" dialog.
		}
	}

	ExitProcess(0);
}
