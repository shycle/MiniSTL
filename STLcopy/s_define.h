#pragma once
typedef __int64          ptrdiff_t;
// Some compilers lack the features that are necessary for concept checks.
// On those compilers we define the concept check macros to do nothing.
#define __STL_REQUIRES(__type_var, __concept) do {} while(0)
#define __STL_CLASS_REQUIRES(__type_var, __concept) \
  static int  __##__type_var##_##__concept
#define __STL_CONVERTIBLE(__type_x, __type_y) do {} while(0)
#define __STL_REQUIRES_SAME_TYPE(__type_x, __type_y) do {} while(0)
#define __STL_CLASS_REQUIRES_SAME_TYPE(__type_x, __type_y) \
  static int  __##__type_x##__type_y##_require_same_type
#define __STL_GENERATOR_CHECK(__func, __ret) do {} while(0)
#define __STL_CLASS_GENERATOR_CHECK(__func, __ret) \
  static int  __##__func##__ret##_generator_check
#define __STL_UNARY_FUNCTION_CHECK(__func, __ret, __arg) do {} while(0)
#define __STL_CLASS_UNARY_FUNCTION_CHECK(__func, __ret, __arg) \
  static int  __##__func##__ret##__arg##_unary_function_check
#define __STL_BINARY_FUNCTION_CHECK(__func, __ret, __first, __second) \
  do {} while(0)
#define __STL_CLASS_BINARY_FUNCTION_CHECK(__func, __ret, __first, __second) \
  static int  __##__func##__ret##__first##__second##_binary_function_check
#define __STL_REQUIRES_BINARY_OP(__opname, __ret, __first, __second) \
  do {} while(0)
#define __STL_CLASS_REQUIRES_BINARY_OP(__opname, __ret, __first, __second) \
  static int __##__opname##__ret##__first##__second##_require_binary_op

