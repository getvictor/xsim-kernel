#include "FutureEvents.h"

namespace xsim {

FutureEvents::FutureEvents() {
    // Create event at time 0.
    TimeSliceEvent* event = new TimeSliceEvent(0);
    eventList.push_front(event);
}

FutureEvents::~FutureEvents() {
    for (TimeSliceEvent* event : eventList) {
        delete event;
    }
}

void FutureEvents::removeFirstEvent() {
    TimeSliceEvent* event = eventList.front();
    eventList.remove(event);
    delete event;
}

}
