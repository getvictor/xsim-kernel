#ifndef XSIM_CORE_H
#define XSIM_CORE_H

#include "KernelInterface.h"
#include <map>
#include <queue>
#include <unordered_set>

namespace xsim {

typedef void (*FunctionPtr)(void*);

/**
 * The simulator kernel core, which runs the main simulator loop.
 */
class XsimCore {
public:

    /**
     * Constructor.
     */
    XsimCore() {
        // empty
    }

    /**
     * Destructor.
     */
    ~XsimCore();

    /**
     * Start the simulation.
     */
    void startSim();

    /**
     * Register a function with the simulator kernel. Functions cannot take time.
     * @param function The function.
     * @param userData The user data to pass the function.
     * @param numberOfSignals The number of signals that the function is sensitive to.
     * @param signals The array of signals the function is sensitive to.
     */
    void registerFunction(FunctionPtr function, void* userData, int numberOfSignals, void* signals[]);

    /**
     * Tell the simulator kernel that a signal has been updated.
     * @param signal The signal that has been updated.
     * @param blocking Flag indicating whether the update is blocking.
     */
    void signalUpdate(void* signal, XsimBlockingEnum blocking);

private:
    /**
     * The simulator time.
     */
    unsigned long time = 0;

    /**
     * Map of function user data.
     */
    std::map<FunctionPtr, void*> functionUserData;

    /**
     * Map of signals and the functions they trigger.
     */
    std::map<void*, std::unordered_set<FunctionPtr>* > signalFunctions;

    /**
     * Queue of functions to execute at the new time.
     * TODO: Combine these into one class.
     */
    std::queue<FunctionPtr> futureFunctionQueue;
    std::unordered_set<FunctionPtr> futureFunctionSet;

};

}

#endif
