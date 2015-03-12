#include "gol.h"
#include "unity.h"
#include <string.h>
#include <stdlib.h>

extern int8_t *result_cache;
extern int cache_size;
extern int cache_next;

extern int8_t **table;
extern int max_x, max_y;

void setUp(void)
{
}

void tearDown(void)
{
}

void test_count_neighbours(void)
{
    init_gol(1,1);    
    TEST_ASSERT_EQUAL_INT(0, count_neighbours(0, 0));

    init_gol(2,2);
    table[1][1] = 1;
    table[1][0] = 1;
    table[0][1] = 1;
    table[0][0] = 1;
    TEST_ASSERT_EQUAL_INT(3, count_neighbours(0, 0));

    init_gol(3,3);    
    TEST_ASSERT_EQUAL_INT(0, count_neighbours(1, 1));
    TEST_ASSERT_EQUAL_INT(0, count_neighbours(0, 0));
    TEST_ASSERT_EQUAL_INT(0, count_neighbours(0, 2));
    TEST_ASSERT_EQUAL_INT(0, count_neighbours(2, 0));
    TEST_ASSERT_EQUAL_INT(0, count_neighbours(2, 2));
    TEST_ASSERT_EQUAL_INT(0, count_neighbours(0, 1));
    TEST_ASSERT_EQUAL_INT(0, count_neighbours(1, 0));
    TEST_ASSERT_EQUAL_INT(0, count_neighbours(2, 1));
    TEST_ASSERT_EQUAL_INT(0, count_neighbours(1, 2));

    init_gol(3,3);
    memset(table[0], 1, 3);
    memset(table[1], 1, 3);
    memset(table[2], 1, 3);
    TEST_ASSERT_EQUAL_INT(8, count_neighbours(1, 1));
    TEST_ASSERT_EQUAL_INT(3, count_neighbours(0, 0));
    TEST_ASSERT_EQUAL_INT(3, count_neighbours(0, 2));
    TEST_ASSERT_EQUAL_INT(3, count_neighbours(2, 0));
    TEST_ASSERT_EQUAL_INT(3, count_neighbours(2, 2));
    TEST_ASSERT_EQUAL_INT(5, count_neighbours(0, 1));
    TEST_ASSERT_EQUAL_INT(5, count_neighbours(1, 0));
    TEST_ASSERT_EQUAL_INT(5, count_neighbours(2, 1));
    TEST_ASSERT_EQUAL_INT(5, count_neighbours(1, 2));
}

void test_is_alive(void)
{
    init_gol(3,3);
// Dead by underpopulation
    TEST_ASSERT_EQUAL_INT(0, is_alive(1, 1, 1));
// Dead by overcrowding
    TEST_ASSERT_EQUAL_INT(0, is_alive(1, 1, 4));
// Dead so still dead
    TEST_ASSERT_EQUAL_INT(0, is_alive(1, 1, 2));

// Alive due born or live
    TEST_ASSERT_EQUAL_INT(1, is_alive(1, 1, 3));
// Alive so still alive
    table[1][1] = 1;
    TEST_ASSERT_EQUAL_INT(1, is_alive(1, 1, 2));

}

void test_handle_result_cache(void)
{
    init_gol(3,3);
    int live[13];
    for (int i = 0; i < 13; i++) {
        live[i] = rand() & 1;
        int cache_next_old = cache_next;
        int res = handle_result_cache(live[i]);

        TEST_ASSERT_EQUAL_INT(result_cache[cache_next_old], live[i]);
        if (i > 4) {
            TEST_ASSERT_EQUAL_INT(live[i-5], res);
        }
    }

}

void test_do_gol_iteration(void)
{
    init_gol(4,4);
    table[1][1] = 1;
    table[2][1] = 1;
    table[1][2] = 1;
    table[2][2] = 1;

// Block pattern
    do_gol_iteration();
    TEST_ASSERT_EQUAL_INT(1, table[1][1]);
    TEST_ASSERT_EQUAL_INT(1, table[2][1]);
    TEST_ASSERT_EQUAL_INT(1, table[1][2]);
    TEST_ASSERT_EQUAL_INT(1, table[2][2]);

    TEST_ASSERT_EQUAL_INT(0, table[0][0]);
    TEST_ASSERT_EQUAL_INT(0, table[1][0]);
    TEST_ASSERT_EQUAL_INT(0, table[2][0]);
    TEST_ASSERT_EQUAL_INT(0, table[3][0]);
    TEST_ASSERT_EQUAL_INT(0, table[0][1]);
    TEST_ASSERT_EQUAL_INT(0, table[3][1]);
    TEST_ASSERT_EQUAL_INT(0, table[0][2]);
    TEST_ASSERT_EQUAL_INT(0, table[3][2]);
    TEST_ASSERT_EQUAL_INT(0, table[0][3]);
    TEST_ASSERT_EQUAL_INT(0, table[1][3]);
    TEST_ASSERT_EQUAL_INT(0, table[2][3]);
    TEST_ASSERT_EQUAL_INT(0, table[3][3]);

// Block pattern with an adjacent cell in the
// top-left corner, complete destruction
// after 4 iterations.
    table[0][1] = 1;
    do_gol_iteration();
    TEST_ASSERT_EQUAL_INT(0, table[0][0]);
    TEST_ASSERT_EQUAL_INT(1, table[1][0]);
    TEST_ASSERT_EQUAL_INT(0, table[2][0]);
    TEST_ASSERT_EQUAL_INT(0, table[3][0]);
    TEST_ASSERT_EQUAL_INT(1, table[0][1]);
    TEST_ASSERT_EQUAL_INT(0, table[1][1]);
    TEST_ASSERT_EQUAL_INT(1, table[2][1]);
    TEST_ASSERT_EQUAL_INT(0, table[3][1]);
    TEST_ASSERT_EQUAL_INT(1, table[0][2]);
    TEST_ASSERT_EQUAL_INT(0, table[1][2]);
    TEST_ASSERT_EQUAL_INT(1, table[2][2]);
    TEST_ASSERT_EQUAL_INT(0, table[3][2]);
    TEST_ASSERT_EQUAL_INT(0, table[0][3]);
    TEST_ASSERT_EQUAL_INT(0, table[1][3]);
    TEST_ASSERT_EQUAL_INT(0, table[2][3]);
    TEST_ASSERT_EQUAL_INT(0, table[3][3]);

    do_gol_iteration();
    TEST_ASSERT_EQUAL_INT(0, table[0][0]);
    TEST_ASSERT_EQUAL_INT(1, table[1][0]);
    TEST_ASSERT_EQUAL_INT(0, table[2][0]);
    TEST_ASSERT_EQUAL_INT(0, table[3][0]);
    TEST_ASSERT_EQUAL_INT(1, table[0][1]);
    TEST_ASSERT_EQUAL_INT(0, table[1][1]);
    TEST_ASSERT_EQUAL_INT(1, table[2][1]);
    TEST_ASSERT_EQUAL_INT(0, table[3][1]);
    TEST_ASSERT_EQUAL_INT(0, table[0][2]);
    TEST_ASSERT_EQUAL_INT(0, table[1][2]);
    TEST_ASSERT_EQUAL_INT(0, table[2][2]);
    TEST_ASSERT_EQUAL_INT(0, table[3][2]);
    TEST_ASSERT_EQUAL_INT(0, table[0][3]);
    TEST_ASSERT_EQUAL_INT(0, table[1][3]);
    TEST_ASSERT_EQUAL_INT(0, table[2][3]);
    TEST_ASSERT_EQUAL_INT(0, table[3][3]);

    do_gol_iteration();
    TEST_ASSERT_EQUAL_INT(0, table[0][0]);
    TEST_ASSERT_EQUAL_INT(1, table[1][0]);
    TEST_ASSERT_EQUAL_INT(0, table[2][0]);
    TEST_ASSERT_EQUAL_INT(0, table[3][0]);
    TEST_ASSERT_EQUAL_INT(0, table[0][1]);
    TEST_ASSERT_EQUAL_INT(1, table[1][1]);
    TEST_ASSERT_EQUAL_INT(0, table[2][1]);
    TEST_ASSERT_EQUAL_INT(0, table[3][1]);
    TEST_ASSERT_EQUAL_INT(0, table[0][2]);
    TEST_ASSERT_EQUAL_INT(0, table[1][2]);
    TEST_ASSERT_EQUAL_INT(0, table[2][2]);
    TEST_ASSERT_EQUAL_INT(0, table[3][2]);
    TEST_ASSERT_EQUAL_INT(0, table[0][3]);
    TEST_ASSERT_EQUAL_INT(0, table[1][3]);
    TEST_ASSERT_EQUAL_INT(0, table[2][3]);
    TEST_ASSERT_EQUAL_INT(0, table[3][3]);

    do_gol_iteration();
    TEST_ASSERT_EQUAL_INT(0, table[0][0]);
    TEST_ASSERT_EQUAL_INT(0, table[1][0]);
    TEST_ASSERT_EQUAL_INT(0, table[2][0]);
    TEST_ASSERT_EQUAL_INT(0, table[3][0]);
    TEST_ASSERT_EQUAL_INT(0, table[0][1]);
    TEST_ASSERT_EQUAL_INT(0, table[1][1]);
    TEST_ASSERT_EQUAL_INT(0, table[2][1]);
    TEST_ASSERT_EQUAL_INT(0, table[3][1]);
    TEST_ASSERT_EQUAL_INT(0, table[0][2]);
    TEST_ASSERT_EQUAL_INT(0, table[1][2]);
    TEST_ASSERT_EQUAL_INT(0, table[2][2]);
    TEST_ASSERT_EQUAL_INT(0, table[3][2]);
    TEST_ASSERT_EQUAL_INT(0, table[0][3]);
    TEST_ASSERT_EQUAL_INT(0, table[1][3]);
    TEST_ASSERT_EQUAL_INT(0, table[2][3]);
    TEST_ASSERT_EQUAL_INT(0, table[3][3]);
}

