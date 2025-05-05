#ifndef COMMON_H
#define COMMON_H

#include "include_asm.h"

#define NULL ((void *) 0)

typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long s64;
typedef signed long s128 __attribute__((mode(TI)));

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;
typedef unsigned long u128 __attribute__((mode(TI)));

typedef float f32;
typedef double f64;

typedef volatile s8 vs8;
typedef volatile s16 vs16;
typedef volatile s32 vs32;
typedef volatile s64 vs64;
typedef volatile s128 vs128;

typedef volatile u8 vu8;
typedef volatile u16 vu16;
typedef volatile u32 vu32;
typedef volatile u64 vu64;
typedef volatile u128 vu128;

typedef volatile float vf32;
typedef volatile double vf64;

// SCE types
typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;
typedef unsigned long u_long;
typedef unsigned long u_long128 __attribute__((mode(TI)));

typedef int qword[4] __attribute__((aligned(16)));
typedef int sceVu0IVECTOR[4] __attribute__((aligned(16)));
typedef int sceVu0IMATRIX[4][4] __attribute__((aligned(16)));
typedef float sceVu0FVECTOR[4] __attribute__((aligned(16)));
typedef float sceVu0FMATRIX[4][4] __attribute__((aligned(16)));

typedef void (*sceSifEndFunc)(/* parameters unknown */);
typedef void *(*sceSifRpcFunc)(/* parameters unknown */);

#define UNK_TYPE s32
#define UNK_PTR void *
#define UNK_RET void
#define UNK_FUN_ARG void (*)(void)
#define UNK_FUN_PTR(name) void (*name)(void)
#define UNK_ARGS

#define TRUE 1
#define FALSE 0

#define ARRAYSIZEU(arr) (sizeof(arr) / sizeof(arr[0]))
#define ARRAYSIZE(arr) (s32)(sizeof(arr) / sizeof(arr[0]))
#define CLEAR(x)                                                               \
  {                                                                            \
    u_int i;                                                                   \
    char *p = (char *) &x;                                                     \
    for (i = 0; i < sizeof(x); i++)                                            \
      *p++ = 0;                                                                \
  }

struct XVECTOR
{// 0x10
  /* 0x0 */ float x;
  /* 0x4 */ float y;
  /* 0x8 */ float z;
  /* 0xc */ float w;

  // float *operator();
  XVECTOR();
  ~XVECTOR();
  XVECTOR &operator=(const XVECTOR &rval);
};

struct XMATRIX
{// 0x40
  /* 0x00 */ union
  {// 0x40
    /* 0x00 */ struct
    {// 0x40
      /* 0x00 */ float __11;
      /* 0x04 */ float __12;
      /* 0x08 */ float __13;
      /* 0x0c */ float __14;
      /* 0x10 */ float __21;
      /* 0x14 */ float __22;
      /* 0x18 */ float __23;
      /* 0x1c */ float __24;
      /* 0x20 */ float __31;
      /* 0x24 */ float __32;
      /* 0x28 */ float __33;
      /* 0x2c */ float __34;
      /* 0x30 */ float __41;
      /* 0x34 */ float __42;
      /* 0x38 */ float __43;
      /* 0x3c */ float __44;
    };
    /* 0x00 */ float m[4][4];
  };

  XMATRIX &operator=(const XMATRIX &rval);
  XMATRIX();
  ~XMATRIX();
};

typedef XVECTOR &RXVECTOR;
typedef XMATRIX &RXMATRIX;

struct _sif_rpc_data
{
  void *paddr;
  unsigned int pid;
  int tid;
  unsigned int mode;
};

typedef _sif_rpc_data sceSifRpcData;

struct _sif_client_data
{
  _sif_rpc_data rpcd;
  unsigned int command;
  void *buff;
  void *gp;
  sceSifEndFunc func;
  void *para;

  /// _sif_serve_data
  void *serve;
};

typedef _sif_client_data sceSifClientData;

struct _sif_serve_data
{
  unsigned int command;
  sceSifRpcFunc func;
  void *buff;
  int size;
  sceSifRpcFunc cfunc;
  void *cbuff;
  int csize;
  sceSifClientData *client;
  void *paddr;
  unsigned int fno;
  void *receive;
  int rsize;
  int rmode;
  unsigned int rid;
  _sif_serve_data *link;
  _sif_serve_data *next;

  /// _sif_queue_data
  void *base;
};

typedef _sif_serve_data sceSifServeData;

struct _sif_queue_data
{
  int key;
  int active;

  /// _sif_serve_data
  void *link;

  /// _sif_serve_data
  void *start;
  _sif_serve_data *end;
  _sif_queue_data *next;
};

typedef _sif_queue_data sceSifQueueData;

#endif /* COMMON_H */
