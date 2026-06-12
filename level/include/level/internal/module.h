#if !defined(LEVEL_API)
	#define LEVEL_API /* NOTHING */

	#if defined(WIN32) || defined(WIN64)
		#undef LEVEL_API
		#if defined(level_EXPORTS)
			#define LEVEL_API __declspec(dllexport)
		#else
			#define LEVEL_API __declspec(dllimport)
		#endif
	#endif // defined(WIN32) || defined(WIN64)

#endif // !defined(LEVEL_API)

