#pragma once 

#ifndef NULL
#define NULL 0
#endif 

#define FMINSTRLEN  3
#define FMINRLELEN  1

// - значения в поле смещения для специальных символов
enum { FBASEOFFFLAG, FRAWOFFFLAG, FCNTOFFFLAG1, FCNTOFFFLAG2, FCNTOFFFLAG3, FRLEOFFFLAG, FRESERVEDOFF };
enum { FOFFIDXBITS = 7 };
