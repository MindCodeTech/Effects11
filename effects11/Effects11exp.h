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

#define NAMESPACE_DirectX namespace DirectX {
#define NAMESPACE_DirectX_END }

#define NAMESPACE_D3DX11Effects namespace D3DX11Effects {
#define NAMESPACE_D3DX11Effects_END }

#define NAMESPACE_D3DX11Core namespace D3DX11Core {
#define NAMESPACE_D3DX11Core_END }

#define NAMESPACE_D3DX11Debug namespace D3DX11Debug {
#define NAMESPACE_D3DX11Debug_END }

//#define extern_cplus

// Defined for Templates functions export
#define extern_cplusplus

#ifndef EXTERN_C_BEGIN
#ifdef __cplusplus
#ifdef extern_cplus
#define EXTERN_C_BEGIN extern "C" {
#define EXTERN_C_END }
#endif
#ifdef extern_cplusplus
#define EXTERN_C_BEGIN extern "C++" {
#define EXTERN_C_END }
#endif
#else
#define EXTERN_C_BEGIN
#define EXTERN_C_END
#endif
#endif

#ifdef __cplusplus
EXTERN_C_BEGIN
#endif

#ifdef __cplusplus
EXTERN_C_END
#endif

#ifdef __cplusplus
#if defined(extern_cplus)
#define _EXTERN extern "C"
#elif defined(extern_cplusplus)
#define _EXTERN extern "C++"
#endif
#else
#define _EXTERN extern
#endif

#ifdef __cplusplus
#if defined(extern_cplus)
#define EFFECTSEXTERN extern "C"
#elif defined(extern_cplusplus)
#define EFFECTSEXTERN extern "C++"
#endif
#else
#define EFFECTSEXTERN extern
#endif

#if defined(_WINDOWS) || defined(_WIN32)
/* If building or using DXUTlib as a DLL, define DXUTLIB_DLL.
* This is not mandatory, but it offers a little performance increase.
*/

#if defined(LIB_EXPORTS) || defined(EFFECTS11LIB_EXPORTS)
#ifndef EFFECTS11LIB_IMPORTS
#ifndef EFFECTS11LIB_EXPORTS
#define EFFECTS11LIB_EXPORTS 1
#endif
#endif
#endif

#if defined(LIB_IMPORTS) || defined(EFFECTS11LIB_IMPORTS)
#if defined(EFFECTS11LIB_EXPORTS)
#error ("!!!You are bulding EFFECTS11 export and import simultaniously")
#else
#ifndef EFFECTS11LIB_IMPORTS
#define EFFECTS11LIB_IMPORTS 1
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

#  ifdef EFFECTS11LIB_DLL
#    if defined(WIN32) && (!defined(__BORLANDC__) || (__BORLANDC__ >= 0x500))
#      if defined(EFFECTS11LIB_EXPORTS) && !defined(EFFECTS11LIB_STATIC)
#        define EFFECTSAPI __declspec(dllexport)
#      elif defined(EFFECTS11LIB_IMPORTS) && (defined(_DLL) || defined(_WINDLL) || defined(_USRDLL)) && !defined(EFFECTS11LIB_STATIC)
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
