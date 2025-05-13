#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <queue.h>

#define TEST_ASSERT(assert)				\
do {									\
	printf("ASSERT: " #assert " ... ");	\
	if (assert) {						\
		printf("PASS\n");				\
	} else	{							\
		printf("FAIL\n");				\
		exit(1);						\
	}									\
} while(0)

/* Create */
void test_create(void)
{
	queue_t q;
	
	fprintf(stderr, "*** TEST create ***\n");
	q = queue_create();
	TEST_ASSERT(q!=NULL);
	TEST_ASSERT(queue_destroy(q) == 0);
}

/* Destroy */
void test_queue_destroy_full(void)
{
	int data = 3, *ptr;
	queue_t q;

	fprintf(stderr, "*** TEST queue_destroy_full ***\n");

	q = queue_create();
	TEST_ASSERT(queue_enqueue(q, &data) == 0);
	TEST_ASSERT(queue_length(q) == 1);

	//TEST DESTROYING A NON-EMPTY QUEUE DOES NOT WORK
	TEST_ASSERT(queue_destroy(q) == -1);

	TEST_ASSERT(queue_length(q) == 1);

	TEST_ASSERT(queue_dequeue(q, (void**)&ptr) == 0);
	TEST_ASSERT(ptr == &data);
	TEST_ASSERT(queue_length(q) == 0);

	//TEST DESTROYING A EMPTY QUEUE DOES WORK
	TEST_ASSERT(queue_destroy(q) == 0);

	q = (queue_t) NULL;

	//TEST DESTROYING A NULL QUEUE DOES NOT WORK
	TEST_ASSERT(queue_destroy(q) == -1);
}

/* Enqueue/Dequeue simple */
void test_queue_simple(void)
{
	int data = 3, *ptr;
	queue_t q;

	fprintf(stderr, "*** TEST queue_simple ***\n");

	q = queue_create();
	queue_enqueue(q, &data);
	queue_dequeue(q, (void**)&ptr);

	TEST_ASSERT(queue_destroy(q) == 0);
	TEST_ASSERT(ptr == &data);
}

/* ENQUEUE / DEQUEUE COMPREHENSIVE*/
void test_queue_enqueue_dequeue_full(void)
{
	int data0 = 0, data1 = 1, data2 = 2, *ptr;
	queue_t q;

	fprintf(stderr, "*** TEST queue_enqueue_dequeue_full ***\n");

	//TEST ENQUEING A NULL QUEUE DOES NOT WORK
	q = (queue_t) NULL;
	TEST_ASSERT(queue_enqueue(q, (void*)NULL) == -1);

	q = queue_create();
    TEST_ASSERT(queue_length(q) == 0);
	TEST_ASSERT(queue_enqueue(q, &data0) == 0);
	TEST_ASSERT(queue_length(q) == 1);

	//Enqueue NULL data should fail
	TEST_ASSERT(queue_enqueue(q, (void*)NULL) == -1);

	TEST_ASSERT(queue_length(q) == 1);	
	TEST_ASSERT(queue_enqueue(q, &data1) == 0);
	TEST_ASSERT(queue_length(q) == 2);
	TEST_ASSERT(queue_enqueue(q, &data2) == 0);
	TEST_ASSERT(queue_length(q) == 3);

	//Queue should look like data0(0) -> data1(1) -> data2(2)

	TEST_ASSERT(queue_dequeue(q, (void**)&ptr) == 0);
	TEST_ASSERT(queue_length(q) == 2);
	TEST_ASSERT(ptr == &data0);

	TEST_ASSERT(queue_dequeue(q, (void**)&ptr) == 0);
	TEST_ASSERT(queue_length(q) == 1);
	TEST_ASSERT(ptr == &data1);

	TEST_ASSERT(queue_dequeue(q, (void**)&ptr) == 0);
	TEST_ASSERT(queue_length(q) == 0);
	TEST_ASSERT(ptr == &data2);

	//TEST DEQUEUEING AN EMPTY QUEUE DOESNT WORK
	TEST_ASSERT(queue_dequeue(q, (void**)&ptr) == -1);
	TEST_ASSERT(queue_length(q) == 0);
	TEST_ASSERT(ptr == &data2);
	
	TEST_ASSERT(queue_destroy(q) == 0);

	//TEST DEQUEUEING A NULL QUEUE DOES NOT WORK
	q = (queue_t) NULL;
	TEST_ASSERT(queue_dequeue(q, (void**)&ptr) == -1);
	TEST_ASSERT(ptr == &data2);
}

/* Delete last */
void test_queue_delete_last(void)
{
	int data0 = 0, data1 = 1, data2 = 2, *ptr;
	queue_t q;

	fprintf(stderr, "*** TEST queue_delete_last ***\n");

	q = queue_create();
    TEST_ASSERT(queue_length(q) == 0);
	TEST_ASSERT(queue_enqueue(q, &data0) == 0);
	TEST_ASSERT(queue_length(q) == 1);
	TEST_ASSERT(queue_enqueue(q, &data1) == 0);
	TEST_ASSERT(queue_length(q) == 2);
	TEST_ASSERT(queue_enqueue(q, &data2) == 0);
	TEST_ASSERT(queue_length(q) == 3);

	//Queue should look like data0(0) -> data1(1) -> data2(2)

	ptr = &data2;
	TEST_ASSERT(queue_delete(q, (void*)ptr) == 0);
	TEST_ASSERT(queue_length(q) == 2);

	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(queue_length(q) == 1);
	TEST_ASSERT(ptr == &data0);

	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(queue_length(q) == 0);
	TEST_ASSERT(ptr == &data1);

	TEST_ASSERT(queue_destroy(q) == 0);

	//TEST DEQUEUEING A NULL QUEUE DOES NOT WORK
	q = (queue_t) NULL;
	TEST_ASSERT(queue_delete(q, (void*)ptr) == -1);
}

/* Delete middle */
void test_queue_delete_middle(void)
{
	int data0 = 0, data1 = 1, data2 = 2, *ptr;
	queue_t q;

	fprintf(stderr, "*** TEST queue_delete_middle ***\n");

	q = queue_create();
    TEST_ASSERT(queue_length(q) == 0);
	TEST_ASSERT(queue_enqueue(q, &data0) == 0);
	TEST_ASSERT(queue_length(q) == 1);
	TEST_ASSERT(queue_enqueue(q, &data1) == 0);
	TEST_ASSERT(queue_length(q) == 2);
	TEST_ASSERT(queue_enqueue(q, &data2) == 0);
	TEST_ASSERT(queue_length(q) == 3);

	//Queue should look like data0(0) -> data1(1) -> data2(2)

	ptr = &data1;
	TEST_ASSERT(queue_delete(q, (void*)ptr) == 0);
	TEST_ASSERT(queue_length(q) == 2);

	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(queue_length(q) == 1);
	TEST_ASSERT(ptr == &data0);

	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(queue_length(q) == 0);
	TEST_ASSERT(ptr == &data2);

	TEST_ASSERT(queue_destroy(q) == 0);
}

/* Delete first */
void test_queue_delete_first(void)
{
	int data0 = 0, data1 = 1, data2 = 2, *ptr;
	queue_t q;

	fprintf(stderr, "*** TEST queue_delete_first ***\n");

	q = queue_create();
    TEST_ASSERT(queue_length(q) == 0);
	TEST_ASSERT(queue_enqueue(q, &data0) == 0);
	TEST_ASSERT(queue_length(q) == 1);
	TEST_ASSERT(queue_enqueue(q, &data1) == 0);
	TEST_ASSERT(queue_length(q) == 2);
	TEST_ASSERT(queue_enqueue(q, &data2) == 0);
	TEST_ASSERT(queue_length(q) == 3);

	//Queue should look like data0(0) -> data1(1) -> data2(2)

	ptr = &data0;
	TEST_ASSERT(queue_delete(q, (void*)ptr) == 0);
	TEST_ASSERT(queue_length(q) == 2);

	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(queue_length(q) == 1);
	TEST_ASSERT(ptr == &data1);

	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(queue_length(q) == 0);
	TEST_ASSERT(ptr == &data2);

	TEST_ASSERT(queue_destroy(q) == 0);
}

/* Delete from empty queue*/
void test_queue_delete_from_empty(void)
{
	int data0 = 0, *ptr;
	queue_t q;

	fprintf(stderr, "*** TEST queue_delete_from_empty ***\n");

	q = queue_create();
    TEST_ASSERT(queue_length(q) == 0);
	TEST_ASSERT(queue_enqueue(q, &data0) == 0);
	TEST_ASSERT(queue_length(q) == 1);

	ptr = &data0;
	TEST_ASSERT(queue_delete(q, (void*)ptr) == 0);
	TEST_ASSERT(queue_length(q) == 0);
	TEST_ASSERT(queue_delete(q, (void*)ptr) == -1);
	TEST_ASSERT(queue_length(q) == 0);
	TEST_ASSERT(queue_destroy(q) == 0);
}

/* Delete with NULL data*/
void test_queue_delete_with_NULL_data(void)
{
	int data0 = 0, *ptr;
	queue_t q;

	fprintf(stderr, "*** TEST queue_delete_with_NULL_data ***\n");

	q = queue_create();
    TEST_ASSERT(queue_length(q) == 0);
	TEST_ASSERT(queue_enqueue(q, &data0) == 0);
	TEST_ASSERT(queue_length(q) == 1);

	ptr = NULL;
	TEST_ASSERT(queue_delete(q, (void*)ptr) == -1);
	TEST_ASSERT(queue_length(q) == 1);
	TEST_ASSERT(queue_dequeue(q, (void**)&ptr) == 0);
	TEST_ASSERT(ptr == &data0);
	TEST_ASSERT(queue_length(q) == 0);
	TEST_ASSERT(queue_destroy(q) == 0);
}

/* Delete duplicate data*/
void test_queue_delete_duplicate_data(void)
{
	int data0 = 0, data1 = 1, *ptr;
	queue_t q;

	fprintf(stderr, "*** TEST queue_delete_duplicate_data ***\n");

	q = queue_create();
    TEST_ASSERT(queue_length(q) == 0);
	TEST_ASSERT(queue_enqueue(q, &data0) == 0);
	TEST_ASSERT(queue_enqueue(q, &data1) == 0);
	TEST_ASSERT(queue_enqueue(q, &data0) == 0);
	TEST_ASSERT(queue_length(q) == 3);

	ptr = NULL;
	TEST_ASSERT(queue_delete(q, &data0) == 0);
	TEST_ASSERT(queue_length(q) == 2);
	TEST_ASSERT(queue_dequeue(q, (void**)&ptr) == 0);
	TEST_ASSERT(ptr == &data1);
	TEST_ASSERT(queue_dequeue(q, (void**)&ptr) == 0);
	TEST_ASSERT(ptr == &data0);
	TEST_ASSERT(queue_length(q) == 0);
	TEST_ASSERT(queue_destroy(q) == 0);
}

/* Delete data that is not contained in queue*/
void test_queue_delete_data_not_contained(void)
{
	int data0 = 0, data1 = 1, data2 = 2, dataNOTFOUND = 4, *ptr;
	queue_t q;

	fprintf(stderr, "*** TEST queue_delete_data_not_contained ***\n");

	q = queue_create();
    TEST_ASSERT(queue_length(q) == 0);
	TEST_ASSERT(queue_enqueue(q, &data0) == 0);
	TEST_ASSERT(queue_length(q) == 1);
	TEST_ASSERT(queue_enqueue(q, &data1) == 0);
	TEST_ASSERT(queue_length(q) == 2);
	TEST_ASSERT(queue_enqueue(q, &data2) == 0);
	TEST_ASSERT(queue_length(q) == 3);

	//Queue should look like data0(0) -> data1(1) -> data2(2)

	ptr = &dataNOTFOUND;
	TEST_ASSERT(queue_delete(q, (void*)ptr) == -1);
	TEST_ASSERT(queue_length(q) == 3);

	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(queue_length(q) == 2);
	TEST_ASSERT(ptr == &data0);

	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(queue_length(q) == 1);
	TEST_ASSERT(ptr == &data1);

	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(queue_length(q) == 0);
	TEST_ASSERT(ptr == &data2);

	TEST_ASSERT(queue_destroy(q) == 0);
}

/* Callback function that increments items */
static void iterator_inc(queue_t q, void *data)
{
    int *a = (int*)data;

    if (*a == 42)
        queue_delete(q, data);
    else
        *a += 1;
} 

/* Test iterator callback function*/
void test_queue_iterate(void)
{
	queue_t q;
    int data[] = {1, 2, 3, 4, 5, 42, 6, 7, 8, 9};
    size_t i;

	fprintf(stderr, "*** TEST test_queue_iterate_iterate_inc ***\n");
    /* Initialize the queue and enqueue items */
    q = queue_create();
    for (i = 0; i < sizeof(data) / sizeof(data[0]); i++)
        queue_enqueue(q, &data[i]);

    /* Increment every item of the queue, delete item '42' */
    queue_iterate(q, iterator_inc);
    TEST_ASSERT(data[0] == 2);
	TEST_ASSERT(data[5] == 42); //data array's 42 should not be touched
    TEST_ASSERT(queue_length(q) == 9);

	int *ptr;
	while(queue_length(q) > 0)
	{
		queue_dequeue(q, (void**)&ptr);
	}
	TEST_ASSERT(queue_destroy(q) == 0);
}

/* Callback function that increments items */
static void iterator_apocalypse(queue_t q, void *data)
{
	queue_delete(q, data);
} 

/* Test iterator fn that deletes all nodes*/
void test_queue_iterate_delete_all(void)
{
	queue_t q;
    int data[] = {1, 2, 3, 4, 5, 42, 6, 7, 8, 9};
    size_t i;

	fprintf(stderr, "*** TEST test_queue_iterate_iterate_apocalypse ***\n");

    /* Initialize the queue and enqueue items */
    q = queue_create();
    for (i = 0; i < sizeof(data) / sizeof(data[0]); i++)
        queue_enqueue(q, &data[i]);

    /* Increment every item of the queue, delete item '42' */
    queue_iterate(q, iterator_apocalypse);
    TEST_ASSERT(data[0] == 1);
	TEST_ASSERT(data[5] == 42);
    TEST_ASSERT(queue_length(q) == 0);
	TEST_ASSERT(queue_destroy(q) == 0);
}

int main(void)
{
	// create
	test_create();

	// destroy
	test_queue_destroy_full();

	// enqueue / dequeue
	test_queue_simple();
	test_queue_enqueue_dequeue_full();

	// delete
	test_queue_delete_last();
	test_queue_delete_first();
	test_queue_delete_middle();
	test_queue_delete_from_empty();
	test_queue_delete_with_NULL_data();
	test_queue_delete_duplicate_data();
	test_queue_delete_data_not_contained();

	// iterator (tests deleting from queue, changing data, and not changing data)
	test_queue_iterate();
	test_queue_iterate_delete_all();

	// length tested MANY times as we go

	return 0;
}
