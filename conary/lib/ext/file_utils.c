/*
 * Copyright (c) SAS Institute Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#define PY_SSIZE_T_CLEAN
#include "Python.h"
#ifndef Py_PYTHON_H
    #error Python headers needed to compile C extensions, please install development version of Python.
#else

#include <stddef.h> /* For offsetof */
#ifndef offsetof
#define offsetof(type, member) ( (size_t) & ((type*)0) -> member )
#endif

#if !defined(WIN32) && !defined(MS_WINDOWS)
  #ifndef __stdcall
    #define __stdcall
  #endif
  #ifndef __cdecl
    #define __cdecl
  #endif
  #ifndef __fastcall
    #define __fastcall
  #endif
#endif

#ifndef DL_IMPORT
  #define DL_IMPORT(t) t
#endif
#ifndef DL_EXPORT
  #define DL_EXPORT(t) t
#endif

#ifndef PY_LONG_LONG
  #define PY_LONG_LONG LONG_LONG
#endif

#if PY_VERSION_HEX < 0x02040000
  #define METH_COEXIST 0
  #define PyDict_CheckExact(op) (Py_TYPE(op) == &PyDict_Type)
  #define PyDict_Contains(d,o)   PySequence_Contains(d,o)
#endif

#if PY_VERSION_HEX < 0x02050000
  typedef int Py_ssize_t;
  #define PY_SSIZE_T_MAX INT_MAX
  #define PY_SSIZE_T_MIN INT_MIN
  #define PY_FORMAT_SIZE_T ""
  #define PyInt_FromSsize_t(z) PyInt_FromLong(z)
  #define PyInt_AsSsize_t(o)   PyInt_AsLong(o)
  #define PyNumber_Index(o)    PyNumber_Int(o)
  #define PyIndex_Check(o)     PyNumber_Check(o)
  #define PyErr_WarnEx(category, message, stacklevel) PyErr_Warn(category, message)
#endif

#if PY_VERSION_HEX < 0x02060000
  #define Py_REFCNT(ob) (((PyObject*)(ob))->ob_refcnt)
  #define Py_TYPE(ob)   (((PyObject*)(ob))->ob_type)
  #define Py_SIZE(ob)   (((PyVarObject*)(ob))->ob_size)
  #define PyVarObject_HEAD_INIT(type, size) \
          PyObject_HEAD_INIT(type) size,
  #define PyType_Modified(t)

  typedef struct {
     void *buf;
     PyObject *obj;
     Py_ssize_t len;
     Py_ssize_t itemsize;
     int readonly;
     int ndim;
     char *format;
     Py_ssize_t *shape;
     Py_ssize_t *strides;
     Py_ssize_t *suboffsets;
     void *internal;
  } Py_buffer;

  #define PyBUF_SIMPLE 0
  #define PyBUF_WRITABLE 0x0001
  #define PyBUF_FORMAT 0x0004
  #define PyBUF_ND 0x0008
  #define PyBUF_STRIDES (0x0010 | PyBUF_ND)
  #define PyBUF_C_CONTIGUOUS (0x0020 | PyBUF_STRIDES)
  #define PyBUF_F_CONTIGUOUS (0x0040 | PyBUF_STRIDES)
  #define PyBUF_ANY_CONTIGUOUS (0x0080 | PyBUF_STRIDES)
  #define PyBUF_INDIRECT (0x0100 | PyBUF_STRIDES)

#endif

#if PY_MAJOR_VERSION < 3
  #define __Pyx_BUILTIN_MODULE_NAME "__builtin__"
#else
  #define __Pyx_BUILTIN_MODULE_NAME "builtins"
#endif

#if PY_MAJOR_VERSION >= 3
  #define Py_TPFLAGS_CHECKTYPES 0
  #define Py_TPFLAGS_HAVE_INDEX 0
#endif

#if (PY_VERSION_HEX < 0x02060000) || (PY_MAJOR_VERSION >= 3)
  #define Py_TPFLAGS_HAVE_NEWBUFFER 0
#endif

#if PY_MAJOR_VERSION >= 3
  #define PyBaseString_Type            PyUnicode_Type
  #define PyStringObject               PyUnicodeObject
  #define PyString_Type                PyUnicode_Type
  #define PyString_Check               PyUnicode_Check
  #define PyString_CheckExact          PyUnicode_CheckExact
#endif

#if PY_VERSION_HEX < 0x02060000
  #define PyBytesObject                PyStringObject
  #define PyBytes_Type                 PyString_Type
  #define PyBytes_Check                PyString_Check
  #define PyBytes_CheckExact           PyString_CheckExact
  #define PyBytes_FromString           PyString_FromString
  #define PyBytes_FromStringAndSize    PyString_FromStringAndSize
  #define PyBytes_FromFormat           PyString_FromFormat
  #define PyBytes_DecodeEscape         PyString_DecodeEscape
  #define PyBytes_AsString             PyString_AsString
  #define PyBytes_AsStringAndSize      PyString_AsStringAndSize
  #define PyBytes_Size                 PyString_Size
  #define PyBytes_AS_STRING            PyString_AS_STRING
  #define PyBytes_GET_SIZE             PyString_GET_SIZE
  #define PyBytes_Repr                 PyString_Repr
  #define PyBytes_Concat               PyString_Concat
  #define PyBytes_ConcatAndDel         PyString_ConcatAndDel
#endif

#if PY_VERSION_HEX < 0x02060000
  #define PySet_Check(obj)             PyObject_TypeCheck(obj, &PySet_Type)
  #define PyFrozenSet_Check(obj)       PyObject_TypeCheck(obj, &PyFrozenSet_Type)
#endif
#ifndef PySet_CheckExact
  #define PySet_CheckExact(obj)        (Py_TYPE(obj) == &PySet_Type)
#endif

#define __Pyx_TypeCheck(obj, type) PyObject_TypeCheck(obj, (PyTypeObject *)type)

#if PY_MAJOR_VERSION >= 3
  #define PyIntObject                  PyLongObject
  #define PyInt_Type                   PyLong_Type
  #define PyInt_Check(op)              PyLong_Check(op)
  #define PyInt_CheckExact(op)         PyLong_CheckExact(op)
  #define PyInt_FromString             PyLong_FromString
  #define PyInt_FromUnicode            PyLong_FromUnicode
  #define PyInt_FromLong               PyLong_FromLong
  #define PyInt_FromSize_t             PyLong_FromSize_t
  #define PyInt_FromSsize_t            PyLong_FromSsize_t
  #define PyInt_AsLong                 PyLong_AsLong
  #define PyInt_AS_LONG                PyLong_AS_LONG
  #define PyInt_AsSsize_t              PyLong_AsSsize_t
  #define PyInt_AsUnsignedLongMask     PyLong_AsUnsignedLongMask
  #define PyInt_AsUnsignedLongLongMask PyLong_AsUnsignedLongLongMask
#endif

#if PY_MAJOR_VERSION >= 3
  #define PyBoolObject                 PyLongObject
#endif


#if PY_MAJOR_VERSION >= 3
  #define __Pyx_PyNumber_Divide(x,y)         PyNumber_TrueDivide(x,y)
  #define __Pyx_PyNumber_InPlaceDivide(x,y)  PyNumber_InPlaceTrueDivide(x,y)
#else
  #define __Pyx_PyNumber_Divide(x,y)         PyNumber_Divide(x,y)
  #define __Pyx_PyNumber_InPlaceDivide(x,y)  PyNumber_InPlaceDivide(x,y)
#endif

#if (PY_MAJOR_VERSION < 3) || (PY_VERSION_HEX >= 0x03010300)
  #define __Pyx_PySequence_GetSlice(obj, a, b) PySequence_GetSlice(obj, a, b)
  #define __Pyx_PySequence_SetSlice(obj, a, b, value) PySequence_SetSlice(obj, a, b, value)
  #define __Pyx_PySequence_DelSlice(obj, a, b) PySequence_DelSlice(obj, a, b)
#else
  #define __Pyx_PySequence_GetSlice(obj, a, b) (unlikely(!(obj)) ? \
        (PyErr_SetString(PyExc_SystemError, "null argument to internal routine"), (PyObject*)0) : \
        (likely((obj)->ob_type->tp_as_mapping) ? (PySequence_GetSlice(obj, a, b)) : \
            (PyErr_Format(PyExc_TypeError, "'%.200s' object is unsliceable", (obj)->ob_type->tp_name), (PyObject*)0)))
  #define __Pyx_PySequence_SetSlice(obj, a, b, value) (unlikely(!(obj)) ? \
        (PyErr_SetString(PyExc_SystemError, "null argument to internal routine"), -1) : \
        (likely((obj)->ob_type->tp_as_mapping) ? (PySequence_SetSlice(obj, a, b, value)) : \
            (PyErr_Format(PyExc_TypeError, "'%.200s' object doesn't support slice assignment", (obj)->ob_type->tp_name), -1)))
  #define __Pyx_PySequence_DelSlice(obj, a, b) (unlikely(!(obj)) ? \
        (PyErr_SetString(PyExc_SystemError, "null argument to internal routine"), -1) : \
        (likely((obj)->ob_type->tp_as_mapping) ? (PySequence_DelSlice(obj, a, b)) : \
            (PyErr_Format(PyExc_TypeError, "'%.200s' object doesn't support slice deletion", (obj)->ob_type->tp_name), -1)))
#endif

#if PY_MAJOR_VERSION >= 3
  #define PyMethod_New(func, self, klass) ((self) ? PyMethod_New(func, self) : PyInstanceMethod_New(func))
#endif

#if PY_VERSION_HEX < 0x02050000
  #define __Pyx_GetAttrString(o,n)   PyObject_GetAttrString((o),((char *)(n)))
  #define __Pyx_SetAttrString(o,n,a) PyObject_SetAttrString((o),((char *)(n)),(a))
  #define __Pyx_DelAttrString(o,n)   PyObject_DelAttrString((o),((char *)(n)))
#else
  #define __Pyx_GetAttrString(o,n)   PyObject_GetAttrString((o),(n))
  #define __Pyx_SetAttrString(o,n,a) PyObject_SetAttrString((o),(n),(a))
  #define __Pyx_DelAttrString(o,n)   PyObject_DelAttrString((o),(n))
#endif

#if PY_VERSION_HEX < 0x02050000
  #define __Pyx_NAMESTR(n) ((char *)(n))
  #define __Pyx_DOCSTR(n)  ((char *)(n))
#else
  #define __Pyx_NAMESTR(n) (n)
  #define __Pyx_DOCSTR(n)  (n)
#endif

#ifdef __cplusplus
#define __PYX_EXTERN_C extern "C"
#else
#define __PYX_EXTERN_C extern
#endif

#if defined(WIN32) || defined(MS_WINDOWS)
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#define __PYX_HAVE_API__conary__lib__ext__file_utils
#include "sys/types.h"
#include "errno.h"
#include "stdio.h"
#include "stdlib.h"
#include "alloca.h"
#include "unistd.h"
#include "poll.h"

#ifdef PYREX_WITHOUT_ASSERTIONS
#define CYTHON_WITHOUT_ASSERTIONS
#endif


/* inline attribute */
#ifndef CYTHON_INLINE
  #if defined(__GNUC__)
    #define CYTHON_INLINE __inline__
  #elif defined(_MSC_VER)
    #define CYTHON_INLINE __inline
  #elif defined (__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
    #define CYTHON_INLINE inline
  #else
    #define CYTHON_INLINE
  #endif
#endif

/* unused attribute */
#ifndef CYTHON_UNUSED
# if defined(__GNUC__)
#   if !(defined(__cplusplus)) || (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4))
#     define CYTHON_UNUSED __attribute__ ((__unused__))
#   else
#     define CYTHON_UNUSED
#   endif
# elif defined(__ICC) || defined(__INTEL_COMPILER)
#   define CYTHON_UNUSED __attribute__ ((__unused__))
# else
#   define CYTHON_UNUSED
# endif
#endif

typedef struct {PyObject **p; char *s; const long n; const char* encoding; const char is_unicode; const char is_str; const char intern; } __Pyx_StringTabEntry; /*proto*/


/* Type Conversion Predeclarations */

#define __Pyx_PyBytes_FromUString(s) PyBytes_FromString((char*)s)
#define __Pyx_PyBytes_AsUString(s)   ((unsigned char*) PyBytes_AsString(s))

#define __Pyx_PyBool_FromLong(b) ((b) ? (Py_INCREF(Py_True), Py_True) : (Py_INCREF(Py_False), Py_False))
static CYTHON_INLINE int __Pyx_PyObject_IsTrue(PyObject*);
static CYTHON_INLINE PyObject* __Pyx_PyNumber_Int(PyObject* x);

static CYTHON_INLINE Py_ssize_t __Pyx_PyIndex_AsSsize_t(PyObject*);
static CYTHON_INLINE PyObject * __Pyx_PyInt_FromSize_t(size_t);
static CYTHON_INLINE size_t __Pyx_PyInt_AsSize_t(PyObject*);

#define __pyx_PyFloat_AsDouble(x) (PyFloat_CheckExact(x) ? PyFloat_AS_DOUBLE(x) : PyFloat_AsDouble(x))


#ifdef __GNUC__
/* Test for GCC > 2.95 */
#if __GNUC__ > 2 || (__GNUC__ == 2 && (__GNUC_MINOR__ > 95))
#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#else /* __GNUC__ > 2 ... */
#define likely(x)   (x)
#define unlikely(x) (x)
#endif /* __GNUC__ > 2 ... */
#else /* __GNUC__ */
#define likely(x)   (x)
#define unlikely(x) (x)
#endif /* __GNUC__ */
    
static PyObject *__pyx_m;
static PyObject *__pyx_b;
static PyObject *__pyx_empty_tuple;
static PyObject *__pyx_empty_bytes;
static int __pyx_lineno;
static int __pyx_clineno = 0;
static const char * __pyx_cfilenm= __FILE__;
static const char *__pyx_filename;


static const char *__pyx_f[] = {
  "file_utils.pyx",
};

/* Type declarations */

#ifndef CYTHON_REFNANNY
  #define CYTHON_REFNANNY 0
#endif

#if CYTHON_REFNANNY
  typedef struct {
    void (*INCREF)(void*, PyObject*, int);
    void (*DECREF)(void*, PyObject*, int);
    void (*GOTREF)(void*, PyObject*, int);
    void (*GIVEREF)(void*, PyObject*, int);
    void* (*SetupContext)(const char*, int, const char*);
    void (*FinishContext)(void**);
  } __Pyx_RefNannyAPIStruct;
  static __Pyx_RefNannyAPIStruct *__Pyx_RefNanny = NULL;
  static __Pyx_RefNannyAPIStruct * __Pyx_RefNannyImportAPI(const char *modname) {
    PyObject *m = NULL, *p = NULL;
    void *r = NULL;
    m = PyImport_ImportModule((char *)modname);
    if (!m) goto end;
    p = PyObject_GetAttrString(m, (char *)"RefNannyAPI");
    if (!p) goto end;
    r = PyLong_AsVoidPtr(p);
  end:
    Py_XDECREF(p);
    Py_XDECREF(m);
    return (__Pyx_RefNannyAPIStruct *)r;
  }
  #define __Pyx_RefNannySetupContext(name)           void *__pyx_refnanny = __Pyx_RefNanny->SetupContext((name), __LINE__, __FILE__)
  #define __Pyx_RefNannyFinishContext()           __Pyx_RefNanny->FinishContext(&__pyx_refnanny)
  #define __Pyx_INCREF(r) __Pyx_RefNanny->INCREF(__pyx_refnanny, (PyObject *)(r), __LINE__)
  #define __Pyx_DECREF(r) __Pyx_RefNanny->DECREF(__pyx_refnanny, (PyObject *)(r), __LINE__)
  #define __Pyx_GOTREF(r) __Pyx_RefNanny->GOTREF(__pyx_refnanny, (PyObject *)(r), __LINE__)
  #define __Pyx_GIVEREF(r) __Pyx_RefNanny->GIVEREF(__pyx_refnanny, (PyObject *)(r), __LINE__)
  #define __Pyx_XDECREF(r) do { if((r) != NULL) {__Pyx_DECREF(r);} } while(0)
#else
  #define __Pyx_RefNannySetupContext(name)
  #define __Pyx_RefNannyFinishContext()
  #define __Pyx_INCREF(r) Py_INCREF(r)
  #define __Pyx_DECREF(r) Py_DECREF(r)
  #define __Pyx_GOTREF(r)
  #define __Pyx_GIVEREF(r)
  #define __Pyx_XDECREF(r) Py_XDECREF(r)
#endif /* CYTHON_REFNANNY */
#define __Pyx_XGIVEREF(r) do { if((r) != NULL) {__Pyx_GIVEREF(r);} } while(0)
#define __Pyx_XGOTREF(r) do { if((r) != NULL) {__Pyx_GOTREF(r);} } while(0)

static PyObject *__Pyx_GetName(PyObject *dict, PyObject *name); /*proto*/

static void __Pyx_RaiseArgtupleInvalid(const char* func_name, int exact,
    Py_ssize_t num_min, Py_ssize_t num_max, Py_ssize_t num_found); /*proto*/

static void __Pyx_RaiseDoubleKeywordsError(
    const char* func_name, PyObject* kw_name); /*proto*/

static int __Pyx_ParseOptionalKeywords(PyObject *kwds, PyObject **argnames[],     PyObject *kwds2, PyObject *values[], Py_ssize_t num_pos_args,     const char* function_name); /*proto*/

static CYTHON_INLINE void __Pyx_ErrRestore(PyObject *type, PyObject *value, PyObject *tb); /*proto*/
static CYTHON_INLINE void __Pyx_ErrFetch(PyObject **type, PyObject **value, PyObject **tb); /*proto*/

static void __Pyx_Raise(PyObject *type, PyObject *value, PyObject *tb); /*proto*/

#ifndef __PYX_FORCE_INIT_THREADS
  #if PY_VERSION_HEX < 0x02040200
    #define __PYX_FORCE_INIT_THREADS 1
  #else
    #define __PYX_FORCE_INIT_THREADS 0
  #endif
#endif

static CYTHON_INLINE unsigned char __Pyx_PyInt_AsUnsignedChar(PyObject *);

static CYTHON_INLINE unsigned short __Pyx_PyInt_AsUnsignedShort(PyObject *);

static CYTHON_INLINE unsigned int __Pyx_PyInt_AsUnsignedInt(PyObject *);

static CYTHON_INLINE char __Pyx_PyInt_AsChar(PyObject *);

static CYTHON_INLINE short __Pyx_PyInt_AsShort(PyObject *);

static CYTHON_INLINE int __Pyx_PyInt_AsInt(PyObject *);

static CYTHON_INLINE signed char __Pyx_PyInt_AsSignedChar(PyObject *);

static CYTHON_INLINE signed short __Pyx_PyInt_AsSignedShort(PyObject *);

static CYTHON_INLINE signed int __Pyx_PyInt_AsSignedInt(PyObject *);

static CYTHON_INLINE int __Pyx_PyInt_AsLongDouble(PyObject *);

static CYTHON_INLINE unsigned long __Pyx_PyInt_AsUnsignedLong(PyObject *);

static CYTHON_INLINE unsigned PY_LONG_LONG __Pyx_PyInt_AsUnsignedLongLong(PyObject *);

static CYTHON_INLINE long __Pyx_PyInt_AsLong(PyObject *);

static CYTHON_INLINE PY_LONG_LONG __Pyx_PyInt_AsLongLong(PyObject *);

static CYTHON_INLINE signed long __Pyx_PyInt_AsSignedLong(PyObject *);

static CYTHON_INLINE signed PY_LONG_LONG __Pyx_PyInt_AsSignedLongLong(PyObject *);

static CYTHON_INLINE off_t __Pyx_PyInt_from_py_off_t(PyObject *);

static void __Pyx_AddTraceback(const char *funcname); /*proto*/

static int __Pyx_InitStrings(__Pyx_StringTabEntry *t); /*proto*/
/* Module declarations from conary.lib.ext.file_utils */

#define __Pyx_MODULE_NAME "conary.lib.ext.file_utils"
static int __pyx_module_is_main_conary__lib__ext__file_utils = 0;

/* Implementation of conary.lib.ext.file_utils */
static PyObject *__pyx_builtin_MemoryError;
static PyObject *__pyx_builtin_range;
static PyObject *__pyx_builtin_OSError;
static PyObject *__pyx_builtin_ValueError;
static char __pyx_k_1[] = "Exactly one of count and end must be zero.";
static char __pyx_k_3[] = "\nCollection of wrappers around file-related C functions.\n";
static char __pyx_k_4[] = "conary.lib.ext.file_utils";
static char __pyx_k_5[] = "countOpenFileDescriptors";
static char __pyx_k_6[] = "massCloseFileDescriptors";
static char __pyx_k__fd[] = "fd";
static char __pyx_k__end[] = "end";
static char __pyx_k__fobj[] = "fobj";
static char __pyx_k__mode[] = "mode";
static char __pyx_k__path[] = "path";
static char __pyx_k__count[] = "count";
static char __pyx_k__pread[] = "pread";
static char __pyx_k__range[] = "range";
static char __pyx_k__start[] = "start";
static char __pyx_k__events[] = "events";
static char __pyx_k__fchmod[] = "fchmod";
static char __pyx_k__offset[] = "offset";
static char __pyx_k__OSError[] = "OSError";
static char __pyx_k__lexists[] = "lexists";
static char __pyx_k__revents[] = "revents";
static char __pyx_k____main__[] = "__main__";
static char __pyx_k____test__[] = "__test__";
static char __pyx_k__ValueError[] = "ValueError";
static char __pyx_k__MemoryError[] = "MemoryError";
static char __pyx_k__fopenIfExists[] = "fopenIfExists";
static char __pyx_k__mkdirIfMissing[] = "mkdirIfMissing";
static char __pyx_k__removeIfExists[] = "removeIfExists";
static PyObject *__pyx_kp_s_1;
static PyObject *__pyx_n_s_4;
static PyObject *__pyx_n_s_5;
static PyObject *__pyx_n_s_6;
static PyObject *__pyx_n_s__MemoryError;
static PyObject *__pyx_n_s__OSError;
static PyObject *__pyx_n_s__ValueError;
static PyObject *__pyx_n_s____main__;
static PyObject *__pyx_n_s____test__;
static PyObject *__pyx_n_s__count;
static PyObject *__pyx_n_s__end;
static PyObject *__pyx_n_s__events;
static PyObject *__pyx_n_s__fchmod;
static PyObject *__pyx_n_s__fd;
static PyObject *__pyx_n_s__fobj;
static PyObject *__pyx_n_s__fopenIfExists;
static PyObject *__pyx_n_s__lexists;
static PyObject *__pyx_n_s__mkdirIfMissing;
static PyObject *__pyx_n_s__mode;
static PyObject *__pyx_n_s__offset;
static PyObject *__pyx_n_s__path;
static PyObject *__pyx_n_s__pread;
static PyObject *__pyx_n_s__range;
static PyObject *__pyx_n_s__removeIfExists;
static PyObject *__pyx_n_s__revents;
static PyObject *__pyx_n_s__start;
static PyObject *__pyx_k_tuple_2;

/* "conary/lib/ext/file_utils.pyx":43
 * 
 * 
 * def countOpenFileDescriptors():             # <<<<<<<<<<<<<<
 *     """Return a count of the number of open file descriptors."""
 *     cdef int maxfd, count, i, rc
 */

static PyObject *__pyx_pf_6conary_3lib_3ext_10file_utils_countOpenFileDescriptors(PyObject *__pyx_self, CYTHON_UNUSED PyObject *unused); /*proto*/
static char __pyx_doc_6conary_3lib_3ext_10file_utils_countOpenFileDescriptors[] = "Return a count of the number of open file descriptors.";
static PyMethodDef __pyx_mdef_6conary_3lib_3ext_10file_utils_countOpenFileDescriptors = {__Pyx_NAMESTR("countOpenFileDescriptors"), (PyCFunction)__pyx_pf_6conary_3lib_3ext_10file_utils_countOpenFileDescriptors, METH_NOARGS, __Pyx_DOCSTR(__pyx_doc_6conary_3lib_3ext_10file_utils_countOpenFileDescriptors)};
static PyObject *__pyx_pf_6conary_3lib_3ext_10file_utils_countOpenFileDescriptors(PyObject *__pyx_self, CYTHON_UNUSED PyObject *unused) {
  int __pyx_v_maxfd;
  int __pyx_v_count;
  int __pyx_v_i;
  int __pyx_v_rc;
  struct pollfd *__pyx_v_ufds;
  PyObject *__pyx_r = NULL;
  int __pyx_t_1;
  int __pyx_t_2;
  int __pyx_t_3;
  int __pyx_t_4;
  int __pyx_t_5;
  PyObject *__pyx_t_6 = NULL;
  __Pyx_RefNannySetupContext("countOpenFileDescriptors");
  __pyx_self = __pyx_self;

  /* "conary/lib/ext/file_utils.pyx":48
 *     cdef pollfd *ufds
 * 
 *     maxfd = getdtablesize()             # <<<<<<<<<<<<<<
 *     ufds = <pollfd*>malloc(maxfd * sizeof(pollfd))
 *     if ufds == NULL:
 */
  __pyx_v_maxfd = getdtablesize();

  /* "conary/lib/ext/file_utils.pyx":49
 * 
 *     maxfd = getdtablesize()
 *     ufds = <pollfd*>malloc(maxfd * sizeof(pollfd))             # <<<<<<<<<<<<<<
 *     if ufds == NULL:
 *         raise MemoryError
 */
  __pyx_v_ufds = ((struct pollfd *)malloc((__pyx_v_maxfd * (sizeof(struct pollfd)))));

  /* "conary/lib/ext/file_utils.pyx":50
 *     maxfd = getdtablesize()
 *     ufds = <pollfd*>malloc(maxfd * sizeof(pollfd))
 *     if ufds == NULL:             # <<<<<<<<<<<<<<
 *         raise MemoryError
 * 
 */
  __pyx_t_1 = (__pyx_v_ufds == NULL);
  if (__pyx_t_1) {

    /* "conary/lib/ext/file_utils.pyx":51
 *     ufds = <pollfd*>malloc(maxfd * sizeof(pollfd))
 *     if ufds == NULL:
 *         raise MemoryError             # <<<<<<<<<<<<<<
 * 
 *     with nogil:
 */
    PyErr_NoMemory(); {__pyx_filename = __pyx_f[0]; __pyx_lineno = 51; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    goto __pyx_L5;
  }
  __pyx_L5:;

  /* "conary/lib/ext/file_utils.pyx":53
 *         raise MemoryError
 * 
 *     with nogil:             # <<<<<<<<<<<<<<
 *         for i in range(maxfd):
 *             ufds[i].fd = i
 */
  {
    #ifdef WITH_THREAD
    PyThreadState *_save;
    #endif
    Py_UNBLOCK_THREADS
    /*try:*/ {

      /* "conary/lib/ext/file_utils.pyx":54
 * 
 *     with nogil:
 *         for i in range(maxfd):             # <<<<<<<<<<<<<<
 *             ufds[i].fd = i
 *             ufds[i].events = POLLIN | POLLPRI | POLLOUT
 */
      __pyx_t_2 = __pyx_v_maxfd;
      for (__pyx_t_3 = 0; __pyx_t_3 < __pyx_t_2; __pyx_t_3+=1) {
        __pyx_v_i = __pyx_t_3;

        /* "conary/lib/ext/file_utils.pyx":55
 *     with nogil:
 *         for i in range(maxfd):
 *             ufds[i].fd = i             # <<<<<<<<<<<<<<
 *             ufds[i].events = POLLIN | POLLPRI | POLLOUT
 * 
 */
        (__pyx_v_ufds[__pyx_v_i]).fd = __pyx_v_i;

        /* "conary/lib/ext/file_utils.pyx":56
 *         for i in range(maxfd):
 *             ufds[i].fd = i
 *             ufds[i].events = POLLIN | POLLPRI | POLLOUT             # <<<<<<<<<<<<<<
 * 
 *         # Loop until poll() succeeds without being interrupted by a signal
 */
        (__pyx_v_ufds[__pyx_v_i]).events = ((POLLIN | POLLPRI) | POLLOUT);
      }

      /* "conary/lib/ext/file_utils.pyx":59
 * 
 *         # Loop until poll() succeeds without being interrupted by a signal
 *         while True:             # <<<<<<<<<<<<<<
 *             rc = poll(ufds, maxfd, 0)
 *             if rc >= 0 or errno != EINTR:
 */
      while (1) {
        if (!1) break;

        /* "conary/lib/ext/file_utils.pyx":60
 *         # Loop until poll() succeeds without being interrupted by a signal
 *         while True:
 *             rc = poll(ufds, maxfd, 0)             # <<<<<<<<<<<<<<
 *             if rc >= 0 or errno != EINTR:
 *                 break
 */
        __pyx_v_rc = poll(__pyx_v_ufds, __pyx_v_maxfd, 0);

        /* "conary/lib/ext/file_utils.pyx":61
 *         while True:
 *             rc = poll(ufds, maxfd, 0)
 *             if rc >= 0 or errno != EINTR:             # <<<<<<<<<<<<<<
 *                 break
 * 
 */
        __pyx_t_1 = (__pyx_v_rc >= 0);
        if (!__pyx_t_1) {
          __pyx_t_4 = (errno != EINTR);
          __pyx_t_5 = __pyx_t_4;
        } else {
          __pyx_t_5 = __pyx_t_1;
        }
        if (__pyx_t_5) {

          /* "conary/lib/ext/file_utils.pyx":62
 *             rc = poll(ufds, maxfd, 0)
 *             if rc >= 0 or errno != EINTR:
 *                 break             # <<<<<<<<<<<<<<
 * 
 *     if rc < 0:
 */
          goto __pyx_L12_break;
          goto __pyx_L13;
        }
        __pyx_L13:;
      }
      __pyx_L12_break:;
    }

    /* "conary/lib/ext/file_utils.pyx":53
 *         raise MemoryError
 * 
 *     with nogil:             # <<<<<<<<<<<<<<
 *         for i in range(maxfd):
 *             ufds[i].fd = i
 */
    /*finally:*/ {
      Py_BLOCK_THREADS
    }
  }

  /* "conary/lib/ext/file_utils.pyx":64
 *                 break
 * 
 *     if rc < 0:             # <<<<<<<<<<<<<<
 *         free(ufds)
 *         PyErr_SetFromErrno(OSError)
 */
  __pyx_t_5 = (__pyx_v_rc < 0);
  if (__pyx_t_5) {

    /* "conary/lib/ext/file_utils.pyx":65
 * 
 *     if rc < 0:
 *         free(ufds)             # <<<<<<<<<<<<<<
 *         PyErr_SetFromErrno(OSError)
 * 
 */
    free(__pyx_v_ufds);

    /* "conary/lib/ext/file_utils.pyx":66
 *     if rc < 0:
 *         free(ufds)
 *         PyErr_SetFromErrno(OSError)             # <<<<<<<<<<<<<<
 * 
 *     count = 0
 */
    __pyx_t_6 = PyErr_SetFromErrno(__pyx_builtin_OSError); if (unlikely(!__pyx_t_6)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 66; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    __Pyx_GOTREF(__pyx_t_6);
    __Pyx_DECREF(__pyx_t_6); __pyx_t_6 = 0;
    goto __pyx_L14;
  }
  __pyx_L14:;

  /* "conary/lib/ext/file_utils.pyx":68
 *         PyErr_SetFromErrno(OSError)
 * 
 *     count = 0             # <<<<<<<<<<<<<<
 *     for i in range(maxfd):
 *         if ufds[i].revents != POLLNVAL:
 */
  __pyx_v_count = 0;

  /* "conary/lib/ext/file_utils.pyx":69
 * 
 *     count = 0
 *     for i in range(maxfd):             # <<<<<<<<<<<<<<
 *         if ufds[i].revents != POLLNVAL:
 *             count += 1
 */
  __pyx_t_2 = __pyx_v_maxfd;
  for (__pyx_t_3 = 0; __pyx_t_3 < __pyx_t_2; __pyx_t_3+=1) {
    __pyx_v_i = __pyx_t_3;

    /* "conary/lib/ext/file_utils.pyx":70
 *     count = 0
 *     for i in range(maxfd):
 *         if ufds[i].revents != POLLNVAL:             # <<<<<<<<<<<<<<
 *             count += 1
 * 
 */
    __pyx_t_5 = ((__pyx_v_ufds[__pyx_v_i]).revents != POLLNVAL);
    if (__pyx_t_5) {

      /* "conary/lib/ext/file_utils.pyx":71
 *     for i in range(maxfd):
 *         if ufds[i].revents != POLLNVAL:
 *             count += 1             # <<<<<<<<<<<<<<
 * 
 *     free(ufds)
 */
      __pyx_v_count = (__pyx_v_count + 1);
      goto __pyx_L17;
    }
    __pyx_L17:;
  }

  /* "conary/lib/ext/file_utils.pyx":73
 *             count += 1
 * 
 *     free(ufds)             # <<<<<<<<<<<<<<
 *     return count
 * 
 */
  free(__pyx_v_ufds);

  /* "conary/lib/ext/file_utils.pyx":74
 * 
 *     free(ufds)
 *     return count             # <<<<<<<<<<<<<<
 * 
 * 
 */
  __Pyx_XDECREF(__pyx_r);
  __pyx_t_6 = PyInt_FromLong(__pyx_v_count); if (unlikely(!__pyx_t_6)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 74; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_6);
  __pyx_r = __pyx_t_6;
  __pyx_t_6 = 0;
  goto __pyx_L0;

  __pyx_r = Py_None; __Pyx_INCREF(Py_None);
  goto __pyx_L0;
  __pyx_L1_error:;
  __Pyx_XDECREF(__pyx_t_6);
  __Pyx_AddTraceback("conary.lib.ext.file_utils.countOpenFileDescriptors");
  __pyx_r = NULL;
  __pyx_L0:;
  __Pyx_XGIVEREF(__pyx_r);
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

/* "conary/lib/ext/file_utils.pyx":77
 * 
 * 
 * def fchmod(fobj, int mode):             # <<<<<<<<<<<<<<
 *     """Change the permissions of an open file."""
 *     cdef int fd, rc
 */

static PyObject *__pyx_pf_6conary_3lib_3ext_10file_utils_1fchmod(PyObject *__pyx_self, PyObject *__pyx_args, PyObject *__pyx_kwds); /*proto*/
static char __pyx_doc_6conary_3lib_3ext_10file_utils_1fchmod[] = "Change the permissions of an open file.";
static PyMethodDef __pyx_mdef_6conary_3lib_3ext_10file_utils_1fchmod = {__Pyx_NAMESTR("fchmod"), (PyCFunction)__pyx_pf_6conary_3lib_3ext_10file_utils_1fchmod, METH_VARARGS|METH_KEYWORDS, __Pyx_DOCSTR(__pyx_doc_6conary_3lib_3ext_10file_utils_1fchmod)};
static PyObject *__pyx_pf_6conary_3lib_3ext_10file_utils_1fchmod(PyObject *__pyx_self, PyObject *__pyx_args, PyObject *__pyx_kwds) {
  PyObject *__pyx_v_fobj = 0;
  int __pyx_v_mode;
  int __pyx_v_fd;
  int __pyx_v_rc;
  PyObject *__pyx_r = NULL;
  int __pyx_t_1;
  int __pyx_t_2;
  PyObject *__pyx_t_3 = NULL;
  static PyObject **__pyx_pyargnames[] = {&__pyx_n_s__fobj,&__pyx_n_s__mode,0};
  __Pyx_RefNannySetupContext("fchmod");
  __pyx_self = __pyx_self;
  if (unlikely(__pyx_kwds)) {
    Py_ssize_t kw_args = PyDict_Size(__pyx_kwds);
    PyObject* values[2] = {0,0};
    switch (PyTuple_GET_SIZE(__pyx_args)) {
      case  2: values[1] = PyTuple_GET_ITEM(__pyx_args, 1);
      case  1: values[0] = PyTuple_GET_ITEM(__pyx_args, 0);
      case  0: break;
      default: goto __pyx_L5_argtuple_error;
    }
    switch (PyTuple_GET_SIZE(__pyx_args)) {
      case  0:
      values[0] = PyDict_GetItem(__pyx_kwds, __pyx_n_s__fobj);
      if (likely(values[0])) kw_args--;
      else goto __pyx_L5_argtuple_error;
      case  1:
      values[1] = PyDict_GetItem(__pyx_kwds, __pyx_n_s__mode);
      if (likely(values[1])) kw_args--;
      else {
        __Pyx_RaiseArgtupleInvalid("fchmod", 1, 2, 2, 1); {__pyx_filename = __pyx_f[0]; __pyx_lineno = 77; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
      }
    }
    if (unlikely(kw_args > 0)) {
      if (unlikely(__Pyx_ParseOptionalKeywords(__pyx_kwds, __pyx_pyargnames, 0, values, PyTuple_GET_SIZE(__pyx_args), "fchmod") < 0)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 77; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
    }
    __pyx_v_fobj = values[0];
    __pyx_v_mode = __Pyx_PyInt_AsInt(values[1]); if (unlikely((__pyx_v_mode == (int)-1) && PyErr_Occurred())) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 77; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
  } else if (PyTuple_GET_SIZE(__pyx_args) != 2) {
    goto __pyx_L5_argtuple_error;
  } else {
    __pyx_v_fobj = PyTuple_GET_ITEM(__pyx_args, 0);
    __pyx_v_mode = __Pyx_PyInt_AsInt(PyTuple_GET_ITEM(__pyx_args, 1)); if (unlikely((__pyx_v_mode == (int)-1) && PyErr_Occurred())) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 77; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
  }
  goto __pyx_L4_argument_unpacking_done;
  __pyx_L5_argtuple_error:;
  __Pyx_RaiseArgtupleInvalid("fchmod", 1, 2, 2, PyTuple_GET_SIZE(__pyx_args)); {__pyx_filename = __pyx_f[0]; __pyx_lineno = 77; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
  __pyx_L3_error:;
  __Pyx_AddTraceback("conary.lib.ext.file_utils.fchmod");
  __Pyx_RefNannyFinishContext();
  return NULL;
  __pyx_L4_argument_unpacking_done:;

  /* "conary/lib/ext/file_utils.pyx":80
 *     """Change the permissions of an open file."""
 *     cdef int fd, rc
 *     fd = PyObject_AsFileDescriptor(fobj)             # <<<<<<<<<<<<<<
 *     with nogil:
 *         rc = c_fchmod(fd, mode)
 */
  __pyx_t_1 = PyObject_AsFileDescriptor(__pyx_v_fobj); if (unlikely(__pyx_t_1 == -1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 80; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_v_fd = __pyx_t_1;

  /* "conary/lib/ext/file_utils.pyx":81
 *     cdef int fd, rc
 *     fd = PyObject_AsFileDescriptor(fobj)
 *     with nogil:             # <<<<<<<<<<<<<<
 *         rc = c_fchmod(fd, mode)
 *     if rc == -1:
 */
  {
    #ifdef WITH_THREAD
    PyThreadState *_save;
    #endif
    Py_UNBLOCK_THREADS
    /*try:*/ {

      /* "conary/lib/ext/file_utils.pyx":82
 *     fd = PyObject_AsFileDescriptor(fobj)
 *     with nogil:
 *         rc = c_fchmod(fd, mode)             # <<<<<<<<<<<<<<
 *     if rc == -1:
 *         PyErr_SetFromErrno(OSError)
 */
      __pyx_v_rc = fchmod(__pyx_v_fd, __pyx_v_mode);
    }

    /* "conary/lib/ext/file_utils.pyx":81
 *     cdef int fd, rc
 *     fd = PyObject_AsFileDescriptor(fobj)
 *     with nogil:             # <<<<<<<<<<<<<<
 *         rc = c_fchmod(fd, mode)
 *     if rc == -1:
 */
    /*finally:*/ {
      Py_BLOCK_THREADS
    }
  }

  /* "conary/lib/ext/file_utils.pyx":83
 *     with nogil:
 *         rc = c_fchmod(fd, mode)
 *     if rc == -1:             # <<<<<<<<<<<<<<
 *         PyErr_SetFromErrno(OSError)
 * 
 */
  __pyx_t_2 = (__pyx_v_rc == -1);
  if (__pyx_t_2) {

    /* "conary/lib/ext/file_utils.pyx":84
 *         rc = c_fchmod(fd, mode)
 *     if rc == -1:
 *         PyErr_SetFromErrno(OSError)             # <<<<<<<<<<<<<<
 * 
 * 
 */
    __pyx_t_3 = PyErr_SetFromErrno(__pyx_builtin_OSError); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 84; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    __Pyx_GOTREF(__pyx_t_3);
    __Pyx_DECREF(__pyx_t_3); __pyx_t_3 = 0;
    goto __pyx_L9;
  }
  __pyx_L9:;

  __pyx_r = Py_None; __Pyx_INCREF(Py_None);
  goto __pyx_L0;
  __pyx_L1_error:;
  __Pyx_XDECREF(__pyx_t_3);
  __Pyx_AddTraceback("conary.lib.ext.file_utils.fchmod");
  __pyx_r = NULL;
  __pyx_L0:;
  __Pyx_XGIVEREF(__pyx_r);
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

/* "conary/lib/ext/file_utils.pyx":87
 * 
 * 
 * def fopenIfExists(char *path, char *mode):             # <<<<<<<<<<<<<<
 *     """Open a file, or return C{None} if opening failed."""
 *     cdef FILE *fp
 */

static PyObject *__pyx_pf_6conary_3lib_3ext_10file_utils_2fopenIfExists(PyObject *__pyx_self, PyObject *__pyx_args, PyObject *__pyx_kwds); /*proto*/
static char __pyx_doc_6conary_3lib_3ext_10file_utils_2fopenIfExists[] = "Open a file, or return C{None} if opening failed.";
static PyMethodDef __pyx_mdef_6conary_3lib_3ext_10file_utils_2fopenIfExists = {__Pyx_NAMESTR("fopenIfExists"), (PyCFunction)__pyx_pf_6conary_3lib_3ext_10file_utils_2fopenIfExists, METH_VARARGS|METH_KEYWORDS, __Pyx_DOCSTR(__pyx_doc_6conary_3lib_3ext_10file_utils_2fopenIfExists)};
static PyObject *__pyx_pf_6conary_3lib_3ext_10file_utils_2fopenIfExists(PyObject *__pyx_self, PyObject *__pyx_args, PyObject *__pyx_kwds) {
  char *__pyx_v_path;
  char *__pyx_v_mode;
  FILE *__pyx_v_fp;
  PyObject *__pyx_r = NULL;
  int __pyx_t_1;
  PyObject *__pyx_t_2 = NULL;
  static PyObject **__pyx_pyargnames[] = {&__pyx_n_s__path,&__pyx_n_s__mode,0};
  __Pyx_RefNannySetupContext("fopenIfExists");
  __pyx_self = __pyx_self;
  if (unlikely(__pyx_kwds)) {
    Py_ssize_t kw_args = PyDict_Size(__pyx_kwds);
    PyObject* values[2] = {0,0};
    switch (PyTuple_GET_SIZE(__pyx_args)) {
      case  2: values[1] = PyTuple_GET_ITEM(__pyx_args, 1);
      case  1: values[0] = PyTuple_GET_ITEM(__pyx_args, 0);
      case  0: break;
      default: goto __pyx_L5_argtuple_error;
    }
    switch (PyTuple_GET_SIZE(__pyx_args)) {
      case  0:
      values[0] = PyDict_GetItem(__pyx_kwds, __pyx_n_s__path);
      if (likely(values[0])) kw_args--;
      else goto __pyx_L5_argtuple_error;
      case  1:
      values[1] = PyDict_GetItem(__pyx_kwds, __pyx_n_s__mode);
      if (likely(values[1])) kw_args--;
      else {
        __Pyx_RaiseArgtupleInvalid("fopenIfExists", 1, 2, 2, 1); {__pyx_filename = __pyx_f[0]; __pyx_lineno = 87; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
      }
    }
    if (unlikely(kw_args > 0)) {
      if (unlikely(__Pyx_ParseOptionalKeywords(__pyx_kwds, __pyx_pyargnames, 0, values, PyTuple_GET_SIZE(__pyx_args), "fopenIfExists") < 0)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 87; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
    }
    __pyx_v_path = PyBytes_AsString(values[0]); if (unlikely((!__pyx_v_path) && PyErr_Occurred())) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 87; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
    __pyx_v_mode = PyBytes_AsString(values[1]); if (unlikely((!__pyx_v_mode) && PyErr_Occurred())) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 87; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
  } else if (PyTuple_GET_SIZE(__pyx_args) != 2) {
    goto __pyx_L5_argtuple_error;
  } else {
    __pyx_v_path = PyBytes_AsString(PyTuple_GET_ITEM(__pyx_args, 0)); if (unlikely((!__pyx_v_path) && PyErr_Occurred())) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 87; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
    __pyx_v_mode = PyBytes_AsString(PyTuple_GET_ITEM(__pyx_args, 1)); if (unlikely((!__pyx_v_mode) && PyErr_Occurred())) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 87; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
  }
  goto __pyx_L4_argument_unpacking_done;
  __pyx_L5_argtuple_error:;
  __Pyx_RaiseArgtupleInvalid("fopenIfExists", 1, 2, 2, PyTuple_GET_SIZE(__pyx_args)); {__pyx_filename = __pyx_f[0]; __pyx_lineno = 87; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
  __pyx_L3_error:;
  __Pyx_AddTraceback("conary.lib.ext.file_utils.fopenIfExists");
  __Pyx_RefNannyFinishContext();
  return NULL;
  __pyx_L4_argument_unpacking_done:;

  /* "conary/lib/ext/file_utils.pyx":91
 *     cdef FILE *fp
 * 
 *     with nogil:             # <<<<<<<<<<<<<<
 *         fp = fopen(path, mode)
 * 
 */
  {
    #ifdef WITH_THREAD
    PyThreadState *_save;
    #endif
    Py_UNBLOCK_THREADS
    /*try:*/ {

      /* "conary/lib/ext/file_utils.pyx":92
 * 
 *     with nogil:
 *         fp = fopen(path, mode)             # <<<<<<<<<<<<<<
 * 
 *     if fp == NULL:
 */
      __pyx_v_fp = fopen(__pyx_v_path, __pyx_v_mode);
    }

    /* "conary/lib/ext/file_utils.pyx":91
 *     cdef FILE *fp
 * 
 *     with nogil:             # <<<<<<<<<<<<<<
 *         fp = fopen(path, mode)
 * 
 */
    /*finally:*/ {
      Py_BLOCK_THREADS
    }
  }

  /* "conary/lib/ext/file_utils.pyx":94
 *         fp = fopen(path, mode)
 * 
 *     if fp == NULL:             # <<<<<<<<<<<<<<
 *         return None
 *     else:
 */
  __pyx_t_1 = (__pyx_v_fp == NULL);
  if (__pyx_t_1) {

    /* "conary/lib/ext/file_utils.pyx":95
 * 
 *     if fp == NULL:
 *         return None             # <<<<<<<<<<<<<<
 *     else:
 *         return PyFile_FromFile(fp, path, mode, fclose)
 */
    __Pyx_XDECREF(__pyx_r);
    __Pyx_INCREF(Py_None);
    __pyx_r = Py_None;
    goto __pyx_L0;
    goto __pyx_L9;
  }
  /*else*/ {

    /* "conary/lib/ext/file_utils.pyx":97
 *         return None
 *     else:
 *         return PyFile_FromFile(fp, path, mode, fclose)             # <<<<<<<<<<<<<<
 * 
 * 
 */
    __Pyx_XDECREF(__pyx_r);
    __pyx_t_2 = PyFile_FromFile(__pyx_v_fp, __pyx_v_path, __pyx_v_mode, fclose); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 97; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    __Pyx_GOTREF(__pyx_t_2);
    __pyx_r = __pyx_t_2;
    __pyx_t_2 = 0;
    goto __pyx_L0;
  }
  __pyx_L9:;

  __pyx_r = Py_None; __Pyx_INCREF(Py_None);
  goto __pyx_L0;
  __pyx_L1_error:;
  __Pyx_XDECREF(__pyx_t_2);
  __Pyx_AddTraceback("conary.lib.ext.file_utils.fopenIfExists");
  __pyx_r = NULL;
  __pyx_L0:;
  __Pyx_XGIVEREF(__pyx_r);
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

/* "conary/lib/ext/file_utils.pyx":100
 * 
 * 
 * def lexists(char *path):             # <<<<<<<<<<<<<<
 *     """Return C{True} if C{path} exists."""
 *     cdef stat sb
 */

static PyObject *__pyx_pf_6conary_3lib_3ext_10file_utils_3lexists(PyObject *__pyx_self, PyObject *__pyx_arg_path); /*proto*/
static char __pyx_doc_6conary_3lib_3ext_10file_utils_3lexists[] = "Return C{True} if C{path} exists.";
static PyMethodDef __pyx_mdef_6conary_3lib_3ext_10file_utils_3lexists = {__Pyx_NAMESTR("lexists"), (PyCFunction)__pyx_pf_6conary_3lib_3ext_10file_utils_3lexists, METH_O, __Pyx_DOCSTR(__pyx_doc_6conary_3lib_3ext_10file_utils_3lexists)};
static PyObject *__pyx_pf_6conary_3lib_3ext_10file_utils_3lexists(PyObject *__pyx_self, PyObject *__pyx_arg_path) {
  char *__pyx_v_path;
  struct stat __pyx_v_sb;
  PyObject *__pyx_r = NULL;
  int __pyx_t_1;
  int __pyx_t_2;
  int __pyx_t_3;
  int __pyx_t_4;
  PyObject *__pyx_t_5 = NULL;
  __Pyx_RefNannySetupContext("lexists");
  __pyx_self = __pyx_self;
  assert(__pyx_arg_path); {
    __pyx_v_path = PyBytes_AsString(__pyx_arg_path); if (unlikely((!__pyx_v_path) && PyErr_Occurred())) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 100; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
  }
  goto __pyx_L4_argument_unpacking_done;
  __pyx_L3_error:;
  __Pyx_AddTraceback("conary.lib.ext.file_utils.lexists");
  __Pyx_RefNannyFinishContext();
  return NULL;
  __pyx_L4_argument_unpacking_done:;

  /* "conary/lib/ext/file_utils.pyx":103
 *     """Return C{True} if C{path} exists."""
 *     cdef stat sb
 *     if lstat(path, &sb) == -1:             # <<<<<<<<<<<<<<
 *         if errno in (ENOENT, ENOTDIR, ENAMETOOLONG, EACCES):
 *             return False
 */
  __pyx_t_1 = (lstat(__pyx_v_path, (&__pyx_v_sb)) == -1);
  if (__pyx_t_1) {

    /* "conary/lib/ext/file_utils.pyx":104
 *     cdef stat sb
 *     if lstat(path, &sb) == -1:
 *         if errno in (ENOENT, ENOTDIR, ENAMETOOLONG, EACCES):             # <<<<<<<<<<<<<<
 *             return False
 *         PyErr_SetFromErrnoWithFilename(OSError, path)
 */
    __pyx_t_2 = errno;
    __pyx_t_1 = (__pyx_t_2 == ENOENT);
    if (!__pyx_t_1) {
      __pyx_t_3 = (__pyx_t_2 == ENOTDIR);
      __pyx_t_4 = __pyx_t_3;
    } else {
      __pyx_t_4 = __pyx_t_1;
    }
    if (!__pyx_t_4) {
      __pyx_t_1 = (__pyx_t_2 == ENAMETOOLONG);
      __pyx_t_3 = __pyx_t_1;
    } else {
      __pyx_t_3 = __pyx_t_4;
    }
    if (!__pyx_t_3) {
      __pyx_t_4 = (__pyx_t_2 == EACCES);
      __pyx_t_1 = __pyx_t_4;
    } else {
      __pyx_t_1 = __pyx_t_3;
    }
    __pyx_t_3 = __pyx_t_1;
    if (__pyx_t_3) {

      /* "conary/lib/ext/file_utils.pyx":105
 *     if lstat(path, &sb) == -1:
 *         if errno in (ENOENT, ENOTDIR, ENAMETOOLONG, EACCES):
 *             return False             # <<<<<<<<<<<<<<
 *         PyErr_SetFromErrnoWithFilename(OSError, path)
 *     return True
 */
      __Pyx_XDECREF(__pyx_r);
      __pyx_t_5 = __Pyx_PyBool_FromLong(0); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 105; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_GOTREF(__pyx_t_5);
      __pyx_r = __pyx_t_5;
      __pyx_t_5 = 0;
      goto __pyx_L0;
      goto __pyx_L6;
    }
    __pyx_L6:;

    /* "conary/lib/ext/file_utils.pyx":106
 *         if errno in (ENOENT, ENOTDIR, ENAMETOOLONG, EACCES):
 *             return False
 *         PyErr_SetFromErrnoWithFilename(OSError, path)             # <<<<<<<<<<<<<<
 *     return True
 * 
 */
    __pyx_t_5 = PyErr_SetFromErrnoWithFilename(__pyx_builtin_OSError, __pyx_v_path); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 106; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    __Pyx_GOTREF(__pyx_t_5);
    __Pyx_DECREF(__pyx_t_5); __pyx_t_5 = 0;
    goto __pyx_L5;
  }
  __pyx_L5:;

  /* "conary/lib/ext/file_utils.pyx":107
 *             return False
 *         PyErr_SetFromErrnoWithFilename(OSError, path)
 *     return True             # <<<<<<<<<<<<<<
 * 
 * 
 */
  __Pyx_XDECREF(__pyx_r);
  __pyx_t_5 = __Pyx_PyBool_FromLong(1); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 107; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_5);
  __pyx_r = __pyx_t_5;
  __pyx_t_5 = 0;
  goto __pyx_L0;

  __pyx_r = Py_None; __Pyx_INCREF(Py_None);
  goto __pyx_L0;
  __pyx_L1_error:;
  __Pyx_XDECREF(__pyx_t_5);
  __Pyx_AddTraceback("conary.lib.ext.file_utils.lexists");
  __pyx_r = NULL;
  __pyx_L0:;
  __Pyx_XGIVEREF(__pyx_r);
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

/* "conary/lib/ext/file_utils.pyx":110
 * 
 * 
 * def massCloseFileDescriptors(int start, int count, int end):             # <<<<<<<<<<<<<<
 *     """Close file descriptors from C{start} to either C{end} or after C{count}
 *     unused descriptors have been encountered."""
 */

static PyObject *__pyx_pf_6conary_3lib_3ext_10file_utils_4massCloseFileDescriptors(PyObject *__pyx_self, PyObject *__pyx_args, PyObject *__pyx_kwds); /*proto*/
static char __pyx_doc_6conary_3lib_3ext_10file_utils_4massCloseFileDescriptors[] = "Close file descriptors from C{start} to either C{end} or after C{count}\n    unused descriptors have been encountered.";
static PyMethodDef __pyx_mdef_6conary_3lib_3ext_10file_utils_4massCloseFileDescriptors = {__Pyx_NAMESTR("massCloseFileDescriptors"), (PyCFunction)__pyx_pf_6conary_3lib_3ext_10file_utils_4massCloseFileDescriptors, METH_VARARGS|METH_KEYWORDS, __Pyx_DOCSTR(__pyx_doc_6conary_3lib_3ext_10file_utils_4massCloseFileDescriptors)};
static PyObject *__pyx_pf_6conary_3lib_3ext_10file_utils_4massCloseFileDescriptors(PyObject *__pyx_self, PyObject *__pyx_args, PyObject *__pyx_kwds) {
  int __pyx_v_start;
  int __pyx_v_count;
  int __pyx_v_end;
  int __pyx_v_i;
  int __pyx_v_j;
  int __pyx_v_rc;
  PyObject *__pyx_r = NULL;
  int __pyx_t_1;
  PyObject *__pyx_t_2 = NULL;
  static PyObject **__pyx_pyargnames[] = {&__pyx_n_s__start,&__pyx_n_s__count,&__pyx_n_s__end,0};
  __Pyx_RefNannySetupContext("massCloseFileDescriptors");
  __pyx_self = __pyx_self;
  if (unlikely(__pyx_kwds)) {
    Py_ssize_t kw_args = PyDict_Size(__pyx_kwds);
    PyObject* values[3] = {0,0,0};
    switch (PyTuple_GET_SIZE(__pyx_args)) {
      case  3: values[2] = PyTuple_GET_ITEM(__pyx_args, 2);
      case  2: values[1] = PyTuple_GET_ITEM(__pyx_args, 1);
      case  1: values[0] = PyTuple_GET_ITEM(__pyx_args, 0);
      case  0: break;
      default: goto __pyx_L5_argtuple_error;
    }
    switch (PyTuple_GET_SIZE(__pyx_args)) {
      case  0:
      values[0] = PyDict_GetItem(__pyx_kwds, __pyx_n_s__start);
      if (likely(values[0])) kw_args--;
      else goto __pyx_L5_argtuple_error;
      case  1:
      values[1] = PyDict_GetItem(__pyx_kwds, __pyx_n_s__count);
      if (likely(values[1])) kw_args--;
      else {
        __Pyx_RaiseArgtupleInvalid("massCloseFileDescriptors", 1, 3, 3, 1); {__pyx_filename = __pyx_f[0]; __pyx_lineno = 110; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
      }
      case  2:
      values[2] = PyDict_GetItem(__pyx_kwds, __pyx_n_s__end);
      if (likely(values[2])) kw_args--;
      else {
        __Pyx_RaiseArgtupleInvalid("massCloseFileDescriptors", 1, 3, 3, 2); {__pyx_filename = __pyx_f[0]; __pyx_lineno = 110; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
      }
    }
    if (unlikely(kw_args > 0)) {
      if (unlikely(__Pyx_ParseOptionalKeywords(__pyx_kwds, __pyx_pyargnames, 0, values, PyTuple_GET_SIZE(__pyx_args), "massCloseFileDescriptors") < 0)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 110; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
    }
    __pyx_v_start = __Pyx_PyInt_AsInt(values[0]); if (unlikely((__pyx_v_start == (int)-1) && PyErr_Occurred())) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 110; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
    __pyx_v_count = __Pyx_PyInt_AsInt(values[1]); if (unlikely((__pyx_v_count == (int)-1) && PyErr_Occurred())) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 110; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
    __pyx_v_end = __Pyx_PyInt_AsInt(values[2]); if (unlikely((__pyx_v_end == (int)-1) && PyErr_Occurred())) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 110; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
  } else if (PyTuple_GET_SIZE(__pyx_args) != 3) {
    goto __pyx_L5_argtuple_error;
  } else {
    __pyx_v_start = __Pyx_PyInt_AsInt(PyTuple_GET_ITEM(__pyx_args, 0)); if (unlikely((__pyx_v_start == (int)-1) && PyErr_Occurred())) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 110; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
    __pyx_v_count = __Pyx_PyInt_AsInt(PyTuple_GET_ITEM(__pyx_args, 1)); if (unlikely((__pyx_v_count == (int)-1) && PyErr_Occurred())) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 110; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
    __pyx_v_end = __Pyx_PyInt_AsInt(PyTuple_GET_ITEM(__pyx_args, 2)); if (unlikely((__pyx_v_end == (int)-1) && PyErr_Occurred())) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 110; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
  }
  goto __pyx_L4_argument_unpacking_done;
  __pyx_L5_argtuple_error:;
  __Pyx_RaiseArgtupleInvalid("massCloseFileDescriptors", 1, 3, 3, PyTuple_GET_SIZE(__pyx_args)); {__pyx_filename = __pyx_f[0]; __pyx_lineno = 110; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
  __pyx_L3_error:;
  __Pyx_AddTraceback("conary.lib.ext.file_utils.massCloseFileDescriptors");
  __Pyx_RefNannyFinishContext();
  return NULL;
  __pyx_L4_argument_unpacking_done:;

  /* "conary/lib/ext/file_utils.pyx":115
 *     cdef int i, j, rc
 * 
 *     if count and end:             # <<<<<<<<<<<<<<
 *         raise ValueError("Exactly one of count and end must be zero.")
 * 
 */
  if (__pyx_v_count) {
    __pyx_t_1 = __pyx_v_end;
  } else {
    __pyx_t_1 = __pyx_v_count;
  }
  if (__pyx_t_1) {

    /* "conary/lib/ext/file_utils.pyx":116
 * 
 *     if count and end:
 *         raise ValueError("Exactly one of count and end must be zero.")             # <<<<<<<<<<<<<<
 * 
 *     rc = 0
 */
    __pyx_t_2 = PyObject_Call(__pyx_builtin_ValueError, ((PyObject *)__pyx_k_tuple_2), NULL); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 116; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    __Pyx_GOTREF(__pyx_t_2);
    __Pyx_Raise(__pyx_t_2, 0, 0);
    __Pyx_DECREF(__pyx_t_2); __pyx_t_2 = 0;
    {__pyx_filename = __pyx_f[0]; __pyx_lineno = 116; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    goto __pyx_L6;
  }
  __pyx_L6:;

  /* "conary/lib/ext/file_utils.pyx":118
 *         raise ValueError("Exactly one of count and end must be zero.")
 * 
 *     rc = 0             # <<<<<<<<<<<<<<
 *     i = start
 *     j = count
 */
  __pyx_v_rc = 0;

  /* "conary/lib/ext/file_utils.pyx":119
 * 
 *     rc = 0
 *     i = start             # <<<<<<<<<<<<<<
 *     j = count
 *     with nogil:
 */
  __pyx_v_i = __pyx_v_start;

  /* "conary/lib/ext/file_utils.pyx":120
 *     rc = 0
 *     i = start
 *     j = count             # <<<<<<<<<<<<<<
 *     with nogil:
 *         while True:
 */
  __pyx_v_j = __pyx_v_count;

  /* "conary/lib/ext/file_utils.pyx":121
 *     i = start
 *     j = count
 *     with nogil:             # <<<<<<<<<<<<<<
 *         while True:
 *             if count:
 */
  {
    #ifdef WITH_THREAD
    PyThreadState *_save;
    #endif
    Py_UNBLOCK_THREADS
    /*try:*/ {

      /* "conary/lib/ext/file_utils.pyx":122
 *     j = count
 *     with nogil:
 *         while True:             # <<<<<<<<<<<<<<
 *             if count:
 *                 # Stopping after a contiguous number of fds
 */
      while (1) {
        if (!1) break;

        /* "conary/lib/ext/file_utils.pyx":123
 *     with nogil:
 *         while True:
 *             if count:             # <<<<<<<<<<<<<<
 *                 # Stopping after a contiguous number of fds
 *                 if j == 0:
 */
        if (__pyx_v_count) {

          /* "conary/lib/ext/file_utils.pyx":125
 *             if count:
 *                 # Stopping after a contiguous number of fds
 *                 if j == 0:             # <<<<<<<<<<<<<<
 *                     break
 *             elif i == end:
 */
          __pyx_t_1 = (__pyx_v_j == 0);
          if (__pyx_t_1) {

            /* "conary/lib/ext/file_utils.pyx":126
 *                 # Stopping after a contiguous number of fds
 *                 if j == 0:
 *                     break             # <<<<<<<<<<<<<<
 *             elif i == end:
 *                 # Stopping at specific value
 */
            goto __pyx_L11_break;
            goto __pyx_L13;
          }
          __pyx_L13:;
          goto __pyx_L12;
        }

        /* "conary/lib/ext/file_utils.pyx":127
 *                 if j == 0:
 *                     break
 *             elif i == end:             # <<<<<<<<<<<<<<
 *                 # Stopping at specific value
 *                 break
 */
        __pyx_t_1 = (__pyx_v_i == __pyx_v_end);
        if (__pyx_t_1) {

          /* "conary/lib/ext/file_utils.pyx":129
 *             elif i == end:
 *                 # Stopping at specific value
 *                 break             # <<<<<<<<<<<<<<
 * 
 *             rc = close(i)
 */
          goto __pyx_L11_break;
          goto __pyx_L12;
        }
        __pyx_L12:;

        /* "conary/lib/ext/file_utils.pyx":131
 *                 break
 * 
 *             rc = close(i)             # <<<<<<<<<<<<<<
 *             if rc == 0:
 *                 # Successful close -- reset contiguous counter
 */
        __pyx_v_rc = close(__pyx_v_i);

        /* "conary/lib/ext/file_utils.pyx":132
 * 
 *             rc = close(i)
 *             if rc == 0:             # <<<<<<<<<<<<<<
 *                 # Successful close -- reset contiguous counter
 *                 j = count
 */
        __pyx_t_1 = (__pyx_v_rc == 0);
        if (__pyx_t_1) {

          /* "conary/lib/ext/file_utils.pyx":134
 *             if rc == 0:
 *                 # Successful close -- reset contiguous counter
 *                 j = count             # <<<<<<<<<<<<<<
 *             elif errno == EBADF:
 *                 # FD was not in use
 */
          __pyx_v_j = __pyx_v_count;
          goto __pyx_L14;
        }

        /* "conary/lib/ext/file_utils.pyx":135
 *                 # Successful close -- reset contiguous counter
 *                 j = count
 *             elif errno == EBADF:             # <<<<<<<<<<<<<<
 *                 # FD was not in use
 *                 j -= 1
 */
        __pyx_t_1 = (errno == EBADF);
        if (__pyx_t_1) {

          /* "conary/lib/ext/file_utils.pyx":137
 *             elif errno == EBADF:
 *                 # FD was not in use
 *                 j -= 1             # <<<<<<<<<<<<<<
 *             else:
 *                 # Some other error
 */
          __pyx_v_j = (__pyx_v_j - 1);
          goto __pyx_L14;
        }
        /*else*/ {

          /* "conary/lib/ext/file_utils.pyx":140
 *             else:
 *                 # Some other error
 *                 break             # <<<<<<<<<<<<<<
 *             rc = 0
 *             i += 1
 */
          goto __pyx_L11_break;
        }
        __pyx_L14:;

        /* "conary/lib/ext/file_utils.pyx":141
 *                 # Some other error
 *                 break
 *             rc = 0             # <<<<<<<<<<<<<<
 *             i += 1
 * 
 */
        __pyx_v_rc = 0;

        /* "conary/lib/ext/file_utils.pyx":142
 *                 break
 *             rc = 0
 *             i += 1             # <<<<<<<<<<<<<<
 * 
 *     if rc != 0:
 */
        __pyx_v_i = (__pyx_v_i + 1);
      }
      __pyx_L11_break:;
    }

    /* "conary/lib/ext/file_utils.pyx":121
 *     i = start
 *     j = count
 *     with nogil:             # <<<<<<<<<<<<<<
 *         while True:
 *             if count:
 */
    /*finally:*/ {
      Py_BLOCK_THREADS
    }
  }

  /* "conary/lib/ext/file_utils.pyx":144
 *             i += 1
 * 
 *     if rc != 0:             # <<<<<<<<<<<<<<
 *         PyErr_SetFromErrno(OSError)
 * 
 */
  __pyx_t_1 = (__pyx_v_rc != 0);
  if (__pyx_t_1) {

    /* "conary/lib/ext/file_utils.pyx":145
 * 
 *     if rc != 0:
 *         PyErr_SetFromErrno(OSError)             # <<<<<<<<<<<<<<
 * 
 * 
 */
    __pyx_t_2 = PyErr_SetFromErrno(__pyx_builtin_OSError); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 145; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    __Pyx_GOTREF(__pyx_t_2);
    __Pyx_DECREF(__pyx_t_2); __pyx_t_2 = 0;
    goto __pyx_L15;
  }
  __pyx_L15:;

  __pyx_r = Py_None; __Pyx_INCREF(Py_None);
  goto __pyx_L0;
  __pyx_L1_error:;
  __Pyx_XDECREF(__pyx_t_2);
  __Pyx_AddTraceback("conary.lib.ext.file_utils.massCloseFileDescriptors");
  __pyx_r = NULL;
  __pyx_L0:;
  __Pyx_XGIVEREF(__pyx_r);
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

/* "conary/lib/ext/file_utils.pyx":148
 * 
 * 
 * def mkdirIfMissing(char *path):             # <<<<<<<<<<<<<<
 *     """Make a directory at C{path} if it does not exist."""
 *     if mkdir(path, 0777) == -1:
 */

static PyObject *__pyx_pf_6conary_3lib_3ext_10file_utils_5mkdirIfMissing(PyObject *__pyx_self, PyObject *__pyx_arg_path); /*proto*/
static char __pyx_doc_6conary_3lib_3ext_10file_utils_5mkdirIfMissing[] = "Make a directory at C{path} if it does not exist.";
static PyMethodDef __pyx_mdef_6conary_3lib_3ext_10file_utils_5mkdirIfMissing = {__Pyx_NAMESTR("mkdirIfMissing"), (PyCFunction)__pyx_pf_6conary_3lib_3ext_10file_utils_5mkdirIfMissing, METH_O, __Pyx_DOCSTR(__pyx_doc_6conary_3lib_3ext_10file_utils_5mkdirIfMissing)};
static PyObject *__pyx_pf_6conary_3lib_3ext_10file_utils_5mkdirIfMissing(PyObject *__pyx_self, PyObject *__pyx_arg_path) {
  char *__pyx_v_path;
  PyObject *__pyx_r = NULL;
  int __pyx_t_1;
  PyObject *__pyx_t_2 = NULL;
  __Pyx_RefNannySetupContext("mkdirIfMissing");
  __pyx_self = __pyx_self;
  assert(__pyx_arg_path); {
    __pyx_v_path = PyBytes_AsString(__pyx_arg_path); if (unlikely((!__pyx_v_path) && PyErr_Occurred())) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 148; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
  }
  goto __pyx_L4_argument_unpacking_done;
  __pyx_L3_error:;
  __Pyx_AddTraceback("conary.lib.ext.file_utils.mkdirIfMissing");
  __Pyx_RefNannyFinishContext();
  return NULL;
  __pyx_L4_argument_unpacking_done:;

  /* "conary/lib/ext/file_utils.pyx":150
 * def mkdirIfMissing(char *path):
 *     """Make a directory at C{path} if it does not exist."""
 *     if mkdir(path, 0777) == -1:             # <<<<<<<<<<<<<<
 *         if errno == EEXIST:
 *             return False
 */
  __pyx_t_1 = (mkdir(__pyx_v_path, 0777) == -1);
  if (__pyx_t_1) {

    /* "conary/lib/ext/file_utils.pyx":151
 *     """Make a directory at C{path} if it does not exist."""
 *     if mkdir(path, 0777) == -1:
 *         if errno == EEXIST:             # <<<<<<<<<<<<<<
 *             return False
 *         PyErr_SetFromErrnoWithFilename(OSError, path)
 */
    __pyx_t_1 = (errno == EEXIST);
    if (__pyx_t_1) {

      /* "conary/lib/ext/file_utils.pyx":152
 *     if mkdir(path, 0777) == -1:
 *         if errno == EEXIST:
 *             return False             # <<<<<<<<<<<<<<
 *         PyErr_SetFromErrnoWithFilename(OSError, path)
 *     return True
 */
      __Pyx_XDECREF(__pyx_r);
      __pyx_t_2 = __Pyx_PyBool_FromLong(0); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 152; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_GOTREF(__pyx_t_2);
      __pyx_r = __pyx_t_2;
      __pyx_t_2 = 0;
      goto __pyx_L0;
      goto __pyx_L6;
    }
    __pyx_L6:;

    /* "conary/lib/ext/file_utils.pyx":153
 *         if errno == EEXIST:
 *             return False
 *         PyErr_SetFromErrnoWithFilename(OSError, path)             # <<<<<<<<<<<<<<
 *     return True
 * 
 */
    __pyx_t_2 = PyErr_SetFromErrnoWithFilename(__pyx_builtin_OSError, __pyx_v_path); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 153; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    __Pyx_GOTREF(__pyx_t_2);
    __Pyx_DECREF(__pyx_t_2); __pyx_t_2 = 0;
    goto __pyx_L5;
  }
  __pyx_L5:;

  /* "conary/lib/ext/file_utils.pyx":154
 *             return False
 *         PyErr_SetFromErrnoWithFilename(OSError, path)
 *     return True             # <<<<<<<<<<<<<<
 * 
 * 
 */
  __Pyx_XDECREF(__pyx_r);
  __pyx_t_2 = __Pyx_PyBool_FromLong(1); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 154; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_2);
  __pyx_r = __pyx_t_2;
  __pyx_t_2 = 0;
  goto __pyx_L0;

  __pyx_r = Py_None; __Pyx_INCREF(Py_None);
  goto __pyx_L0;
  __pyx_L1_error:;
  __Pyx_XDECREF(__pyx_t_2);
  __Pyx_AddTraceback("conary.lib.ext.file_utils.mkdirIfMissing");
  __pyx_r = NULL;
  __pyx_L0:;
  __Pyx_XGIVEREF(__pyx_r);
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

/* "conary/lib/ext/file_utils.pyx":157
 * 
 * 
 * def pread(fobj, size_t count, off_t offset):             # <<<<<<<<<<<<<<
 *     """Read C{count} bytes at C{offset} in file C{fobj}."""
 *     cdef Py_ssize_t rc
 */

static PyObject *__pyx_pf_6conary_3lib_3ext_10file_utils_6pread(PyObject *__pyx_self, PyObject *__pyx_args, PyObject *__pyx_kwds); /*proto*/
static char __pyx_doc_6conary_3lib_3ext_10file_utils_6pread[] = "Read C{count} bytes at C{offset} in file C{fobj}.";
static PyMethodDef __pyx_mdef_6conary_3lib_3ext_10file_utils_6pread = {__Pyx_NAMESTR("pread"), (PyCFunction)__pyx_pf_6conary_3lib_3ext_10file_utils_6pread, METH_VARARGS|METH_KEYWORDS, __Pyx_DOCSTR(__pyx_doc_6conary_3lib_3ext_10file_utils_6pread)};
static PyObject *__pyx_pf_6conary_3lib_3ext_10file_utils_6pread(PyObject *__pyx_self, PyObject *__pyx_args, PyObject *__pyx_kwds) {
  PyObject *__pyx_v_fobj = 0;
  size_t __pyx_v_count;
  off_t __pyx_v_offset;
  Py_ssize_t __pyx_v_rc;
  char *__pyx_v_data;
  int __pyx_v_fd;
  PyObject *__pyx_v_ret;
  PyObject *__pyx_r = NULL;
  int __pyx_t_1;
  int __pyx_t_2;
  PyObject *__pyx_t_3 = NULL;
  static PyObject **__pyx_pyargnames[] = {&__pyx_n_s__fobj,&__pyx_n_s__count,&__pyx_n_s__offset,0};
  __Pyx_RefNannySetupContext("pread");
  __pyx_self = __pyx_self;
  if (unlikely(__pyx_kwds)) {
    Py_ssize_t kw_args = PyDict_Size(__pyx_kwds);
    PyObject* values[3] = {0,0,0};
    switch (PyTuple_GET_SIZE(__pyx_args)) {
      case  3: values[2] = PyTuple_GET_ITEM(__pyx_args, 2);
      case  2: values[1] = PyTuple_GET_ITEM(__pyx_args, 1);
      case  1: values[0] = PyTuple_GET_ITEM(__pyx_args, 0);
      case  0: break;
      default: goto __pyx_L5_argtuple_error;
    }
    switch (PyTuple_GET_SIZE(__pyx_args)) {
      case  0:
      values[0] = PyDict_GetItem(__pyx_kwds, __pyx_n_s__fobj);
      if (likely(values[0])) kw_args--;
      else goto __pyx_L5_argtuple_error;
      case  1:
      values[1] = PyDict_GetItem(__pyx_kwds, __pyx_n_s__count);
      if (likely(values[1])) kw_args--;
      else {
        __Pyx_RaiseArgtupleInvalid("pread", 1, 3, 3, 1); {__pyx_filename = __pyx_f[0]; __pyx_lineno = 157; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
      }
      case  2:
      values[2] = PyDict_GetItem(__pyx_kwds, __pyx_n_s__offset);
      if (likely(values[2])) kw_args--;
      else {
        __Pyx_RaiseArgtupleInvalid("pread", 1, 3, 3, 2); {__pyx_filename = __pyx_f[0]; __pyx_lineno = 157; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
      }
    }
    if (unlikely(kw_args > 0)) {
      if (unlikely(__Pyx_ParseOptionalKeywords(__pyx_kwds, __pyx_pyargnames, 0, values, PyTuple_GET_SIZE(__pyx_args), "pread") < 0)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 157; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
    }
    __pyx_v_fobj = values[0];
    __pyx_v_count = __Pyx_PyInt_AsSize_t(values[1]); if (unlikely((__pyx_v_count == (size_t)-1) && PyErr_Occurred())) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 157; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
    __pyx_v_offset = __Pyx_PyInt_from_py_off_t(values[2]); if (unlikely((__pyx_v_offset == (off_t)-1) && PyErr_Occurred())) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 157; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
  } else if (PyTuple_GET_SIZE(__pyx_args) != 3) {
    goto __pyx_L5_argtuple_error;
  } else {
    __pyx_v_fobj = PyTuple_GET_ITEM(__pyx_args, 0);
    __pyx_v_count = __Pyx_PyInt_AsSize_t(PyTuple_GET_ITEM(__pyx_args, 1)); if (unlikely((__pyx_v_count == (size_t)-1) && PyErr_Occurred())) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 157; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
    __pyx_v_offset = __Pyx_PyInt_from_py_off_t(PyTuple_GET_ITEM(__pyx_args, 2)); if (unlikely((__pyx_v_offset == (off_t)-1) && PyErr_Occurred())) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 157; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
  }
  goto __pyx_L4_argument_unpacking_done;
  __pyx_L5_argtuple_error:;
  __Pyx_RaiseArgtupleInvalid("pread", 1, 3, 3, PyTuple_GET_SIZE(__pyx_args)); {__pyx_filename = __pyx_f[0]; __pyx_lineno = 157; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
  __pyx_L3_error:;
  __Pyx_AddTraceback("conary.lib.ext.file_utils.pread");
  __Pyx_RefNannyFinishContext();
  return NULL;
  __pyx_L4_argument_unpacking_done:;
  __pyx_v_ret = Py_None; __Pyx_INCREF(Py_None);

  /* "conary/lib/ext/file_utils.pyx":163
 *     cdef int fd
 * 
 *     fd = PyObject_AsFileDescriptor(fobj)             # <<<<<<<<<<<<<<
 * 
 *     data = <char*>malloc(count)
 */
  __pyx_t_1 = PyObject_AsFileDescriptor(__pyx_v_fobj); if (unlikely(__pyx_t_1 == -1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 163; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_v_fd = __pyx_t_1;

  /* "conary/lib/ext/file_utils.pyx":165
 *     fd = PyObject_AsFileDescriptor(fobj)
 * 
 *     data = <char*>malloc(count)             # <<<<<<<<<<<<<<
 *     if data == NULL:
 *         raise MemoryError
 */
  __pyx_v_data = ((char *)malloc(__pyx_v_count));

  /* "conary/lib/ext/file_utils.pyx":166
 * 
 *     data = <char*>malloc(count)
 *     if data == NULL:             # <<<<<<<<<<<<<<
 *         raise MemoryError
 * 
 */
  __pyx_t_2 = (__pyx_v_data == NULL);
  if (__pyx_t_2) {

    /* "conary/lib/ext/file_utils.pyx":167
 *     data = <char*>malloc(count)
 *     if data == NULL:
 *         raise MemoryError             # <<<<<<<<<<<<<<
 * 
 *     with nogil:
 */
    PyErr_NoMemory(); {__pyx_filename = __pyx_f[0]; __pyx_lineno = 167; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    goto __pyx_L6;
  }
  __pyx_L6:;

  /* "conary/lib/ext/file_utils.pyx":169
 *         raise MemoryError
 * 
 *     with nogil:             # <<<<<<<<<<<<<<
 *         rc = c_pread(fd, data, count, offset)
 * 
 */
  {
    #ifdef WITH_THREAD
    PyThreadState *_save;
    #endif
    Py_UNBLOCK_THREADS
    /*try:*/ {

      /* "conary/lib/ext/file_utils.pyx":170
 * 
 *     with nogil:
 *         rc = c_pread(fd, data, count, offset)             # <<<<<<<<<<<<<<
 * 
 *     if rc == -1:
 */
      __pyx_v_rc = pread(__pyx_v_fd, __pyx_v_data, __pyx_v_count, __pyx_v_offset);
    }

    /* "conary/lib/ext/file_utils.pyx":169
 *         raise MemoryError
 * 
 *     with nogil:             # <<<<<<<<<<<<<<
 *         rc = c_pread(fd, data, count, offset)
 * 
 */
    /*finally:*/ {
      Py_BLOCK_THREADS
    }
  }

  /* "conary/lib/ext/file_utils.pyx":172
 *         rc = c_pread(fd, data, count, offset)
 * 
 *     if rc == -1:             # <<<<<<<<<<<<<<
 *         free(data)
 *         PyErr_SetFromErrno(OSError)
 */
  __pyx_t_2 = (__pyx_v_rc == -1);
  if (__pyx_t_2) {

    /* "conary/lib/ext/file_utils.pyx":173
 * 
 *     if rc == -1:
 *         free(data)             # <<<<<<<<<<<<<<
 *         PyErr_SetFromErrno(OSError)
 * 
 */
    free(__pyx_v_data);

    /* "conary/lib/ext/file_utils.pyx":174
 *     if rc == -1:
 *         free(data)
 *         PyErr_SetFromErrno(OSError)             # <<<<<<<<<<<<<<
 * 
 *     ret = PyString_FromStringAndSize(data, rc)
 */
    __pyx_t_3 = PyErr_SetFromErrno(__pyx_builtin_OSError); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 174; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    __Pyx_GOTREF(__pyx_t_3);
    __Pyx_DECREF(__pyx_t_3); __pyx_t_3 = 0;
    goto __pyx_L10;
  }
  __pyx_L10:;

  /* "conary/lib/ext/file_utils.pyx":176
 *         PyErr_SetFromErrno(OSError)
 * 
 *     ret = PyString_FromStringAndSize(data, rc)             # <<<<<<<<<<<<<<
 *     free(data)
 *     return ret
 */
  __pyx_t_3 = PyString_FromStringAndSize(__pyx_v_data, __pyx_v_rc); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 176; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_3);
  __Pyx_DECREF(__pyx_v_ret);
  __pyx_v_ret = __pyx_t_3;
  __pyx_t_3 = 0;

  /* "conary/lib/ext/file_utils.pyx":177
 * 
 *     ret = PyString_FromStringAndSize(data, rc)
 *     free(data)             # <<<<<<<<<<<<<<
 *     return ret
 * 
 */
  free(__pyx_v_data);

  /* "conary/lib/ext/file_utils.pyx":178
 *     ret = PyString_FromStringAndSize(data, rc)
 *     free(data)
 *     return ret             # <<<<<<<<<<<<<<
 * 
 * 
 */
  __Pyx_XDECREF(__pyx_r);
  __Pyx_INCREF(__pyx_v_ret);
  __pyx_r = __pyx_v_ret;
  goto __pyx_L0;

  __pyx_r = Py_None; __Pyx_INCREF(Py_None);
  goto __pyx_L0;
  __pyx_L1_error:;
  __Pyx_XDECREF(__pyx_t_3);
  __Pyx_AddTraceback("conary.lib.ext.file_utils.pread");
  __pyx_r = NULL;
  __pyx_L0:;
  __Pyx_DECREF(__pyx_v_ret);
  __Pyx_XGIVEREF(__pyx_r);
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

/* "conary/lib/ext/file_utils.pyx":181
 * 
 * 
 * def removeIfExists(char *path):             # <<<<<<<<<<<<<<
 *     """Try to unlink C{path}, but don't fail if it doesn't exist."""
 *     if unlink(path):
 */

static PyObject *__pyx_pf_6conary_3lib_3ext_10file_utils_7removeIfExists(PyObject *__pyx_self, PyObject *__pyx_arg_path); /*proto*/
static char __pyx_doc_6conary_3lib_3ext_10file_utils_7removeIfExists[] = "Try to unlink C{path}, but don't fail if it doesn't exist.";
static PyMethodDef __pyx_mdef_6conary_3lib_3ext_10file_utils_7removeIfExists = {__Pyx_NAMESTR("removeIfExists"), (PyCFunction)__pyx_pf_6conary_3lib_3ext_10file_utils_7removeIfExists, METH_O, __Pyx_DOCSTR(__pyx_doc_6conary_3lib_3ext_10file_utils_7removeIfExists)};
static PyObject *__pyx_pf_6conary_3lib_3ext_10file_utils_7removeIfExists(PyObject *__pyx_self, PyObject *__pyx_arg_path) {
  char *__pyx_v_path;
  PyObject *__pyx_r = NULL;
  int __pyx_t_1;
  int __pyx_t_2;
  int __pyx_t_3;
  int __pyx_t_4;
  PyObject *__pyx_t_5 = NULL;
  __Pyx_RefNannySetupContext("removeIfExists");
  __pyx_self = __pyx_self;
  assert(__pyx_arg_path); {
    __pyx_v_path = PyBytes_AsString(__pyx_arg_path); if (unlikely((!__pyx_v_path) && PyErr_Occurred())) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 181; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
  }
  goto __pyx_L4_argument_unpacking_done;
  __pyx_L3_error:;
  __Pyx_AddTraceback("conary.lib.ext.file_utils.removeIfExists");
  __Pyx_RefNannyFinishContext();
  return NULL;
  __pyx_L4_argument_unpacking_done:;

  /* "conary/lib/ext/file_utils.pyx":183
 * def removeIfExists(char *path):
 *     """Try to unlink C{path}, but don't fail if it doesn't exist."""
 *     if unlink(path):             # <<<<<<<<<<<<<<
 *         if errno in (ENOENT, ENAMETOOLONG):
 *             return False
 */
  __pyx_t_1 = unlink(__pyx_v_path);
  if (__pyx_t_1) {

    /* "conary/lib/ext/file_utils.pyx":184
 *     """Try to unlink C{path}, but don't fail if it doesn't exist."""
 *     if unlink(path):
 *         if errno in (ENOENT, ENAMETOOLONG):             # <<<<<<<<<<<<<<
 *             return False
 *         PyErr_SetFromErrnoWithFilename(OSError, path)
 */
    __pyx_t_1 = errno;
    __pyx_t_2 = (__pyx_t_1 == ENOENT);
    if (!__pyx_t_2) {
      __pyx_t_3 = (__pyx_t_1 == ENAMETOOLONG);
      __pyx_t_4 = __pyx_t_3;
    } else {
      __pyx_t_4 = __pyx_t_2;
    }
    __pyx_t_2 = __pyx_t_4;
    if (__pyx_t_2) {

      /* "conary/lib/ext/file_utils.pyx":185
 *     if unlink(path):
 *         if errno in (ENOENT, ENAMETOOLONG):
 *             return False             # <<<<<<<<<<<<<<
 *         PyErr_SetFromErrnoWithFilename(OSError, path)
 *     return True
 */
      __Pyx_XDECREF(__pyx_r);
      __pyx_t_5 = __Pyx_PyBool_FromLong(0); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 185; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_GOTREF(__pyx_t_5);
      __pyx_r = __pyx_t_5;
      __pyx_t_5 = 0;
      goto __pyx_L0;
      goto __pyx_L6;
    }
    __pyx_L6:;

    /* "conary/lib/ext/file_utils.pyx":186
 *         if errno in (ENOENT, ENAMETOOLONG):
 *             return False
 *         PyErr_SetFromErrnoWithFilename(OSError, path)             # <<<<<<<<<<<<<<
 *     return True
 */
    __pyx_t_5 = PyErr_SetFromErrnoWithFilename(__pyx_builtin_OSError, __pyx_v_path); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 186; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    __Pyx_GOTREF(__pyx_t_5);
    __Pyx_DECREF(__pyx_t_5); __pyx_t_5 = 0;
    goto __pyx_L5;
  }
  __pyx_L5:;

  /* "conary/lib/ext/file_utils.pyx":187
 *             return False
 *         PyErr_SetFromErrnoWithFilename(OSError, path)
 *     return True             # <<<<<<<<<<<<<<
 */
  __Pyx_XDECREF(__pyx_r);
  __pyx_t_5 = __Pyx_PyBool_FromLong(1); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 187; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_5);
  __pyx_r = __pyx_t_5;
  __pyx_t_5 = 0;
  goto __pyx_L0;

  __pyx_r = Py_None; __Pyx_INCREF(Py_None);
  goto __pyx_L0;
  __pyx_L1_error:;
  __Pyx_XDECREF(__pyx_t_5);
  __Pyx_AddTraceback("conary.lib.ext.file_utils.removeIfExists");
  __pyx_r = NULL;
  __pyx_L0:;
  __Pyx_XGIVEREF(__pyx_r);
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

static PyMethodDef __pyx_methods[] = {
  {0, 0, 0, 0}
};

#if PY_MAJOR_VERSION >= 3
static struct PyModuleDef __pyx_moduledef = {
    PyModuleDef_HEAD_INIT,
    __Pyx_NAMESTR("file_utils"),
    __Pyx_DOCSTR(__pyx_k_3), /* m_doc */
    -1, /* m_size */
    __pyx_methods /* m_methods */,
    NULL, /* m_reload */
    NULL, /* m_traverse */
    NULL, /* m_clear */
    NULL /* m_free */
};
#endif

static __Pyx_StringTabEntry __pyx_string_tab[] = {
  {&__pyx_kp_s_1, __pyx_k_1, sizeof(__pyx_k_1), 0, 0, 1, 0},
  {&__pyx_n_s_4, __pyx_k_4, sizeof(__pyx_k_4), 0, 0, 1, 1},
  {&__pyx_n_s_5, __pyx_k_5, sizeof(__pyx_k_5), 0, 0, 1, 1},
  {&__pyx_n_s_6, __pyx_k_6, sizeof(__pyx_k_6), 0, 0, 1, 1},
  {&__pyx_n_s__MemoryError, __pyx_k__MemoryError, sizeof(__pyx_k__MemoryError), 0, 0, 1, 1},
  {&__pyx_n_s__OSError, __pyx_k__OSError, sizeof(__pyx_k__OSError), 0, 0, 1, 1},
  {&__pyx_n_s__ValueError, __pyx_k__ValueError, sizeof(__pyx_k__ValueError), 0, 0, 1, 1},
  {&__pyx_n_s____main__, __pyx_k____main__, sizeof(__pyx_k____main__), 0, 0, 1, 1},
  {&__pyx_n_s____test__, __pyx_k____test__, sizeof(__pyx_k____test__), 0, 0, 1, 1},
  {&__pyx_n_s__count, __pyx_k__count, sizeof(__pyx_k__count), 0, 0, 1, 1},
  {&__pyx_n_s__end, __pyx_k__end, sizeof(__pyx_k__end), 0, 0, 1, 1},
  {&__pyx_n_s__events, __pyx_k__events, sizeof(__pyx_k__events), 0, 0, 1, 1},
  {&__pyx_n_s__fchmod, __pyx_k__fchmod, sizeof(__pyx_k__fchmod), 0, 0, 1, 1},
  {&__pyx_n_s__fd, __pyx_k__fd, sizeof(__pyx_k__fd), 0, 0, 1, 1},
  {&__pyx_n_s__fobj, __pyx_k__fobj, sizeof(__pyx_k__fobj), 0, 0, 1, 1},
  {&__pyx_n_s__fopenIfExists, __pyx_k__fopenIfExists, sizeof(__pyx_k__fopenIfExists), 0, 0, 1, 1},
  {&__pyx_n_s__lexists, __pyx_k__lexists, sizeof(__pyx_k__lexists), 0, 0, 1, 1},
  {&__pyx_n_s__mkdirIfMissing, __pyx_k__mkdirIfMissing, sizeof(__pyx_k__mkdirIfMissing), 0, 0, 1, 1},
  {&__pyx_n_s__mode, __pyx_k__mode, sizeof(__pyx_k__mode), 0, 0, 1, 1},
  {&__pyx_n_s__offset, __pyx_k__offset, sizeof(__pyx_k__offset), 0, 0, 1, 1},
  {&__pyx_n_s__path, __pyx_k__path, sizeof(__pyx_k__path), 0, 0, 1, 1},
  {&__pyx_n_s__pread, __pyx_k__pread, sizeof(__pyx_k__pread), 0, 0, 1, 1},
  {&__pyx_n_s__range, __pyx_k__range, sizeof(__pyx_k__range), 0, 0, 1, 1},
  {&__pyx_n_s__removeIfExists, __pyx_k__removeIfExists, sizeof(__pyx_k__removeIfExists), 0, 0, 1, 1},
  {&__pyx_n_s__revents, __pyx_k__revents, sizeof(__pyx_k__revents), 0, 0, 1, 1},
  {&__pyx_n_s__start, __pyx_k__start, sizeof(__pyx_k__start), 0, 0, 1, 1},
  {0, 0, 0, 0, 0, 0, 0}
};
static int __Pyx_InitCachedBuiltins(void) {
  __pyx_builtin_MemoryError = __Pyx_GetName(__pyx_b, __pyx_n_s__MemoryError); if (!__pyx_builtin_MemoryError) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 51; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_builtin_range = __Pyx_GetName(__pyx_b, __pyx_n_s__range); if (!__pyx_builtin_range) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 54; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_builtin_OSError = __Pyx_GetName(__pyx_b, __pyx_n_s__OSError); if (!__pyx_builtin_OSError) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 66; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_builtin_ValueError = __Pyx_GetName(__pyx_b, __pyx_n_s__ValueError); if (!__pyx_builtin_ValueError) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 116; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  return 0;
  __pyx_L1_error:;
  return -1;
}

static int __Pyx_InitCachedConstants(void) {
  __Pyx_RefNannySetupContext("__Pyx_InitCachedConstants");

  /* "conary/lib/ext/file_utils.pyx":116
 * 
 *     if count and end:
 *         raise ValueError("Exactly one of count and end must be zero.")             # <<<<<<<<<<<<<<
 * 
 *     rc = 0
 */
  __pyx_k_tuple_2 = PyTuple_New(1); if (unlikely(!__pyx_k_tuple_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 116; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(((PyObject *)__pyx_k_tuple_2));
  __Pyx_INCREF(((PyObject *)__pyx_kp_s_1));
  PyTuple_SET_ITEM(__pyx_k_tuple_2, 0, ((PyObject *)__pyx_kp_s_1));
  __Pyx_GIVEREF(((PyObject *)__pyx_kp_s_1));
  __Pyx_GIVEREF(((PyObject *)__pyx_k_tuple_2));
  __Pyx_RefNannyFinishContext();
  return 0;
  __pyx_L1_error:;
  __Pyx_RefNannyFinishContext();
  return -1;
}

static int __Pyx_InitGlobals(void) {
  if (__Pyx_InitStrings(__pyx_string_tab) < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;};
  return 0;
  __pyx_L1_error:;
  return -1;
}

#if PY_MAJOR_VERSION < 3
PyMODINIT_FUNC initfile_utils(void); /*proto*/
PyMODINIT_FUNC initfile_utils(void)
#else
PyMODINIT_FUNC PyInit_file_utils(void); /*proto*/
PyMODINIT_FUNC PyInit_file_utils(void)
#endif
{
  PyObject *__pyx_t_1 = NULL;
  #if CYTHON_REFNANNY
  void* __pyx_refnanny = NULL;
  __Pyx_RefNanny = __Pyx_RefNannyImportAPI("refnanny");
  if (!__Pyx_RefNanny) {
      PyErr_Clear();
      __Pyx_RefNanny = __Pyx_RefNannyImportAPI("Cython.Runtime.refnanny");
      if (!__Pyx_RefNanny)
          Py_FatalError("failed to import 'refnanny' module");
  }
  __pyx_refnanny = __Pyx_RefNanny->SetupContext("PyMODINIT_FUNC PyInit_file_utils(void)", __LINE__, __FILE__);
  #endif
  __pyx_empty_tuple = PyTuple_New(0); if (unlikely(!__pyx_empty_tuple)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_empty_bytes = PyBytes_FromStringAndSize("", 0); if (unlikely(!__pyx_empty_bytes)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  #ifdef __pyx_binding_PyCFunctionType_USED
  if (__pyx_binding_PyCFunctionType_init() < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  #endif
  /*--- Library function declarations ---*/
  /*--- Threads initialization code ---*/
  #if defined(__PYX_FORCE_INIT_THREADS) && __PYX_FORCE_INIT_THREADS
  #ifdef WITH_THREAD /* Python build with threading support? */
  PyEval_InitThreads();
  #endif
  #endif
  /*--- Module creation code ---*/
  #if PY_MAJOR_VERSION < 3
  __pyx_m = Py_InitModule4(__Pyx_NAMESTR("file_utils"), __pyx_methods, __Pyx_DOCSTR(__pyx_k_3), 0, PYTHON_API_VERSION);
  #else
  __pyx_m = PyModule_Create(&__pyx_moduledef);
  #endif
  if (!__pyx_m) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;};
  #if PY_MAJOR_VERSION < 3
  Py_INCREF(__pyx_m);
  #endif
  __pyx_b = PyImport_AddModule(__Pyx_NAMESTR(__Pyx_BUILTIN_MODULE_NAME));
  if (!__pyx_b) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;};
  if (__Pyx_SetAttrString(__pyx_m, "__builtins__", __pyx_b) < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;};
  /*--- Initialize various global constants etc. ---*/
  if (unlikely(__Pyx_InitGlobals() < 0)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  if (__pyx_module_is_main_conary__lib__ext__file_utils) {
    if (__Pyx_SetAttrString(__pyx_m, "__name__", __pyx_n_s____main__) < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;};
  }
  /*--- Builtin init code ---*/
  if (unlikely(__Pyx_InitCachedBuiltins() < 0)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  /*--- Constants init code ---*/
  if (unlikely(__Pyx_InitCachedConstants() < 0)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  /*--- Global init code ---*/
  /*--- Function export code ---*/
  /*--- Type init code ---*/
  /*--- Type import code ---*/
  /*--- Function import code ---*/
  /*--- Execution code ---*/

  /* "conary/lib/ext/file_utils.pyx":43
 * 
 * 
 * def countOpenFileDescriptors():             # <<<<<<<<<<<<<<
 *     """Return a count of the number of open file descriptors."""
 *     cdef int maxfd, count, i, rc
 */
  __pyx_t_1 = PyCFunction_NewEx(&__pyx_mdef_6conary_3lib_3ext_10file_utils_countOpenFileDescriptors, NULL, __pyx_n_s_4); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 43; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  if (PyObject_SetAttr(__pyx_m, __pyx_n_s_5, __pyx_t_1) < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 43; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;

  /* "conary/lib/ext/file_utils.pyx":77
 * 
 * 
 * def fchmod(fobj, int mode):             # <<<<<<<<<<<<<<
 *     """Change the permissions of an open file."""
 *     cdef int fd, rc
 */
  __pyx_t_1 = PyCFunction_NewEx(&__pyx_mdef_6conary_3lib_3ext_10file_utils_1fchmod, NULL, __pyx_n_s_4); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 77; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  if (PyObject_SetAttr(__pyx_m, __pyx_n_s__fchmod, __pyx_t_1) < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 77; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;

  /* "conary/lib/ext/file_utils.pyx":87
 * 
 * 
 * def fopenIfExists(char *path, char *mode):             # <<<<<<<<<<<<<<
 *     """Open a file, or return C{None} if opening failed."""
 *     cdef FILE *fp
 */
  __pyx_t_1 = PyCFunction_NewEx(&__pyx_mdef_6conary_3lib_3ext_10file_utils_2fopenIfExists, NULL, __pyx_n_s_4); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 87; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  if (PyObject_SetAttr(__pyx_m, __pyx_n_s__fopenIfExists, __pyx_t_1) < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 87; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;

  /* "conary/lib/ext/file_utils.pyx":100
 * 
 * 
 * def lexists(char *path):             # <<<<<<<<<<<<<<
 *     """Return C{True} if C{path} exists."""
 *     cdef stat sb
 */
  __pyx_t_1 = PyCFunction_NewEx(&__pyx_mdef_6conary_3lib_3ext_10file_utils_3lexists, NULL, __pyx_n_s_4); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 100; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  if (PyObject_SetAttr(__pyx_m, __pyx_n_s__lexists, __pyx_t_1) < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 100; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;

  /* "conary/lib/ext/file_utils.pyx":110
 * 
 * 
 * def massCloseFileDescriptors(int start, int count, int end):             # <<<<<<<<<<<<<<
 *     """Close file descriptors from C{start} to either C{end} or after C{count}
 *     unused descriptors have been encountered."""
 */
  __pyx_t_1 = PyCFunction_NewEx(&__pyx_mdef_6conary_3lib_3ext_10file_utils_4massCloseFileDescriptors, NULL, __pyx_n_s_4); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 110; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  if (PyObject_SetAttr(__pyx_m, __pyx_n_s_6, __pyx_t_1) < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 110; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;

  /* "conary/lib/ext/file_utils.pyx":148
 * 
 * 
 * def mkdirIfMissing(char *path):             # <<<<<<<<<<<<<<
 *     """Make a directory at C{path} if it does not exist."""
 *     if mkdir(path, 0777) == -1:
 */
  __pyx_t_1 = PyCFunction_NewEx(&__pyx_mdef_6conary_3lib_3ext_10file_utils_5mkdirIfMissing, NULL, __pyx_n_s_4); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 148; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  if (PyObject_SetAttr(__pyx_m, __pyx_n_s__mkdirIfMissing, __pyx_t_1) < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 148; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;

  /* "conary/lib/ext/file_utils.pyx":157
 * 
 * 
 * def pread(fobj, size_t count, off_t offset):             # <<<<<<<<<<<<<<
 *     """Read C{count} bytes at C{offset} in file C{fobj}."""
 *     cdef Py_ssize_t rc
 */
  __pyx_t_1 = PyCFunction_NewEx(&__pyx_mdef_6conary_3lib_3ext_10file_utils_6pread, NULL, __pyx_n_s_4); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 157; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  if (PyObject_SetAttr(__pyx_m, __pyx_n_s__pread, __pyx_t_1) < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 157; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;

  /* "conary/lib/ext/file_utils.pyx":181
 * 
 * 
 * def removeIfExists(char *path):             # <<<<<<<<<<<<<<
 *     """Try to unlink C{path}, but don't fail if it doesn't exist."""
 *     if unlink(path):
 */
  __pyx_t_1 = PyCFunction_NewEx(&__pyx_mdef_6conary_3lib_3ext_10file_utils_7removeIfExists, NULL, __pyx_n_s_4); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 181; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  if (PyObject_SetAttr(__pyx_m, __pyx_n_s__removeIfExists, __pyx_t_1) < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 181; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;

  /* "conary/lib/ext/file_utils.pyx":1
 * #             # <<<<<<<<<<<<<<
 * # Copyright (c) rPath, Inc.
 * #
 */
  __pyx_t_1 = PyDict_New(); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(((PyObject *)__pyx_t_1));
  if (PyObject_SetAttr(__pyx_m, __pyx_n_s____test__, ((PyObject *)__pyx_t_1)) < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_DECREF(((PyObject *)__pyx_t_1)); __pyx_t_1 = 0;
  goto __pyx_L0;
  __pyx_L1_error:;
  __Pyx_XDECREF(__pyx_t_1);
  if (__pyx_m) {
    __Pyx_AddTraceback("init conary.lib.ext.file_utils");
    Py_DECREF(__pyx_m); __pyx_m = 0;
  } else if (!PyErr_Occurred()) {
    PyErr_SetString(PyExc_ImportError, "init conary.lib.ext.file_utils");
  }
  __pyx_L0:;
  __Pyx_RefNannyFinishContext();
  #if PY_MAJOR_VERSION < 3
  return;
  #else
  return __pyx_m;
  #endif
}

/* Runtime support code */

static PyObject *__Pyx_GetName(PyObject *dict, PyObject *name) {
    PyObject *result;
    result = PyObject_GetAttr(dict, name);
    if (!result)
        PyErr_SetObject(PyExc_NameError, name);
    return result;
}

static void __Pyx_RaiseArgtupleInvalid(
    const char* func_name,
    int exact,
    Py_ssize_t num_min,
    Py_ssize_t num_max,
    Py_ssize_t num_found)
{
    Py_ssize_t num_expected;
    const char *number, *more_or_less;

    if (num_found < num_min) {
        num_expected = num_min;
        more_or_less = "at least";
    } else {
        num_expected = num_max;
        more_or_less = "at most";
    }
    if (exact) {
        more_or_less = "exactly";
    }
    number = (num_expected == 1) ? "" : "s";
    PyErr_Format(PyExc_TypeError,
        #if PY_VERSION_HEX < 0x02050000
            "%s() takes %s %d positional argument%s (%d given)",
        #else
            "%s() takes %s %zd positional argument%s (%zd given)",
        #endif
        func_name, more_or_less, num_expected, number, num_found);
}

static void __Pyx_RaiseDoubleKeywordsError(
    const char* func_name,
    PyObject* kw_name)
{
    PyErr_Format(PyExc_TypeError,
        #if PY_MAJOR_VERSION >= 3
        "%s() got multiple values for keyword argument '%U'", func_name, kw_name);
        #else
        "%s() got multiple values for keyword argument '%s'", func_name,
        PyString_AS_STRING(kw_name));
        #endif
}

static int __Pyx_ParseOptionalKeywords(
    PyObject *kwds,
    PyObject **argnames[],
    PyObject *kwds2,
    PyObject *values[],
    Py_ssize_t num_pos_args,
    const char* function_name)
{
    PyObject *key = 0, *value = 0;
    Py_ssize_t pos = 0;
    PyObject*** name;
    PyObject*** first_kw_arg = argnames + num_pos_args;

    while (PyDict_Next(kwds, &pos, &key, &value)) {
        name = first_kw_arg;
        while (*name && (**name != key)) name++;
        if (*name) {
            values[name-argnames] = value;
        } else {
            #if PY_MAJOR_VERSION < 3
            if (unlikely(!PyString_CheckExact(key)) && unlikely(!PyString_Check(key))) {
            #else
            if (unlikely(!PyUnicode_CheckExact(key)) && unlikely(!PyUnicode_Check(key))) {
            #endif
                goto invalid_keyword_type;
            } else {
                for (name = first_kw_arg; *name; name++) {
                    #if PY_MAJOR_VERSION >= 3
                    if (PyUnicode_GET_SIZE(**name) == PyUnicode_GET_SIZE(key) &&
                        PyUnicode_Compare(**name, key) == 0) break;
                    #else
                    if (PyString_GET_SIZE(**name) == PyString_GET_SIZE(key) &&
                        _PyString_Eq(**name, key)) break;
                    #endif
                }
                if (*name) {
                    values[name-argnames] = value;
                } else {
                    /* unexpected keyword found */
                    for (name=argnames; name != first_kw_arg; name++) {
                        if (**name == key) goto arg_passed_twice;
                        #if PY_MAJOR_VERSION >= 3
                        if (PyUnicode_GET_SIZE(**name) == PyUnicode_GET_SIZE(key) &&
                            PyUnicode_Compare(**name, key) == 0) goto arg_passed_twice;
                        #else
                        if (PyString_GET_SIZE(**name) == PyString_GET_SIZE(key) &&
                            _PyString_Eq(**name, key)) goto arg_passed_twice;
                        #endif
                    }
                    if (kwds2) {
                        if (unlikely(PyDict_SetItem(kwds2, key, value))) goto bad;
                    } else {
                        goto invalid_keyword;
                    }
                }
            }
        }
    }
    return 0;
arg_passed_twice:
    __Pyx_RaiseDoubleKeywordsError(function_name, **name);
    goto bad;
invalid_keyword_type:
    PyErr_Format(PyExc_TypeError,
        "%s() keywords must be strings", function_name);
    goto bad;
invalid_keyword:
    PyErr_Format(PyExc_TypeError,
    #if PY_MAJOR_VERSION < 3
        "%s() got an unexpected keyword argument '%s'",
        function_name, PyString_AsString(key));
    #else
        "%s() got an unexpected keyword argument '%U'",
        function_name, key);
    #endif
bad:
    return -1;
}

static CYTHON_INLINE void __Pyx_ErrRestore(PyObject *type, PyObject *value, PyObject *tb) {
    PyObject *tmp_type, *tmp_value, *tmp_tb;
    PyThreadState *tstate = PyThreadState_GET();

    tmp_type = tstate->curexc_type;
    tmp_value = tstate->curexc_value;
    tmp_tb = tstate->curexc_traceback;
    tstate->curexc_type = type;
    tstate->curexc_value = value;
    tstate->curexc_traceback = tb;
    Py_XDECREF(tmp_type);
    Py_XDECREF(tmp_value);
    Py_XDECREF(tmp_tb);
}

static CYTHON_INLINE void __Pyx_ErrFetch(PyObject **type, PyObject **value, PyObject **tb) {
    PyThreadState *tstate = PyThreadState_GET();
    *type = tstate->curexc_type;
    *value = tstate->curexc_value;
    *tb = tstate->curexc_traceback;

    tstate->curexc_type = 0;
    tstate->curexc_value = 0;
    tstate->curexc_traceback = 0;
}


#if PY_MAJOR_VERSION < 3
static void __Pyx_Raise(PyObject *type, PyObject *value, PyObject *tb) {
    Py_XINCREF(type);
    Py_XINCREF(value);
    Py_XINCREF(tb);
    /* First, check the traceback argument, replacing None with NULL. */
    if (tb == Py_None) {
        Py_DECREF(tb);
        tb = 0;
    }
    else if (tb != NULL && !PyTraceBack_Check(tb)) {
        PyErr_SetString(PyExc_TypeError,
            "raise: arg 3 must be a traceback or None");
        goto raise_error;
    }
    /* Next, replace a missing value with None */
    if (value == NULL) {
        value = Py_None;
        Py_INCREF(value);
    }
    #if PY_VERSION_HEX < 0x02050000
    if (!PyClass_Check(type))
    #else
    if (!PyType_Check(type))
    #endif
    {
        /* Raising an instance.  The value should be a dummy. */
        if (value != Py_None) {
            PyErr_SetString(PyExc_TypeError,
                "instance exception may not have a separate value");
            goto raise_error;
        }
        /* Normalize to raise <class>, <instance> */
        Py_DECREF(value);
        value = type;
        #if PY_VERSION_HEX < 0x02050000
            if (PyInstance_Check(type)) {
                type = (PyObject*) ((PyInstanceObject*)type)->in_class;
                Py_INCREF(type);
            }
            else {
                type = 0;
                PyErr_SetString(PyExc_TypeError,
                    "raise: exception must be an old-style class or instance");
                goto raise_error;
            }
        #else
            type = (PyObject*) Py_TYPE(type);
            Py_INCREF(type);
            if (!PyType_IsSubtype((PyTypeObject *)type, (PyTypeObject *)PyExc_BaseException)) {
                PyErr_SetString(PyExc_TypeError,
                    "raise: exception class must be a subclass of BaseException");
                goto raise_error;
            }
        #endif
    }

    __Pyx_ErrRestore(type, value, tb);
    return;
raise_error:
    Py_XDECREF(value);
    Py_XDECREF(type);
    Py_XDECREF(tb);
    return;
}

#else /* Python 3+ */

static void __Pyx_Raise(PyObject *type, PyObject *value, PyObject *tb) {
    if (tb == Py_None) {
        tb = 0;
    } else if (tb && !PyTraceBack_Check(tb)) {
        PyErr_SetString(PyExc_TypeError,
            "raise: arg 3 must be a traceback or None");
        goto bad;
    }
    if (value == Py_None)
        value = 0;

    if (PyExceptionInstance_Check(type)) {
        if (value) {
            PyErr_SetString(PyExc_TypeError,
                "instance exception may not have a separate value");
            goto bad;
        }
        value = type;
        type = (PyObject*) Py_TYPE(value);
    } else if (!PyExceptionClass_Check(type)) {
        PyErr_SetString(PyExc_TypeError,
            "raise: exception class must be a subclass of BaseException");
        goto bad;
    }

    PyErr_SetObject(type, value);

    if (tb) {
        PyThreadState *tstate = PyThreadState_GET();
        PyObject* tmp_tb = tstate->curexc_traceback;
        if (tb != tmp_tb) {
            Py_INCREF(tb);
            tstate->curexc_traceback = tb;
            Py_XDECREF(tmp_tb);
        }
    }

bad:
    return;
}
#endif

static CYTHON_INLINE unsigned char __Pyx_PyInt_AsUnsignedChar(PyObject* x) {
    const unsigned char neg_one = (unsigned char)-1, const_zero = 0;
    const int is_unsigned = neg_one > const_zero;
    if (sizeof(unsigned char) < sizeof(long)) {
        long val = __Pyx_PyInt_AsLong(x);
        if (unlikely(val != (long)(unsigned char)val)) {
            if (!unlikely(val == -1 && PyErr_Occurred())) {
                PyErr_SetString(PyExc_OverflowError,
                    (is_unsigned && unlikely(val < 0)) ?
                    "can't convert negative value to unsigned char" :
                    "value too large to convert to unsigned char");
            }
            return (unsigned char)-1;
        }
        return (unsigned char)val;
    }
    return (unsigned char)__Pyx_PyInt_AsUnsignedLong(x);
}

static CYTHON_INLINE unsigned short __Pyx_PyInt_AsUnsignedShort(PyObject* x) {
    const unsigned short neg_one = (unsigned short)-1, const_zero = 0;
    const int is_unsigned = neg_one > const_zero;
    if (sizeof(unsigned short) < sizeof(long)) {
        long val = __Pyx_PyInt_AsLong(x);
        if (unlikely(val != (long)(unsigned short)val)) {
            if (!unlikely(val == -1 && PyErr_Occurred())) {
                PyErr_SetString(PyExc_OverflowError,
                    (is_unsigned && unlikely(val < 0)) ?
                    "can't convert negative value to unsigned short" :
                    "value too large to convert to unsigned short");
            }
            return (unsigned short)-1;
        }
        return (unsigned short)val;
    }
    return (unsigned short)__Pyx_PyInt_AsUnsignedLong(x);
}

static CYTHON_INLINE unsigned int __Pyx_PyInt_AsUnsignedInt(PyObject* x) {
    const unsigned int neg_one = (unsigned int)-1, const_zero = 0;
    const int is_unsigned = neg_one > const_zero;
    if (sizeof(unsigned int) < sizeof(long)) {
        long val = __Pyx_PyInt_AsLong(x);
        if (unlikely(val != (long)(unsigned int)val)) {
            if (!unlikely(val == -1 && PyErr_Occurred())) {
                PyErr_SetString(PyExc_OverflowError,
                    (is_unsigned && unlikely(val < 0)) ?
                    "can't convert negative value to unsigned int" :
                    "value too large to convert to unsigned int");
            }
            return (unsigned int)-1;
        }
        return (unsigned int)val;
    }
    return (unsigned int)__Pyx_PyInt_AsUnsignedLong(x);
}

static CYTHON_INLINE char __Pyx_PyInt_AsChar(PyObject* x) {
    const char neg_one = (char)-1, const_zero = 0;
    const int is_unsigned = neg_one > const_zero;
    if (sizeof(char) < sizeof(long)) {
        long val = __Pyx_PyInt_AsLong(x);
        if (unlikely(val != (long)(char)val)) {
            if (!unlikely(val == -1 && PyErr_Occurred())) {
                PyErr_SetString(PyExc_OverflowError,
                    (is_unsigned && unlikely(val < 0)) ?
                    "can't convert negative value to char" :
                    "value too large to convert to char");
            }
            return (char)-1;
        }
        return (char)val;
    }
    return (char)__Pyx_PyInt_AsLong(x);
}

static CYTHON_INLINE short __Pyx_PyInt_AsShort(PyObject* x) {
    const short neg_one = (short)-1, const_zero = 0;
    const int is_unsigned = neg_one > const_zero;
    if (sizeof(short) < sizeof(long)) {
        long val = __Pyx_PyInt_AsLong(x);
        if (unlikely(val != (long)(short)val)) {
            if (!unlikely(val == -1 && PyErr_Occurred())) {
                PyErr_SetString(PyExc_OverflowError,
                    (is_unsigned && unlikely(val < 0)) ?
                    "can't convert negative value to short" :
                    "value too large to convert to short");
            }
            return (short)-1;
        }
        return (short)val;
    }
    return (short)__Pyx_PyInt_AsLong(x);
}

static CYTHON_INLINE int __Pyx_PyInt_AsInt(PyObject* x) {
    const int neg_one = (int)-1, const_zero = 0;
    const int is_unsigned = neg_one > const_zero;
    if (sizeof(int) < sizeof(long)) {
        long val = __Pyx_PyInt_AsLong(x);
        if (unlikely(val != (long)(int)val)) {
            if (!unlikely(val == -1 && PyErr_Occurred())) {
                PyErr_SetString(PyExc_OverflowError,
                    (is_unsigned && unlikely(val < 0)) ?
                    "can't convert negative value to int" :
                    "value too large to convert to int");
            }
            return (int)-1;
        }
        return (int)val;
    }
    return (int)__Pyx_PyInt_AsLong(x);
}

static CYTHON_INLINE signed char __Pyx_PyInt_AsSignedChar(PyObject* x) {
    const signed char neg_one = (signed char)-1, const_zero = 0;
    const int is_unsigned = neg_one > const_zero;
    if (sizeof(signed char) < sizeof(long)) {
        long val = __Pyx_PyInt_AsLong(x);
        if (unlikely(val != (long)(signed char)val)) {
            if (!unlikely(val == -1 && PyErr_Occurred())) {
                PyErr_SetString(PyExc_OverflowError,
                    (is_unsigned && unlikely(val < 0)) ?
                    "can't convert negative value to signed char" :
                    "value too large to convert to signed char");
            }
            return (signed char)-1;
        }
        return (signed char)val;
    }
    return (signed char)__Pyx_PyInt_AsSignedLong(x);
}

static CYTHON_INLINE signed short __Pyx_PyInt_AsSignedShort(PyObject* x) {
    const signed short neg_one = (signed short)-1, const_zero = 0;
    const int is_unsigned = neg_one > const_zero;
    if (sizeof(signed short) < sizeof(long)) {
        long val = __Pyx_PyInt_AsLong(x);
        if (unlikely(val != (long)(signed short)val)) {
            if (!unlikely(val == -1 && PyErr_Occurred())) {
                PyErr_SetString(PyExc_OverflowError,
                    (is_unsigned && unlikely(val < 0)) ?
                    "can't convert negative value to signed short" :
                    "value too large to convert to signed short");
            }
            return (signed short)-1;
        }
        return (signed short)val;
    }
    return (signed short)__Pyx_PyInt_AsSignedLong(x);
}

static CYTHON_INLINE signed int __Pyx_PyInt_AsSignedInt(PyObject* x) {
    const signed int neg_one = (signed int)-1, const_zero = 0;
    const int is_unsigned = neg_one > const_zero;
    if (sizeof(signed int) < sizeof(long)) {
        long val = __Pyx_PyInt_AsLong(x);
        if (unlikely(val != (long)(signed int)val)) {
            if (!unlikely(val == -1 && PyErr_Occurred())) {
                PyErr_SetString(PyExc_OverflowError,
                    (is_unsigned && unlikely(val < 0)) ?
                    "can't convert negative value to signed int" :
                    "value too large to convert to signed int");
            }
            return (signed int)-1;
        }
        return (signed int)val;
    }
    return (signed int)__Pyx_PyInt_AsSignedLong(x);
}

static CYTHON_INLINE int __Pyx_PyInt_AsLongDouble(PyObject* x) {
    const int neg_one = (int)-1, const_zero = 0;
    const int is_unsigned = neg_one > const_zero;
    if (sizeof(int) < sizeof(long)) {
        long val = __Pyx_PyInt_AsLong(x);
        if (unlikely(val != (long)(int)val)) {
            if (!unlikely(val == -1 && PyErr_Occurred())) {
                PyErr_SetString(PyExc_OverflowError,
                    (is_unsigned && unlikely(val < 0)) ?
                    "can't convert negative value to int" :
                    "value too large to convert to int");
            }
            return (int)-1;
        }
        return (int)val;
    }
    return (int)__Pyx_PyInt_AsLong(x);
}

static CYTHON_INLINE unsigned long __Pyx_PyInt_AsUnsignedLong(PyObject* x) {
    const unsigned long neg_one = (unsigned long)-1, const_zero = 0;
    const int is_unsigned = neg_one > const_zero;
#if PY_VERSION_HEX < 0x03000000
    if (likely(PyInt_Check(x))) {
        long val = PyInt_AS_LONG(x);
        if (is_unsigned && unlikely(val < 0)) {
            PyErr_SetString(PyExc_OverflowError,
                            "can't convert negative value to unsigned long");
            return (unsigned long)-1;
        }
        return (unsigned long)val;
    } else
#endif
    if (likely(PyLong_Check(x))) {
        if (is_unsigned) {
            if (unlikely(Py_SIZE(x) < 0)) {
                PyErr_SetString(PyExc_OverflowError,
                                "can't convert negative value to unsigned long");
                return (unsigned long)-1;
            }
            return PyLong_AsUnsignedLong(x);
        } else {
            return PyLong_AsLong(x);
        }
    } else {
        unsigned long val;
        PyObject *tmp = __Pyx_PyNumber_Int(x);
        if (!tmp) return (unsigned long)-1;
        val = __Pyx_PyInt_AsUnsignedLong(tmp);
        Py_DECREF(tmp);
        return val;
    }
}

static CYTHON_INLINE unsigned PY_LONG_LONG __Pyx_PyInt_AsUnsignedLongLong(PyObject* x) {
    const unsigned PY_LONG_LONG neg_one = (unsigned PY_LONG_LONG)-1, const_zero = 0;
    const int is_unsigned = neg_one > const_zero;
#if PY_VERSION_HEX < 0x03000000
    if (likely(PyInt_Check(x))) {
        long val = PyInt_AS_LONG(x);
        if (is_unsigned && unlikely(val < 0)) {
            PyErr_SetString(PyExc_OverflowError,
                            "can't convert negative value to unsigned PY_LONG_LONG");
            return (unsigned PY_LONG_LONG)-1;
        }
        return (unsigned PY_LONG_LONG)val;
    } else
#endif
    if (likely(PyLong_Check(x))) {
        if (is_unsigned) {
            if (unlikely(Py_SIZE(x) < 0)) {
                PyErr_SetString(PyExc_OverflowError,
                                "can't convert negative value to unsigned PY_LONG_LONG");
                return (unsigned PY_LONG_LONG)-1;
            }
            return PyLong_AsUnsignedLongLong(x);
        } else {
            return PyLong_AsLongLong(x);
        }
    } else {
        unsigned PY_LONG_LONG val;
        PyObject *tmp = __Pyx_PyNumber_Int(x);
        if (!tmp) return (unsigned PY_LONG_LONG)-1;
        val = __Pyx_PyInt_AsUnsignedLongLong(tmp);
        Py_DECREF(tmp);
        return val;
    }
}

static CYTHON_INLINE long __Pyx_PyInt_AsLong(PyObject* x) {
    const long neg_one = (long)-1, const_zero = 0;
    const int is_unsigned = neg_one > const_zero;
#if PY_VERSION_HEX < 0x03000000
    if (likely(PyInt_Check(x))) {
        long val = PyInt_AS_LONG(x);
        if (is_unsigned && unlikely(val < 0)) {
            PyErr_SetString(PyExc_OverflowError,
                            "can't convert negative value to long");
            return (long)-1;
        }
        return (long)val;
    } else
#endif
    if (likely(PyLong_Check(x))) {
        if (is_unsigned) {
            if (unlikely(Py_SIZE(x) < 0)) {
                PyErr_SetString(PyExc_OverflowError,
                                "can't convert negative value to long");
                return (long)-1;
            }
            return PyLong_AsUnsignedLong(x);
        } else {
            return PyLong_AsLong(x);
        }
    } else {
        long val;
        PyObject *tmp = __Pyx_PyNumber_Int(x);
        if (!tmp) return (long)-1;
        val = __Pyx_PyInt_AsLong(tmp);
        Py_DECREF(tmp);
        return val;
    }
}

static CYTHON_INLINE PY_LONG_LONG __Pyx_PyInt_AsLongLong(PyObject* x) {
    const PY_LONG_LONG neg_one = (PY_LONG_LONG)-1, const_zero = 0;
    const int is_unsigned = neg_one > const_zero;
#if PY_VERSION_HEX < 0x03000000
    if (likely(PyInt_Check(x))) {
        long val = PyInt_AS_LONG(x);
        if (is_unsigned && unlikely(val < 0)) {
            PyErr_SetString(PyExc_OverflowError,
                            "can't convert negative value to PY_LONG_LONG");
            return (PY_LONG_LONG)-1;
        }
        return (PY_LONG_LONG)val;
    } else
#endif
    if (likely(PyLong_Check(x))) {
        if (is_unsigned) {
            if (unlikely(Py_SIZE(x) < 0)) {
                PyErr_SetString(PyExc_OverflowError,
                                "can't convert negative value to PY_LONG_LONG");
                return (PY_LONG_LONG)-1;
            }
            return PyLong_AsUnsignedLongLong(x);
        } else {
            return PyLong_AsLongLong(x);
        }
    } else {
        PY_LONG_LONG val;
        PyObject *tmp = __Pyx_PyNumber_Int(x);
        if (!tmp) return (PY_LONG_LONG)-1;
        val = __Pyx_PyInt_AsLongLong(tmp);
        Py_DECREF(tmp);
        return val;
    }
}

static CYTHON_INLINE signed long __Pyx_PyInt_AsSignedLong(PyObject* x) {
    const signed long neg_one = (signed long)-1, const_zero = 0;
    const int is_unsigned = neg_one > const_zero;
#if PY_VERSION_HEX < 0x03000000
    if (likely(PyInt_Check(x))) {
        long val = PyInt_AS_LONG(x);
        if (is_unsigned && unlikely(val < 0)) {
            PyErr_SetString(PyExc_OverflowError,
                            "can't convert negative value to signed long");
            return (signed long)-1;
        }
        return (signed long)val;
    } else
#endif
    if (likely(PyLong_Check(x))) {
        if (is_unsigned) {
            if (unlikely(Py_SIZE(x) < 0)) {
                PyErr_SetString(PyExc_OverflowError,
                                "can't convert negative value to signed long");
                return (signed long)-1;
            }
            return PyLong_AsUnsignedLong(x);
        } else {
            return PyLong_AsLong(x);
        }
    } else {
        signed long val;
        PyObject *tmp = __Pyx_PyNumber_Int(x);
        if (!tmp) return (signed long)-1;
        val = __Pyx_PyInt_AsSignedLong(tmp);
        Py_DECREF(tmp);
        return val;
    }
}

static CYTHON_INLINE signed PY_LONG_LONG __Pyx_PyInt_AsSignedLongLong(PyObject* x) {
    const signed PY_LONG_LONG neg_one = (signed PY_LONG_LONG)-1, const_zero = 0;
    const int is_unsigned = neg_one > const_zero;
#if PY_VERSION_HEX < 0x03000000
    if (likely(PyInt_Check(x))) {
        long val = PyInt_AS_LONG(x);
        if (is_unsigned && unlikely(val < 0)) {
            PyErr_SetString(PyExc_OverflowError,
                            "can't convert negative value to signed PY_LONG_LONG");
            return (signed PY_LONG_LONG)-1;
        }
        return (signed PY_LONG_LONG)val;
    } else
#endif
    if (likely(PyLong_Check(x))) {
        if (is_unsigned) {
            if (unlikely(Py_SIZE(x) < 0)) {
                PyErr_SetString(PyExc_OverflowError,
                                "can't convert negative value to signed PY_LONG_LONG");
                return (signed PY_LONG_LONG)-1;
            }
            return PyLong_AsUnsignedLongLong(x);
        } else {
            return PyLong_AsLongLong(x);
        }
    } else {
        signed PY_LONG_LONG val;
        PyObject *tmp = __Pyx_PyNumber_Int(x);
        if (!tmp) return (signed PY_LONG_LONG)-1;
        val = __Pyx_PyInt_AsSignedLongLong(tmp);
        Py_DECREF(tmp);
        return val;
    }
}

static CYTHON_INLINE off_t __Pyx_PyInt_from_py_off_t(PyObject* x) {
    const off_t neg_one = (off_t)-1, const_zero = (off_t)0;
    const int is_unsigned = const_zero < neg_one;
    if (sizeof(off_t) == sizeof(char)) {
        if (is_unsigned)
            return (off_t)__Pyx_PyInt_AsUnsignedChar(x);
        else
            return (off_t)__Pyx_PyInt_AsSignedChar(x);
    } else if (sizeof(off_t) == sizeof(short)) {
        if (is_unsigned)
            return (off_t)__Pyx_PyInt_AsUnsignedShort(x);
        else
            return (off_t)__Pyx_PyInt_AsSignedShort(x);
    } else if (sizeof(off_t) == sizeof(int)) {
        if (is_unsigned)
            return (off_t)__Pyx_PyInt_AsUnsignedInt(x);
        else
            return (off_t)__Pyx_PyInt_AsSignedInt(x);
    } else if (sizeof(off_t) == sizeof(long)) {
        if (is_unsigned)
            return (off_t)__Pyx_PyInt_AsUnsignedLong(x);
        else
            return (off_t)__Pyx_PyInt_AsSignedLong(x);
    } else if (sizeof(off_t) == sizeof(PY_LONG_LONG)) {
        if (is_unsigned)
            return (off_t)__Pyx_PyInt_AsUnsignedLongLong(x);
        else
            return (off_t)__Pyx_PyInt_AsSignedLongLong(x);
    }  else {
        off_t val;
        PyObject *v = __Pyx_PyNumber_Int(x);
        #if PY_VERSION_HEX < 0x03000000
        if (likely(v) && !PyLong_Check(v)) {
            PyObject *tmp = v;
            v = PyNumber_Long(tmp);
            Py_DECREF(tmp);
        }
        #endif
        if (likely(v)) {
            int one = 1; int is_little = (int)*(unsigned char *)&one;
            unsigned char *bytes = (unsigned char *)&val;
            int ret = _PyLong_AsByteArray((PyLongObject *)v,
                                          bytes, sizeof(val),
                                          is_little, !is_unsigned);
            Py_DECREF(v);
            if (likely(!ret))
                return val;
        }
        return (off_t)-1;
    }
}

#include "compile.h"
#include "frameobject.h"
#include "traceback.h"

static void __Pyx_AddTraceback(const char *funcname) {
    PyObject *py_srcfile = 0;
    PyObject *py_funcname = 0;
    PyObject *py_globals = 0;
    PyCodeObject *py_code = 0;
    PyFrameObject *py_frame = 0;

    #if PY_MAJOR_VERSION < 3
    py_srcfile = PyString_FromString(__pyx_filename);
    #else
    py_srcfile = PyUnicode_FromString(__pyx_filename);
    #endif
    if (!py_srcfile) goto bad;
    if (__pyx_clineno) {
        #if PY_MAJOR_VERSION < 3
        py_funcname = PyString_FromFormat( "%s (%s:%d)", funcname, __pyx_cfilenm, __pyx_clineno);
        #else
        py_funcname = PyUnicode_FromFormat( "%s (%s:%d)", funcname, __pyx_cfilenm, __pyx_clineno);
        #endif
    }
    else {
        #if PY_MAJOR_VERSION < 3
        py_funcname = PyString_FromString(funcname);
        #else
        py_funcname = PyUnicode_FromString(funcname);
        #endif
    }
    if (!py_funcname) goto bad;
    py_globals = PyModule_GetDict(__pyx_m);
    if (!py_globals) goto bad;
    py_code = PyCode_New(
        0,            /*int argcount,*/
        #if PY_MAJOR_VERSION >= 3
        0,            /*int kwonlyargcount,*/
        #endif
        0,            /*int nlocals,*/
        0,            /*int stacksize,*/
        0,            /*int flags,*/
        __pyx_empty_bytes, /*PyObject *code,*/
        __pyx_empty_tuple,  /*PyObject *consts,*/
        __pyx_empty_tuple,  /*PyObject *names,*/
        __pyx_empty_tuple,  /*PyObject *varnames,*/
        __pyx_empty_tuple,  /*PyObject *freevars,*/
        __pyx_empty_tuple,  /*PyObject *cellvars,*/
        py_srcfile,   /*PyObject *filename,*/
        py_funcname,  /*PyObject *name,*/
        __pyx_lineno,   /*int firstlineno,*/
        __pyx_empty_bytes  /*PyObject *lnotab*/
    );
    if (!py_code) goto bad;
    py_frame = PyFrame_New(
        PyThreadState_GET(), /*PyThreadState *tstate,*/
        py_code,             /*PyCodeObject *code,*/
        py_globals,          /*PyObject *globals,*/
        0                    /*PyObject *locals*/
    );
    if (!py_frame) goto bad;
    py_frame->f_lineno = __pyx_lineno;
    PyTraceBack_Here(py_frame);
bad:
    Py_XDECREF(py_srcfile);
    Py_XDECREF(py_funcname);
    Py_XDECREF(py_code);
    Py_XDECREF(py_frame);
}

static int __Pyx_InitStrings(__Pyx_StringTabEntry *t) {
    while (t->p) {
        #if PY_MAJOR_VERSION < 3
        if (t->is_unicode) {
            *t->p = PyUnicode_DecodeUTF8(t->s, t->n - 1, NULL);
        } else if (t->intern) {
            *t->p = PyString_InternFromString(t->s);
        } else {
            *t->p = PyString_FromStringAndSize(t->s, t->n - 1);
        }
        #else  /* Python 3+ has unicode identifiers */
        if (t->is_unicode | t->is_str) {
            if (t->intern) {
                *t->p = PyUnicode_InternFromString(t->s);
            } else if (t->encoding) {
                *t->p = PyUnicode_Decode(t->s, t->n - 1, t->encoding, NULL);
            } else {
                *t->p = PyUnicode_FromStringAndSize(t->s, t->n - 1);
            }
        } else {
            *t->p = PyBytes_FromStringAndSize(t->s, t->n - 1);
        }
        #endif
        if (!*t->p)
            return -1;
        ++t;
    }
    return 0;
}

/* Type Conversion Functions */

static CYTHON_INLINE int __Pyx_PyObject_IsTrue(PyObject* x) {
   int is_true = x == Py_True;
   if (is_true | (x == Py_False) | (x == Py_None)) return is_true;
   else return PyObject_IsTrue(x);
}

static CYTHON_INLINE PyObject* __Pyx_PyNumber_Int(PyObject* x) {
  PyNumberMethods *m;
  const char *name = NULL;
  PyObject *res = NULL;
#if PY_VERSION_HEX < 0x03000000
  if (PyInt_Check(x) || PyLong_Check(x))
#else
  if (PyLong_Check(x))
#endif
    return Py_INCREF(x), x;
  m = Py_TYPE(x)->tp_as_number;
#if PY_VERSION_HEX < 0x03000000
  if (m && m->nb_int) {
    name = "int";
    res = PyNumber_Int(x);
  }
  else if (m && m->nb_long) {
    name = "long";
    res = PyNumber_Long(x);
  }
#else
  if (m && m->nb_int) {
    name = "int";
    res = PyNumber_Long(x);
  }
#endif
  if (res) {
#if PY_VERSION_HEX < 0x03000000
    if (!PyInt_Check(res) && !PyLong_Check(res)) {
#else
    if (!PyLong_Check(res)) {
#endif
      PyErr_Format(PyExc_TypeError,
                   "__%s__ returned non-%s (type %.200s)",
                   name, name, Py_TYPE(res)->tp_name);
      Py_DECREF(res);
      return NULL;
    }
  }
  else if (!PyErr_Occurred()) {
    PyErr_SetString(PyExc_TypeError,
                    "an integer is required");
  }
  return res;
}

static CYTHON_INLINE Py_ssize_t __Pyx_PyIndex_AsSsize_t(PyObject* b) {
  Py_ssize_t ival;
  PyObject* x = PyNumber_Index(b);
  if (!x) return -1;
  ival = PyInt_AsSsize_t(x);
  Py_DECREF(x);
  return ival;
}

static CYTHON_INLINE PyObject * __Pyx_PyInt_FromSize_t(size_t ival) {
#if PY_VERSION_HEX < 0x02050000
   if (ival <= LONG_MAX)
       return PyInt_FromLong((long)ival);
   else {
       unsigned char *bytes = (unsigned char *) &ival;
       int one = 1; int little = (int)*(unsigned char*)&one;
       return _PyLong_FromByteArray(bytes, sizeof(size_t), little, 0);
   }
#else
   return PyInt_FromSize_t(ival);
#endif
}

static CYTHON_INLINE size_t __Pyx_PyInt_AsSize_t(PyObject* x) {
   unsigned PY_LONG_LONG val = __Pyx_PyInt_AsUnsignedLongLong(x);
   if (unlikely(val == (unsigned PY_LONG_LONG)-1 && PyErr_Occurred())) {
       return (size_t)-1;
   } else if (unlikely(val != (unsigned PY_LONG_LONG)(size_t)val)) {
       PyErr_SetString(PyExc_OverflowError,
                       "value too large to convert to size_t");
       return (size_t)-1;
   }
   return (size_t)val;
}


#endif /* Py_PYTHON_H */
