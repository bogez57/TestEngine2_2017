#pragma once

#define SPDLOG_DEBUG_ON
#include <SDL.h>
#include <SpdLog\spdlog.h>
#include <assert.h>
#include <boost/assert.hpp>
#include "ErrorContext.h"

#if (DEBUG)
	#define BGZ_ERRCTXT1(errorDescription) \
		Bgz::Err::ErrContext ec(errorDescription);
#else
	#define BGZ_ERRCTXT1(errorDescription) ((void)0)
#endif

#if(DEBUG)
	#define BGZ_ERRCTXT2(errorDescription, errorData) \
		Bgz::Err::ErrContext ec(errorDescription, errorData);
#else
	#define BGZ_ERRCTXT2(errorDescription) ((void)0)
#endif

#if (DEBUG)
	#define ERRASSERT(condition, msg, ...) \
		do { if (!(condition)) Bgz::Err::ErrorReport(#condition, __func__, __FILE__, __LINE__, msg, __VA_ARGS__); } while (0)

#elif (PROFILE)
	#define ERRASSERT(condition, msg, ...) ((void)0)
#endif

#if (DEBUG) || (PROFILE)
	#define RUNTIME_ASSERT \
		BOOST_ASSERT_MSG 

	#define COMPILETIME_ASSERT(expr, msg) \
		static_assert(expr, msg) 

//Turn off log messages when unit testing to console log (To prevent log messages from getting in the way of unit tests assert messages)
#elif (DEBUGUNIT) || (PROFILEUNIT)
	#define RUNTIME_ASSERT 
	#define ERRASSERT(condition, msg, ...)

#else
	#define NDEBUG
	#define RUNTIME_ASSERT 
	#define COMPILETIME_ASSERT(expr, msg)
	#define ERRASSERT(condition, msg, ...)
#endif

namespace Bgz
{
	namespace Err
	{
		template<typename... ArgTypes>
		inline auto ErrorReport(const char* condition, const char* functionName, const char* file, int32 lineNumber, const Blz::string& c_errMessage, ArgTypes... args) -> void
		{
			//Create console object with spdlog
			auto console = spdlog::stdout_color_mt("console");
			//Controls how message is displayed. Different formatters % display different things. https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
			console->set_pattern("  %l: %v");

			//Combinging both 'condition' message and 'c_errMessage' first to keep everything on one line in console
			Blz::string combinedErrMessages("Assertion failed: " + Blz::string(condition) + " \n         " + c_errMessage);

			ErrContext::LogContext();

			console->error(combinedErrMessages.c_str(), args...);
			console->info("In function {}() \n        {}: {}", functionName, file, lineNumber);
			SDL_Quit();
			exit(0);
		}
	}
}