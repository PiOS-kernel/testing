#include "tests.h"
#include "../../pios-kernel/kernel/heap/heap.h"

bool heap_init_test() {
    uint8_t heap_mem[1024];
    Heap heap = new_heap();
    heap_init(&heap, (uint8_t*) heap_mem, 1024);
    
    ASSERT(available_space(&heap) == 1024);
    ASSERT(count_segments(&heap) == 1);
    return true;
}