#pragma once
#include <iostream>

struct Move
{
    int source;
    int target;
    int piece;
    int captured;
    int flags;

    Move(int s, int t, int p, int c, int fl = 0)
        : source(s), target(t), piece(p), captured(c), flags(fl) {}

};