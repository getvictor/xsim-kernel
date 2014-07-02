#include "TimeSliceEvent.h"

namespace xsim {

void TimeSliceEvent::scheduleFunction(FunctionPtr function) {
    if (functionSet.find(function) == functionSet.end()) {
        functionSet.insert(function);
        functionQueue.push(function);
    }
}

void TimeSliceEvent::scheduleCoroutine(Coroutine* coroutine) {
    if (coroutineSet.find(coroutine) == coroutineSet.end()) {
        coroutineSet.insert(coroutine);
        coroutineQueue.push(coroutine);
    }
}

FunctionPtr TimeSliceEvent::popNextFunction() {
    FunctionPtr function = functionQueue.front();
    functionQueue.pop();
    functionSet.erase(function);
    return function;
}

Coroutine* TimeSliceEvent::popNextCoroutine() {
    Coroutine* coroutine = coroutineQueue.front();
    coroutineQueue.pop();
    coroutineSet.erase(coroutine);
    return coroutine;
}

}
