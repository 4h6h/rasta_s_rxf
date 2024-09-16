#include "main.h"

extern int32_t modelMain(void);

/*## class TopLevel::main */
/*## operation main(int,char**) */
int main(int argc, char** argv) {
/*#[ operation main(int,char**) */
    return (int)modelMain();
}