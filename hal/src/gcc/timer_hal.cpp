#include "timer_hal.h"
#include "boost_posix_time_wrap.h"

namespace {

const auto start = boost::posix_time::microsec_clock::universal_time();

} // namespace

system_tick_t HAL_Timer_Get_Micro_Seconds(void)
{
    return hal_timer_micros(nullptr);
}

system_tick_t HAL_Timer_Get_Milli_Seconds(void)
{
    return hal_timer_millis(nullptr);
}

uint64_t hal_timer_millis(void* reserved)
{
    const auto now = boost::posix_time::microsec_clock::universal_time();
    return (now - start).total_milliseconds();
}

uint64_t hal_timer_micros(void* reserved)
{
    const auto now = boost::posix_time::microsec_clock::universal_time();
    return (now - start).total_microseconds();
}
