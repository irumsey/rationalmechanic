#pragma once

///
///
///

#define LUCID_PREVENT_COPY(c) c(c const &) = delete
#define LUCID_PREVENT_ASSIGNMENT(c) c &operator=(c const &) = delete
