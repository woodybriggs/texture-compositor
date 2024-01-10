#ifndef CSTR_H
#define CSTR_H

typedef const char * CStr;

typedef const char * CStr_Array[3];

#define len(static_array) (sizeof(static_array) / sizeof(static_array[0])) 


#endif
