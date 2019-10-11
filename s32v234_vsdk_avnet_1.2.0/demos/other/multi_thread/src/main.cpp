/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2014 Freescale Semiconductor;
* All Rights Reserved
*
*****************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
****************************************************************************/

/****************************************************************************
* Includes
****************************************************************************/
#include <stdio.h>
#include <oal.h>

// Thread function definition
void* my_thread(void* threadid);

// Number of threads
#define NUM_TASKS 30

// Semaphore array - the semaphores are used to start the threads in reversed order
OAL_SEMAPHORE semaphores[NUM_TASKS];

/****************************************************************************
* Main function
****************************************************************************/
int main(int, char**)
{
  // Create semaphore for every thread - all the semaphores are red - i.e. all threads are stopped
  for(long i = 0; i < NUM_TASKS; i++)
  {
    char name[256];
    sprintf(name, "sem_%li", i);
    OAL_SemaphoreCreate(&(semaphores[i]), name, OAL_SEMAPHORE_BINARY, 0);
    OAL_SemaphoreObtain(semaphores[i], 0);
  }

  // Initialize all threads and run them (every thread will wait for green semaphore)
  OAL_TCB tasks[NUM_TASKS];
  for(long i = 0; i < NUM_TASKS; i++)
  {
    if(OAL_TaskInit(&my_thread, (void*)i, OAL_TASK_DEFAULT_STACK, OAL_TASK_DEFAULT_PRIORITY, (char*)"mytask",
                    &tasks[i]) == OAL_FAILURE)
    {
      printf("(%ld)FAIL    - Failed to create thread.\n", i);
      ;
      return -1;
    }
  }

  // Start the last thread
  OAL_SemaphoreRelease(semaphores[NUM_TASKS - 1]);

  // Wait for all threads to end
  for(long i = 0; i < NUM_TASKS; ++i)
  {
    OAL_TaskJoin(tasks[i]);
  }

  // Delete all semaphores
  for(long i = 0; i < NUM_TASKS; ++i)
  {
    OAL_SemaphoreDelete(semaphores[i]);
  }

  printf("All thereads merged, done...\n");
  return 0;
}

/****************************************************************************
* Thread function
****************************************************************************/
void* my_thread(void* threadid)
{
  // Wait for semaphore corresponding with thread (-1 = infinite wait)
  OAL_SemaphoreObtain(semaphores[(long)threadid], -1);

  // Print out something
  printf("Hello from thread %li\n", (long)threadid);

  // If we're not the first thread, let' release the semaphore of the id-1 thread
  if((long)threadid - 1 >= 0)
  {
    OAL_SemaphoreRelease(semaphores[(long)threadid - 1]);
  }

  // Release our own semaphore (just for sanity)
  OAL_SemaphoreRelease(semaphores[(long)threadid]);

  return NULL;
}
