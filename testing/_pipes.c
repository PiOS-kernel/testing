#include "../pios-kernel/kernel/kernel.h"
#include <stdbool.h>
#include <stdint.h>

extern EventHandle test_completed_event;

void writer_task1(PIPE* pipe){
    int msg=0;
    pub_msg(pipe, (void*) &msg);
    //writes a single message in the pipe 
    task_exit();
}

void writer_task2(PIPE* pipe){
    int msg1=1;
    pub_msg(pipe, (void*) &msg1);
    int msg2=2;
    pub_msg(pipe, (void*) &msg2);
    //writes two messages in the pipe
    task_exit();
}

void writer_task3(PIPE* pipe){
    int msg1=3;
    pub_msg(pipe, (void*) &msg1);
    int msg2=4;
    pub_msg(pipe, (void*) &msg2);
    //writes two messages in the pipe
    task_exit();
}

void reader_task1(PIPE* pipe){
    bool test_result = true;
    int msg;
    for(int i=0; i<3; i++){
        read_msg(pipe, (void*) &msg);
        if(msg != 0 && msg != 1 && msg != 2 && msg != 3 && msg != 4 ){
            test_result = false;
            event_post(test_completed_event, &test_result); //message not valid
        }else if(msg == 4){
            event_post(test_completed_event, &test_result); //read last message
        }
    }
    
    //reads 3 messages from the pipe and control that the messages are valid
    task_exit();
}

void reader_task2(PIPE* pipe){
    bool test_result = true;
    int msg;
    for(int i=0; i<2; i++){
        read_msg(pipe, (void*) &msg);
        if(msg != 0 && msg != 1 && msg != 2 && msg != 3 && msg != 4 ){
            test_result = false;
            event_post(test_completed_event, &test_result); //message not valid
        }else if(msg == 4){
            event_post(test_completed_event, &test_result); //read last message
        }
    }
    //reads 2 messages from the pipe and control that the messages are valid
    task_exit();
}

void test_pipe(){
    PIPE* pipe = NEW_PIPE(4,int);

    //create the tasks
    create_task((void(*)(void*)) reader_task1, (void*)pipe, 0, NULL);
    create_task((void(*)(void*)) writer_task1, (void*)pipe, 0, NULL);
    create_task((void(*)(void*)) writer_task2, (void*)pipe, 0, NULL);
    create_task((void(*)(void*)) writer_task3, (void*)pipe, 0, NULL);
    create_task((void(*)(void*)) reader_task2, (void*)pipe, 0, NULL);
}
