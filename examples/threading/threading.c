#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include<syslog.h>

// Optional: use these functions to add debug or error prints to your application
#define DEBUG_LOG(msg,...)
//#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)

void* threadfunc(void* thread_param)
{

    // TODO: wait, obtain mutex, wait, release mutex as described by thread_data structure
    // hint: use a cast like the one below to obtain thread arguments from your parameter
    struct thread_data* thread_func_args = (struct thread_data *) thread_param;

    // Wait.
    if (nanosleep(&thread_func_args->wait_to_obtain_ms, NULL) != 0)
    {
        syslog(LOG_INFO, "1st nanosleep was successful\n");
    }

    // Obtain Mutex.
    if (pthread_mutex_lock(thread_func_args->mutex) != 0) 
    {
        thread_func_args->thread_complete_success = false;
        return thread_param;
    }

    // Wait.
    if (nanosleep(&thread_func_args->wait_to_obtain_ms, NULL) != 0)
    {
        syslog(LOG_INFO, "2nd nanosleep was successful\n");
    }

    // Release Mutex.
    if(pthread_mutex_unlock(thread_func_args->mutex) != 0)
    {
        thread_func_args->thread_complete_success = false;
        return thread_param;
    }
    thread_func_args->thread_complete_success = true;
    return thread_param;
}


bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex,int wait_to_obtain_ms, int wait_to_release_ms)
{
    /**
     * TODO: allocate memory for thread_data, setup mutex and wait arguments, pass thread_data to created thread
     * using threadfunc() as entry point.
     *
     * return true if successful.
     *
     * See implementation details in threading.h file comment block
     */

    struct thread_data *t_data = (struct thread_data *)malloc(sizeof(struct thread_data));
    t_data->wait_to_obtain_ms.tv_sec = 0;
    t_data->wait_to_obtain_ms.tv_nsec = (wait_to_obtain_ms*1000000);
    t_data->wait_to_release_ms.tv_sec = 0;
    t_data->wait_to_release_ms.tv_nsec = (wait_to_obtain_ms*1000000);
    t_data->mutex = mutex;

    // create pthread.
    if(pthread_create(&t_data->thread, NULL, threadfunc, t_data) == 0)
    {
        syslog(LOG_INFO, "pthread created successfully\n");
    } 
    *thread = t_data->thread;
    return true;
}

