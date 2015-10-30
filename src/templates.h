#ifndef TEMPLATES_H
#define TEMPLATES_H

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

static const struct option longOptions[] =
{
    { "address", 1, NULL, 'a' },
    { "help", 0, NULL, 'h' },
    { "log-path", 1, NULL, 'l' },
    { "module-dir", 1, NULL, 'm' },
    { "port", 1, NULL, 'p' },
};

static const char* const shortOptions = "a:hl:m:p:";

static const char* const usageTemplate =
        "Usage: %s [ options ]\n"
        "\t-a, --address ADDR\n"
        "\t-h, --help\n"
        "\t-l, --log-path LOG_PATH\n"
        "\t-m, --module-dir DIR\n"
        "\t-p, --port PORT\n";

static void printUsage(bool isError, char* programName)
{
    fprintf(isError ? stderr : stdout, usageTemplate, programName);
    exit(isError ? EXIT_FAILURE : EXIT_SUCCESS);
}

#endif // TEMPLATES_H
