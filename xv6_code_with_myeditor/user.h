// system calls
int fork(void);
int exit(void) __attribute__((noreturn));
int wait(void);
int pipe(int*);
int write(int, void*, int);
int read(int, void*, int);
int close(int);
int kill(int);
int exec(char*, char**);
int open(char*, int);
int mknod(char*, short, short, short);
int unlink(char*);
int fstat(int fd, struct stat*);
int link(char*, char*);
int mkdir(char*);
int chdir(char*);
int dup(int);
int getpid();
char* sbrk(int);

// ulib.c
int stat(char*, struct stat*);
int puts(char*);
char* strcpy(char*, char*);
char* strchr(const char*, char c);
int strcmp(const char*, const char*);
void printf(int, char*, ...);
char* gets(char*, int max);
unsigned int strlen(char*);
void* memset(void*, int, unsigned int);//将已开辟内存空间 dst 的首 n 个字节的值设为值 c
void* malloc(uint);
void free(void*);
int atoi(char* src);//ooo
char* strcat(char* dest, char* src, int n);//ooo
//int atoi(char s[]); //ooo
