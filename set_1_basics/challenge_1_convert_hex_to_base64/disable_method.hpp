#ifndef INCLUDED_DISABLE_METHOD
#define INCLUDED_DISABLE_METHOD

// This file contains macros to "disable" (i.e. false-assert) certain protected
// methods in std::streambuf that by default have no effect and are meant to be
// overridden in derived classes. This is to have fail-fast behavior if such
// methods are called for streambuf-derivatives that do not implement them.

#include <cassert>

#define DISABLE_METHOD(declaration) \
declaration \
{ \
    assert(false); \
    return 0; \
}


#define DISABLE_VOID_METHOD(declaration) \
declaration \
{ \
    assert(false); \
}


// This default sync() method is meant to call .flush() on the connected ostream
#define DEFINE_DEFAULT_SYNC_METHOD(outputStreamPtr) \
int sync() \
{ \
    assert(outputStreamPtr); \
    try { \
        return (outputStreamPtr)->flush() ? 0 : -1; \
    } catch (...) { \
        return -1; \
    } \
}

#endif
