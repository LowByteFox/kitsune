#include <numbers.h>
#include <execinfo.h>
#include <crashtrace.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/signal.h>
#include <unistd.h>
#include <spawn.h>
#include <sys/wait.h>
#include <fcntl.h>

void
kitsune_print_backtrace(usize size, void **arr)
{
        char **strs = backtrace_symbols_fmt(arr, size, "%a %f");
        usize i;

        for (i = 0; i < size; free(strs[i]), i++) {
                /* 
                 * The 1 is being added so when strtok places 0 there, the file
                 * path is accesible
                 */
                char *file = strstr(strs[i], " ") + 1;
                char *addr = strtok(strs[i], " ");

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

                char *argv[] = {"addr2line", "-e", file, addr, "-f", NULL};

                if (posix_spawnp(&pid, "addr2line", &actions, NULL, argv, NULL)
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

        fprintf(stderr, "\n!!! Recieved signal: %s\n", strsignal(sig));
        kitsune_print_backtrace(size, array);
        exit(1);
}

void
kitsune_install_crashtrace()
{
        signal(SIGSEGV, handler);
        signal(SIGABRT, handler);
}
