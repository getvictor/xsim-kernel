typedef enum {BLOCKING, NON_BLOCKING} XsimBlockingEnum;
typedef enum {
    FS,
    PS,
    NS,
    US,
    MS,
    SEC
} XsimTimeUnitEnum;

// TODO: Specify whether function/coroutine is started at first time tick or after sensitivity is triggered. (dont_initialize in SystemC)

/**
 * Register a function with the simulator kernel. Functions cannot take time.
 * @param function The function.
 * @param numberOfSignals The number of signals that the function is sensitive to.
 * @param signals The array of signals the function is sensitive to.
 */
extern "C" void xsimRegisterFunction(void (*function)(), int numberOfSignals, void* signals[]);

/**
 * Register a coroutine with the simulator kernel. Coroutines can take time.
 * @param coroutine The coroutine.
 * @param numberOfSignals The number of signals that the coroutine is sensitive to.
 * @param signals The array of signals the coroutine is sensitive to.
 */
extern "C" void xsimRegisterCoroutine(void (*coroutine)(), int numberOfSignals, void* signals[]);

/**
 * Tell the simulator kernel that a signal has been updated.
 * @param signal The signal that has been updated.
 * @param blocking Flag indicating whether the update is blocking.
 */
extern "C" void xsimSignalUpdate(void* signal, XsimBlockingEnum blocking);

/**
 * Block the execution of a coroutine for a certain time.
 * @param coroutine The coroutine to block.
 * @param time The number of time units to block.
 * @param timeUnit The time unit to use.
 */
extern "C" void xsimBlockCoroutine(void (*coroutine)(), double time, XsimTimeUnitEnum timeUnit);
