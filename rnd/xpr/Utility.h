#pragma once

///	
///	some classes, as they currently exist, require base/member initialization.
///	therefore, they require C++ "deleted constructor" features for the following to work.
///
#ifdef XPR_ENABLE_PREVENT_COPY
#	define XPR_PREVENT_COPY(c) c(c const &) = delete
#	define XPR_PREVENT_ASSIGNMENT(c) c &operator=(c const &) = delete
#else
#	define XPR_PREVENT_COPY(c) /* */
#	define XPR_PREVENT_ASSIGNMENT(c) /* */
#endif
