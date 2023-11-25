/*
 * hardware.h

 */

#ifndef INC_HARDWARE_H_
	#define INC_HARDWARE_H_

	/* Includes ------------------------------------------------------------------*/

	/* Private includes ----------------------------------------------------------*/

	/* Exported types ------------------------------------------------------------*/
	typedef struct {
		/* OUT 1 settings 64bit */
		uint16_t out1_lowvlim;
		uint16_t out1_highvlim;
		uint16_t out1_curlim;
		uint8_t out1_defState;
		uint8_t out1_spare;

		/* OUT 2 settings 64bit */
		uint16_t out2_lowvlim;
		uint16_t out2_highvlim;
		uint16_t out2_curlim;
		uint8_t out2_defState;
		uint8_t out2_spare;

		/* OUT 3 settings 64bit */
		uint16_t out3_lowvlim;
		uint16_t out3_highvlim;
		uint16_t out3_curlim;
		uint8_t out3_defState;
		uint8_t out3_spare;

		/* OUT 4 settings 64bit */
		uint16_t out4_lowvlim;
		uint16_t out4_highvlim;
		uint16_t out4_curlim;
		uint8_t out4_defState;
		uint8_t out4_spare;

		/* Other 32bit*/
		uint16_t overcurrentTime;
		uint8_t  pacAddress;
		uint8_t spare;
	} settings_t;

	typedef struct {
			/* OUT 1 settings 64bit */

			uint16_t out1_voltage;
			uint16_t out2_current;
			uint16_t out3_voltage;
			uint16_t out4_voltage;

			uint16_t out1_current;
			uint16_t out2_voltage;
			uint16_t out3_current;
			uint16_t out4_current;

	} voltcur;

	extern settings_t *GLOBAL_settings_ptr;
	/* Exported constants --------------------------------------------------------*/

	/* Exported macro ------------------------------------------------------------*/

	/* Exported functions prototypes ---------------------------------------------*/
	/* Private defines -----------------------------------------------------------*/

	/* Hearbeat */
	#define HEART_ON  GPIOB->ODR |= GPIO_ODR_OD5
	#define HEART_OFF GPIOB->ODR &= ~(GPIO_ODR_OD5)

	// I2C
	/* Refresh cmds */
	#define PAC1954_REFRESH				0x00
	#define PAC1954_REFRESH_V			0x1F
	/* Registers */
	#define PAC1954_CTRL_REG			0x01
	#define PAC1954_ACC_CNT_REG			0x02	// 4 bytes (32bit)
	#define PAC1954_VACCN_1_REG			0x03	// 7 bytes (56bit)
	#define PAC1954_VACCN_2_REG			0x04	// 7 bytes (56bit)
	#define PAC1954_VACCN_3_REG			0x05	// 7 bytes (56bit)
	#define PAC1954_VACCN_4_REG			0x06	// 7 bytes (56bit)

	#define PAC1954_VBUSN_REG			0x07	// 4x2byte (64bit)
	#define PAC1954_SENSEN_REG			0x0B	// 4x2byte (64bit)
	#define PAC1954_VBUSN_AVG_REG		0x0F	// 4x2byte (64bit)
	#define PAC1954_VSENSEN_AVG_REG		0x13	// 4x2byte (64bit)
	#define PAC1954_VPOWERN_REG			0x17	// 4x4byte (128bit)

	#define PAC1954_ACCUM_CFG_REG		0x25	// 1 byte (8bit)
	#define PAC1954_ALERT_STS_REG		0x26	// 3 byte (24bit)
	#define PAC1954_SLOW_ALERT1_REG		0x27	// 3 byte (24bit)
	#define PAC1954_GPIO_ALERT2_REG		0x28	// 3 byte (24bit)
	/* Limit reg */
	#define PAC1954_OC_LMTN_1_REG		0x30	// 2 byte (16bit)
	#define PAC1954_OC_LMTN_2_REG		0x31	// 2 byte (16bit)
	#define PAC1954_OC_LMTN_3_REG		0x32	// 2 byte (16bit)
	#define PAC1954_OC_LMTN_4_REG		0x33	// 2 byte (16bit)
	#define PAC1954_UC_LMTN_1_REG		0x34	// 2 byte (16bit)
	#define PAC1954_UC_LMTN_2_REG		0x35	// 2 byte (16bit)
	#define PAC1954_UC_LMTN_3_REG		0x36	// 2 byte (16bit)
	#define PAC1954_UC_LMTN_4_REG		0x37	// 2 byte (16bit)
	#define PAC1954_OP_LMTN_1_REG		0x38	// 2 byte (16bit)
	#define PAC1954_OP_LMTN_2_REG		0x39	// 2 byte (16bit)
	#define PAC1954_OP_LMTN_3_REG		0x3A	// 2 byte (16bit)
	#define PAC1954_OP_LMTN_4_REG		0x3B	// 2 byte (16bit)
	#define PAC1954_OV_LMTN_1_REG		0x3C	// 2 byte (16bit)
	#define PAC1954_OV_LMTN_2_REG		0x3D	// 2 byte (16bit)
	#define PAC1954_OV_LMTN_3_REG		0x3E	// 2 byte (16bit)
	#define PAC1954_OV_LMTN_4_REG		0x3F	// 2 byte (16bit)
	#define PAC1954_UV_LMTN_1_REG		0x40	// 2 byte (16bit)
	#define PAC1954_UV_LMTN_2_REG		0x41	// 2 byte (16bit)
	#define PAC1954_UV_LMTN_3_REG		0x42	// 2 byte (16bit)
	#define PAC1954_UV_LMTN_4_REG		0x43	// 2 byte (16bit)
	/* Limit trigger samples */
	#define PAC1954_OC_LMTN_S_REG		0x44	// 1 byte (8bit)
	#define PAC1954_UC_LMTN_S_REG		0x45	// 1 byte (8bit)
	#define PAC1954_OP_LMTN_S_REG		0x46	// 1 byte (8bit)
	#define PAC1954_OV_LMTN_S_REG		0x47	// 1 byte (8bit)
	#define PAC1954_UV_LMTN_S_REG		0x48	// 1 byte (8bit)
	/* Other */
	#define PAC1954_ALERT_EN_REG		0x49	// 3 byte (24bit)
	#define PAC1954_ACC_CFG_ACT_REG		0x4A	// 1 byte (8bit)
	#define PAC1954_ACC_CFG_LAT_REG		0x4B	// 1 byte (8bit)
	#define PAC1954_PRODUCT_ID			0xFD	// 1 byte (8bit)
	#define PAC1954_MANUF_ID_REG		0xFE	// 1 byte (8bit)
	#define PAC1954_REVISION_ID_REG		0xFF	// 1 byte (8bit)

#endif /* INC_HARDWARE_H_ */
