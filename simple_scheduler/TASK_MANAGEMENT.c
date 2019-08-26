/*
 * File:   TASK_MANAGEMENT.c
 * Author: Sasha Aleyan
 *
 * Created on May 9, 2015, 6:43 PM
 */

#include <plib.h>
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

#include "TASK_MANAGEMENT.h"

int initial_function(void)
{
    //printf("\nINITIAL FUNCTION!\n");
    return -1;
}


void task_initiate(void)
{

    //printf("\n-----------------INITIALIZE TASK-----------------");

    //This variable will be used to iterate through the list array
    unsigned int counter = 0;

    // Let's point the head pointer to the beginning of the list structure (remember structures are organized like array - sequentially)
    // Also, we know that the moment we initialize the task module, the task list will be empty and the firstly pushed task will be
    // pushed into 0's array
    list_s.que_head = NULL; //&list_s.array[0];
    //printf("\nThe QUE head points to this address: %d", list_s.que_head);

    //Let's run this loop and initialize the list by setting every array element to default values (no task)
    for(counter = 0; counter < (array_size); counter++)
    {
        //Let's initialize each variable in our array of task structures:
        list_s.array[counter].prior = 0xFFFF;
        list_s.array[counter].func = initial_function;
        list_s.array[counter].next_p = NULL;
        list_s.array[counter].repeat_period = 0;
        list_s.array[counter].times_repeat = 0;
        list_s.array[counter].time_exe = 0;

    }
}

/*
void show_list(void){

//printf("\n-----------------SHOW LIST-----------------");
    int counter = 0;
    //printf("\n\nAddress of the first task(QUEUE HEAD POINTS TO) %d\n\n", list_s.que_head);

    for(counter = 0; counter < (array_size); counter++)
    {
      printf("\n\nPriority is list_s.array[%d].prior: %X", counter, list_s.array[counter].prior);
        printf("\n\nRepeat the task every list_s.array[%d].repeat_period: %d seconds", counter, list_s.array[counter].repeat_period);
        printf("\n\nRepeat the task this number of times list_s.array[%d].times_repeat: %d", counter, list_s.array[counter].times_repeat);
        printf("\n\nNext time to repeat is scheduled at list_s.array[%d].time_exe: %d seconds", counter, list_s.array[counter].time_exe);
        printf("\n\nAddress of this task &list_s.array[%d] is: %d", counter, &list_s.array[counter]);
        printf("\n\n\"Next pointer\" points to list_s.array[%d].next_p: %d", counter, list_s.array[counter].next_p);
        printf("\n\nThe function is: \n");
        list_s.array[counter].func();
        printf("\n\n\n\n");

    }

}

void show_que(void){

struct task *que_tracker = list_s.que_head;
printf("\n--------------------------SHOW QUE------------------------");
        printf("\n\nAddress of the first task(QUEUE HEAD POINTS TO) %d\n\n", list_s.que_head);

        //iterate through the queued list printing out tasks' information
        while ( (*que_tracker).next_p != NULL )
        {
                printf("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
                printf("\n que_tracker: %d \n", que_tracker);
                printf("\n (*array_tracker).prior: %d \n", (*que_tracker).prior);
                printf("\n (*que_tracker).repeat_period: %d \n", (*que_tracker).repeat_period);
                printf("\n (*que_tracker).times_repeat: %d \n", (*que_tracker).times_repeat);
                printf("\n (*que_tracker).time_exe: %d \n", (*que_tracker).time_exe);
                printf("\n (*que_tracker).next_p: %d \n", (*que_tracker).next_p);
                (*que_tracker).func();

                que_tracker = (*que_tracker).next_p;
        }

        printf("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
        printf("\n que_tracker: %d \n", que_tracker);
        printf("\n (*array_tracker).prior: %d \n", (*que_tracker).prior);
        printf("\n (*que_tracker).repeat_period: %d \n", (*que_tracker).repeat_period);
        printf("\n (*que_tracker).times_repeat: %d \n", (*que_tracker).times_repeat);
        printf("\n (*que_tracker).time_exe: %d \n", (*que_tracker).time_exe);
        printf("\n (*que_tracker).next_p: %d \n", (*que_tracker).next_p);
        (*que_tracker).func();
        printf("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
}
*/





int PUSH_TAIL( void(*func)(void), unsigned int priority) // PUSH_TAIL (QUEUING)
{

    //printf("\n-----------------PUSH TAIL (FIFO)-----------------\n");

    //This index will be used to access a particular task array element
    unsigned int index = 0;

    //Point the que_tracker to the head of the queue (firstly pushed task, NULL if nothing in queue)
    struct task *que_tracker = list_s.que_head;

    //let's push the task to be queued into the array location such that
    //the location's priority is 0xFFFF (no task was queued into this location) and times_repeat is
    //equal 0 (not task was scheduled into this location)
    while( list_s.array[index].prior != 0xFFFF || list_s.array[index].times_repeat != 0 )
    {
        //increment through the task array
        index = index++;

        //If no space is left in the array to store the task, "failed to push"
        if(index == array_size)
        {
            //printf("\n FAILED TO PUSH, LIST IS FULL AND INDEX IS BEYOND ARRAY'S SIZE: %d\n", index);
            return -1;
        }
    }

    //point the next_p of just pushed task to NULL since it will be the last task in queue (tail)
    list_s.array[index].next_p = NULL;

    //set the priority as provided by the user
    list_s.array[index].prior = priority;

    //set the function as provided by the user
    list_s.array[index].func = func;

    //printf("\n SUCCEDED TO PUSH TAIL");

    //If queue is empty
    if (que_tracker == NULL)
    {
        //then point the head to the task we are currently pushing into the queue
        list_s.que_head = &list_s.array[index];

        return 0;
    }
    //if queue is partially occupied
    else{

        //Locate the last element in the queue
        while ( (*que_tracker).next_p != NULL)
        {
                //iterate through the queue
                que_tracker = (*que_tracker).next_p;
        }

        //Make sure the task we are currently pushing is not the only task in the queue...
        //This step became unnecessary since the code was modified for the SCHEDULING such that
        //the header pointer points to NULL when no tasks in queue
        if ( (*que_tracker).next_p == NULL && (que_tracker !=  &list_s.array[index]) )
        {
                    //Point the last task in queue to the just pushed task
                    (*que_tracker).next_p = &list_s.array[index];
        }


    return 1;
    }
}

int POP_HEAD()//FIFO_pop
{
    //printf("\n-----------------POP HEAD (FIFO)-----------------\n");

    //if no task in the queue
    if ( list_s.que_head == NULL)// && (*list_s.que_head).prior == 0xFFFF)
    {
        //printf("\nNOTHING TO POP\n");
        return -1;
    }

    //if only one task in the queue
    else if ( (*list_s.que_head).next_p == NULL && (*list_s.que_head).prior != 0xFFFF)
    {
        (*list_s.que_head).func();
        (*list_s.que_head).prior = 0xFFFF;
        list_s.que_head = NULL;
        //printf("\n SUCCEDED TO POP HEAD");
        return 0;
    }

    //if more than 1 task in the queue
    else
    {
        (*list_s.que_head).func();
        (*list_s.que_head).prior = 0xFFFF;
        list_s.que_head = (*list_s.que_head).next_p;

        //printf("\n SUCCEDED TO POP HEAD");

        return 1;
    }

}



int POP_TAIL(){//FILO_pop

    //printf("\n-----------------POP TAIL (FILO)-----------------\n");

    struct task *que_tail = list_s.que_head;

    struct task *que_new_tail;

//    list_s.que_head = (*list_s.que_head).next_p;

    //if no task in the queue
    if (que_tail == NULL)
    {
        //printf("NOTHING TO POP");
        return -1;
    }

    //if only one task in the queue
    else if ((*list_s.que_head).next_p == NULL && (*que_tail).prior != 0xFFFF)
    {
        (*que_tail).func();
        (*que_tail).prior = 0xFFFF;
        list_s.que_head = NULL;
        //(*que_new_tail).next_p = NULL;
        //printf("\n SUCCEDED TO POP HEAD");

        return 0;
    }

    else
    {

        while ( (*que_tail).next_p != NULL )
        {
                //chase the que_tail with the que_new_tail such that once we find the tail of the queue (lastly pushed task)
                //and pop it, the task before the last one (just popped) can become new tail (the task that was pushed lastly)
                que_new_tail = que_tail;
                que_tail = (*que_tail).next_p;
        }
        //execute the function we are to pop
        (*que_tail).func();
        //free the space by setting priority to 0xFFFF
        (*que_tail).prior = 0xFFFF;
        //Set the task before last one (just popped task) as a new tail
        (*que_new_tail).next_p = NULL;
        //printf("\n SUCCEDED TO POP HEAD");
        return 1;
    }

}
/*MODIFY THE CODE ABOVE THIS POINT TO ACCOMODATE FOR THE SCHEDULE MODULARITY OF THIS CODE:
1) When popping:
    - see if task is used for a schedule too (times_repeat > 0). If so, pop it but don't change the priority.
    - maybe use an extra variable in the structure to indicate whether the task in in the queue or not. If in
      the schedule but not in the queue and priority isn't FFFF, using such variable will allows us to reuse this
      scheduled task in queue!!! So, instead of using PRIORITY when pushing the QUEUE, use this variable .When pushing,
      see if a task is already in the array and used for scheduling; if so, add it to the queue list and change priority
      as desired

        //In PUSH_SCHED: wont need to set prior to 1 if we add an extra condition in push to verify whether
        //the task in also scheduled... if so, don't push the queued task into this locations unless the
        //function address matches!

3) Write a function to push head!!!!
*/
int PUSH_SCHED(void (*func)(void), unsigned int repeat_period, unsigned int times_repeat)
{
    //printf("\n-----------------PUSH SCHEDDULE-----------------\n");

    unsigned int index = 0;

    //
    while(index < array_size)
    {

        //if already scheduled with the same repeat period, use the same task slot.
        if (list_s.array[index].func == func && list_s.array[index].repeat_period == repeat_period)
        {   //increment the number of times the task to be repeated/executed by the number requested by the user
            list_s.array[index].times_repeat = list_s.array[index].times_repeat + times_repeat;
            //printf("\n SUCCEDED TO PUSH SCHEDULE INTO EXISTING ELEMENT");
            return 0;
        }

        //if already listed with the same function, but not scheduled. Schedule it with a given repeat period
        else if (list_s.array[index].func == func && list_s.array[index].repeat_period == 0)
        {
            //set the number of time the task to be repeated as requested by the user
            list_s.array[index].repeat_period = repeat_period;
            //increment the number of times the task to be repeated/executed by the number requested by the user
            list_s.array[index].times_repeat = list_s.array[index].times_repeat + times_repeat;
            //set the time the task is scheduled to be executed
            list_s.array[index].time_exe = repeat_period + current_time;
            //printf("\n SUCCEDED TO PUSH SCHEDULE INTO EXISTING ELEMENT");
            return 1;
        }

        index++;
    }

    index = 0;

    //If no such task has been queued, or if was scheduled but with different period,
    //then we will need to find a new location for this task in the array.
    while( (list_s.array[index].prior != 0xFFFF) && (index <= array_size) )
    {
        //If no space is left in the array to store the task, "failed to push"
        if(index == array_size)
        {
            //printf("\n FAILED TO SCHEDULE, LIST IS FULL AND INDEX IS BEYOND ARRAY'S SIZE: %d\n", index);
            return -1;
        }
        index = index++;
    }

        //wont need to set prior to 1 if we add an extra condition in push to pop to verify whether
        //the task in also scheduled... if so, don't push the queued task into this locations unless the
        //function address matches!
        //list_s.array[index].prior = 1; // set to one so its not overwritten by queue push when 0xFFFF is located
        list_s.array[index].times_repeat = times_repeat;
        list_s.array[index].repeat_period = repeat_period;
        list_s.array[index].time_exe = repeat_period + current_time;
        list_s.array[index].func = func;
        //printf("\n SUCCEDED TO PUSH SCHEDULE INTO A NEW ELEMENT");
        return 2;


}

int tick(int pop)
{
    //INTDisableInterrupts();

    unsigned int index = 0;

    //iterate through the array looking for the scheduled tasks that due to be executed
    while(index < array_size)
    {
        //if scheduled (times_repeat != 0) and the value of the timer's counter has passed the scheduled time,
        //or the time has just reached the time the task is scheduled to be executed
        if ( (list_s.array[index].times_repeat!= 0) && (list_s.array[index].time_exe <= current_time) )
        {
            //INTEnableInterrupts();
            list_s.array[index].func();
            //INTDisableInterrupts();

            list_s.array[index].times_repeat--;

            //if the task is to be executed again
            if(list_s.array[index].times_repeat > 0)
            {
                //reschedule the task
                list_s.array[index].time_exe = list_s.array[index].repeat_period + current_time;
            }
        }

        index++;
    }

    if(pop == 0)
    {
        //Use POP_TAIL() when using as a stack
        //INTEnableInterrupts();
        POP_TAIL();
        //INTDisableInterrupts();

        return 0;
    }

    else if(pop == 1)
    {
        //Use POP_HEAD when using as a queue
        //INTEnableInterrupts();
        POP_HEAD();
        //INTDisableInterrupts();

        return 1;
    }
    else return -1;

    //INTEnableInterrupts();
}




