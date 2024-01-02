#ifndef XDYN_LIB_EXPORT_H
#define XDYN_LIB_EXPORT_H


#pragma once

#ifdef XDYN_LIB_SHARED_BUILD
#ifdef _WIN32
#ifdef XDYN_LIB_EXPORTS
#define XDYN_LIB_API __declspec(dllexport)
#else
#define XDYN_LIB_API __declspec(dllimport)
#endif  // XDYN_LIB_EXPORTS
#else
#define XDYN_LIB_API
#endif  // _WIN32
#else
#define XDYN_LIB_API
#endif  // XDYN_LIB_SHARED_BUILD

#endif // XDYN_LIB_EXPORT_H
