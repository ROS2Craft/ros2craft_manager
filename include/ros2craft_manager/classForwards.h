#ifndef RC_MAN_CLASS_FORWARD_
#define RC_MAN_CLASS_FORWARD_

#include <memory> // for std::shared_ptr

/** \file */

/**
 * \def RC_MAN_CLASS_FORWARD
 * \brief Macro that forward declares a class and defines two shared ptrs types:
 *  - ${Class}Ptr      = shared_ptr<${Class}>
 *  - ${Class}ConstPtr = shared_ptr<const ${Class}>
 */
#define RC_MAN_CLASS_FORWARD(C)                                                                    \
    class C;                                                                                       \
    typedef std::shared_ptr<C> C##Ptr;                                                             \
    typedef std::shared_ptr<const C> C##ConstPtr;
#endif // RC_MAN_CLASS_FORWARD_