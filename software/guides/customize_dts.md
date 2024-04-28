#  Customize the Device Tree files
* First generate default DTS's using the CubeMX project found at `tios/Core/cubemx`
* This is based on a ST HandsOn Lab, but with various issues fixed.

## 1. Alter TF-A

### A. stm32mp151a-tios-mx-fw-config.dts
No changes needed

### B. stm32mp151a-tios-mx.dts

* USER CODE for root, to set console output and vin 
```c
	aliases {		
		serial0 = &uart4;
	};
	chosen {
		stdout-path = "serial0:115200n8";
	};
	vin: vin {
		compatible = "regulator-fixed";
		regulator-name = "vin";
		regulator-min-microvolt = <5000000>;
		regulator-max-microvolt = <5000000>;
		regulator-always-on;
    };
```
* USER CODE for i2c4, to set up the eeprom and pmic
````c
clock-frequency = <400000>;
	eeprom@50 {
		compatible = "atmel,24c32";
		reg = <0x50>;
		pagesize = <32>;
		//wp-gpios = <&gpioa 14 0>;
		//num-addresses = <8>;
	};
	pmic: stpmic@33 {
		compatible = "st,stpmic1";
		reg = <0x33>;
		interrupts-extended = <&exti_pwr 55 IRQ_TYPE_EDGE_FALLING>;
		interrupt-controller;
		#interrupt-cells = <2>;
		status = "okay";

		regulators {
			compatible = "st,stpmic1-regulators";
			buck1-supply = <&vin>;
			buck2-supply = <&vin>;
			buck3-supply = <&vin>;
			buck4-supply = <&vin>;
			
			ldo3-supply = <&vdd_ddr>;
			ldo4-supply = <&vin>;
			ldo5-supply = <&vin>;
			
			vref_ddr-supply = <&vin>;
			boost-supply = <&vin>;

			vddcore: buck1 {
				regulator-name = "vddcore";
				regulator-min-microvolt = <1200000>;
				regulator-max-microvolt = <1350000>;
				regulator-always-on;
				regulator-initial-mode = <0>;
				regulator-over-current-protection;
			};

			vdd_ddr: buck2 {
				regulator-name = "vdd_ddr";
				regulator-min-microvolt = <1350000>;
				regulator-max-microvolt = <1350000>;
				regulator-always-on;
				regulator-initial-mode = <0>;
				regulator-over-current-protection;
			};

			vdd: buck3 {
				regulator-name = "vdd";
				regulator-min-microvolt = <3300000>;
				regulator-max-microvolt = <3300000>;
				regulator-always-on;
				st,mask-reset;
				regulator-initial-mode = <0>;
				regulator-over-current-protection;
			};

			v3v3: buck4 {
				regulator-name = "v3v3";
				regulator-min-microvolt = <3300000>;
				regulator-max-microvolt = <3300000>;
				regulator-always-on;
				regulator-over-current-protection;
				regulator-initial-mode = <0>;
			};

			vtt_ddr: ldo3 {
				regulator-name = "vtt_ddr";
				regulator-always-on;
				regulator-over-current-protection;
				st,regulator-sink-source;
			};
			vref_ddr: vref_ddr {
				regulator-name = "vref_ddr";
				regulator-always-on;
			};
		};
````
* USER CODE for sdmmc1, not actually used at boot for now but that might change
````c
	disable-wp;
	st,neg-edge;
	bus-width = <4>;
	vmmc-supply = <&v3v3>;
````
* USER CODE for sdmmc2, connects the emmc
````c
	non-removable;
	no-sd;
	no-sdio;
	st,neg-edge;
	bus-width = <4>;
	vmmc-supply = <&v3v3>;
	vqmmc-supply = <&v3v3>;
	mmc-ddr-3_3v;
````
* USER CODE for addons
````c
&pwr_regulators {
	vdd-supply = <&vdd>;
};
&bsec {
	board_id: board_id@ec {
		reg = <0xec 0x4>;
		st,non-secure-otp;
	};
};
````
### C. stm32mp15-mx.dtsi

* Line 84ish: Change value of #define DDR_ADDRMAP6 from `0x0F070706` to `0x0F060606`
  * Not sure why this needs to be done, but if not you'll get a DDR addr issue on boot
* Append the calibration values using following lines
````
  #define DDR_DX0DLLCR 0x40000000
  #define DDR_DX0DQTR 0xFFFFFFFF
  #define DDR_DX0DQSTR 0x3DB02000
  #define DDR_DX1DLLCR 0x40000000
  #define DDR_DX1DQTR 0xFFFFFFFF
  #define DDR_DX1DQSTR 0x3DB02000
  #define DDR_DX2DLLCR 0x40000000
  #define DDR_DX2DQTR 0xFFFFFFFF
  #define DDR_DX2DQSTR 0x3DB02000
  #define DDR_DX3DLLCR 0x40000000
  #define DDR_DX3DQTR 0xFFFFFFFF
  #define DDR_DX3DQSTR 0x3DB02000
````
## 2. Alter Optee-os
### stm32mp151a-tios-mx
* USER CODE for root, to set console output and vdd/vcore
```c
aliases {
		serial0 = &uart4;
	};
	chosen {
		stdout-path = "serial0:115200n8";
	};
	vddcore: vddcore {
		compatible = "regulator-fixed";
		regulator-name = "vddcore";
		regulator-min-microvolt = <1200000>;
		regulator-max-microvolt = <1350000>;
		regulator-always-on;
	};
	vdd: vdd {
		compatible = "regulator-fixed";
		regulator-name = "vdd";
		regulator-min-microvolt = <3300000>;
		regulator-max-microvolt = <3300000>;
		regulator-always-on;
	};
````
* USER CODE for pinctrl, set the pins of the console port
````c	
	uart4_pins_mx: uart4_mx-0 {
		pins1 {
			pinmux = <STM32_PINMUX('A', 11, AF6)>; /* UART4_RX */
			bias-disable;
		};
		pins2 {
			pinmux = <STM32_PINMUX('G', 11, AF6)>; /* UART4_TX */
			bias-disable;
			drive-push-pull;
			slew-rate = <0>;
		};
	};
````
* USER CODE for bsec
````c
	board_id: board_id@ec {
		reg = <0xec 0x4>;
		st,non-secure-otp;
	};
````
* USER CODE for ADDONS
````c
&uart4 {
	pinctrl-names = "default";
	pinctrl-0 = <&uart4_pins_mx>;
	status = "okay";
};
&cpu0 {
	cpu-supply = <&vddcore>;
};
&pwr_regulators {
	vdd-supply = <&vdd>;
};
````
## 3. Alter U-Boot
* Add a file with the name `stm32mp151a-tios-scmi.dtsi` and following content
````c
// SPDX-License-Identifier: (GPL-2.0+ OR BSD-3-Clause)
/*
 * Copyright (C) STMicroelectronics 2022 - All Rights Reserved
 * Author: Alexandre Torgue <alexandre.torgue@foss.st.com> for STMicroelectronics.
 */

#include "stm32mp15-scmi.dtsi"

/ {
	reserved-memory {
		optee@de000000 {
			reg = <0xde000000 0x2000000>;
			no-map;
		};
	};
};

&cpu0 {
	clocks = <&scmi_clk CK_SCMI_MPU>;
};
/*
&cpu1 {
	clocks = <&scmi_clk CK_SCMI_MPU>;
};

&cryp1 {
	clocks = <&scmi_clk CK_SCMI_CRYP1>;
	resets = <&scmi_reset RST_SCMI_CRYP1>;
};

&dsi {
	clocks = <&rcc DSI_K>, <&scmi_clk CK_SCMI_HSE>, <&rcc DSI_PX>;
};
*/
&gpioz {
	clocks = <&scmi_clk CK_SCMI_GPIOZ>;
};

&hash1 {
	clocks = <&scmi_clk CK_SCMI_HASH1>;
	resets = <&scmi_reset RST_SCMI_HASH1>;
};

&i2c4 {
	clocks = <&scmi_clk CK_SCMI_I2C4>;
	resets = <&scmi_reset RST_SCMI_I2C4>;
};

&iwdg2 {
	clocks = <&rcc IWDG2>, <&scmi_clk CK_SCMI_LSI>;
};

&m4_rproc {
	resets = <&scmi_reset RST_SCMI_MCU>,
		 <&scmi_reset RST_SCMI_MCU_HOLD_BOOT>;
	reset-names = "mcu_rst", "hold_boot";
	/delete-property/ st,syscfg-holdboot;
};

&mdma1 {
	resets = <&scmi_reset RST_SCMI_MDMA>;
};

&mlahb {
	resets = <&scmi_reset RST_SCMI_MCU>;
};

&rcc {
	compatible = "st,stm32mp1-rcc-secure", "syscon";
	clock-names = "hse", "hsi", "csi", "lse", "lsi";
	clocks = <&scmi_clk CK_SCMI_HSE>,
		 <&scmi_clk CK_SCMI_HSI>,
		 <&scmi_clk CK_SCMI_CSI>,
		 <&scmi_clk CK_SCMI_LSE>,
		 <&scmi_clk CK_SCMI_LSI>;
};

&rng1 {
	clocks = <&scmi_clk CK_SCMI_RNG1>;
	resets = <&scmi_reset RST_SCMI_RNG1>;
};

&rtc {
	clocks = <&scmi_clk CK_SCMI_RTCAPB>, <&scmi_clk CK_SCMI_RTC>;
};

&usart1 {
	clocks = <&scmi_clk CK_SCMI_USART1>;
};
````
### stm32mp151a-tios-mx-u-boot.dtsi
* USER CODE for includes, to add the earlier made file
````c
#include "stm32mp15-scmi-u-boot.dtsi"
````
* USER CODE for root, to add aliases & config
````c
	aliases{
		mmc0 = &sdmmc1;
		mmc1 = &sdmmc2;
		usb0 = &usbotg_hs;
	};
	config{
		u-boot,mmc-env-partition = "u-boot-env";
	};
````
* USER CODE for addons, to set up console port and usb for UMS (?)
````c
&uart4{
	u-boot,dm-pre-reloc;
};
&uart4_pins_mx {
	u-boot,dm-pre-reloc;
};
&usbotg_hs {
	u-boot,force-b-session-valid;
	dr_mode = "peripheral";
};
````
### stm32mp151a-tios-mx.dts
* USER CODE for includes, to add the earlier made file and bindings for the STPMIC1
````c
#include "stm32mp151a-tios-scmi.dtsi"
#include <dt-bindings/mfd/st,stpmic1.h>
````
* USER CODE for root, to add aliases, debug out and vin
````c
	aliases {		
		serial0 = &uart4;
	};
	chosen {
		stdout-path = "serial0:115200n8";
	};
	vin: vin {
		compatible = "regulator-fixed";
		regulator-name = "vin";
		regulator-min-microvolt = <5000000>;
		regulator-max-microvolt = <5000000>;
		regulator-always-on;
	};
````
* USER CODE for i2c4, adding pmic
````c
      pmic: stpmic@33 {
            compatible = "st,stpmic1";
            reg = <0x33>;
            status = "okay";
            regulators {
            compatible = "st,stpmic1-regulators";
            buck1-supply = <&vin>;
            buck2-supply = <&vin>;
            buck3-supply = <&vin>;
            buck4-supply = <&vin>;

			ldo3-supply = <&vdd_ddr>;
			ldo4-supply = <&vin>;
			ldo5-supply = <&vin>;
			
			vref_ddr-supply = <&vin>;
			boost-supply = <&vin>;
			pwr_sw1-supply = <&bst_out>;
			pwr_sw2-supply = <&bst_out>;

			vddcore: buck1 {
				regulator-name = "vddcore";
				regulator-min-microvolt = <1200000>;
				regulator-max-microvolt = <1350000>;
				regulator-always-on;
				regulator-initial-mode = <0>;
				regulator-over-current-protection;
			};

			vdd_ddr: buck2 {
				regulator-name = "vdd_ddr";
				regulator-min-microvolt = <1350000>;
				regulator-max-microvolt = <1350000>;
				regulator-always-on;
				regulator-initial-mode = <0>;
				regulator-over-current-protection;
			};

			vdd: buck3 {
				regulator-name = "vdd";
				regulator-min-microvolt = <3300000>;
				regulator-max-microvolt = <3300000>;
				regulator-always-on;
				st,mask-reset;
				regulator-initial-mode = <0>;
				regulator-over-current-protection;
			};

			v3v3: buck4 {
				regulator-name = "v3v3";
				regulator-min-microvolt = <3300000>;
				regulator-max-microvolt = <3300000>;
				regulator-always-on;
				regulator-over-current-protection;
				regulator-initial-mode = <0>;
			};

			vtt_ddr: ldo3 {
				regulator-name = "vtt_ddr";
				regulator-always-on;
				regulator-over-current-protection;
				st,regulator-sink-source;
			};
			vdd_usb: ldo4 {
				regulator-name = "vdd_usb";
				regulator-min-microvolt = <3300000>;
				regulator-max-microvolt = <3300000>;
				regulator-over-current-protection;
				regulator-always-on;
			};
			vref_ddr: vref_ddr {
				regulator-name = "vref_ddr";
				regulator-always-on;
			};
			vbus_otg: pwr_sw1 {
				regulator-name = "vbus_otg";
				interrupts = <IT_OCP_OTG 0>;
			 };
			vbus_sw: pwr_sw2 {
				regulator-name = "vbus_sw";
				interrupts = <IT_OCP_SWOUT 0>;
				regulator-active-discharge = <1>;
			};
			bst_out: boost {
				regulator-name = "bst_out";
				interrupts = <IT_OCP_BOOST 0>;
			 };
		};
	};
````
* USER CODE for sdmmc1, not actually used at boot for now but that might change
````c
	cd-gpios = <&gpiob 7 (GPIO_ACTIVE_LOW | GPIO_PULL_UP)>;
	disable-wp;
	st,neg-edge;
	bus-width = <4>;
	vmmc-supply = <&v3v3>;
````
* USER CODE for sdmmc2, connects the emmc
````c
	non-removable;
	no-sd;
	no-sdio;
	st,neg-edge;
	bus-width = <4>;
	vmmc-supply = <&v3v3>;
	vqmmc-supply = <&vdd>;
	mmc-ddr-3_3v;
````
* USER CODE for usbotg_hs

	phys = <&usbphyc_port1 0>;
	phy-names = "usb2-phy";
	usb-role-switch;

* USER CODE for usbphyc_port0 
````c
  phy-supply = <&vdd_usb>;
````
* USER CODE for usbphyc_port1
````c
    phy-supply = <&vdd_usb>;
    st,tune-hs-dc-level = <2>;
    st,enable-fs-rftime-tuning;
    st,enable-hs-rftime-reduction;
    st,trim-hs-current = <15>;
    st,trim-hs-impedance = <1>;
    st,tune-squelch-level = <3>;
    st,tune-hs-rx-offset = <2>;
    st,no-lsfs-sc;
````
* USER CODE for addons
````c
  &pwr_regulators {
    vdd-supply = <&vdd>;
    vdd_3v3_usbfs-supply = <&vdd_usb>;
  };
````
## 4. Alter kernel