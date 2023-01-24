#include "../pios-kernel/kernel/kernel.h"
#include <stdbool.h>
#include <stdint.h>

extern EventHandle test_completed_event;

void writer_task1(Channel* channel){
    int msg=0;
    pub_msg(channel, (void*) &msg);
    //writes a single message in the channel 
    task_exit();
}

void writer_task2(Channel* channel){
    int msg1=1;
    pub_msg(channel, (void*) &msg1);
    int msg2=2;
    pub_msg(channel, (void*) &msg2);
    //writes two messages in the channel
    task_exit();
}

void writer_task3(Channel* channel){
    int msg1=3;
    pub_msg(channel, (void*) &msg1);
    int msg2=4;
    pub_msg(channel, (void*) &msg2);
    //writes two messages in the channel
    task_exit();
}

void reader_task1(Channel* channel){
    bool test_result = true;
    int msg;
    for(int i=0; i<3; i++){
        read_msg(channel, (void*) &msg);
        if(msg != 0 && msg != 1 && msg != 2 && msg != 3 && msg != 4 ){
            test_result = false;
            event_post(test_completed_event, &test_result); //message not valid
        }else if(msg == 4){
            event_post(test_completed_event, &test_result); //read last message
        }
    }
    
    //reads 3 messages from the channel and control that the messages are valid
    task_exit();
}

void reader_task2(Channel* channel){
    bool test_result = true;
    int msg;
    for(int i=0; i<2; i++){
        read_msg(channel, (void*) &msg);
        if(msg != 0 && msg != 1 && msg != 2 && msg != 3 && msg != 4 ){
            test_result = false;
            event_post(test_completed_event, &test_result); //message not valid
        }else if(msg == 4){
            event_post(test_completed_event, &test_result); //read last message
        }
    }
    //reads 2 messages from the channel and control that the messages are valid
    task_exit();
}

void test_channels(){
    Channel* channel = NEW_CHANNEL(4, int);

    //create the tasks
    create_task((void(*)(void*)) reader_task1, (void*)channel, 0, NULL);
    create_task((void(*)(void*)) writer_task1, (void*)channel, 0, NULL);
    create_task((void(*)(void*)) writer_task2, (void*)channel, 0, NULL);
    create_task((void(*)(void*)) writer_task3, (void*)channel, 0, NULL);
    create_task((void(*)(void*)) reader_task2, (void*)channel, 0, NULL);
}
