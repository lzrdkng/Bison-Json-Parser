#ifndef _JSON_TEST_STRUCT_H
#define _JSON_TEST_STRUCT_H

/*=============================================================================+
 |                                  Includes                                   |
 +=============================================================================*/
#include <pthread.h>




/*=============================================================================+
 |                                   Macros                                    |
 +=============================================================================*/
#define INIT_WORKER(PTR, NAME, BUFF, OK)    \
  JSON_WorkerRetVal* PTR = calloc(1, sizeof(JSON_WorkerRetVal)); \
  PTR->name = NAME; \
  PTR->buff = BUFF; \
  PTR->ok = OK      \



typedef struct JSON_WorkerRetVal
{
  char ok: 1;
  const char* name;
  const char* buff;
} JSON_WorkerRetVal;




typedef struct JSON_WorkerArg
{
  long int flags;
} JSON_WorkerArg;




typedef struct JSON_Tester
{
  void* (*fn)(void*);
  JSON_WorkerArg arg;
} JSON_Tester;




typedef struct JSON_Worker
{
  pthread_t id;
  struct JSON_Worker* next;
} JSON_Worker;
#endif // _JSON_TEST_STRUCT
