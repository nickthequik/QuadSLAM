/*
 * quaternionFilters.h
 *
 *  Created on: Nov 10, 2017
 *      Author: Ryan
 */

#ifndef SRC_QUATERNIONFILTERS_H_
#define SRC_QUATERNIONFILTERS_H_

#include <math.h>

void MadgwickQuaternionUpdate(float ax, float ay, float az, float gx, float gy,
                              float gz, float mx, float my, float mz,
                              float deltat);
void MahonyQuaternionUpdate(float ax, float ay, float az, float gx, float gy,
                            float gz, float mx, float my, float mz,
                            float deltat);
const float * getQ();

#endif /* SRC_QUATERNIONFILTERS_H_ */
