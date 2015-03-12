#ifndef __GOL_H_
#define __GOL_H_

int count_neighbours(int x, int y);
int is_alive(int x, int y, int n_neighbours);
int handle_result_cache(int r);
void do_gol_iteration();
void init_gol(int x, int y);
__attribute__((destructor)) void end(void);

#endif

