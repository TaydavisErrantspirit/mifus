#ifndef CLOCK_H
#define CLOCK_H

#include <Arduino.h>
#include <RTClib.h>

class IClock
{
public:
	virtual bool begin() = 0;
	virtual DateTime now() = 0;
	virtual void setTime(DateTime time) = 0;
};
class Clock:public IClock
{
  private:
  RTC_DS1307 rtc;
  public:
  Clock():rtc()
  {
    rtc.begin();
  }
  virtual bool begin()
  {
    return rtc.isrunning();
  }
  virtual DateTime now()
  {
    return rtc.now();
  }
  virtual void setTime(DateTime time)
  {
    rtc.adjust(time);
  } 
};
#endif//CLOCK_H
