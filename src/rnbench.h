#ifndef _RNBENCH_H
#define _RNBENCH_H

#define RNBENCH_VERSION "1.0.0"

#define RNBENCH_PROGRESS_FINISHED 999

/* benchmark progress callback function, 0 - 100 */
typedef void (*rnbench_progress_fn)(int progress);

/* start benchmark */
unsigned int rnbench_run(rnbench_progress_fn progress_fn);

#endif  /* _RNBENCH_H */