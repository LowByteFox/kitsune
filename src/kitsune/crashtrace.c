#include <kitsune/numbers.h>
#include <execinfo.h>
#include <kitsune/crashtrace.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/signal.h>
#include <unistd.h>
#include <spawn.h>
#include <sys/wait.h>
#include <fcntl.h>

#if defined(__FreeBSD__)
#define TRACE_BIN "llvm-symbolizer"
#elif defined(__linux__)
#define TRACE_BIN "addr2line"
#endif

void
kitsune_print_backtrace(usize size, void **arr, bool skip)
{
        char **strs = backtrace_symbols_fmt(arr, size, "%a %f");
        usize i;

        for (i = 0; i < size; free(strs[i]), i++) {
                if (i == 0 && skip)
                        continue;
                /* 
                 * The 1 is being added so when strtok places 0 there, the file
                 * path is accesible
                 */
                char *file = strstr(strs[i], " ") + 1;
                char *addr = strtok(strs[i], " ");

                if (strncmp(file, "???", 3) == 0)
                        continue;

                int pipefd[2];
                pid_t pid;
                posix_spawn_file_actions_t actions;
                int dev_null = open("/dev/null", O_WRONLY);

                if (pipe(pipefd) == -1) {
                        perror("pipe");
                        exit(1);
                }

                posix_spawn_file_actions_init(&actions);
                posix_spawn_file_actions_adddup2(&actions, pipefd[1],
                    STDOUT_FILENO);
                posix_spawn_file_actions_adddup2(&actions, dev_null,
                    STDERR_FILENO);
                posix_spawn_file_actions_addclose(&actions, pipefd[0]);

                char *argv[] = {TRACE_BIN, "-e", file, addr, "-f", NULL};

                if (posix_spawnp(&pid, TRACE_BIN, &actions, NULL, argv, NULL)
                    != 0) {
                        perror("posix_spawn");
                        exit(EXIT_FAILURE);
                }

                close(pipefd[1]);

                char buffer[1024];
                memset(buffer, 0, 1024);
                read(pipefd[0], buffer, sizeof(buffer));

                close(pipefd[0]);
                int status;
                waitpid(pid, &status, 0);
                posix_spawn_file_actions_destroy(&actions);

                char *path = strstr(buffer, "\n") + 1;
                char *fn = strtok(buffer, "\n");

                if (strncmp(buffer, "??", 2) == 0)
                        continue;

                char *line_num = strstr(path, ":") + 1;
                line_num = strtok(line_num, "\n");
                path = strtok(path, ":");
                printf("  #%ld [%s] in %s => %s:%s\n", i, addr, path, fn, line_num);
        }

        free(strs);
}

static void
handler(int sig)
{
        void *array[16];
        usize size;

        size = backtrace(array, 16);

        fprintf(stderr, "\n!!! Crashtrace recieved a signal: %s\n",
            strsignal(sig));
#if defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__)
        kitsune_print_backtrace(size, array, true);
#elif defined(__linux__)
        backtrace_symbols_fd(array, 16, STDERR_FILENO);
#else
#error "Unknown platform, does it have backtrace? If yes, check and make PR"
#endif
        exit(1);
}

void
kitsune_install_crashtrace()
{
        signal(SIGSEGV, handler);
        signal(SIGABRT, handler);
}
