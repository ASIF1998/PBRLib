//
//  intervalAreithmetic.inl
//  PRGE
//
//  Created by Асиф Мамедов on 30/08/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include "../core.h"

#include <cmath>

using namespace std;

#define NAN_OR_INF_LOW_HIGH(low, high) if(isnan(low) || isnan(high) || isinf(low) || isinf(high)) throw invalid_argument("The low and high arguments passed are not valid")

namespace PRGE
{
    /**
     * Данная структура представляет собой математическую структуру, которая 
     * позволяет выполнять операции над интервалами аналогично тем, которые
     * выполняются в обычной арифметике.
    */
    template<typename Type>
    struct IntervalArithmetic
    {
        inline IntervalArithmetic(Type low, Type high) NOEXCEPT_PRGE :
            low{low},
            high{high}
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_LOW_HIGH(low, high);
#endif
        }

        inline IntervalArithmetic(Type x) :
            low{x},
            high{x}
        {
#if DEBUG_PRGE == 1
            if (isnan(x) || isnan(x)) {
                throw invalid_argument("The x argument passed are not valid");
            }
#endif
        }

        inline IntervalArithmetic(const IntervalArithmetic& interval) NOEXCEPT_PRGE :
            low{interval.low},
            high{interval.high}
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_LOW_HIGH(interval.low, interval.high);
#endif
        }

        inline IntervalArithmetic(IntervalArithmetic&& interval) NOEXCEPT_PRGE :
            low{interval.low},
            high{interval.high}
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_LOW_HIGH(interval.low, interval.high);
#endif
        }

        inline IntervalArithmetic& operator = (const IntervalArithmetic& interval) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_LOW_HIGH(interval.low, interval.high);
#endif

            low = interval.low;
            high = interval.high;

            return *this;
        }

        inline IntervalArithmetic& operator = (IntervalArithmetic&& interval) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_LOW_HIGH(interval.low, interval.high);
#endif

            low = interval.low;
            high = interval.high;

            return *this;
        }

        inline IntervalArithmetic operator - () const noexcept
        {
            return {-high, -low};
        }

        inline IntervalArithmetic operator + (const IntervalArithmetic& interval) const NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_LOW_HIGH(interval.low, interval.high);
#endif

        return {low + interval.low, high + interval.high};
        }

        inline IntervalArithmetic operator - (const IntervalArithmetic& interval) const NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_LOW_HIGH(interval.low, interval.high);
#endif

            return {low - interval.high, high - interval.low};
        }

        inline IntervalArithmetic operator * (const IntervalArithmetic& interval) const NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_LOW_HIGH(interval.low, interval.high);
#endif

            auto t1 = low * interval.low;
            auto t2 = low * interval.high;
            auto t3 = high * interval.low;
            auto t4 = high * interval.high;

            return {min(min(t1, t2), min(t3, t4)), max(max(t1, t2), max(t3, t4))};
        }

        inline IntervalArithmetic operator / (const IntervalArithmetic& interval) const NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_LOW_HIGH(interval.low, interval.high);

            if (interval.low <= static_cast<Type>(0) && interval.high >= static_cast<Type>(0)) {
                throw invalid_argument ("Division by zero");
            }
#endif

            return (*this) * IntervalArithmetic{static_cast<Type>(1) / interval.high, static_cast<Type>(1) / interval.low};
        }

#if DEBUG_PRGE == 1
        friend ostream& operator << (ostream& os, const IntervalArithmetic& interval)
        {
            int w = static_cast<int>(os.width());
            w = !w ? 2 : w;

            os << "Low:" << setw(w) << interval.low << endl
               << "High:" << setw(w) << interval.high;

            return os;
        }
#endif

public:
        Type low;
        Type high;
    };

    template<typename Type>
    IntervalArithmetic<Type> sin(const IntervalArithmetic<Type>& interval) NOEXCEPT_PRGE
    {
#if DEBUG_PRGE == 1
            NAN_OR_INF_LOW_HIGH(interval.low, interval.high);
            assert(interval.low >= 0 && interval.high <= 2.0001 * M_PI);
#endif

        auto sinLow = std::sin(interval.low);
        auto sinHigh = std::sin(interval.high);

        if (sinLow > sinHigh) {
            swap(sinLow, sinHigh);
        }

        if (interval.low < M_PI_2 && interval.high > M_PI_2) {
            sinHigh = 1.0f;
        }

        if (interval.low < 3.0f * M_PI_2 && interval.high > 3.0f * M_PI_2) {
            sinLow = -1.0f;
        }

        return {sinLow, sinHigh};
    }

    template<typename Type>
    IntervalArithmetic<Type> cos(const IntervalArithmetic<Type>& interval) NOEXCEPT_PRGE
    {
#if DEBUG_PRGE == 1
            NAN_OR_INF_LOW_HIGH(interval.low, interval.high);
            assert(interval.low >= 0 && interval.high <= 2.0001 * M_PI);
#endif

        auto cosLow = std::cos(interval.low);
        auto cosHigh = std::cos(interval.high);

        if (cosLow > cosHigh) {
            swap(cosLow, cosHigh);
        }

        if (cosLow < M_PI && cosHigh > M_PI) {
            cosLow = -1.0f;
        }

        return {cosLow, cosHigh};
    }
}
