#ifndef AMALGAM_TIME_HPP
#define AMALGAM_TIME_HPP

#include <ratio>
#include <chrono>
#include <iostream>
#include <iomanip>

namespace amalgam {

template <typename TimePoint>
void printTime(std::ostream& out, const TimePoint& then) {
    // Convert to the old style seconds-since-epoch time
    std::time_t then_c = TimePoint::clock::to_time_t(then);
        
    // Copy to our own tm struct
    std::tm local = *(std::localtime(&then_c));
    
    if(!local.tm_isdst) {
    
        // Hack to fix DST for platforms where tzdata doesn't do it past 2038:
        // if it reports as not DST, and mktime says DST is on in 2016, fix
        // up the time forward by an hour.
        std::tm dst_test = local;
        dst_test.tm_isdst = -1;
        dst_test.tm_year = 116;
        std::mktime(&dst_test);
        
        // TODO: this might work badly when DST starts or ends
        
        if(dst_test.tm_isdst) {
            // Pop forward an hour and do it again
            auto new_then = then + std::chrono::hours(1);
            then_c = TimePoint::clock::to_time_t(new_then);
            local = *(std::localtime(&then_c));
            local.tm_isdst = 1;
        }
        
    }
        
    // Convert to local time and print it
    // TODO: lock this or something to make it thread safe
    out << std::put_time(&local, "%b %d %Y %H:%M:%S");
}

}

#endif

