#ifndef __NEODAIN_FUNC_DEF__
#define __NEODAIN_FUNC_DEF__

const int MAX_FUNC_TYPE_NAME 128

typedef void (*Func)(char* pFileName, char* pFuncType);

typedef struct funcTypeMap_t {
    char funcType[MAX_FUNC_TYPE_NAME];
    Func run;
} FuncTypeMap;

extern FuncTypeMap funcMap[];

Func searchFunc(char* pFuncType);

#endif
