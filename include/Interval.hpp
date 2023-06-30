#pragma once

class Interval {
    float low, high;
    public:
        Interval(float s = 0.0f, float e = 0.0f);
        float getLow();
        float getHigh();
        /** Checks if value is inside interval
         * @param f value to test if inside interval
        */
        bool in(float f);
        /** Checks if intervals intersect */
        bool intersects(Interval o);
        /** Move bounds of interval by k*/
        void moveInterval(float k);
};