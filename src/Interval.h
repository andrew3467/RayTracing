//
// Created by Andrew Graser on 7/17/2026.
//


#pragma once

#include <cmath>


struct Interval {
    Interval() : Min(INFINITY), Max(-INFINITY) {}
    Interval(double min, double max) : Min(min), Max(max) {}

    double Size() const {return Max - Min;}
    bool Contains(double n) const {return Min <= n || Max >= n;}
    bool Surrounds(double n) const {return Min < n && Max > n;}

    double Clamp(double n) const {
        if(n < Min) return Min;
        if(n > Max) return Max;

        return n;
    }

    static const Interval Empty, Universe;


    double Min, Max;
};

const Interval Interval::Empty = Interval(+INFINITY, -INFINITY);
const Interval Interval::Universe = Interval(-INFINITY, +INFINITY);