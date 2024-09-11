#ifndef SANDBOX_OPERATION_H
#define SANDBOX_OPERATION_H

/* Forward Declarations */
struct cDsipOP;
struct cFecOP;
struct cRastaSOP;
struct RastaSConfig;
struct SandboxInstances {
    struct cDispOP* itsCDispOP;
    struct cFecOP* itsCFecOP;
    struct cRastaSOP* itsCRastaSOP;
    struct RastaSConfig* rastaConfig;
};
typedef struct SandboxInstances SandboxInstances;

void SandboxOperation_Main(void);

#endif /* SANDBOX_OPERATION_H */