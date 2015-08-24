#ifndef INT_H
#define INT_H
#include <boost/math/special_functions/round.hpp>
struct Int {

    Int() {}
    Int(int const& i) : value(i) {}
    Int(long long const& i) : value(i) {}
    
    long long divRoundClosest(long long const& n, long long const& d) const {
        return ((n < 0) ^ (d < 0)) ? ((n - (d/2))/d) : ((n + (d/2))/d);
    }

    Int operator+(long long const& other) const {return value + other;}
    
	Int& operator+=(long long const& other) {value += other;}

    Int operator-(long long const& other) const {return value - other;}
    Int& operator-=(long long const& other) {value -= other;}

    Int operator*(long long const& other) const {return value * other;}
    Int& operator*=(long long const& other) {value *= other;}

    Int operator*(float const& k) const {
        long long ans = boost::math::iround(k * value);
        return ans;
    }
    
    Int& operator*=(float const& k) {
        value = boost::math::iround(k * value);
    }
    
    Int operator/(long long const& other) const {
        return divRoundClosest(value, other);
    }
    Int& operator/=(long long const& other) {
        value = divRoundClosest(value, other);
    }

    Int operator/(float const& k) const {
        long long ans = boost::math::iround(value*1.f/k);
        return ans;
    }
    
    Int& operator/=(float const& k) {
        value = boost::math::iround(value*1.f/k);
    }
    


    /*Int operator+(Int const& other) const {return value + other.value;}
    Int& operator+=(Int const& other) {value += other.value;}

    Int operator-(Int const& other) const {return value - other.value;}
    Int& operator-=(Int const& other) {value -= other.value;}

    Int operator*(Int const& other) const {return value * other.value;}
    Int& operator*=(Int const& other) {value *= other.value;}
    */

    Int operator/(Int const& other) const {
        return divRoundClosest(value, other.value);
    }
    Int& operator/=(Int const& other) {
        value = divRoundClosest(value, other.value);
    	return *this;
    }
    
    long long value;
};

	
	

#endif