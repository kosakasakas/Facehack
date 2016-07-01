
#ifndef VTKINTERACTIONWIDGETS_EXPORT_H
#define VTKINTERACTIONWIDGETS_EXPORT_H

#ifdef VTKINTERACTIONWIDGETS_STATIC_DEFINE
#  define VTKINTERACTIONWIDGETS_EXPORT
#  define VTKINTERACTIONWIDGETS_NO_EXPORT
#else
#  ifndef VTKINTERACTIONWIDGETS_EXPORT
#    ifdef vtkInteractionWidgets_EXPORTS
        /* We are building this library */
#      define VTKINTERACTIONWIDGETS_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define VTKINTERACTIONWIDGETS_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef VTKINTERACTIONWIDGETS_NO_EXPORT
#    define VTKINTERACTIONWIDGETS_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef VTKINTERACTIONWIDGETS_DEPRECATED
#  define VTKINTERACTIONWIDGETS_DEPRECATED __attribute__ ((__deprecated__))
#  define VTKINTERACTIONWIDGETS_DEPRECATED_EXPORT VTKINTERACTIONWIDGETS_EXPORT __attribute__ ((__deprecated__))
#  define VTKINTERACTIONWIDGETS_DEPRECATED_NO_EXPORT VTKINTERACTIONWIDGETS_NO_EXPORT __attribute__ ((__deprecated__))
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define VTKINTERACTIONWIDGETS_NO_DEPRECATED
#endif

/* AutoInit dependencies.  */
#include "vtkFiltersHybridModule.h"
#include "vtkFiltersModelingModule.h"
#include "vtkImagingHybridModule.h"
#include "vtkInteractionStyleModule.h"
#include "vtkRenderingAnnotationModule.h"
#include "vtkRenderingFreeTypeModule.h"
#include "vtkRenderingVolumeModule.h"

#endif
