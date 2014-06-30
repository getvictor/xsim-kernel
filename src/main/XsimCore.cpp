#include "XsimCore.h"

#include <iostream>

namespace xsim {

XsimCore::~XsimCore() {
    // For each entry in signal functions, delete the set.
    for (const auto& entry : signalFunctions) {
        delete entry.second;
    }
}

void XsimCore::startSim() {

    // This version runs 10 ticks.
    while (time < 10) {
        std::cout << "Time: " << time << std::endl;

        // Process functions from the function queue.
        while (!futureFunctionQueue.empty()) {
            FunctionPtr function = futureFunctionQueue.front();
            // Execute function
            function(functionUserData[function]);
            futureFunctionQueue.pop();
            futureFunctionSet.erase(function);
        }
        time++;
    }

}

void XsimCore::registerFunction(FunctionPtr function, void* userData, int numberOfSignals, void* signals[]) {

    // TODO: Add logging.

    for (int i = 0; i < numberOfSignals; i++) {
        // Insert a new function set if needed
        if (signalFunctions.find(signals[i]) == signalFunctions.end()) {
            std::unordered_set<FunctionPtr>* set = new std::unordered_set<FunctionPtr>();
            signalFunctions[signals[i]] = set;
        }
        signalFunctions[signals[i]]->insert(function);
        functionUserData[function] = userData;
    }

    if (futureFunctionSet.find(function) == futureFunctionSet.end()) {
        futureFunctionSet.insert(function);
        futureFunctionQueue.push(function);
    }

}

void XsimCore::signalUpdate(void* signal, XsimBlockingEnum blocking) {

    int* signalPtr = (int*) signal;
    std::cout << "Update signal " << (*signalPtr) << std::endl;

    // Signal updates trigger function execution.
    if (signalFunctions.find(signal) != signalFunctions.end()) {
        std::unordered_set<FunctionPtr>* set = signalFunctions[signal];
        for (const FunctionPtr& function : *set) {
            if (futureFunctionSet.find(function) == futureFunctionSet.end()) {
                futureFunctionSet.insert(function);
                futureFunctionQueue.push(function);
            }
        }
    }

}

}
