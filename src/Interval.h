//
// Created by Andrew Graser on 7/17/2026.
//


#pragma once

#include <cmath>


struct Interval {
    Interval() : Min(INFINITY), Max(-INFINITY) {}
    Interval(double min, double max) : Min(min), Max(max) {}
    Interval(const Interval& a, const Interval& b) {
        Min = a.Min <= b.Min ? a.Min : b.Min;
        Max = a.Max >= b.Max ? a.Max : b.Max;
    }

    double Size() const {return Max - Min;}
    bool Contains(double n) const {return Min <= n && Max >= n;}
    bool Surrounds(double n) const {return Min < n && Max > n;}

    double Clamp(double n) const {
        if(n < Min) return Min;
        if(n > Max) return Max;

        return n;
    }

    Interval Expand(double delta) const {
        double padding = delta / 2.0;
        return Interval(Min - padding, Max + padding);
    }

    static const Interval Empty, Universe;


    double Min, Max;
};

const Interval Interval::Empty = Interval(+INFINITY, -INFINITY);
const Interval Interval::Universe = Interval(-INFINITY, +INFINITY);