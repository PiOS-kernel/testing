#include "../pios-kernel/kernel/synch/pipe.h"
#include <stdbool.h>
#include <stdint.h>

extern EventHandle test_completed_event;

void writer_task1(PIPE pipe){
    int msg=0;
    pub_msg(pipe, msg);
    //writes a single message in the pipe 
    task_exit();
}

void writer_task2(PIPE pipe){
    int msg1=1;
    pub_msg(pipe, msg);
    char msg2=2;
    pub_msg(pipe, msg);
    //writes two messages in the pipe
    task_exit();
}

void writer_task3(PIPE pipe){
    int msg1=3;
    pub_msg(pipe, msg);
    int msg2=4;
    pub_msg(pipe, msg);
    //writes two messages in the pipe
    task_exit();
}

void reader_task1(PIPE pipe){
    uint32_t test_result = 1;
    int msg;
    while(messages > 0){
        read_msg(pipe, msg);
        if(msg == 0 | msg == 1 | msg == 2 | msg == 3 | msg == 4 ){
            test_result = 0;
        }
        messages--;
    }
    //reads and control that the message read is valid
    event_post(test_completed_event, &test_result);
    task_exit();
}

void reader_task2(PIPE pipe){
    uint32_t test_result = 1;
    int msg;
    while(messages > 0){
        read_msg(pipe, msg);
        if(msg == 0 | msg == 1 | msg == 2 | msg == 3 | msg == 4 ){
            test_result = 0;
        }
        messages--;
    }
    //reads and control that the message read is valid
    event_post(test_completed_event, &test_result);
    task_exit();
}

void test_pipe(){
    PIPE pipe = NEW_PIPE(4,int);

    //create the tasks
    create_task((void(*)(void*)) reader_task1, (void*)pipe, 0, NULL);
    create_task((void(*)(void*)) writer_task1, (void*)pipe, 0, NULL);
    create_task((void(*)(void*)) writer_task2, (void*)pipe, 0, NULL);
    create_task((void(*)(void*)) writer_task3, (void*)pipe, 0, NULL);
    create_task((void(*)(void*)) reader_task2, (void*)pipe, 0, NULL);
}
