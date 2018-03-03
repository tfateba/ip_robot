
/**
 *
 * @file    ip_encoder.h
 *
 * @brief   Encoder driver header file.
 *
 * @author  Theodore Ateba, tf.ateba@gmail.com
 *
 * @date    27 August 2017
 *
 */

#ifndef IP_ENCODER_H
#define IP_ENCODER_H

/**
 * @brief Encoders enumerations.
 */
typedef enum {
  ENCODER_L,          /**< Left encoder.                  */
  ENCODER_R,          /**< Right encoder.                 */
  ENCODER_L_STATE_A,  /**< State A of the left encoder.   */
  ENCODER_L_STATE_B,  /**< State B of the left encoder.   */
  ENCODER_R_STATE_A,  /**< State A of the right encoder.  */
  ENCODER_R_STATE_B   /**< State B of the right encoder.  */
}encoder_e;

/* Arduino Interruption */
#define INT2 2      /**< D19 [PD2], BLACK wire motor Right.                 */
#define INT3 3      /**< D18 [PD3], RED wire Motor Left.                    */

#define L_ENCODER_A_PORT  IOPORT4 /**< Left encoder A port.                 */
#define L_ENCODER_B_PORT  IOPORT7 /**< Left encoder B port.                 */

#define L_ENCODER_A       PD3     /**< Left encoder A pin.                  */
#define L_ENCODER_B       PG5     /**< Left encoder B pin.                  */

#define R_ENCODER_B_PORT  IOPORT5 /**< Right encoder B port.                */
#define R_ENCODER_A_PORT  IOPORT4 /**< Right encoder A port.                */

#define R_ENCODER_A       PD2     /**< Right encoder A pin.                 */
#define R_ENCODER_B       PE3     /**< Right encoder B pin.                 */

struct ENCODERDriver {
  uint8_t     id;       /**< Encoder identification name.                   */
  uint8_t     eichan;   /**< Encoder external interruption channel.         */
  ioportid_t  porta;    /**< Encoder port A.                                */
  uint8_t     pina;     /**< Encoder pin A.                                 */
  ioportid_t  portb;    /**< Encoder port B.                                */
  uint8_t     pinb;     /**< Encoder pin B.                                 */
  long        counter;  /**< Right encoder counter.                         */
  bool        statea;   /**< Left motor encoder A.                          */
  bool        stateb;   /**< Left motor encoder B.                          */
};

typedef struct ENCODERDriver ENCODERDriver;

/*==========================================================================*/
/* Functions prototypes.                                                    */
/*==========================================================================*/

bool encoderReadLeftStateA(void);
bool encoderReadLeftStateB(void);
bool encoderReadRightStateA(void);
bool encoderReadRightStateB(void);
void encoderInit(void);
void encoderGetWheelVelocity(void);
void encoderInit(void);

#endif /* IP_ENCODER_H */
