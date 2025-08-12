#define SetLineInfo(MESSAGE) _SetLineInfo(__FILE__, __LINE__, __FUNCTION__, MESSAGE)

#define g3dbg(EXPRESSION) g3ddbgPrintf("[G3DRETURN]%s(%d)(%s):%s\n", __FILE__, __LINE__, __FUNCTION__, EXPRESSION)

#define g3dbgMessage(...)                                                                                              \
    _SetLineInfo(__FILE__, __LINE__, __FUNCTION__, "");                                                                \
    g3ddbgAssert(false, __VA_ARGS__);

#define g3dbSetLineWarning(MESSAGE)                                                                                    \
    _SetLineInfo(__FILE__, __LINE__, __FUNCTION__, MESSAGE);                                                           \
    g3ddbgWarning(false, "");

#define ASSERT(cond, ...)                                                                                              \
    if (!(cond)) {                                                                                                     \
        _SetLineInfo(__FILE__, __LINE__, __FUNCTION__, #cond);                                                         \
        g3ddbgAssert(cond, __VA_ARGS__);                                                                               \
    }

#define g3ddbg_ASSERT_WARNING(cond, ...)                                                                               \
    if (!(cond)) {                                                                                                     \
        _SetLineInfo(__FILE__, __LINE__, __FUNCTION__, #cond);                                                         \
        g3ddbgWarning(cond, __VA_ARGS__);                                                                              \
    }

#define g3ddbg_ASSERT_RETURN(cond, ...)                                                                                \
    if (!(cond)) {                                                                                                     \
        SetLineInfo(#cond);                                                                                            \
        g3ddbgWarning(cond, "");                                                                                       \
        if (!(cond))                                                                                                   \
        {                                                                                                              \
            g3dbg(!#cond);                                                                                             \
            return;                                                                                                    \
        }                                                                                                              \
    }     

#define ASSERT_RETURN(cond, ...)                                                                                       \
    if (cond) {                                                                                                        \
        return;                                                                                                        \
    }  
