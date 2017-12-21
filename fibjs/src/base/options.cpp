/*
 * options.cpp
 *
 *  Created on: Jul 23, 2015
 *      Author: lion
 */

#include "v8/src/flags.h"

#include "version.h"
#include "console.h"
#include "path.h"
#include "Fiber.h"
#include "options.h"

namespace fibjs {

#ifdef amd64
int32_t stack_size = 512;
#else
int32_t stack_size = 256;
#endif

bool g_prof = false;
int32_t g_prof_interval = 1000;

FILE* g_cov = nullptr;

bool g_tracetcp = false;

#ifdef DEBUG
#define GUARD_SIZE 32
#else
#define GUARD_SIZE 16
#endif

extern const char* opt_tools[];

static void printHelp()
{
    puts("Usage: fibjs [options] [script.js] [arguments] \n"
         "\n"
         "Options:\n"
         "  -h, --help           print fibjs command line options.\n"
         "  -v, --version        print fibjs version.\n"
         "\n"
         "  --prof               log statistical profiling information.\n"
         "  --prof-interval=n    interval for --prof samples (in microseconds, default: 1000).\n"
         "  --prof-process       process log file generated by profiler.start.\n"
         "\n"
         "  --cov[=filename]     collect code coverage information (only work on the main Worker).\n"
         "  --cov-process        generate code coverage analysis report.\n"
         "\n"
         "  --v8-options         print v8 command line options.\n"
         "\n"
         "Documentation can be found at http://fibjs.org\n");
}

void options(int32_t& pos, char* argv[])
{
    int32_t argc = pos;
    int32_t i;

    for (pos = 1; (pos < argc) && (argv[pos][0] == '-'); pos++)
        if (argv[pos][1] == '-') {
            exlib::string tmp("opt_tools/");
            tmp += argv[pos] + 2;

            for (i = 0; opt_tools[i] && qstrcmp(opt_tools[i], tmp.c_str()); i += 2)
                ;

            if (opt_tools[i])
                break;
        }

    argc = pos;
    int32_t df = 0;

    for (int32_t i = 0; i < argc; i++) {
        char* arg = argv[i];

        if (df)
            argv[i - df] = arg;

        if (!qstrcmp(arg, "--help") || !qstrcmp(arg, "-h")) {
            printHelp();
            _exit(0);
        } else if (!qstrcmp(arg, "--version") || !qstrcmp(arg, "-v")) {
            printf("v%s\n", fibjs_version);
            _exit(0);
        } else if (!qstrcmp(arg, "--prof")) {
            g_prof = true;
            df++;
        } else if (!qstrcmp(arg, "--prof-interval=", 16)) {
            g_prof_interval = atoi(arg + 16);
            if (g_prof_interval < 50)
                g_prof_interval = 50;
            df++;
        } else if (!qstrcmp(arg, "--cov=", 6)) {
            g_cov = fopen(arg + 6, "a");
            if (g_cov == nullptr) {
                printf("Invalid filename: %s\n", arg + 6);
                _exit(0);
            }
            df++;
        } else if (!qstrcmp(arg, "--cov")) {
            char name[22];
            date_t d;
            d.now();
            sprintf(name, "fibjs-%d.lcov", (int32_t)d.date());
            g_cov = fopen(name, "a");
            if (g_cov == nullptr) {
                printf("Can't open file: %s, please try again", name);
                _exit(0);
            }
            df++;
        } else if (!qstrcmp(arg, "--v8-options")) {
            v8::internal::FlagList::PrintHelp();
            _exit(0);
        }
    }

    v8::internal::FLAG_lazy = false;
    v8::internal::FLAG_stack_size = stack_size - GUARD_SIZE;

    v8::internal::FLAG_wasm_async_compilation = false;
    v8::internal::FLAG_parallel_scavenge = false;
    v8::internal::FLAG_parallel_marking = false;

    if (df)
        argc -= df;

    v8::V8::SetFlagsFromCommandLine(&argc, argv, true);
}
}
