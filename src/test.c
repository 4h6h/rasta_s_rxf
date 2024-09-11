#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>

#include "log.h"
#include "SandboxOperation.h"

extern int test_sm_connection(void);

int main(int argc, char* argv[]) {
    int return_value = -1;

    printf("LOG_LEVEL: %i\n", get_loglevel_filter());
    set_loglevel_filter(LOG_DEBUG);
    printf("LOG_LEVEL: %i\n", get_loglevel_filter());

    return_value = test_sm_connection();

    return return_value;
}