#ifndef XSIM_CORE_H
#define XSIM_CORE_H

#include <map>
#include <queue>
#include <unordered_set>

#include "KernelInterface.h"
#include "XsimTypedefs.h"
#include "FutureEvents.h"

namespace xsim {

/**
 * The simulator kernel core, which runs the main simulator loop.
 */
class XsimCore {

private:

    /**
     * The simulator time.
     */
    Time time = 0;

    /**
     * The list of future events.
     */
    FutureEvents futureEvents;

    /**
     * Map of function user data.
     */
    std::map<FunctionPtr, void*> functionUserData;

    /**
     * Map of signals and the functions they trigger.
     */
    std::map<void*, std::unordered_set<FunctionPtr>* > signalFunctions;

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

};

}

#endif
