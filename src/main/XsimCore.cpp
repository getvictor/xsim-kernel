#include "XsimCore.h"

#include <iostream>

namespace xsim {

void XsimCore::startSim() {

    // This version runs 100 ticks.
    while (time < 100) {
        std::cout << "Time: " << time << std::endl;
        // Process new functions.
        while (!futureFunctionQueue.empty()) {
            void (*function)() = futureFunctionQueue.front();
            // Execute function
            function();
            futureFunctionQueue.pop();
        }
        time++;
    }

}

void XsimCore::registerFunction(void (*function)(), int numberOfSignals, void* signals[]) {

    // TODO: Add logging.

    for (int i = 0; i < numberOfSignals; i++) {
        signalFunctions[signals[i]] = function;
    }

    if (futureFunctionSet.find(function) == futureFunctionSet.end()) {
        futureFunctionSet.insert(function);
        futureFunctionQueue.push(function);
    }

}

void XsimCore::signalUpdate(void* signal, XsimBlockingEnum blocking) {

    // Signal updates trigger function execution.
    void (*function)() = signalFunctions[signal];
    if (futureFunctionSet.find(function) == futureFunctionSet.end()) {
        futureFunctionSet.insert(function);
        futureFunctionQueue.push(function);
    }

}

}
