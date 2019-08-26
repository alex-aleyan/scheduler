/* 
 * File:   TASK_MANAGEMENT.h
 * Author: Sasha
 *
 * Created on May 9, 2015, 6:41 PM
 */

#ifndef TASK_MANAGEMENT_H
#define	TASK_MANAGEMENT_H

/** @file
 * @defgroup Group1 TASK_H
 *
 * The provided functions allow the user to initialize a task management module and
 * to use the list of tasks as a stack, a queue, or a schedule. User is required to
 * provide the module with a counter source (current_time) and the desired size of the list (array_size).
 *
 * \attention The module provided does not sort the pushed task and does not disable the interrupts.
 * This functionalities are yet to be added in the near future. It should also be noted that in this documentation
 * terms task and array element both refer to a task on a list.
 *
 *
 * @author Alex Aleyan
 * @version 5.3 The task management module is optimized to be used with PIC32MX250F128B
 *
 * @{
 */


#define array_size 10 /**< The maximum number of tasks to be listed (user defineable) */
#define current_time TMR4 /**< The 32-bit counter used by the tick() function for scheduling */
#define POP_QUEUE 1
#define POP_STACK 0

struct task {

    //The funct is a pointer to a function/task to run.
    void (*func)(void); /**< The funct is a pointer to a function/task to run. */

    //SCHEDULING:
    //Every how many seconds is the task to be executed
    unsigned int repeat_period; /**< Every how many seconds is the task to be executed. */

    // The number of times the scheduled task to be repeated (rescheduled after execution if times_repeat > 1)
    unsigned int times_repeat;/**< The number of times the scheduled task to be repeated (rescheduled after execution if times_repeat > 1). */

    // The next time the task to be executed (current_time + repeat_period)
    unsigned int time_exe;/**< The next time the task to be executed (current_time + repeat_period). */

    //QUEUING:
    //In the final version, the priority of the task can be used to sort the queue
    int prior;/**< In the final version, the priority of the task can be used to sort the queue. */
    //The next_p of array element array[n] is to be used to point to the array[n+1]
    //If n = array_size, the next_p is to point to NULL
    struct task* next_p;/**< The next_p of array element array[n] is to be used to point to the array[n+1]. If n = array_size, the next_p is to point to NULL. */


};


struct list{

    struct task array[array_size]; /**< Instead of allocating space using malloc(), we create an array of structures.
                                    * As a result, we don't lose time by allocating it but we are locked into the size of the array. */

    struct task *que_head;/**< This pointer is to be used to point to the first task in the queue or last task on the stack.
                           *  Every time the POP_HEAD() is used, the array element is popped and que_head is repointed to
                           *  the next array element based on the address contained in (*que_head).next_p */

};
struct list list_s;
//struct list* list_p = &list_s;


int initial_function(void);

/**Initialize the Task Module.
 *
 * This functions initiates the task module.
 * The parameters of each task are initialized to default values following way:
 * - The priority is set to 0xFFFF (unless the task is in the queue/stack, it's priority
 *   is always set to 0xFFFF).
 * - void (*func)(void) is set to point to the initial_function() which does not
 *   accept any parameters and is set to return -1.
 * - The next_p pointer to the next task in queue/stack is set to point to NULL (it is always set to NULL when no task is present in queue/stock)
 * - The repeat_period (the duration of time to wait to execute the scheduled task) is set to 0.
 * - The times_repeat (the number of times the scheduled task is to be executed/repeated;always set to 0 when the task is not scheduled) is set to 0.
 * - The time_exe (the time at which the scheduled task is currently due to be executed) is set to 0
 *
 *
 * Example code:
 * @code
 * #define array_size 10
 * #define current_time TMR2
 *
 * ...
 * task_initiate();
 *
 * main()
 * {
 * ...
 * }
 * @endcode
 */
void task_initiate(void);

/**Shows the tasks present in the list
 * This function was used only during the development of the task module in CodeBlocks environment.
 */
void show_list(void);

/**Shows the tasks present in the queue/stack
 * This function was used only during the development of the task module in CodeBlocks environment.
 */
void show_que(void);

/**Push a task into a queue/stack.
 *
 * This function allows the user to push a task into a queue/stack. Whether the list
 * is used as a queue or a stack depends on the way the linked tasks are popped.
 *
 * - To push a task onto the list, this function first iterates through the array
 * of structures.
 * - To ensure that the pushed task does not overwrite an already pushed task,
 * the prior (priority) of each array element check to ensure that no queued/stacked task is occupying
 * a list element.
 * - To ensure that the pushed task does not overwrite an already scheduled task,
 * the times_repeat of each array element is checked to ensure that no scheduled task is occupying a
 * list element.
 * - If the number of iterations exceeds the size of the array of structures (where each element
 * of the array is used to contain a task), the push proceedure is aborted and the function returs -1 indicating
 * that the function failed to push a task.
 * - If the pushed task is the only task in the queue/stack, the function returns 0.
 * - if the pushed task is not the only task in the queue/stack, the function returns 1
 *
 *
 * Example code:
 * @code
 * #define array_size 10
 * #define current_time TMR2
 *
 * void toggle_led(void)
 * {
 * LATAINV = 1;
 * }
 *
 * ...
 * TRISACLR = 1;
 * ANSELACLR = 1;
 *
 * task_initiate();
 * PUSH_TAIL(toggle_led, 2);
 *
 * main()
 * {
 * ...
 *      tick();
 * }
 * @endcode
 *
 * \attention It should be mentioned that the tasks pushed are not sorted based on their priorities and
 * such functionality is yet to be added to this module in the future. This functionality is
 * most likely to be implemented such that a seperate pointer is used in each task structure
 * to link the tasks in the order of their priority. This will require the POP functions to be implemented
 * such that seperate two pop_tail() functions to be used to pop the tasks of the sorted and non-sorted lists (only 1 array element is used
 * to store a given task, but two different ways are used to link the lists), and two pop_head() functions to be used to pop the tasks
 * of sorted and non-sorted lists (one function pops off of the sorted list, and relinks the non-sorted list as required; the other
 * pop function does the opposite).
 *
 *
 * @param void(*func)(void) the function to be pushed into the queue/stack.
 * @param unsigned int priority the priority assigned to this function.
 */
int PUSH_TAIL( void(*func)(void), unsigned int priority);

/**Pop a task of the list (Queue).
 *
 * This function allows the user to pop a task off of a queue. Whether the list
 * is used as a queue or a stack depends on the way the linked tasks are popped.
 *
 * To pop a task off of the list, this function first checks whether there are any tasks in the queue:
 * - If no task is present in the queue, the function returs -1.
 * - If only one task is present in the queue, the function executes the function in the task,
 * sets the priority of the task to 0xFFFF indicating that the current array element is not used for queuing/stacking a task,
 * sets the head pointer to point to NULL value, and returns 0.
 * - If more than one task is present in the queue, the function executes the function in the task,
 * sets the priority of the task to 0xFFFF indicating that the current array element is not used for queuing/stacking a task,
 * sets the head pointer to point to the next task, and returns 1.
 *
 *
 * Example code:
 * @code
 * #define array_size 10
 * #define current_time TMR2
 *
 * int var = 99;
 *
 * void toggle_led(void)
 * {
 * LATAINV = 1;
 * }
 *
 * ...
 * TRISACLR = 1;
 * ANSELACLR = 1;
 *
 * task_initiate();
 * PUSH_TAIL(toggle_led, 2);
 *
 * main()
 * {
 * ...
 *      var = POP_HEAD();
 * }
 * @endcode
 *
 * \attention It should be mentioned that the tasks popped are not sorted based on their priorities and
 * such functionality is yet to be added to this module in the future. This functionality is
 * most likely to be implemented such that a seperate pointer is used in each task structure
 * to link the tasks in the order of their priority. This will require the POP functions to be implemented
 * such that seperate two pop_tail() functions to be used to pop the tasks of the sorted and non-sorted lists (only 1 array element is used
 * to store a given task, but two different ways are used to link the lists), and two pop_head() functions to be used to pop the tasks
 * of sorted and non-sorted lists (one function pops off of the sorted list, and relinks the sorted and non-sorted list as required; the other
 * pop function does the opposite).
 *
 */
int POP_HEAD();

/**Pop a task of the list (Stack).
 *
 * This function allows the user to pop a task off of a stack. Whether the list
 * is used as a queue or a stack depends on the way the linked tasks are popped.
 *
 * To pop a task off of the list, this function first checks whether there are any tasks in the stack:
 * - If no task is present on the stack, the function returs -1.
 * - If only one task is present on the stack, the function executes the function in the task,
 * sets the priority of the task to 0xFFFF indicating that the current array element is not used for queuing/stacking a task,
 * sets the head pointer to point to NULL value, and returns 0.
 * - If more than one task is present on the stack, the function executes the function on the task,
 * sets the priority of the task to 0xFFFF indicating that the current array element is not used for queuing/stacking a task,
 * sets the next_p pointer of the previously pushed task to point to the NULL indicating it's the last task in queue,
 * (or the first task on stack) and returns 1.
 *
 *
 * Example code:
 * @code
 * #define array_size 10
 * #define current_time TMR2
 *
 * int var = 99;
 *
 * void toggle_led(void)
 * {
 * LATAINV = 1;
 * }
 *
 * ...
 * TRISACLR = 1;
 * ANSELACLR = 1;
 *
 * task_initiate();
 * PUSH_TAIL(toggle_led, 2);
 *
 * main()
 * {
 * ...
 *      var = POP_TAIL();
 * }
 * @endcode
 *
 * \attention It should be mentioned that the tasks popped are not sorted based on their priorities and
 * such functionality is yet to be added to this module in the future. This functionality is
 * most likely to be implemented such that a seperate pointer is used in each task structure
 * to link the tasks in the order of their priority. This will require the POP functions to be implemented
 * such that seperate two pop_tail() functions to be used to pop the tasks of the sorted and non-sorted lists (only 1 array element is used
 * to store a given task, but two different ways are used to link the lists), and two pop_head() functions to be used to pop the tasks
 * of sorted and non-sorted lists (one function pops off of the sorted list, and relinks the sorted and non-sorted lists as required; the other
 * pop function does the opposite).
 *
 */
int POP_TAIL();


/**Schedule a task into the list.
 *
 * This function allows the user to schedule a task.
 *
 * To schedule a task on the list, this function first checks whether there are any tasks in the list already containing the same function and
 * the same repeat_period:
 * - If a task containing the same function with the same repeat_period is found, the repeat period of this task will be increased
 * by the number of the repeat_periods of the function being pushed and the value of 0 will be returned.
 * - If a task containing the same function with the repeat_period set to 0 is found (possibly queued/stacked, but definitely not scheduled),
 * this task will be used to schedule the function and its repeat_period, times_repeat, and time_exe parameters will be set accordingly, and
 * the value of 1 will be returned.
 * - If such function cannot be found in a task list, the list will be iterated through searching for unoccupied task. If an unoccupied array element
 * is found, the function will be scheduled into that unoccupied array element/task and the return value will be equal to 2. If an unoccupied array
 * element is not found, the value of -1 will be returned.
 *
 *
 * Example code:
 * @code
 * #define array_size 10
 * #define current_time TMR2
 *
 * int var = 99;
 *
 * void toggle_led(void)
 * {
 * LATAINV = 1;
 * }
 *
 * ...
 * TRISACLR = 1;
 * ANSELACLR = 1;
 *
 * task_initiate();
 *
 * main()
 * {
 * ...
 *      var = PUSH_SCHED(toggle_led, 4000,500);;
 * }
 * @endcode
 *
 * \attention It should be mentioned that the tasks popped are not sorted based on their priorities and
 * such functionality is yet to be added to this module in the future. This functionality is
 * most likely to be implemented such that a seperate pointer is used in each task structure
 * to link the tasks in the order of their priority. This will require the POP functions to be implemented
 * such that seperate two pop_tail() functions to be used to pop the tasks of the sorted and non-sorted lists (only 1 array element is used
 * to store a given task, but two different ways are used to link the lists), and two pop_head() functions to be used to pop the tasks
 * of sorted and non-sorted lists (one function pops off of the sorted list, and relinks the sorted and non-sorted lists as required; the other
 * pop function does the opposite).
 *
 * @param void(*func)(void) the function to be scheduled.
 * @param unsigned int repeat_period the # of seconds the function to be executed in since the schedule event.
 * @param unsigned int times_repeat the # of times the function to be executed/rescheduled
 *
 */
int PUSH_SCHED(void (*func)(void), unsigned int repeat_period, unsigned int times_repeat);

/**Initiate a tick.
 *
 * This function iterates through the list and executes scheduled tasks that are due. It will also pop a task of the queue/stack depending on the parameter passed.
 * To operate, this function requires a counter provide by the user. To provide the module with a counter accumulated by a hardware clock,
 * define the current_time macro above such that it's replaced with the value of counter everytime its "called."
 *
 * @param int pop when pop is set to 0, the tick will pop a task off of the list as if the list was used as a stack and 0 will be returned by the function.
 * When pop is set to 1, the tick will pop a task off of the list as if the list was used as a queue and 1 will be returned by the function.
 * Otherwise, no task will be popped of the queue.
 */
int tick(int pop);
/** @} */

#endif	/* TASK_MANAGEMENT_H */



