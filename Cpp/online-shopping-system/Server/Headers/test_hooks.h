#pragma once

// When building unit tests, avoid blocking on keyboard input.
#ifdef OSHOP_TEST_BUILD
inline void oshop_pause_for_tests() {}
inline void oshop_stdin_pause_for_tests() {}
#else
#include <conio.h>
#include <cstdio>
inline void oshop_pause_for_tests() { (void)getch(); }
inline void oshop_stdin_pause_for_tests()
{
    (void)fflush(stdin);
    (void)getchar();
}
#endif
