/*
 * Clock.h
 *
 *  Created on: May 19, 2014
 *      Author: whj-linux
 */

#ifndef CLOCK_H_
#define CLOCK_H_

class Clock {
public:
	Clock()
	{
		clock = 0;
	}
	unsigned int getCurrentTime()
	{
		return clock;
	}
	unsigned int increaseTime()
	{
		clock++;
		return clock;
	}
protected:
	unsigned int clock;
};

#endif /* CLOCK_H_ */
