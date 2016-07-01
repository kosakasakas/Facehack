
#ifndef VTKFILTERSPYTHON_EXPORT_H
#define VTKFILTERSPYTHON_EXPORT_H

#ifdef VTKFILTERSPYTHON_STATIC_DEFINE
#  define VTKFILTERSPYTHON_EXPORT
#  define VTKFILTERSPYTHON_NO_EXPORT
#else
#  ifndef VTKFILTERSPYTHON_EXPORT
#    ifdef vtkFiltersPython_EXPORTS
        /* We are building this library */
#      define VTKFILTERSPYTHON_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define VTKFILTERSPYTHON_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef VTKFILTERSPYTHON_NO_EXPORT
#    define VTKFILTERSPYTHON_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef VTKFILTERSPYTHON_DEPRECATED
#  define VTKFILTERSPYTHON_DEPRECATED __attribute__ ((__deprecated__))
#  define VTKFILTERSPYTHON_DEPRECATED_EXPORT VTKFILTERSPYTHON_EXPORT __attribute__ ((__deprecated__))
#  define VTKFILTERSPYTHON_DEPRECATED_NO_EXPORT VTKFILTERSPYTHON_NO_EXPORT __attribute__ ((__deprecated__))
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define VTKFILTERSPYTHON_NO_DEPRECATED
#endif



#endif
