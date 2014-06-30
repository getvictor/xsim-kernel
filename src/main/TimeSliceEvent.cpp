#include "TimeSliceEvent.h"

namespace xsim {

void TimeSliceEvent::scheduleFunction(FunctionPtr function) {
    if (functionSet.find(function) == functionSet.end()) {
        functionSet.insert(function);
        functionQueue.push(function);
    }
}

FunctionPtr TimeSliceEvent::popNextFunction() {
    FunctionPtr function = functionQueue.front();
    functionQueue.pop();
    functionSet.erase(function);
    return function;
}

}
