#include "Coroutine.h"

namespace xsim {

Coroutine::Coroutine(CoroutinePtr coroutine, void* userData) {
    started = false;
    paused = true;
    source = new boost::coroutines::coroutine<int>::pull_type(
            [&](boost::coroutines::coroutine<int>::push_type& coroutineSink){
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
    });
}

Coroutine::~Coroutine() {
    if (source != nullptr) {
        delete source;
    }
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
        (*this->source)();
    }
}

}
