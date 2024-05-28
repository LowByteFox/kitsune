#include <numbers.h>
#include <execinfo.h>
#include <crashtrace.h>
#include <stdio.h>
#include <string.h>
#include <sys/signal.h>
#include <unistd.h>

static void
handler(int sig)
{
        void *array[10];
        usize size;

        size = backtrace(array, 10);

        fprintf(stderr, "Recieved signal %s\n", strsignal(sig));
        backtrace_symbols_fd(array, size, STDERR_FILENO);
}

void
kitsune_install_crashtrace()
{
        signal(SIGSEGV, handler);
        signal(SIGABRT, handler);
}
