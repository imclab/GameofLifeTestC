#include "gol.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int8_t *result_cache;
int cache_size;
int cache_next;

int8_t **table;
int max_x, max_y;

int count_neighbours(int x, int y)
{
    int ret = 0;

    int ini_i = (x - 1) >= 0? x - 1 : 0;
    int ini_j = (y - 1) >= 0? y - 1 : 0;
    int max_i = (x + 1) < max_x? x + 1 : x;
    int max_j = (y + 1) < max_y? y + 1 : y;

    for (int i = ini_i; i <= max_i; i++) {
        for (int j = ini_j; j <= max_j; j++) {
            if (table[i][j]) {
                ++ret;
            }
        }
    }

    return ret - table[x][y];
}

int is_alive(int x, int y, int n_neighbours)
{
    int ret = 0;
    if ((n_neighbours == 3) || 
        ((n_neighbours == 2) && (table[x][y]))) {
        ret = 1;
    }
    return ret;
}


// FIFO cache, return the value that leaves the cache
int handle_result_cache(int r)
{
    int ret = result_cache[cache_next];
    result_cache[cache_next] = r;

    ++cache_next;
    cache_next %= cache_size;

    return ret;
}

void do_gol_iteration()
{
    for (int j = 0; j < max_y; j++) {
        for (int i = 0; i < max_x; i++) {
            int n = count_neighbours(i, j);
            int res = is_alive(i, j, n);
            res = handle_result_cache(res);

//            if (j > 1) {
//                table[i][j-2] = res;

            // Cache refactor
            if ((j > 1) || (j && (i > 1))) {
                int x = (i - 2) >= 0? (i - 2) : max_x + (i - 2);
                int y = (i - 2) >= 0? (j - 1) : (j - 2);
                table[x][y] = res;
            }
        }
    }

//    // Coppy last two lines from the cache.
//    for (int j = 0; j < 2; j++) {
//        for (int i = 0; i < max_x; i++) {
//            int res = handle_result_cache(0);
//            table[i][max_y - 2 + j] = res;
//        }

    // Cache refactor
    // Coppy last line line plus 2 last cells from
    // next-to-last from the cache to the table
    int res = handle_result_cache(0);
    table[max_x - 2][max_y - 2] = res;
    res = handle_result_cache(0);
    table[max_x - 1][max_y - 2] = res;

    for (int i = 0; i < max_x; i++) {
        int res = handle_result_cache(0);
        table[i][max_y - 1] = res;
    }
}

void init_gol(int x, int y)
{
    end();

//    cache_size = 2*x;

    // Cache refactor
    cache_size = x + 2;
    result_cache = (int8_t*) malloc(sizeof(int8_t) * cache_size);

    table = (int8_t**) malloc(sizeof(int8_t*) * x);
    for(int i = 0; i < x; i++)
        table[i]=(int8_t*) calloc(sizeof(int8_t), y);
    max_x = x;
    max_y = y;
}

__attribute__((destructor)) void end(void)
{
    if (result_cache) {
        free(result_cache);
    }

    if (table) {
        for(int i = 0; i < max_x; i++)
            free(table[i]);
        free(table);
    }
}

