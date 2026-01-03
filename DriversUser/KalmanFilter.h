/*
 * KalmanFilter.h
 *
 *  Created on: Jan 3, 2026
 *      Author: axel
 */

#ifndef KALMANFILTER_H_
#define KALMANFILTER_H_

#include <stdint.h>

typedef struct {
	float Theta;
	float Theta_dot;
	float bias;
	float P00,P10,P01,P11; // covariance 2x2
	float Q_theta; // process noise
	float Q_bias;
	float R_acc; //acceleration measurment noise
} PitchState;

uint8_t Init_KalmanFilter(PitchState *kf);
void Kalman_Predict(PitchState *kf,float gyro_rad_s, float dt);
void Kalman_Update(PitchState *kf, float acceleration_angle, float dt);

#endif /* KALMANFILTER_H_ */
