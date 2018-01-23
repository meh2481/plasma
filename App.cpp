#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <GL/gl.h>
#include "glext.h"

#ifdef _DEBUG
#include <stdio.h>
#endif

#define TINY
//#define FULL


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

#ifdef FULL
static DEVMODE g_ScreenSettings =
{
	{ 0 }, 0, 0, sizeof(DEVMODE), 0, DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL, { 0 }, 0, 0, 0, 0, 0,
	{ 0 }, 0, 32, WIDTH, HEIGHT, { 0 }, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
#endif


#ifdef _DEBUG

bool g_Keys[256] = { false };

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			PostQuitMessage(0);
			return 0;
		}
		g_Keys[wParam & 0xFF] = true;
		break;
	case WM_KEYUP:
		g_Keys[wParam & 0xFF] = false;
		break;
	case WM_CLOSE:
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	};

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
// Take away prolog and epilog, then put a minial prolog back manually with assembly below. The function never returns so no epilog is necessary.
__declspec(naked) void winmain()
#endif
{
#ifdef _DEBUG
	WNDCLASS wc;
	memset(&wc, 0, sizeof(wc));
	wc.style         = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = WndProc;
	wc.hInstance     = hPrevInstance;
	wc.lpszClassName = "GL";

	if (!RegisterClass(&wc))
		return 0;

	DWORD style = WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	RECT rec = { 0, 0, WIDTH, HEIGHT };
	AdjustWindowRect( &rec, style, 0 );

	int w = rec.right - rec.left;
	int h = rec.bottom - rec.top;
	int x = (GetSystemMetrics(SM_CXSCREEN) - w) / 2;
	int y = (GetSystemMetrics(SM_CYSCREEN) - h) / 2;

	// Create window and initialize OpenGL
	HWND hwnd = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE, "GL", "Humus", style, x, y, w, h, 0, 0, hPrevInstance, 0);
#else
	// Prolog
	__asm enter 0x10, 0;
	__asm pushad;


	{ // Extra scope to make compiler accept the __decalspec(naked) with local variables

#ifdef FULL
		// Set fullscreen mode
		ChangeDisplaySettings(&g_ScreenSettings, CDS_FULLSCREEN);
#endif

		// Create window and initialize OpenGL
		HWND hwnd = CreateWindow("edit", 0, WS_POPUP | WS_VISIBLE | WS_MAXIMIZE, 0, 0, 0, 0, 0, 0, 0, 0);
#endif

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

		for (;;)
		{
			glTexCoord3f(0, 0, 1);

			glRects(-1, -1, 1, 1);
			SwapBuffers(hdc);

			if (GetAsyncKeyState(VK_ESCAPE))
				break;
		}
#ifndef _DEBUG
	}
#endif

	ExitProcess(0);
}
