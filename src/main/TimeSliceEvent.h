#ifndef TIME_SLICE_EVENT_H_
#define TIME_SLICE_EVENT_H_

#include <queue>
#include <unordered_set>

#include "XsimTypedefs.h"
#include "Coroutine.h"

namespace xsim {

/**
 * This data structure represents everything that should happen at a specific time.
 */
class TimeSliceEvent {

private:

    /**
     * The simulator time corresponding to this event.
     */
    Time time;

    /**
     * Queue of functions to execute at this time.
     */
    std::queue<FunctionPtr> functionQueue;

    /**
     * Set of functions that match those in the queue.
     */
    std::unordered_set<FunctionPtr> functionSet;

    /**
     * Queue of coroutines to start at this time.
     */
    std::queue<Coroutine*> coroutineQueue;

    /**
     * Set of coroutines that match those in the queue.
     */
    std::unordered_set<Coroutine*> coroutineSet;

public:

    /**
     * Constructor.
     * @param time The time for this event.
     */
    TimeSliceEvent(Time time) : time(time) {
        // empty
    }

    /**
     * Schedule function for execution.
     * @param function The function to schedule.
     */
    void scheduleFunction(FunctionPtr function);

    /**
     * Schedule coroutine for execution.
     * @param coroutine The coroutine to schedule.
     */
    void scheduleCoroutine(Coroutine* coroutine);

    /**
     * Check whether internal data structures are empty.
     * @return true if empty; false otherwise
     */
    bool isEmpty() const {
        return functionQueue.empty() && coroutineQueue.empty();
    }

    /**
     * Check whether internal function data structures are empty.
     * @return true if empty; false otherwise
     */
    bool isFunctionsEmpty() const {
        return functionQueue.empty();
    }

    /**
     * Check whether internal coroutine data structures are empty.
     * @return true if empty; false otherwise
     */
    bool isCoroutinesEmpty() const {
        return coroutineQueue.empty();
    }

    /**
     * Return the next function for execution and remove it from internal data structures.
     * @return the next function
     */
    FunctionPtr popNextFunction();

    /**
     * Return the next coroutine for execution and remove it from internal data structures.
     * @return the next coroutine
     */
    Coroutine* popNextCoroutine();

    /**
     * Get the time for this event.
     * @return the time
     */
    Time& getTime() {
        return time;
    }

};

}

#endif /* TIME_SLICE_EVENT_H_ */
