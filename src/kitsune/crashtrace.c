/*
 * Copyright (C) 2024 LowByteFox
 * This file is part of kitsune.
 *
 * kitsune is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3.0 of the License, or
 * (at your option) any later version.
 *
 * kitsune is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with kitsune; if not, see <http://www.gnu.org/licenses/>.
 */

#include <kitsune/numbers.h>
#include <kitsune/crashtrace.h>
#include <kitsune/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <spawn.h>
#include <sys/wait.h>
#include <fcntl.h>

#ifdef KITSUNE_TARGET_BSD
#include <execinfo.h>
#define TRACE_BIN "llvm-symbolizer"
#endif

void
kitsune_print_backtrace(usize size, void **arr, bool skip)
{
#ifdef FEATURE_CRASHTRACE
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
#endif
}

static void
handler(int sig)
{
#ifdef FEATURE_CRASHTRACE
        void *array[16];
        usize size;

        size = backtrace(array, 16);

        fprintf(stderr, "\n!!! Crashtrace recieved a signal: %s\n",
            strsignal(sig));
#if KITSUNE_TARGET_BSD
        kitsune_print_backtrace(size, array, true);
#else
#error "Unknown platform, does it have backtrace? If yes, check and make PR"
#endif
#endif
        exit(1);
}

void
kitsune_install_crashtrace()
{
        signal(SIGSEGV, handler);
        signal(SIGABRT, handler);
}
