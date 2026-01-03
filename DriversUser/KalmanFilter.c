/*
 * KalmanFilter.c
 *
 *  Created on: Jan 3, 2026
 *      Author: axel
 */


#include "KalmanFilter.h"

uint8_t Init_KalmanFilter(PitchState *kf){

	kf->Theta_dot = 0.0f;
	kf->bias = 0.0f;

	kf->P00 = 1.0f;
	kf->P01 = 0.0f;
	kf->P10 = 0.0f;
	kf->P11 = 1.0f;

	kf->Q_theta = 0.001f;    // Process noise for angle
	kf->Q_bias = 0.003f;     // Process noise for bias


	kf->R_acc = 0.3f;        // Accelerometer measurement noise

	return 0;
}

void Kalman_Predict(PitchState *kf, float gyro_rad_s, float dt){

	// Predict the state estimate
	float rate = gyro_rad_s - kf-> bias;
	kf -> Theta += dt*rate;

	// Predict the estimate covariance
	// P_k = A * P_k-1 * A**T
	float P00_new = kf->P00 - dt*kf->P10 - dt*kf->P01 + dt*dt*kf->P11;
	float P01_new = kf->P01 - dt*kf->P11;
	float P10_new = kf->P10 -dt*kf->P11;
	float P11_new = kf->P11;

	kf->P00 = P00_new + kf->Q_theta;
	kf->P01 = P01_new;
	kf->P10 = P10_new;
	kf->P11 = P11_new + kf->Q_bias;

}

void Kalman_Update(PitchState *kf, float acceleration_angle, float dt){

	// Innovation step
	// y_k = z_k -H* x_k
	float y = acceleration_angle - kf-> Theta;


	// Innovation covariance
	// S_k = H_k * P_k * H_k**T + R_k
	float S = kf->P00 + kf->R_acc;

	// Optimal Kalman gain
	// K_k = P_k * H**T * S**-1
	float Kalman_gain_0 = kf->P00 / S;
	float Kalman_gain_1 = kf->P10 / S;

	// Update state estimate
	float old_theta = kf ->Theta;
	kf-> Theta += Kalman_gain_0 * y;
	kf-> bias += Kalman_gain_1 * y;

	// Update estimate covariance
	float P00_old = kf->P00;
	float P01_old = kf->P01;

	kf->P00 = P00_old * (1.0f - Kalman_gain_0);
	kf->P01 = P01_old * (1.0f - Kalman_gain_0);
	kf->P10 = kf->P10 - Kalman_gain_1 * P00_old;
	kf->P11 = kf->P11 - Kalman_gain_1 * P01_old;



}
