#ifdef MPCAPIDLL_EXPORTS
#define MPC_API_DECL __declspec(dllexport)
#else
#define MPC_API_DECL __declspec(dllimport)
#endif

#define MPC_API extern "C" MPC_API_DECL
