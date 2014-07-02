#include <iostream>

#include "XsimCore.h"

namespace xsim {

XsimCore::~XsimCore() {
    // For each entry in signal functions, delete the set.
    for (const auto& entry : signalFunctions) {
        delete entry.second;
    }
    // Delete all coroutines.
    for (Coroutine* entry : coroutines) {
        delete entry;
    }
}

void XsimCore::startSim() {

    while (!futureEvents.isEmpty()) {
        TimeSliceEvent* event = futureEvents.getNextEvent();
        time = event->getTime();
        std::cout << "Time: " << time << std::endl;

        while (!event->isEmpty()) {
            // Execute coroutines
            while (!event->isCoroutinesEmpty()) {
                Coroutine* coroutine = event->popNextCoroutine();
                // Execute coroutine if it is not already running.
                if (coroutine->isFinished()) {
                    coroutine->reset();
                }
                coroutine->start();
            }

            // Execute functions
            while (!event->isFunctionsEmpty()) {
                FunctionPtr function = event->popNextFunction();
                // Execute function
                function(functionUserData[function]);
            }
        }

        futureEvents.removeFirstEvent();
    }

}

void XsimCore::registerFunction(FunctionPtr function, void* userData, int numberOfSignals, void* signals[]) {

    // TODO: Add logging.

    functionUserData[function] = userData;

    for (int i = 0; i < numberOfSignals; i++) {
        // Insert a new function set if needed
        if (signalFunctions.find(signals[i]) == signalFunctions.end()) {
            std::unordered_set<FunctionPtr>* set = new std::unordered_set<FunctionPtr>();
            signalFunctions[signals[i]] = set;
        }
        signalFunctions[signals[i]]->insert(function);
    }

    // Schedule the function for execution.
    TimeSliceEvent* event = futureEvents.getNextEvent();
    event->scheduleFunction(function);

}

void XsimCore::registerCoroutine(FunctionPtr coroutine, void* userData, int numberOfSignals, void* signals[]) {

    Coroutine* coroutinePtr = new Coroutine(coroutine, userData);
    for (int i = 0; i < numberOfSignals; i++) {
        // Insert a new coroutine set if needed
        if (signalCoroutines.find(signals[i]) == signalCoroutines.end()) {
            std::unordered_set<Coroutine*>* set = new std::unordered_set<Coroutine*>();
            signalCoroutines[signals[i]] = set;
        }
        signalCoroutines[signals[i]]->insert(coroutinePtr);
    }

    // Schedule the coroutine for execution.
    TimeSliceEvent* event = futureEvents.getNextEvent();
    event->scheduleCoroutine(coroutinePtr);
    coroutines.insert(coroutinePtr);

}

void XsimCore::signalUpdate(void* signal, XsimBlockingEnum blocking) {

    int* signalPtr = (int*) signal;
    std::cout << "Update signal " << (*signalPtr) << std::endl;

    // Signal updates trigger function execution.
    if (signalFunctions.find(signal) != signalFunctions.end()) {
        std::unordered_set<FunctionPtr>* set = signalFunctions[signal];
        TimeSliceEvent* event = futureEvents.getNextEvent();
        for (const FunctionPtr& function : *set) {
            event->scheduleFunction(function);
        }
    }

}

}
