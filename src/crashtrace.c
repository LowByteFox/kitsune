#include <numbers.h>
#include <execinfo.h>
#include <crashtrace.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/signal.h>
#include <unistd.h>

static void
handler(int sig)
{
        void *array[16];
        usize size;

        size = backtrace(array, 16);

        fprintf(stderr, "Recieved signal %s\n", strsignal(sig));
        backtrace_symbols_fd(array, size, STDERR_FILENO);
        exit(1);
}

void
kitsune_install_crashtrace()
{
        signal(SIGSEGV, handler);
        signal(SIGABRT, handler);
}
