
#ifndef VTKRENDERINGFREETYPEFONTCONFIG_EXPORT_H
#define VTKRENDERINGFREETYPEFONTCONFIG_EXPORT_H

#ifdef VTKRENDERINGFREETYPEFONTCONFIG_STATIC_DEFINE
#  define VTKRENDERINGFREETYPEFONTCONFIG_EXPORT
#  define VTKRENDERINGFREETYPEFONTCONFIG_NO_EXPORT
#else
#  ifndef VTKRENDERINGFREETYPEFONTCONFIG_EXPORT
#    ifdef vtkRenderingFreeTypeFontConfig_EXPORTS
        /* We are building this library */
#      define VTKRENDERINGFREETYPEFONTCONFIG_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define VTKRENDERINGFREETYPEFONTCONFIG_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef VTKRENDERINGFREETYPEFONTCONFIG_NO_EXPORT
#    define VTKRENDERINGFREETYPEFONTCONFIG_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef VTKRENDERINGFREETYPEFONTCONFIG_DEPRECATED
#  define VTKRENDERINGFREETYPEFONTCONFIG_DEPRECATED __attribute__ ((__deprecated__))
#  define VTKRENDERINGFREETYPEFONTCONFIG_DEPRECATED_EXPORT VTKRENDERINGFREETYPEFONTCONFIG_EXPORT __attribute__ ((__deprecated__))
#  define VTKRENDERINGFREETYPEFONTCONFIG_DEPRECATED_NO_EXPORT VTKRENDERINGFREETYPEFONTCONFIG_NO_EXPORT __attribute__ ((__deprecated__))
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define VTKRENDERINGFREETYPEFONTCONFIG_NO_DEPRECATED
#endif

/* AutoInit dependencies.  */
#include "vtkRenderingFreeTypeModule.h"

/* AutoInit implementations.  */
#if defined(vtkRenderingFreeTypeFontConfig_INCLUDE)
# include vtkRenderingFreeTypeFontConfig_INCLUDE
#endif
#if defined(vtkRenderingFreeTypeFontConfig_AUTOINIT)
# include "vtkAutoInit.h"
VTK_AUTOINIT(vtkRenderingFreeTypeFontConfig)
#endif

#endif
