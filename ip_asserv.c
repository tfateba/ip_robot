/**
 *
 * @file    ip_asserv.c
 *
 * @brief   Asservissement of inverted pendulum Robot.
 *
 * @author  Theodore Ateba, tf.ateba@gmail.com
 *
 * @date    07 Septembre 2015
 *
 */

/*==========================================================================*/
/* Includes files.                                                          */
/*==========================================================================*/

/* Standard files.  */
#include <math.h>

/* Project local files. */
#include "ip_conf.h"
#include "ip_kalman.h"
#include "ip_motor.h"
#include "ip_mpu6050.h"
#include "ip_pid.h"

/*==========================================================================*/
/* Application macros.                                                      */
/*==========================================================================*/

#if (DEBUG == TRUE)
#define mpu         "MPU6050"      /**< MPU6050 device name.                */
#endif

#define PI          3.14159265359  /**< Mathematical PI constant.           */
#define RAD_TO_DEG  180/PI         /**< Constant for Radian to degre.       */

/*==========================================================================*/
/* Global variables, I2C TX and RX buffers, I2C and Serial Configurations.  */
/*==========================================================================*/

const double   dt = 0.01;     /**< Asservissement period.                   */

bool    layingDown    = true; /**< See if the robot is down.                */
double  targetAngle   = 180;  /**< The angle we want the robot to reach.    */
double  targetOffset  = 0;    /**< Offset for going forward and backwrd.    */
double  turningOffset = 0;    /**< Offset for turning left and right.       */

#if (DEBUG == TRUE)
extern BaseSequentialStream* chp;
#endif
extern mpu6050_t  imu;
extern msg_t      msg;

/*==========================================================================*/
/* Functions.                                                               */
/*==========================================================================*/

// TODO: Implement the multi threading for, acquisition, treatment, control.

/**
 * @brief  Asservissement routine of the robot.
 */
void asserv(void) {

  /* Read the IMU data (x,y,z accel and gyroscope). */
  msg = mpu6050GetData(&I2CD1, &imu);

  if (msg != MSG_OK) {
#if (DEBUG == TRUE)
    chprintf(chp, "\n\r %s: Error while reading the %s sensor data.", mpu, mpu);
#endif
    return;
  }

  /* Calcul of the Pitch angle of the self-balancing robot. */
  imu.pitch = (atan2(imu.y_accel, imu.z_accel) + PI)*(RAD_TO_DEG);

  /* Get the Kalman estimation of the pitch angle of the robot. */
  imu.pitch_k = kalmanGetAngle(imu.pitch, (imu.x_gyro / 131.0), dt);

  if ((layingDown && (imu.pitch_k < 170 || imu.pitch_k > 190)) ||
    (!layingDown && (imu.pitch_k < 135 || imu.pitch_k > 225))) {
    /*
     * The robot is in a unsolvable position, so turn off both motors and
     * wait until it's vertical again.
     */

    layingDown = true;
    motorsStopAndReset();
  }
  else {
    /*
     * It's no longer laying down,
     * so we can try to stabilized the robot now.
     */
    layingDown = false;
    pid(imu.pitch_k, targetAngle, targetOffset, turningOffset);
#if (DEBUG == TRUE)
    chprintf(chp, " pitch:%.3f\r\n", imu.pitch_k);
#endif
  }

  /* Update the robot wheel velocity every 100ms. */
  //motorGetWheelVelocity();
}

