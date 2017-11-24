#include <SDL.h>
#include <GL/glew.h>
#include "GLLogging.h"
#include "Window.h"


namespace Blz::Graphics
{
	auto GLAPIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam) -> void;

	Window::Window(uint16 width, uint16 height) :
		width(width),
		height(height)
	{
		if (SDL_Init(SDL_INIT_VIDEO) != 0)
		{
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL Error", "SDL did not initialize", nullptr);
			SDL_Quit();
		}

		/** All attributes need to be set before window creation **/

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		//Make opengl deprecated functionality unavailable 
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_PROFILE_CORE);
		//In openGL 4.3 a debug context was added for easier debugging. This debug context will automatically print debugging
		//info to console (using a callback func) upon errors. This context can also be queried for versions lower than 4.3
		//using extensions. This SDL function queries these extensions to help guarantee a debug context will be used

		#if (DEBUG) 
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
		#endif

		p_window = SDL_CreateWindow("Shadow Gods", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this->width, this->height, SDL_WINDOW_OPENGL);
		if (!p_window)
		{
			CONSOLE("ERROR: SDL window failed to initialize! SDL error: %s\n", SDL_GetError());
			SDL_Quit();
		}

		glContext = SDL_GL_CreateContext(p_window);


		//Turns on or off v-sync
		SDL_GL_SetSwapInterval(0);

		//Log current computer opengl version and capabilities
		Graphics::RestartGLLogFile();
		Graphics::LogToFile("OpenGL Version %s\n\n", glGetString(GL_VERSION));
		CONSOLE("OpenGL Version %s\n\n", glGetString(GL_VERSION));
		Graphics::LogCurrentGPUCapabilites();

		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
			CONSOLE("ERROR: GLEW failed to initialize!");

			SDL_GL_DeleteContext(glContext);
			SDL_DestroyWindow(p_window);
			SDL_Quit();
		}

		GLint flags;
		glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
		//Checks if opengl Debug context is present by checking the openGL debug context flag. If present then will enable 
		//DEBUG_OUTPUT and pass in callback function
		if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
		{
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(glDebugOutput, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		}

		//Sets the values that the depth and color buffers will be set to when glClear is called
		glClearDepth(1.0);
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	}

	Window::~Window()
	{
		//In case window::Shutdown() function never gets called
		SDL_GL_DeleteContext(glContext);
		SDL_DestroyWindow(p_window);
		SDL_Quit();
	}

	auto Window::ClearBuffers() -> void
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	auto Window::SwapBuffers() -> void
	{
		SDL_GL_SwapWindow(p_window);
	}

	auto GLAPIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam) -> void
	{
		// ignore non-significant error/warning codes
		if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

		CONSOLE("---------------\n");
		CONSOLE("OpenGL Debug message (%i): %s\n", id, message);

		switch (source)
		{
		case GL_DEBUG_SOURCE_API:             CONSOLE("Source: API"); break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   CONSOLE("Source: Window System"); break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: CONSOLE("Source: Shader Compiler"); break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:     CONSOLE("Source: Third Party"); break;
		case GL_DEBUG_SOURCE_APPLICATION:     CONSOLE("Source: Application"); break;
		case GL_DEBUG_SOURCE_OTHER:           CONSOLE("Source: Other"); break;
		}CONSOLE("\n");

		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR:               CONSOLE("Type: Error"); break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: CONSOLE("Type: Deprecated Behaviour"); break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  CONSOLE("Type: Undefined Behaviour"); break;
		case GL_DEBUG_TYPE_PORTABILITY:         CONSOLE("Type: Portability"); break;
		case GL_DEBUG_TYPE_PERFORMANCE:         CONSOLE("Type: Performance"); break;
		case GL_DEBUG_TYPE_MARKER:              CONSOLE("Type: Marker"); break;
		case GL_DEBUG_TYPE_PUSH_GROUP:          CONSOLE("Type: Push Group"); break;
		case GL_DEBUG_TYPE_POP_GROUP:           CONSOLE("Type: Pop Group"); break;
		case GL_DEBUG_TYPE_OTHER:               CONSOLE("Type: Other"); break;
		} CONSOLE("\n");

		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:         CONSOLE("Severity: high"); break;
		case GL_DEBUG_SEVERITY_MEDIUM:       CONSOLE("Severity: medium"); break;
		case GL_DEBUG_SEVERITY_LOW:          CONSOLE("Severity: low"); break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: CONSOLE("Severity: notification"); break;
		}CONSOLE("\n\n");
	}
}
