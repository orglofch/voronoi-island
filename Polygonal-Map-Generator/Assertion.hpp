#ifndef _ASSERTION_HPP_
#define _ASSERTION_HPP_

#include <cassert>

#define NOTREACHED() assert(false)
#define DCHECK(condition) assert(condition);

#endif