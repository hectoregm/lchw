#ifndef __dbg_h_
#define __dbg_h_

#include <stdio.h>
#include <errno.h>
#include <string.h>

#ifdef NDEBUG
#define debug(M, ...)
#else
#define debug(M, ...) fprintf(stderr, "DEBUG %s:%d func: %s " M "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#endif

#define clean_error() (errno == 0 ? "None" : strerror(errno))

#define log_err(M, ...) fprintf(stderr, "[ERROR] (%s:%d func: %s errno: %s) " M "\n", __FILE__, __LINE__, __func__, clean_error(), ##__VA_ARGS__)

#define log_warn(M, ...) fprintf(stderr, "[WARN] (%s:%d func: %s errno: %s) " M "\n", __FILE__, __LINE__, __func__, clean_error(), ##__VA_ARGS__)

#define log_info(M, ...) fprintf(stderr, "[INFO] (%s:%d func: %s) " M "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)

#define check(A, M, ...) if(!(A)) { log_err(M, ##__VA_ARGS__); errno = 0; goto error; }

#define sentinel(M, ...) { log_err(M, ##__VA_ARGS__); errno=0; goto error; }

#define check_mem(A) check((A), "Out of memory.")

#define check_debug(A, M, ...) if(!(A)) { debug(M, ##__VA_ARGS__); errno=0; goto error; }

#endif
