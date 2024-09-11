#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>

int test_sm_connection(void);

int main(int argc, char* argv[]) {
    return test_sm_connection();
}