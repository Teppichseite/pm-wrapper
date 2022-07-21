typedef unsigned long size_t;
typedef int wchar_t;
typedef enum {
    P_ALL,
    P_PID,
    P_PGID
} idtype_t;
typedef float _Float32;
typedef double _Float64;
typedef double _Float32x;
typedef long double _Float64x;
typedef struct {
    int quot;
    int rem;
} div_t;
typedef struct {
    long quot;
    long rem;
} ldiv_t;
typedef struct {
    long long quot;
    long long rem;
} lldiv_t;
extern size_t __ctype_get_mb_cur_max() __attribute__((nothrow));
extern double atof(const char *__nptr) __attribute__((nothrow)) __attribute__((pure)) __attribute__((nonnull(1)));
extern int atoi(const char *__nptr) __attribute__((nothrow)) __attribute__((pure)) __attribute__((nonnull(1)));
extern long atol(const char *__nptr) __attribute__((nothrow)) __attribute__((pure)) __attribute__((nonnull(1)));
extern long long atoll(const char *__nptr) __attribute__((nothrow)) __attribute__((pure)) __attribute__((nonnull(1)));
extern double strtod(const char *__restrict __nptr, char **__restrict __endptr) __attribute__((nothrow)) __attribute__((nonnull(1)));
extern float strtof(const char *__restrict __nptr, char **__restrict __endptr) __attribute__((nothrow)) __attribute__((nonnull(1)));
extern long double strtold(const char *__restrict __nptr, char **__restrict __endptr) __attribute__((nothrow)) __attribute__((nonnull(1)));
extern long strtol(const char *__restrict __nptr, char **__restrict __endptr, int __base) __attribute__((nothrow)) __attribute__((nonnull(1)));
extern unsigned long strtoul(const char *__restrict __nptr, char **__restrict __endptr, int __base) __attribute__((nothrow)) __attribute__((nonnull(1)));
extern long long strtoq(const char *__restrict __nptr, char **__restrict __endptr, int __base) __attribute__((nothrow)) __attribute__((nonnull(1)));
extern unsigned long long strtouq(const char *__restrict __nptr, char **__restrict __endptr, int __base) __attribute__((nothrow)) __attribute__((nonnull(1)));
extern long long strtoll(const char *__restrict __nptr, char **__restrict __endptr, int __base) __attribute__((nothrow)) __attribute__((nonnull(1)));
extern unsigned long long strtoull(const char *__restrict __nptr, char **__restrict __endptr, int __base) __attribute__((nothrow)) __attribute__((nonnull(1)));
extern char *l64a(long __n) __attribute__((nothrow));
extern long a64l(const char *__s) __attribute__((nothrow)) __attribute__((pure)) __attribute__((nonnull(1)));
typedef unsigned char __u_char;
typedef unsigned short __u_short;
typedef unsigned int __u_int;
typedef unsigned long __u_long;
typedef signed char __int8_t;
typedef unsigned char __uint8_t;
typedef short __int16_t;
typedef unsigned short __uint16_t;
typedef int __int32_t;
typedef unsigned int __uint32_t;
typedef long __int64_t;
typedef unsigned long __uint64_t;
typedef __int8_t __int_least8_t;
typedef __uint8_t __uint_least8_t;
typedef __int16_t __int_least16_t;
typedef __uint16_t __uint_least16_t;
typedef __int32_t __int_least32_t;
typedef __uint32_t __uint_least32_t;
typedef __int64_t __int_least64_t;
typedef __uint64_t __uint_least64_t;
typedef long __quad_t;
typedef unsigned long __u_quad_t;
typedef long __intmax_t;
typedef unsigned long __uintmax_t;
typedef unsigned long __dev_t;
typedef unsigned int __uid_t;
typedef unsigned int __gid_t;
typedef unsigned long __ino_t;
typedef unsigned long __ino64_t;
typedef unsigned int __mode_t;
typedef unsigned long __nlink_t;
typedef long __off_t;
typedef long __off64_t;
typedef int __pid_t;
typedef struct {
    int __val[2];
} __fsid_t;
typedef long __clock_t;
typedef unsigned long __rlim_t;
typedef unsigned long __rlim64_t;
typedef unsigned int __id_t;
typedef long __time_t;
typedef unsigned int __useconds_t;
typedef long __suseconds_t;
typedef int __daddr_t;
typedef int __key_t;
typedef int __clockid_t;
typedef void *__timer_t;
typedef long __blksize_t;
typedef long __blkcnt_t;
typedef long __blkcnt64_t;
typedef unsigned long __fsblkcnt_t;
typedef unsigned long __fsblkcnt64_t;
typedef unsigned long __fsfilcnt_t;
typedef unsigned long __fsfilcnt64_t;
typedef long __fsword_t;
typedef long __ssize_t;
typedef long __syscall_slong_t;
typedef unsigned long __syscall_ulong_t;
typedef __off64_t __loff_t;
typedef char *__caddr_t;
typedef long __intptr_t;
typedef unsigned int __socklen_t;
typedef int __sig_atomic_t;
typedef __u_char u_char;
typedef __u_short u_short;
typedef __u_int u_int;
typedef __u_long u_long;
typedef __quad_t quad_t;
typedef __u_quad_t u_quad_t;
typedef __fsid_t fsid_t;
typedef __loff_t loff_t;
typedef __ino_t ino_t;
typedef __dev_t dev_t;
typedef __gid_t gid_t;
typedef __mode_t mode_t;
typedef __nlink_t nlink_t;
typedef __uid_t uid_t;
typedef __off_t off_t;
typedef __pid_t pid_t;
typedef __id_t id_t;
typedef __ssize_t ssize_t;
typedef __daddr_t daddr_t;
typedef __caddr_t caddr_t;
typedef __key_t key_t;
typedef __clock_t clock_t;
typedef __clockid_t clockid_t;
typedef __time_t time_t;
typedef __timer_t timer_t;
typedef unsigned long ulong;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef __int8_t int8_t;
typedef __int16_t int16_t;
typedef __int32_t int32_t;
typedef __int64_t int64_t;
typedef __uint8_t u_int8_t;
typedef __uint16_t u_int16_t;
typedef __uint32_t u_int32_t;
typedef __uint64_t u_int64_t;
typedef int register_t __attribute__((mode(__word__)));
static inline __uint16_t __bswap_16(__uint16_t __bsx) {
    return ((__uint16_t)((((__bsx) >> 8) & 255) | (((__bsx) & 255) << 8)));
}
static inline __uint32_t __bswap_32(__uint32_t __bsx) {
    return ((((__bsx) & 4278190080U) >> 24) | (((__bsx) & 16711680U) >> 8) | (((__bsx) & 65280U) << 8) | (((__bsx) & 255U) << 24));
}
static inline __uint64_t __bswap_64(__uint64_t __bsx) {
    return ((((__bsx) & 18374686479671623680ULL) >> 56) | (((__bsx) & 71776119061217280ULL) >> 40) | (((__bsx) & 280375465082880ULL) >> 24) | (((__bsx) & 1095216660480ULL) >> 8) | (((__bsx) & 4278190080ULL) << 8) | (((__bsx) & 16711680ULL) << 24) | (((__bsx) & 65280ULL) << 40) | (((__bsx) & 255ULL) << 56));
}
static inline __uint16_t __uint16_identity(__uint16_t __x) {
    return __x;
}
static inline __uint32_t __uint32_identity(__uint32_t __x) {
    return __x;
}
static inline __uint64_t __uint64_identity(__uint64_t __x) {
    return __x;
}
typedef struct {
    unsigned long __val[16];
} __sigset_t;
typedef __sigset_t sigset_t;
struct timeval {
    __time_t tv_sec;
    __suseconds_t tv_usec;
};
struct timespec {
    __time_t tv_sec;
    __syscall_slong_t tv_nsec;
};
typedef __suseconds_t suseconds_t;
typedef long __fd_mask;
typedef struct {
    __fd_mask __fds_bits[16];
} fd_set;
typedef __fd_mask fd_mask;
extern int select(int __nfds, fd_set *__restrict __readfds, fd_set *__restrict __writefds, fd_set *__restrict __exceptfds, struct timeval *__restrict __timeout);
extern int pselect(int __nfds, fd_set *__restrict __readfds, fd_set *__restrict __writefds, fd_set *__restrict __exceptfds, const struct timespec *__restrict __timeout, const __sigset_t *__restrict __sigmask);
typedef __blksize_t blksize_t;
typedef __blkcnt_t blkcnt_t;
typedef __fsblkcnt_t fsblkcnt_t;
typedef __fsfilcnt_t fsfilcnt_t;
typedef struct __pthread_internal_list {
    struct __pthread_internal_list *__prev;
    struct __pthread_internal_list *__next;
} __pthread_list_t;
typedef struct __pthread_internal_slist {
    struct __pthread_internal_slist *__next;
} __pthread_slist_t;
struct __pthread_mutex_s {
    int __lock;
    unsigned int __count;
    int __owner;
    unsigned int __nusers;
    int __kind;
    short __spins;
    short __elision;
    __pthread_list_t __list;
};
struct __pthread_rwlock_arch_t {
    unsigned int __readers;
    unsigned int __writers;
    unsigned int __wrphase_futex;
    unsigned int __writers_futex;
    unsigned int __pad3;
    unsigned int __pad4;
    int __cur_writer;
    int __shared;
    signed char __rwelision;
    unsigned char __pad1[7];
    unsigned long __pad2;
    unsigned int __flags;
};
struct __pthread_cond_s {
    union {
        unsigned long long __wseq;
        struct {
            unsigned int __low;
            unsigned int __high;
        } __wseq32;
    };
    union {
        unsigned long long __g1_start;
        struct {
            unsigned int __low;
            unsigned int __high;
        } __g1_start32;
    };
    unsigned int __g_refs[2];
    unsigned int __g_size[2];
    unsigned int __g1_orig_size;
    unsigned int __wrefs;
    unsigned int __g_signals[2];
};
typedef unsigned long pthread_t;
typedef union {
    char __size[4];
    int __align;
} pthread_mutexattr_t;
typedef union {
    char __size[4];
    int __align;
} pthread_condattr_t;
typedef unsigned int pthread_key_t;
typedef int pthread_once_t;
union pthread_attr_t {
    char __size[56];
    long __align;
};
typedef union pthread_attr_t pthread_attr_t;
typedef union {
    struct __pthread_mutex_s __data;
    char __size[40];
    long __align;
} pthread_mutex_t;
typedef union {
    struct __pthread_cond_s __data;
    char __size[48];
    long long __align;
} pthread_cond_t;
typedef union {
    struct __pthread_rwlock_arch_t __data;
    char __size[56];
    long __align;
} pthread_rwlock_t;
typedef union {
    char __size[8];
    long __align;
} pthread_rwlockattr_t;
typedef volatile int pthread_spinlock_t;
typedef union {
    char __size[32];
    long __align;
} pthread_barrier_t;
typedef union {
    char __size[4];
    int __align;
} pthread_barrierattr_t;
extern long random() __attribute__((nothrow));
extern void srandom(unsigned int __seed) __attribute__((nothrow));
extern char *initstate(unsigned int __seed, char *__statebuf, size_t __statelen) __attribute__((nothrow)) __attribute__((nonnull(2)));
extern char *setstate(char *__statebuf) __attribute__((nothrow)) __attribute__((nonnull(1)));
struct random_data {
    int32_t *fptr;
    int32_t *rptr;
    int32_t *state;
    int rand_type;
    int rand_deg;
    int rand_sep;
    int32_t *end_ptr;
};
extern int random_r(struct random_data *__restrict __buf, int32_t *__restrict __result) __attribute__((nothrow)) __attribute__((nonnull(1, 2)));
extern int srandom_r(unsigned int __seed, struct random_data *__buf) __attribute__((nothrow)) __attribute__((nonnull(2)));
extern int initstate_r(unsigned int __seed, char *__restrict __statebuf, size_t __statelen, struct random_data *__restrict __buf) __attribute__((nothrow)) __attribute__((nonnull(2, 4)));
extern int setstate_r(char *__restrict __statebuf, struct random_data *__restrict __buf) __attribute__((nothrow)) __attribute__((nonnull(1, 2)));
extern int rand() __attribute__((nothrow));
extern void srand(unsigned int __seed) __attribute__((nothrow));
extern int rand_r(unsigned int *__seed) __attribute__((nothrow));
extern double drand48() __attribute__((nothrow));
extern double erand48(unsigned short __xsubi[3]) __attribute__((nothrow)) __attribute__((nonnull(1)));
extern long lrand48() __attribute__((nothrow));
extern long nrand48(unsigned short __xsubi[3]) __attribute__((nothrow)) __attribute__((nonnull(1)));
extern long mrand48() __attribute__((nothrow));
extern long jrand48(unsigned short __xsubi[3]) __attribute__((nothrow)) __attribute__((nonnull(1)));
extern void srand48(long __seedval) __attribute__((nothrow));
extern unsigned short *seed48(unsigned short __seed16v[3]) __attribute__((nothrow)) __attribute__((nonnull(1)));
extern void lcong48(unsigned short __param[7]) __attribute__((nothrow)) __attribute__((nonnull(1)));
struct drand48_data {
    unsigned short __x[3];
    unsigned short __old_x[3];
    unsigned short __c;
    unsigned short __init;
    unsigned long long __a;
};
extern int drand48_r(struct drand48_data *__restrict __buffer, double *__restrict __result) __attribute__((nothrow)) __attribute__((nonnull(1, 2)));
extern int erand48_r(unsigned short __xsubi[3], struct drand48_data *__restrict __buffer, double *__restrict __result) __attribute__((nothrow)) __attribute__((nonnull(1, 2)));
extern int lrand48_r(struct drand48_data *__restrict __buffer, long *__restrict __result) __attribute__((nothrow)) __attribute__((nonnull(1, 2)));
extern int nrand48_r(unsigned short __xsubi[3], struct drand48_data *__restrict __buffer, long *__restrict __result) __attribute__((nothrow)) __attribute__((nonnull(1, 2)));
extern int mrand48_r(struct drand48_data *__restrict __buffer, long *__restrict __result) __attribute__((nothrow)) __attribute__((nonnull(1, 2)));
extern int jrand48_r(unsigned short __xsubi[3], struct drand48_data *__restrict __buffer, long *__restrict __result) __attribute__((nothrow)) __attribute__((nonnull(1, 2)));
extern int srand48_r(long __seedval, struct drand48_data *__buffer) __attribute__((nothrow)) __attribute__((nonnull(2)));
extern int seed48_r(unsigned short __seed16v[3], struct drand48_data *__buffer) __attribute__((nothrow)) __attribute__((nonnull(1, 2)));
extern int lcong48_r(unsigned short __param[7], struct drand48_data *__buffer) __attribute__((nothrow)) __attribute__((nonnull(1, 2)));
extern void *malloc(size_t __size) __attribute__((nothrow)) __attribute__((malloc));
extern void *calloc(size_t __nmemb, size_t __size) __attribute__((nothrow)) __attribute__((malloc));
extern void *realloc(void *__ptr, size_t __size) __attribute__((nothrow)) __attribute__((warn_unused_result("")));
extern void *reallocarray(void *__ptr, size_t __nmemb, size_t __size) __attribute__((nothrow)) __attribute__((warn_unused_result("")));
extern void free(void *__ptr) __attribute__((nothrow));
extern void *alloca(size_t __size) __attribute__((nothrow));
extern void *valloc(size_t __size) __attribute__((nothrow)) __attribute__((malloc));
extern int posix_memalign(void **__memptr, size_t __alignment, size_t __size) __attribute__((nothrow)) __attribute__((nonnull(1)));
extern void *aligned_alloc(size_t __alignment, size_t __size) __attribute__((nothrow)) __attribute__((malloc));
extern void abort() __attribute__((nothrow));
extern int atexit(void (*__func)(void)) __attribute__((nothrow)) __attribute__((nonnull(1)));
extern int at_quick_exit(void (*__func)(void)) __attribute__((nothrow)) __attribute__((nonnull(1)));
extern int on_exit(void (*__func)(int, void *), void *__arg) __attribute__((nothrow)) __attribute__((nonnull(1)));
extern void exit(int __status) __attribute__((nothrow));
extern void quick_exit(int __status) __attribute__((nothrow));
extern void _Exit(int __status) __attribute__((nothrow));
extern char *getenv(const char *__name) __attribute__((nothrow)) __attribute__((nonnull(1)));
extern int putenv(char *__string) __attribute__((nothrow)) __attribute__((nonnull(1)));
extern int setenv(const char *__name, const char *__value, int __replace) __attribute__((nothrow)) __attribute__((nonnull(2)));
extern int unsetenv(const char *__name) __attribute__((nothrow)) __attribute__((nonnull(1)));
extern int clearenv() __attribute__((nothrow));
extern char *mktemp(char *__template) __attribute__((nothrow)) __attribute__((nonnull(1)));
extern int mkstemp(char *__template) __attribute__((nonnull(1)));
extern int mkstemps(char *__template, int __suffixlen) __attribute__((nonnull(1)));
extern char *mkdtemp(char *__template) __attribute__((nothrow)) __attribute__((nonnull(1)));
extern int system(const char *__command);
extern char *realpath(const char *__restrict __name, char *__restrict __resolved) __attribute__((nothrow));
typedef int (*__compar_fn_t)(const void *, const void *);
extern void *bsearch(const void *__key, const void *__base, size_t __nmemb, size_t __size, __compar_fn_t __compar) __attribute__((nonnull(1, 2, 5)));
extern void qsort(void *__base, size_t __nmemb, size_t __size, __compar_fn_t __compar) __attribute__((nonnull(1, 4)));
extern int abs(int __x) __attribute__((nothrow)) __attribute__((const));
extern long labs(long __x) __attribute__((nothrow)) __attribute__((const));
extern long long llabs(long long __x) __attribute__((nothrow)) __attribute__((const));
extern div_t div(int __numer, int __denom) __attribute__((nothrow)) __attribute__((const));
extern ldiv_t ldiv(long __numer, long __denom) __attribute__((nothrow)) __attribute__((const));
extern lldiv_t lldiv(long long __numer, long long __denom) __attribute__((nothrow)) __attribute__((const));
extern char *ecvt(double __value, int __ndigit, int *__restrict __decpt, int *__restrict __sign) __attribute__((nothrow)) __attribute__((nonnull(3, 4)));
extern char *fcvt(double __value, int __ndigit, int *__restrict __decpt, int *__restrict __sign) __attribute__((nothrow)) __attribute__((nonnull(3, 4)));
extern char *gcvt(double __value, int __ndigit, char *__buf) __attribute__((nothrow)) __attribute__((nonnull(3)));
extern char *qecvt(long double __value, int __ndigit, int *__restrict __decpt, int *__restrict __sign) __attribute__((nothrow)) __attribute__((nonnull(3, 4)));
extern char *qfcvt(long double __value, int __ndigit, int *__restrict __decpt, int *__restrict __sign) __attribute__((nothrow)) __attribute__((nonnull(3, 4)));
extern char *qgcvt(long double __value, int __ndigit, char *__buf) __attribute__((nothrow)) __attribute__((nonnull(3)));
extern int ecvt_r(double __value, int __ndigit, int *__restrict __decpt, int *__restrict __sign, char *__restrict __buf, size_t __len) __attribute__((nothrow)) __attribute__((nonnull(3, 4, 5)));
extern int fcvt_r(double __value, int __ndigit, int *__restrict __decpt, int *__restrict __sign, char *__restrict __buf, size_t __len) __attribute__((nothrow)) __attribute__((nonnull(3, 4, 5)));
extern int qecvt_r(long double __value, int __ndigit, int *__restrict __decpt, int *__restrict __sign, char *__restrict __buf, size_t __len) __attribute__((nothrow)) __attribute__((nonnull(3, 4, 5)));
extern int qfcvt_r(long double __value, int __ndigit, int *__restrict __decpt, int *__restrict __sign, char *__restrict __buf, size_t __len) __attribute__((nothrow)) __attribute__((nonnull(3, 4, 5)));
extern int mblen(const char *__s, size_t __n) __attribute__((nothrow));
extern int mbtowc(wchar_t *__restrict __pwc, const char *__restrict __s, size_t __n) __attribute__((nothrow));
extern int wctomb(char *__s, wchar_t __wchar) __attribute__((nothrow));
extern size_t mbstowcs(wchar_t *__restrict __pwcs, const char *__restrict __s, size_t __n) __attribute__((nothrow));
extern size_t wcstombs(char *__restrict __s, const wchar_t *__restrict __pwcs, size_t __n) __attribute__((nothrow));
extern int rpmatch(const char *__response) __attribute__((nothrow)) __attribute__((nonnull(1)));
extern int getsubopt(char **__restrict __optionp, char *const *__restrict __tokens, char **__restrict __valuep) __attribute__((nothrow)) __attribute__((nonnull(1, 2, 3)));
extern int getloadavg(double __loadavg[], int __nelem) __attribute__((nothrow)) __attribute__((nonnull(1)));
typedef unsigned long pm_region_offset;
typedef unsigned long pm_region_id;
typedef unsigned short pm_region_reference_id;
typedef union PmThinPointer {
    struct {
        pm_region_offset offset : 48;
        pm_region_reference_id reference_id : 16;
    };
    void *ptr;
} PmThinPointer;
typedef struct PmMainRegionRoot {
    pm_region_offset program_root;
    pm_region_offset region_id_table;
} PmMainRegionRoot;
typedef struct PmRegionConfig {
    pm_region_reference_id reference_id;
    char *file_path;
    size_t root_size;
    size_t size;
} PmRegionConfig;
typedef struct PmBackend PmBackend;
typedef struct PmWrapperConfig {
    PmBackend *backend;
    PmRegionConfig main_region_config;
} PmWrapperConfig;
typedef struct PmBackendContext {
    PmRegionConfig region_config;
    pm_region_reference_id reference_id;
    pm_region_id id;
    void *data;
} PmBackendContext;
struct PmBackend {
    int (*init)();
    int (*open_or_create)(PmBackendContext *, _Bool *);
    void (*close)(PmBackendContext *);
    void (*finalize)();
    pm_region_offset (*get_root)(PmBackendContext *);
    pm_region_offset (*alloc)(PmBackendContext *, size_t);
    pm_region_offset (*calloc)(PmBackendContext *, size_t);
    void (*free)(PmBackendContext *, pm_region_offset);
    void *(*read_object)(PmBackendContext *, pm_region_offset);
    void (*write_object)(PmBackendContext *, void *, char *, size_t);
};
int pm_init(PmWrapperConfig config);
pm_region_reference_id pm_init_reg(PmRegionConfig region_config);
void *pm_get_root_reg(pm_region_reference_id reference_id);
void *pm_get_root();
void *pm_alloc_reg(int size, pm_region_reference_id reference_id);
void *pm_alloc(int size);
void *pm_calloc_reg(int size, pm_region_reference_id reference_id);
void *pm_calloc(int size);
void pm_free(void *ptr);
void *pm_read_object(void *ptr);
void pm_write_object(void *pm_ptr, char *data, int size);
void pm_copy(void *pm_ptr, char *data, int size);
void pm_close_reg(pm_region_reference_id reference_id);
void pm_close();
extern PmBackend PMDK_BACKEND;
struct holder {
    int values[10000];
};
static struct holder *root;
int pm_store_init(char *path) {
    PmRegionConfig main_region_config = {.file_path = path, .root_size = sizeof(struct holder)};
    PmWrapperConfig config = {.backend = &PMDK_BACKEND, .main_region_config = main_region_config};
    if (pm_init(config) != 0) {
        return 1;
    }
    ;
    root = (struct holder *)pm_get_root();
}
void pm_store_run() {
    int rounds = 1000;
    for (int i = 0; i < 10000 * 100000; i++) {
        root->values[i % rounds] = i;
    }
}
void pm_store_close() {
    pm_close();
}
int main() {
    pm_store_init("t");
    pm_store_run();
    pm_store_close();
    return 0;
}
