/* DXUTLibExports.c -- DXUT library export types definitions  Entry point
2008-10-04 : Igor Pavlov : Public domain */
#pragma once

// Exclude rarely-used stuff from Windows headers
#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef STRICT
#define STRICT
#endif

#ifndef _WINDOWS_
#include <windows.h>
#endif

#if defined(_WINDOWS) || defined(_WIN32)
/* If building or using DXUTlib as a DLL, define DXUTLIB_DLL.
* This is not mandatory, but it offers a little performance increase.
*/

#if defined(LIB_EXPORT) || defined(EFFECTS11LIB_EXPORT)
#ifndef EFFECTS11LIB_IMPORT
#ifndef EFFECTS11LIB_EXPORT
#define EFFECTS11LIB_EXPORT 1
#endif
#endif
#endif

#if defined(LIB_IMPORT) || defined(EFFECTS11LIB_IMPORT)
#if defined(EFFECTS11LIB_EXPORT)
#error ("!!!You are bulding EFFECTS11 export and import simultaniously")
#else
#ifndef EFFECTS11LIB_IMPORT
#define EFFECTS11LIB_IMPORT 1
#endif
#endif
#endif

#if defined(LIB_STATIC) || defined(EFFECTS11LIB_STATIC)
#ifndef EFFECTS11LIB_DLL
#ifndef EFFECTS11LIB_STATIC 
#define EFFECTS11LIB_STATIC 1
#endif
#endif
#endif

#if (defined(_DLL) || defined(_USRDLL) || defined(_WINDLL) || defined(LIB_DYNAMIC) || defined(EFFECTS11LIB_DLL)) && !defined(_LIB) && !defined(EFFECTS11LIB_STATIC)
#ifndef EFFECTS11LIB_STATIC
#ifndef EFFECTS11LIB_DLL
#define EFFECTS11LIB_DLL 1
#endif
#endif
#endif

#if !defined(EFFECTS11LIB_DLL) && !defined(EFFECTS11LIB_STATIC)
#error ("!!!Your EFFECTS11 lib type static or dll aren't defined")
#endif

//#define extern_cplus

// Defined for Templates functions export
#define extern_cplusplus

#ifdef extern_cplus
#define _EXTERN_C_START extern "C" {
#define _EXTERN_C_END  }
#endif

#ifdef extern_cplusplus
#define _EXTERN_CPP_START extern "C++" {
#define _EXTERN_CPP_END  }
#endif

#define _EXTERN extern

#  ifdef EFFECTS11LIB_DLL
#    if defined(WIN32) && (!defined(__BORLANDC__) || (__BORLANDC__ >= 0x500))
#      if defined(EFFECTS11LIB_EXPORT) && !defined(EFFECTS11LIB_STATIC)
#        define EFFECTSAPI __declspec(dllexport)
#      elif defined(EFFECTS11LIB_IMPORT) && (defined(_DLL) || defined(_WINDLL) || defined(_USRDLL)) && !defined(EFFECTS11LIB_STATIC)
#        define EFFECTSAPI __declspec(dllimport)
#      else // EFFECTS11LIB_STATIC  _LIB
#        define EFFECTSAPI
#      endif
#    endif
#  else  // EFFECTS11LIB_STATIC
#      define EFFECTSAPI
#  endif  /* EFFECTS11LIB_DLL */

#else
#	define EFFECTSAPI
#endif // _WINDOWS



//#define DXSTDAPI                  EXTERN_C EFFECTSAPI HRESULT STDAPICALLTYPE
//#define DXSTDAPI_(type)           EXTERN_C EFFECTSAPI type STDAPICALLTYPE


