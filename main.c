/**
 * @file main.c
 */

/* Core library */
#define TRACE_MODULE _main_
#include "core_general.h"
#include "core_debug.h"
#include "core_signal.h"

/* Server */
#include "app.h"

static char *compile_time = __DATE__ " " __TIME__;

static void show_version()
{
    printf("NextEPC daemon v%s - %s\n", PACKAGE_VERSION, compile_time);
}

static void show_help(const char *name)
{
    show_version();

    printf("\n"
           "Usage: %s [arguments]\n"
           "\n"
           "Arguments:\n"
           "   -v                   Show version\n"
           "   -h                   Show help\n"
           "   -d                   Start as daemon\n"
           "   -f                   Set configuration file name\n"
           "   -l log_path          Fork log daemon with file path to be logged to\n"
           "\n", name);
}

static int check_signal(int signum)
{
    switch (signum)
    {
        case SIGTERM:
        case SIGINT:
        {
            d_info("%s received", 
                    signum == SIGTERM ? "SIGTERM" : "SIGINT");

            return 1;
        }
        case SIGUSR1:
        {
            break;
        }
        default:
        {
            d_error("Unknown signal number = %d\n", signum);
            break;
        }
            
    }
    return 0;
}

void terminate()
{
    app_terminate();
    core_terminate();
}

int main(int argc, char *argv[])
{
    /**************************************************************************
     * Starting up process.
     *
     * Keep the order of starting-up
     */
    char *config_path = NULL;
    char *log_path = NULL;

    while (1)
    {
        int opt = getopt (argc, argv, "vhdf:l:");
        if (opt == -1)
            break;

        switch (opt)
        {
            case 'v':
                show_version();
                return EXIT_SUCCESS;
            case 'h':
                show_help(argv[0]);
                return EXIT_SUCCESS;
            case 'd':
            {
                pid_t pid;
                pid = fork();

                d_assert(pid >= 0, return EXIT_FAILURE, "fork() failed");

                if (pid != 0)
                {
                    /* Parent */
                    return EXIT_SUCCESS;
                }
                /* Child */

                setsid();
                umask(027);
                break;
            }
            case 'f':
                config_path = optarg;
                break;
            case 'l':
                log_path = optarg;
                break;
            default:
                show_help(argv[0]);
                return EXIT_FAILURE;
        }
    }

    show_version();
    d_print("\n");

    atexit(terminate);

    core_initialize();
    if (app_initialize(config_path, log_path) != CORE_OK)
    {
        d_fatal("NextEPC initialization failed. Aborted");
        return EXIT_FAILURE;
    }

    d_print("\n\n");
    d_info("NextEPC daemon start");
    signal_thread(check_signal);

    d_info("NextEPC daemon terminating...");

    return EXIT_SUCCESS;
}
