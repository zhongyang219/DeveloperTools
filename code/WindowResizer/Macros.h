#pragma once

#ifdef WINDOWRISZER_EXPORTS
#define WINDOWRISZER_API __declspec(dllexport)
#else
#define WINDOWRISZER_API __declspec(dllimport)
#endif