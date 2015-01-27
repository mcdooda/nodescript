#ifndef DEBUG_H
#define DEBUG_H

#ifdef NODESCRIPT_DEBUG

#undef NDEBUG

#include <cstdio>
#include <cassert>
#include <csignal>


#define NODESCRIPT_BREAK() \
	raise(SIGTRAP)

#define NODESCRIPT_ASSERT_MSG(cond, format, ...) \
	if (!(cond)) { fprintf(stderr, "assertion failed: " #cond " ; " format "\n", ##__VA_ARGS__); NODESCRIPT_BREAK(); }

#define NODESCRIPT_ASSERT(cond) \
	if (!(cond)) { std::cerr << "assertion failed: " #cond "\n" << std::endl; NODESCRIPT_BREAK(); }


#else // NODESCRIPT_DEBUG

#ifndef NDEBUG
#define NDEBUG
#endif

#define NODESCRIPT_BREAK() \
	{}

#define NODESCRIPT_ASSERT_MSG(cond, format, ...) \
	{}

#define NODESCRIPT_ASSERT(cond) \
	{}


#endif // NODESCRIPT_DEBUG


#define NODESCRIPT_CT_PRINT(X) namespace { template <int S> struct CtPrint; CtPrint<X> ctprint; }


#endif // DEBUG_H



