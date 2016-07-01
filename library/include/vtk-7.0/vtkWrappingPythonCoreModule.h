
#ifndef VTKWRAPPINGPYTHONCORE_EXPORT_H
#define VTKWRAPPINGPYTHONCORE_EXPORT_H

#ifdef VTKWRAPPINGPYTHONCORE_STATIC_DEFINE
#  define VTKWRAPPINGPYTHONCORE_EXPORT
#  define VTKWRAPPINGPYTHONCORE_NO_EXPORT
#else
#  ifndef VTKWRAPPINGPYTHONCORE_EXPORT
#    ifdef vtkWrappingPythonCore_EXPORTS
        /* We are building this library */
#      define VTKWRAPPINGPYTHONCORE_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define VTKWRAPPINGPYTHONCORE_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef VTKWRAPPINGPYTHONCORE_NO_EXPORT
#    define VTKWRAPPINGPYTHONCORE_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef VTKWRAPPINGPYTHONCORE_DEPRECATED
#  define VTKWRAPPINGPYTHONCORE_DEPRECATED __attribute__ ((__deprecated__))
#  define VTKWRAPPINGPYTHONCORE_DEPRECATED_EXPORT VTKWRAPPINGPYTHONCORE_EXPORT __attribute__ ((__deprecated__))
#  define VTKWRAPPINGPYTHONCORE_DEPRECATED_NO_EXPORT VTKWRAPPINGPYTHONCORE_NO_EXPORT __attribute__ ((__deprecated__))
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define VTKWRAPPINGPYTHONCORE_NO_DEPRECATED
#endif



#endif
