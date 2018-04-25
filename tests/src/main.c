/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file main.c
 *
 * @brief Main entry point to C-Json tests utility.
 */

/*=============================================================================+
 |                                  Includes                                   |
 +=============================================================================*/
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>

#include "test-to-include.h"




/*=============================================================================+
 |                                   Macros                                    |
 +=============================================================================*/
#define SGR(X) "\033[" #X "m"
#define RESET  SGR(0)
#define xBOLD  SGR(1)
#define RED    SGR(31)
#define GREEN  SGR(32)
#define YELLOW SGR(33)
#define BOLD(X) xBOLD  X RESET
#define WARN(X) YELLOW X RESET
#define GOOD(X) GREEN  X RESET
#define FAIL(X) RED    X RESET



/*=============================================================================+
  |                                  Constants                                  |
  +=============================================================================*/
static const char OK[] = GOOD("[OK] ");
static const char NO[] = FAIL("[NO] ");






/*=============================================================================+
 |                                    Main                                     |
 +=============================================================================*/
void spawn_workers(JSON_Worker**);
void wait_workers(JSON_Worker**);



int main(int argc, char* argv[])
{
  JSON_Worker* head = NULL;

  /*  Number of worker to wait for  */
  spawn_workers(&head);

  /*  Wait for all workers to finish  */
  wait_workers(&head);

  return 0;
}




void spawn_workers(JSON_Worker** head)
{
  /*  Worker ID  */
  pthread_t id;

  /*  New Head of linked list of workers  */
  JSON_Worker* w = NULL;

  /*  For every tests, create a thread and allocate a workd at the head  */
  for (JSON_Tester* t=tests; t->fn != NULL; ++t)
  {
    if (pthread_create(&id, NULL, t->fn, &t->arg))
    {
      perror("Create Thread Failed");
    }
    else
    {
      /*  Init new worker  */
      w     = calloc(1, sizeof(JSON_Worker));
      w->id = id;

      /*  Push new worker  */
      w->next = *head;
      *head   = w;
    }
  }
}




void wait_workers(JSON_Worker** head)
{
  JSON_WorkerRetVal* val = NULL;
  JSON_Worker**      pp  = head;
  JSON_Worker*       p   = *pp;

  /*  While the list is not empty  */
  while (p)
  {
    /*  Search the list for all workers that are done  */
    while (p)
    {
      /*  If worker has not finish. Go the next  */
      if (pthread_tryjoin_np(p->id, (void**)&val))
      {
        pp = &p->next;
      }
      /*  Else, evaluate returned value and remove it from the lsit  */
      else
      {
        const char* status = val->ok ? OK:NO;

        /*  Handle returned value  */
        printf("%s" BOLD("%s\n"), status, val->name);
        printf("%s", val->buff);

        free(val);

        /*  Remove worker from list  */
        *pp = p->next;
        free(p);
      }

      p = *pp;
    }

    /*  Return to the head  */
    pp = head;
    p  = *pp;
  }
}
