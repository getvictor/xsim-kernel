#include "Coroutine.h"

namespace xsim {

Coroutine::Coroutine(CoroutinePtr coroutine, void* userData) :
    coroutine(coroutine),
    userData(userData),
    source([&](boost::coroutines::coroutine<int>::push_type& coroutineSink){
        std::cout << "In coroutine" << std::endl;
        sink = &coroutineSink;

        // The coroutine needs local copies of function pointer and user data.
        CoroutinePtr myCoroutine = coroutine;
        void* myUserData = userData;

        // We must always stop the coroutine so that we exit the constructor
        // and the class gets properly initialized.
        coroutineSink(0);
        started = true;
        myCoroutine(myUserData);
        finished = true;
    }) {
    // empty
}

Coroutine::~Coroutine() {
    // empty
}

void Coroutine::reset() {
    finished = false;
    started = false;
    paused = true;
    // TODO: Create a custom cached StackAllocator to increase re-entrant performance.
    // See http://stackoverflow.com/questions/24108030/coroutine-reuse
    source = boost::coroutines::coroutine<int>::pull_type(
            [&](boost::coroutines::coroutine<int>::push_type& coroutineSink){
        sink = &coroutineSink;
        coroutineSink(0);
        started = true;
        coroutine(userData);
        finished = true;
    });
}

void Coroutine::pause() {
    if (!paused) {
        paused = true;
        (*sink)(0);
    }
}

void Coroutine::start() {
    if (paused) {
        paused = false;
        source();
    }
}

}
