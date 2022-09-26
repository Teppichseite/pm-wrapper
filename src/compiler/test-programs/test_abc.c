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
typedef __uint8_t uint8_t;
typedef __uint16_t uint16_t;
typedef __uint32_t uint32_t;
typedef __uint64_t uint64_t;
typedef __int_least8_t int_least8_t;
typedef __int_least16_t int_least16_t;
typedef __int_least32_t int_least32_t;
typedef __int_least64_t int_least64_t;
typedef __uint_least8_t uint_least8_t;
typedef __uint_least16_t uint_least16_t;
typedef __uint_least32_t uint_least32_t;
typedef __uint_least64_t uint_least64_t;
typedef signed char int_fast8_t;
typedef long int_fast16_t;
typedef long int_fast32_t;
typedef long int_fast64_t;
typedef unsigned char uint_fast8_t;
typedef unsigned long uint_fast16_t;
typedef unsigned long uint_fast32_t;
typedef unsigned long uint_fast64_t;
typedef long intptr_t;
typedef unsigned long uintptr_t;
typedef __intmax_t intmax_t;
typedef __uintmax_t uintmax_t;
typedef int __gwchar_t;
typedef struct {
    long quot;
    long rem;
} imaxdiv_t;
extern intmax_t imaxabs(intmax_t __n) __attribute__((nothrow)) __attribute__((const));
extern imaxdiv_t imaxdiv(intmax_t __numer, intmax_t __denom) __attribute__((nothrow)) __attribute__((const));
extern intmax_t strtoimax(const char *__restrict __nptr, char **__restrict __endptr, int __base) __attribute__((nothrow));
extern uintmax_t strtoumax(const char *__restrict __nptr, char **__restrict __endptr, int __base) __attribute__((nothrow));
extern intmax_t wcstoimax(const __gwchar_t *__restrict __nptr, __gwchar_t **__restrict __endptr, int __base) __attribute__((nothrow));
extern uintmax_t wcstoumax(const __gwchar_t *__restrict __nptr, __gwchar_t **__restrict __endptr, int __base) __attribute__((nothrow));
typedef __builtin_va_list va_list;
typedef __builtin_va_list __gnuc_va_list;
typedef struct {
    int __count;
    union {
        unsigned int __wch;
        char __wchb[4];
    } __value;
} __mbstate_t;
typedef struct _G_fpos_t {
    __off_t __pos;
    __mbstate_t __state;
} __fpos_t;
typedef struct _G_fpos64_t {
    __off64_t __pos;
    __mbstate_t __state;
} __fpos64_t;
struct _IO_FILE;
typedef struct _IO_FILE __FILE;
struct _IO_FILE;
typedef struct _IO_FILE FILE;
struct _IO_FILE;
struct _IO_marker;
struct _IO_codecvt;
struct _IO_wide_data;
typedef void _IO_lock_t;
struct _IO_FILE {
    int _flags;
    char *_IO_read_ptr;
    char *_IO_read_end;
    char *_IO_read_base;
    char *_IO_write_base;
    char *_IO_write_ptr;
    char *_IO_write_end;
    char *_IO_buf_base;
    char *_IO_buf_end;
    char *_IO_save_base;
    char *_IO_backup_base;
    char *_IO_save_end;
    struct _IO_marker *_markers;
    struct _IO_FILE *_chain;
    int _fileno;
    int _flags2;
    __off_t _old_offset;
    unsigned short _cur_column;
    signed char _vtable_offset;
    char _shortbuf[1];
    _IO_lock_t *_lock;
    __off64_t _offset;
    struct _IO_codecvt *_codecvt;
    struct _IO_wide_data *_wide_data;
    struct _IO_FILE *_freeres_list;
    void *_freeres_buf;
    size_t __pad5;
    int _mode;
    char _unused2[20];
};
typedef __gnuc_va_list va_list;
typedef __fpos_t fpos_t;
extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;
extern int remove(const char *__filename) __attribute__((nothrow));
extern int rename(const char *__old, const char *__new) __attribute__((nothrow));
extern int renameat(int __oldfd, const char *__old, int __newfd, const char *__new) __attribute__((nothrow));
extern FILE *tmpfile();
extern char *tmpnam(char *__s) __attribute__((nothrow));
extern char *tmpnam_r(char *__s) __attribute__((nothrow));
extern char *tempnam(const char *__dir, const char *__pfx) __attribute__((nothrow)) __attribute__((malloc));
extern int fclose(FILE *__stream);
extern int fflush(FILE *__stream);
extern int fflush_unlocked(FILE *__stream);
extern FILE *fopen(const char *__restrict __filename, const char *__restrict __modes);
extern FILE *freopen(const char *__restrict __filename, const char *__restrict __modes, FILE *__restrict __stream);
extern FILE *fdopen(int __fd, const char *__modes) __attribute__((nothrow));
extern FILE *fmemopen(void *__s, size_t __len, const char *__modes) __attribute__((nothrow));
extern FILE *open_memstream(char **__bufloc, size_t *__sizeloc) __attribute__((nothrow));
extern void setbuf(FILE *__restrict __stream, char *__restrict __buf) __attribute__((nothrow));
extern int setvbuf(FILE *__restrict __stream, char *__restrict __buf, int __modes, size_t __n) __attribute__((nothrow));
extern void setbuffer(FILE *__restrict __stream, char *__restrict __buf, size_t __size) __attribute__((nothrow));
extern void setlinebuf(FILE *__stream) __attribute__((nothrow));
extern int fprintf(FILE *__restrict __stream, const char *__restrict __format, ...);
extern int printf(const char *__restrict __format, ...);
extern int sprintf(char *__restrict __s, const char *__restrict __format, ...) __attribute__((nothrow));
extern int vfprintf(FILE *__restrict __s, const char *__restrict __format, __gnuc_va_list __arg);
extern int vprintf(const char *__restrict __format, __gnuc_va_list __arg);
extern int vsprintf(char *__restrict __s, const char *__restrict __format, __gnuc_va_list __arg) __attribute__((nothrow));
extern int snprintf(char *__restrict __s, size_t __maxlen, const char *__restrict __format, ...) __attribute__((nothrow)) __attribute__((format(printf, 3, 4)));
extern int vsnprintf(char *__restrict __s, size_t __maxlen, const char *__restrict __format, __gnuc_va_list __arg) __attribute__((nothrow)) __attribute__((format(printf, 3, 0)));
extern int vdprintf(int __fd, const char *__restrict __fmt, __gnuc_va_list __arg) __attribute__((format(printf, 2, 0)));
extern int dprintf(int __fd, const char *__restrict __fmt, ...) __attribute__((format(printf, 2, 3)));
extern int fscanf(FILE *__restrict __stream, const char *__restrict __format, ...);
extern int scanf(const char *__restrict __format, ...);
extern int sscanf(const char *__restrict __s, const char *__restrict __format, ...) __attribute__((nothrow));
extern int fscanf(FILE *__restrict __stream, const char *__restrict __format, ...) asm("__isoc99_fscanf");
extern int scanf(const char *__restrict __format, ...) asm("__isoc99_scanf");
extern int sscanf(const char *__restrict __s, const char *__restrict __format, ...) asm("__isoc99_sscanf") __attribute__((nothrow));
extern int vfscanf(FILE *__restrict __s, const char *__restrict __format, __gnuc_va_list __arg) __attribute__((format(scanf, 2, 0)));
extern int vscanf(const char *__restrict __format, __gnuc_va_list __arg) __attribute__((format(scanf, 1, 0)));
extern int vsscanf(const char *__restrict __s, const char *__restrict __format, __gnuc_va_list __arg) __attribute__((nothrow)) __attribute__((format(scanf, 2, 0)));
extern int vfscanf(FILE *__restrict __s, const char *__restrict __format, __gnuc_va_list __arg) asm("__isoc99_vfscanf") __attribute__((format(scanf, 2, 0)));
extern int vscanf(const char *__restrict __format, __gnuc_va_list __arg) asm("__isoc99_vscanf") __attribute__((format(scanf, 1, 0)));
extern int vsscanf(const char *__restrict __s, const char *__restrict __format, __gnuc_va_list __arg) asm("__isoc99_vsscanf") __attribute__((nothrow)) __attribute__((format(scanf, 2, 0)));
extern int fgetc(FILE *__stream);
extern int getc(FILE *__stream);
extern int getchar();
extern int getc_unlocked(FILE *__stream);
extern int getchar_unlocked();
extern int fgetc_unlocked(FILE *__stream);
extern int fputc(int __c, FILE *__stream);
extern int putc(int __c, FILE *__stream);
extern int putchar(int __c);
extern int fputc_unlocked(int __c, FILE *__stream);
extern int putc_unlocked(int __c, FILE *__stream);
extern int putchar_unlocked(int __c);
extern int getw(FILE *__stream);
extern int putw(int __w, FILE *__stream);
extern char *fgets(char *__restrict __s, int __n, FILE *__restrict __stream);
extern __ssize_t __getdelim(char **__restrict __lineptr, size_t *__restrict __n, int __delimiter, FILE *__restrict __stream);
extern __ssize_t getdelim(char **__restrict __lineptr, size_t *__restrict __n, int __delimiter, FILE *__restrict __stream);
extern __ssize_t getline(char **__restrict __lineptr, size_t *__restrict __n, FILE *__restrict __stream);
extern int fputs(const char *__restrict __s, FILE *__restrict __stream);
extern int puts(const char *__s);
extern int ungetc(int __c, FILE *__stream);
extern unsigned long fread(void *__restrict __ptr, size_t __size, size_t __n, FILE *__restrict __stream);
extern unsigned long fwrite(const void *__restrict __ptr, size_t __size, size_t __n, FILE *__restrict __s);
extern size_t fread_unlocked(void *__restrict __ptr, size_t __size, size_t __n, FILE *__restrict __stream);
extern size_t fwrite_unlocked(const void *__restrict __ptr, size_t __size, size_t __n, FILE *__restrict __stream);
extern int fseek(FILE *__stream, long __off, int __whence);
extern long ftell(FILE *__stream);
extern void rewind(FILE *__stream);
extern int fseeko(FILE *__stream, __off_t __off, int __whence);
extern __off_t ftello(FILE *__stream);
extern int fgetpos(FILE *__restrict __stream, fpos_t *__restrict __pos);
extern int fsetpos(FILE *__stream, const fpos_t *__pos);
extern void clearerr(FILE *__stream) __attribute__((nothrow));
extern int feof(FILE *__stream) __attribute__((nothrow));
extern int ferror(FILE *__stream) __attribute__((nothrow));
extern void clearerr_unlocked(FILE *__stream) __attribute__((nothrow));
extern int feof_unlocked(FILE *__stream) __attribute__((nothrow));
extern int ferror_unlocked(FILE *__stream) __attribute__((nothrow));
extern void perror(const char *__s);
extern int sys_nerr;
extern const char *const sys_errlist[];
extern int fileno(FILE *__stream) __attribute__((nothrow));
extern int fileno_unlocked(FILE *__stream) __attribute__((nothrow));
extern FILE *popen(const char *__command, const char *__modes);
extern int pclose(FILE *__stream);
extern char *ctermid(char *__s) __attribute__((nothrow));
extern void flockfile(FILE *__stream) __attribute__((nothrow));
extern int ftrylockfile(FILE *__stream) __attribute__((nothrow));
extern void funlockfile(FILE *__stream) __attribute__((nothrow));
extern int __uflow(FILE *);
extern int __overflow(FILE *, int);
extern void *memcpy(void *__restrict __dest, const void *__restrict __src, size_t __n) __attribute__((nothrow)) __attribute__((nonnull(1, 2)));
extern void *memmove(void *__dest, const void *__src, size_t __n) __attribute__((nothrow)) __attribute__((nonnull(1, 2)));
extern void *memccpy(void *__restrict __dest, const void *__restrict __src, int __c, size_t __n) __attribute__((nothrow)) __attribute__((nonnull(1, 2)));
extern void *memset(void *__s, int __c, size_t __n) __attribute__((nothrow)) __attribute__((nonnull(1)));
extern int memcmp(const void *__s1, const void *__s2, size_t __n) __attribute__((nothrow)) __attribute__((pure)) __attribute__((nonnull(1, 2)));
extern void *memchr(const void *__s, int __c, size_t __n) __attribute__((nothrow)) __attribute__((pure)) __attribute__((nonnull(1)));
extern char *strcpy(char *__restrict __dest, const char *__restrict __src) __attribute__((nothrow)) __attribute__((nonnull(1, 2)));
extern char *strncpy(char *__restrict __dest, const char *__restrict __src, size_t __n) __attribute__((nothrow)) __attribute__((nonnull(1, 2)));
extern char *strcat(char *__restrict __dest, const char *__restrict __src) __attribute__((nothrow)) __attribute__((nonnull(1, 2)));
extern char *strncat(char *__restrict __dest, const char *__restrict __src, size_t __n) __attribute__((nothrow)) __attribute__((nonnull(1, 2)));
extern int strcmp(const char *__s1, const char *__s2) __attribute__((nothrow)) __attribute__((pure)) __attribute__((nonnull(1, 2)));
extern int strncmp(const char *__s1, const char *__s2, size_t __n) __attribute__((nothrow)) __attribute__((pure)) __attribute__((nonnull(1, 2)));
extern int strcoll(const char *__s1, const char *__s2) __attribute__((nothrow)) __attribute__((pure)) __attribute__((nonnull(1, 2)));
extern unsigned long strxfrm(char *__restrict __dest, const char *__restrict __src, size_t __n) __attribute__((nothrow)) __attribute__((nonnull(2)));
struct __locale_struct {
    struct __locale_data *__locales[13];
    const unsigned short *__ctype_b;
    const int *__ctype_tolower;
    const int *__ctype_toupper;
    const char *__names[13];
};
typedef struct __locale_struct *__locale_t;
typedef __locale_t locale_t;
extern int strcoll_l(const char *__s1, const char *__s2, locale_t __l) __attribute__((nothrow)) __attribute__((pure)) __attribute__((nonnull(1, 2, 3)));
extern size_t strxfrm_l(char *__dest, const char *__src, size_t __n, locale_t __l) __attribute__((nothrow)) __attribute__((nonnull(2, 4)));
extern char *strdup(const char *__s) __attribute__((nothrow)) __attribute__((malloc)) __attribute__((nonnull(1)));
extern char *strndup(const char *__string, size_t __n) __attribute__((nothrow)) __attribute__((malloc)) __attribute__((nonnull(1)));
extern char *strchr(const char *__s, int __c) __attribute__((nothrow)) __attribute__((pure)) __attribute__((nonnull(1)));
extern char *strrchr(const char *__s, int __c) __attribute__((nothrow)) __attribute__((pure)) __attribute__((nonnull(1)));
extern unsigned long strcspn(const char *__s, const char *__reject) __attribute__((nothrow)) __attribute__((pure)) __attribute__((nonnull(1, 2)));
extern unsigned long strspn(const char *__s, const char *__accept) __attribute__((nothrow)) __attribute__((pure)) __attribute__((nonnull(1, 2)));
extern char *strpbrk(const char *__s, const char *__accept) __attribute__((nothrow)) __attribute__((pure)) __attribute__((nonnull(1, 2)));
extern char *strstr(const char *__haystack, const char *__needle) __attribute__((nothrow)) __attribute__((pure)) __attribute__((nonnull(1, 2)));
extern char *strtok(char *__restrict __s, const char *__restrict __delim) __attribute__((nothrow)) __attribute__((nonnull(2)));
extern char *__strtok_r(char *__restrict __s, const char *__restrict __delim, char **__restrict __save_ptr) __attribute__((nothrow)) __attribute__((nonnull(2, 3)));
extern char *strtok_r(char *__restrict __s, const char *__restrict __delim, char **__restrict __save_ptr) __attribute__((nothrow)) __attribute__((nonnull(2, 3)));
extern unsigned long strlen(const char *__s) __attribute__((nothrow)) __attribute__((pure)) __attribute__((nonnull(1)));
extern size_t strnlen(const char *__string, size_t __maxlen) __attribute__((nothrow)) __attribute__((pure)) __attribute__((nonnull(1)));
extern char *strerror(int __errnum) __attribute__((nothrow));
extern int strerror_r(int __errnum, char *__buf, size_t __buflen) asm("__xpg_strerror_r") __attribute__((nothrow)) __attribute__((nonnull(2)));
extern char *strerror_l(int __errnum, locale_t __l) __attribute__((nothrow));
extern int bcmp(const void *__s1, const void *__s2, size_t __n) __attribute__((nothrow)) __attribute__((pure)) __attribute__((nonnull(1, 2)));
extern void bcopy(const void *__src, void *__dest, size_t __n) __attribute__((nothrow)) __attribute__((nonnull(1, 2)));
extern void bzero(void *__s, size_t __n) __attribute__((nothrow)) __attribute__((nonnull(1)));
extern char *index(const char *__s, int __c) __attribute__((nothrow)) __attribute__((pure)) __attribute__((nonnull(1)));
extern char *rindex(const char *__s, int __c) __attribute__((nothrow)) __attribute__((pure)) __attribute__((nonnull(1)));
extern int ffs(int __i) __attribute__((nothrow)) __attribute__((const));
extern int ffsl(long __l) __attribute__((nothrow)) __attribute__((const));
extern int ffsll(long long __ll) __attribute__((nothrow)) __attribute__((const));
extern int strcasecmp(const char *__s1, const char *__s2) __attribute__((nothrow)) __attribute__((pure)) __attribute__((nonnull(1, 2)));
extern int strncasecmp(const char *__s1, const char *__s2, size_t __n) __attribute__((nothrow)) __attribute__((pure)) __attribute__((nonnull(1, 2)));
extern int strcasecmp_l(const char *__s1, const char *__s2, locale_t __loc) __attribute__((nothrow)) __attribute__((pure)) __attribute__((nonnull(1, 2, 3)));
extern int strncasecmp_l(const char *__s1, const char *__s2, size_t __n, locale_t __loc) __attribute__((nothrow)) __attribute__((pure)) __attribute__((nonnull(1, 2, 4)));
extern void explicit_bzero(void *__s, size_t __n) __attribute__((nothrow)) __attribute__((nonnull(1)));
extern char *strsep(char **__restrict __stringp, const char *__restrict __delim) __attribute__((nothrow)) __attribute__((nonnull(1, 2)));
extern char *strsignal(int __sig) __attribute__((nothrow));
extern char *__stpcpy(char *__restrict __dest, const char *__restrict __src) __attribute__((nothrow)) __attribute__((nonnull(1, 2)));
extern char *stpcpy(char *__restrict __dest, const char *__restrict __src) __attribute__((nothrow)) __attribute__((nonnull(1, 2)));
extern char *__stpncpy(char *__restrict __dest, const char *__restrict __src, size_t __n) __attribute__((nothrow)) __attribute__((nonnull(1, 2)));
extern char *stpncpy(char *__restrict __dest, const char *__restrict __src, size_t __n) __attribute__((nothrow)) __attribute__((nonnull(1, 2)));
struct stat {
    __dev_t st_dev;
    __ino_t st_ino;
    __nlink_t st_nlink;
    __mode_t st_mode;
    __uid_t st_uid;
    __gid_t st_gid;
    int __pad0;
    __dev_t st_rdev;
    __off_t st_size;
    __blksize_t st_blksize;
    __blkcnt_t st_blocks;
    struct timespec st_atim;
    struct timespec st_mtim;
    struct timespec st_ctim;
    __syscall_slong_t __glibc_reserved[3];
};
extern int stat(const char *__restrict __file, struct stat *__restrict __buf) __attribute__((nothrow)) __attribute__((nonnull(1, 2)));
extern int fstat(int __fd, struct stat *__buf) __attribute__((nothrow)) __attribute__((nonnull(2)));
extern int fstatat(int __fd, const char *__restrict __file, struct stat *__restrict __buf, int __flag) __attribute__((nothrow)) __attribute__((nonnull(2, 3)));
extern int lstat(const char *__restrict __file, struct stat *__restrict __buf) __attribute__((nothrow)) __attribute__((nonnull(1, 2)));
extern int chmod(const char *__file, __mode_t __mode) __attribute__((nothrow)) __attribute__((nonnull(1)));
extern int lchmod(const char *__file, __mode_t __mode) __attribute__((nothrow)) __attribute__((nonnull(1)));
extern int fchmod(int __fd, __mode_t __mode) __attribute__((nothrow));
extern int fchmodat(int __fd, const char *__file, __mode_t __mode, int __flag) __attribute__((nothrow)) __attribute__((nonnull(2)));
extern __mode_t umask(__mode_t __mask) __attribute__((nothrow));
extern int mkdir(const char *__path, __mode_t __mode) __attribute__((nothrow)) __attribute__((nonnull(1)));
extern int mkdirat(int __fd, const char *__path, __mode_t __mode) __attribute__((nothrow)) __attribute__((nonnull(2)));
extern int mknod(const char *__path, __mode_t __mode, __dev_t __dev) __attribute__((nothrow)) __attribute__((nonnull(1)));
extern int mknodat(int __fd, const char *__path, __mode_t __mode, __dev_t __dev) __attribute__((nothrow)) __attribute__((nonnull(2)));
extern int mkfifo(const char *__path, __mode_t __mode) __attribute__((nothrow)) __attribute__((nonnull(1)));
extern int mkfifoat(int __fd, const char *__path, __mode_t __mode) __attribute__((nothrow)) __attribute__((nonnull(2)));
extern int utimensat(int __fd, const char *__path, const struct timespec __times[2], int __flags) __attribute__((nothrow)) __attribute__((nonnull(2)));
extern int futimens(int __fd, const struct timespec __times[2]) __attribute__((nothrow));
extern int __fxstat(int __ver, int __fildes, struct stat *__stat_buf) __attribute__((nothrow)) __attribute__((nonnull(3)));
extern int __xstat(int __ver, const char *__filename, struct stat *__stat_buf) __attribute__((nothrow)) __attribute__((nonnull(2, 3)));
extern int __lxstat(int __ver, const char *__filename, struct stat *__stat_buf) __attribute__((nothrow)) __attribute__((nonnull(2, 3)));
extern int __fxstatat(int __ver, int __fildes, const char *__filename, struct stat *__stat_buf, int __flag) __attribute__((nothrow)) __attribute__((nonnull(3, 4)));
extern int __xmknod(int __ver, const char *__path, __mode_t __mode, __dev_t *__dev) __attribute__((nothrow)) __attribute__((nonnull(2, 4)));
extern int __xmknodat(int __ver, int __fd, const char *__path, __mode_t __mode, __dev_t *__dev) __attribute__((nothrow)) __attribute__((nonnull(3, 5)));
struct btree_node {
    int64_t key;
    struct btree_node *slots[2];
    char value[];
};
struct btree {
    struct btree_node *root;
};
static void btree_insert(struct btree *btree, int64_t key, char *value) {
    struct btree_node **dst = &btree->root;
    while (*dst != ((void *)0))
        {
            dst = &(*dst)->slots[key > (*dst)->key];
        }
    struct btree_node *new_node = (struct btree_node *)pm_alloc(sizeof(struct btree_node *) + strlen(value) + 1);
    new_node->key = key;
    pm_write_object(&new_node->value, value, strlen(value) + 1);
    *dst = new_node;
}
static char *btree_find(struct btree *btree, int64_t key) {
    struct btree_node *node = btree->root;
    while (node != ((void *)0))
        {
            if (node->key == key)
                return node->value;
            else
                node = node->slots[key > node->key];
        }
    return ((void *)0);
}
static void btree_node_print(struct btree_node *node __attribute__((annotate("pointer_type", 0x55ad0dc36a88)))) {
    printf("%lu %s\n", node->key, node->value);
}
static void btree_foreach(struct btree *btree, struct btree_node *node, void (*cb)(struct btree_node *)) {
    if (node == ((void *)0)) {
        return;
    }
    node->key += 10;
    btree_foreach(btree, node->slots[0], cb);
    cb(node);
    btree_foreach(btree, node->slots[1], cb);
}
static void btree_print(struct btree *btree) {
    btree_foreach(btree, btree->root, btree_node_print);
}
int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("usage: %s file-name [p|i|f] [key] [value] \n", argv[0]);
        return 1;
    }
    char *path = argv[1];
    PmRegionConfig main_region_config = {.file_path = "./test", .root_size = sizeof(struct btree)};
    PmWrapperConfig config = {.backend = &PMDK_BACKEND, .main_region_config = main_region_config};
    if (pm_init(config) != 0) {
        return 1;
    }
    ;
    struct btree *btree = (struct btree *)pm_get_root();
    char op = argv[2][0];
    int64_t key;
    char *value;
    switch (op) {
      case 'p':
        btree_print(btree);
        break;
      case 'i':
        key = atoll(argv[3]);
        value = argv[4];
        btree_insert(btree, key, value);
        break;
      case 'f':
        key = atoll(argv[3]);
        char *found_value;
        if ((found_value = btree_find(btree, key)) != ((void *)0))
            printf("%s\n", found_value);
        else
            printf("not found\n");
        break;
      default:
        printf("invalid operation\n");
        break;
    }
    pm_close();
    return 0;
}
