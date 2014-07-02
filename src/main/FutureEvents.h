#ifndef FUTUREEVENTS_H_
#define FUTUREEVENTS_H_

#include <forward_list>

#include "TimeSliceEvent.h"

namespace xsim {

/**
 * List of future events that the simulator kernel must execute.
 */
class FutureEvents {

private:

    /**
     * Linked list of events.
     */
    std::forward_list<TimeSliceEvent*> eventList;

public:

    /**
     * Constructor.
     */
    FutureEvents();

    /**
     * Destructor.
     */
    ~FutureEvents();

    /**
     * Check whether future events exist.
     */
    bool isEmpty() const {
        return eventList.empty();
    }

    /**
     * Get the next even in the list.
     */
    TimeSliceEvent* getNextEvent() const {
        return eventList.front();
    }

    /**
     * Remove the first event in the list.
     */
    void removeFirstEvent();

};

}



#endif /* FUTUREEVENTS_H_ */
