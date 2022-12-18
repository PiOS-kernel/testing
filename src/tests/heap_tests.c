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

bool count_segments_test() {
    uint8_t heap_mem[1024];
    Heap heap = new_heap();
    
    for (int i=0; i<25; ++i) {
        uint8_t* base_address = (uint8_t*) heap_mem + i * 40;
        add_free_segment(&heap, base_address, 40);
        ASSERT(count_segments(&heap) == i+1);
    }
    return true;
}

bool available_space_test() {
    uint8_t heap_mem[1024];
    Heap heap = new_heap();
    heap_init(&heap, (uint8_t*) heap_mem, 1024);
    
    for (int i=0; i<25; ++i) {
        allocate_segment(&heap, 40);
        ASSERT(available_space(&heap) == 1024 - (i+1) * 40);
    }
    return true;
}

bool heap_compaction_test() {
    uint8_t heap_mem[1024];
    Heap heap = new_heap();
    
    add_free_segment(&heap, (uint8_t*) heap_mem, 128);
    ASSERT(available_space(&heap) == 128);
    ASSERT(count_segments(&heap) == 1);

    add_free_segment(&heap, (uint8_t*) heap_mem + 128, 128);
    ASSERT(available_space(&heap) == 256);
    ASSERT(count_segments(&heap) == 2);
    
    heap_compact(&heap);
    ASSERT(available_space(&heap) == 256);
    ASSERT(count_segments(&heap) == 1);
    return true;
}

bool test_allocate_segment() {
    uint8_t heap_mem[1024];
    Heap heap = new_heap();
    heap_init(&heap, (uint8_t*) heap_mem, 1024);
    
    // Segments are allocated

    uint8_t* segment = allocate_segment(&heap, 128);
    ASSERT(segment != NULL);
    ASSERT(available_space(&heap) == 896);
    ASSERT(count_segments(&heap) == 1);
    
    segment = allocate_segment(&heap, 128);
    ASSERT(segment != NULL);
    ASSERT(available_space(&heap) == 768);
    ASSERT(count_segments(&heap) == 1);
    
    segment = allocate_segment(&heap, 128);
    ASSERT(segment != NULL);
    ASSERT(available_space(&heap) == 640);
    ASSERT(count_segments(&heap) == 1);
    
    uint8_t* segment2 = allocate_segment(&heap, 128);
    ASSERT(segment != NULL);
    ASSERT(available_space(&heap) == 512);
    ASSERT(count_segments(&heap) == 1);

    // Free to segments, and observe heap compaction taking place

    free_segment(&heap, segment, 128);
    ASSERT(available_space(&heap) == 640);
    ASSERT(count_segments(&heap) == 2);

    free_segment(&heap, segment2, 128);
    ASSERT(available_space(&heap) == 768);
    ASSERT(count_segments(&heap) == 1);

    // Start allocating segments again
    segment = allocate_segment(&heap, 128);
    ASSERT(segment != NULL);
    ASSERT(available_space(&heap) == 640);
    ASSERT(count_segments(&heap) == 1);

    segment = allocate_segment(&heap, 128);
    ASSERT(segment != NULL);
    ASSERT(available_space(&heap) == 512);
    ASSERT(count_segments(&heap) == 1);

    segment = allocate_segment(&heap, 128);
    ASSERT(segment != NULL);
    ASSERT(available_space(&heap) == 384);
    ASSERT(count_segments(&heap) == 1);
    
    segment = allocate_segment(&heap, 128);
    ASSERT(segment != NULL);
    ASSERT(available_space(&heap) == 256);
    ASSERT(count_segments(&heap) == 1);
    
    segment = allocate_segment(&heap, 128);
    ASSERT(segment != NULL);
    ASSERT(available_space(&heap) == 128);
    ASSERT(count_segments(&heap) == 1);
    
    segment = allocate_segment(&heap, 128);
    ASSERT(segment != NULL);
    ASSERT(available_space(&heap) == 0);
    ASSERT(count_segments(&heap) == 1);

    // Since there is no more space, the next allocation should fail
    // and a NULL pointer should be returned
    
    segment = allocate_segment(&heap, 128);
    ASSERT(segment == NULL);
    ASSERT(available_space(&heap) == 0);
    ASSERT(count_segments(&heap) == 1);
    
    return true;
}