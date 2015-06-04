#ifndef INEXOR_UTIL_OBSERVE_HEADER
#define INEXOR_UTIL_OBSERVE_HEADER

#include <algorithm>

#include "inexor/util/Event.h"

namespace inexor {
namespace util {

/// Observe wrapper for primitive/immutable objects
///
/// This first and foremost is a wrapper around stuff.
/// We provide * and -> operators to work with the value and
/// define a cast operator that casts to the contained
/// value.
/// It best works with primitive or non mutable values, as
/// changes to the value can not be detected.
///
/// The cast operator can make some problems, when the
/// compiler can not figure out what to use and you might
/// see some giant error cascades.
/// In these cases, try using the value itself
/// (`*observ_instance`) or using an explicit cast.
///
/// Observe provides a special assignment operator: If
/// that is called, it first calls the onChange event with
/// the old and the new value.
/// Only afterwards the new value is assigned.
///
/// The value of the variable inside observe is undefended
/// while the listeners are called;
///
/// During the event, do not change the value of the
/// variable (that would create an recursive event),
/// or the parameters.
///
/// ## Example
///
/// ```
/// Observe<int> oi = 22;
///
/// int sum(int &i, int &k) {
///     return i+k;
/// }
///
/// void onChange(int &old, int &new) {
///     cout << "oi changed: " << old << " -> " << new << endl;
/// }
///
/// void main() {
///     oi = 33;
///     oi += 22;
/// }
/// ```
template<typename T>
class Observe {
private:
    typedef Observe<T> TMe;

    T value;
public:
    /// Event that is fired when a new value is assigned;
    ///
    /// This does however not fire, when the value is merely
    /// modified
    ///
    /// params: old value, new value
    Event<const T&, const T&> onChange;

    // Access

    T& operator *() { return value; }
    T* operator ->() { return &value; }

    // Note: Let's see whether this causes too much problems
    operator T&() { return value; }

    // Proxies

    Observe() {}

    /// Consider making thes
    template<typename... Args>
    explicit Observe(Args... args) : value(args...) {}

    explicit Observe(T &otr) : value(otr) {}
    explicit Observe(T &&otr) : value(otr) {}

    // Avoid ambiguity when observing a string
    T operator= (const char *c) {
        T nu(c);
        onChange(value, nu);
        return value = nu;
    }

    T operator =(const T &otr) {
        onChange(value, otr);
        value = otr;
        return value;
    }

    T operator =(T &&otr) {
        onChange(value, otr);
        value = otr;
        return value;
    }

    // TODO: Put all the operator macro invocations into
    // own file so we reuse them in the test.

#define unr(op)                  \
    T operator op (int) {        \
        T old = value;           \
        T ret = value op;        \
        onChange(old, value);    \
        return ret;              \
    }

    unr(++);
    unr(--);

#undef unr

#define unl(op)                  \
    T operator op () {           \
        T old = value;           \
        T ret = op value;        \
        onChange(old, value);    \
        return ret;              \
    }

    unl(++);
    unl(--);

#undef unl


#define asgn(op)                       \
    template<typename O>               \
    T operator op ## =(const O &otr) { \
        T old = value;                 \
        T ret = value op ## = otr;     \
        onChange(old, value);          \
        return ret;                    \
    }

    asgn(+);
    asgn(-);
    asgn(*);
    asgn(/);
    asgn(%);
    asgn(&);
    asgn(|);
    asgn(^);
    asgn(<<);
    asgn(>>);

#undef asgn

};

// Output Operator

template<typename T>
std::ostream& operator<<(std::ostream& os, const Observe<T> &x) {
    os << x;
}

} // ::inexor::util
} // ::inexor

// Specializations for min/max (otherwise they are causing
// frequent problems)
// TODO: Get rid of ::min, ::max; define these for std::*

namespace std {

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

} // ::std

#endif
