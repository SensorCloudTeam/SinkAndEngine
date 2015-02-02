/*
Author:master@aozima.cn
Last update:2009年8月10日21时52分06秒

Examples:(stm32)

struct tm_new time_now;
gmtime_new(&time_now,RTC_GetCounter()); // get timestamp
time_now.tm_year = 2009-YEAR_BASE;      // adj date to 2009-08-10
time_now.tm_mon  = 7;
time_now.tm_mday = 10;
RTC_SetCounter(mktime_new(&time_now));  // set timestamp

*/

#ifndef TIME_NEW_H_INCLUDED
#define TIME_NEW_H_INCLUDED

#ifndef _TM_NEW_DEFINED
struct tm_new
{
    int	tm_sec;       /* second 0-59 */
    int	tm_min;       /* minute 0-59 */
    int	tm_hour;      /* hour   0-23 */
    int	tm_mday;      /* day of month  1-31 */
    int	tm_mon;       /* month  0-11 */
    int	tm_year;      /* year from 1900, Examples:2009 --> 109 */
    int	tm_wday;      /* week 0-6, sunday is 0 */
    int	tm_yday;      /* day of year 0-365 */
    int	tm_isdst;     /* daylight saving time.*/
};
#define _TM_NEW_DEFINED
#endif

#define SECSPERMIN	    60L
#define MINSPERHOUR	    60L
#define HOURSPERDAY	    24L
#define SECSPERHOUR	    (SECSPERMIN * MINSPERHOUR)
#define SECSPERDAY	    (SECSPERHOUR * HOURSPERDAY)
#define DAYSPERWEEK	    7
#define MONSPERYEAR	    12

#define _SEC_IN_MINUTE  60
#define _SEC_IN_HOUR    3600
#define _SEC_IN_DAY     86400

#define YEAR_BASE	    1900
#define EPOCH_YEAR      1970
#define EPOCH_WDAY      4

/* Judge leap year, If true,return 1; else return 0 */
#define isleap(y)       ((((y) % 4) == 0 && ((y) % 100) != 0) || ((y) % 400) == 0)

extern void gmtime_new(struct tm_new * res,unsigned long timestamp);
extern unsigned long mktime_new(struct tm_new * tim_p);

#endif // TIME_NEW_H_INCLUDED
