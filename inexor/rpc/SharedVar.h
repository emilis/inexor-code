#ifndef INEXOR_RPC_SHARED_VAR_HEADER
#define INEXOR_RPC_SHARED_VAR_HEADER

#include "inexor/util/Observe.h"

template<typename T> using SharedVar = inexor::util::Observe<T>;

// Stolen from inexor/util/Observe.h which defines these for
// std::min/max

template<typename T>
const T& min(inexor::util::Observe<T> &a, const T &b) {
    return std::min(*a, b);
}
template<typename T>
const T& min(const T &a, inexor::util::Observe<T> &b) {
    return std::min(a, *b);
}
template<typename T>
const T& min(inexor::util::Observe<T> &a,
       inexor::util::Observe<T> &b) {
    return std::min(*a, *b);
}

template<typename T>
const T& max(inexor::util::Observe<T> &a, const T &b) {
    return std::max(*a, b);
}
template<typename T>
const T& max(const T &a, inexor::util::Observe<T> &b) {
    return std::max(a, *b);
}
template<typename T>
const T& max(inexor::util::Observe<T> &a,
       inexor::util::Observe<T> &b) {
    return std::max(*a, *b);
}

#endif
