#ifndef INCLUDE_JNI_HELPER_H_
#define INCLUDE_JNI_HELPER_H_

#define jlong_to_ptr(value) ((void*)((long)value))
#define ptr_to_jlong(value) (jlong)((long)(value))

#endif /* INCLUDE_JNI_HELPER_H_ */
