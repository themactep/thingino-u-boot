/*
 * (C) Copyright 2012-2013
 * Texas Instruments, <www.ti.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <config.h>
#include <palmas.h>

void palmas_init_settings(void)
{
#ifdef CONFIG_PALMAS_SMPS7_FPWM
	int err;
	/*
	 * Set SMPS7 (1.8 V I/O supply on platforms with TWL6035/37) to
	 * forced PWM mode. This reduces noise (but affects efficiency).
	 */
	u8 val = SMPS_MODE_SLP_FPWM | SMPS_MODE_ACT_FPWM;
	err = palmas_i2c_write_u8(TWL603X_CHIP_P1, SMPS7_CTRL, val);
	if (err)
		printf("palmas: could not force PWM for SMPS7: err = %d\n",
		       err);
#endif
}

int palmas_mmc1_poweron_ldo(void)
{
	u8 val = 0;

#if defined(CONFIG_DRA7XX)
	/*
	 * Currently valid for the dra7xx_evm board:
	 * Set TPS659038 LDO1 to 3.0 V
	 */
	val = LDO_VOLT_3V0;
	if (palmas_i2c_write_u8(TPS65903X_CHIP_P1, LDO1_VOLTAGE, val)) {
		printf("tps65903x: could not set LDO1 voltage.\n");
		return 1;
	}
	/* TURN ON LDO1 */
	val = RSC_MODE_SLEEP | RSC_MODE_ACTIVE;
	if (palmas_i2c_write_u8(TPS65903X_CHIP_P1, LDO1_CTRL, val)) {
		printf("tps65903x: could not turn on LDO1.\n");
		return 1;
	}
	return 0;
#else
	/*
	 * We assume that this is a OMAP543X + TWL603X board:
	 * Set TWL6035/37 LDO9 to 3.0 V
	 */
	val = LDO_VOLT_3V0;
	return twl603x_mmc1_set_ldo9(val);
#endif
}

/*
 * On some OMAP5 + TWL603X hardware the SD card socket and LDO9_IN are
 * powered by an external 3.3 V regulator, while the output of LDO9
 * supplies VDDS_SDCARD for the OMAP5 interface only. This implies that
 * LDO9 could be set to 'bypass' mode when required (e.g. for 3.3 V cards).
 */
int twl603x_mmc1_set_ldo9(u8 vsel)
{
	u8 cval = 0, vval = 0;	/* Off by default */
	int err;

	if (vsel) {
		/* Turn on */
		if (vsel > LDO_VOLT_3V3) {
			/* Put LDO9 in bypass */
			cval = LDO9_BYP_EN | RSC_MODE_SLEEP | RSC_MODE_ACTIVE;
			vval = LDO_VOLT_3V3;
		} else {
			cval = RSC_MODE_SLEEP | RSC_MODE_ACTIVE;
			vval = vsel & 0x3f;
		}
	}
	err = palmas_i2c_write_u8(TWL603X_CHIP_P1, LDO9_VOLTAGE, vval);
	if (err) {
		printf("twl603x: could not set LDO9 %s: err = %d\n",
		       vsel > LDO_VOLT_3V3 ? "bypass" : "voltage", err);
		return err;
	}
	err = palmas_i2c_write_u8(TWL603X_CHIP_P1, LDO9_CTRL, cval);
	if (err)
		printf("twl603x: could not turn %s LDO9: err = %d\n",
		       cval ? "on" : "off", err);
	return err;
}

#ifdef CONFIG_PALMAS_AUDPWR
/*
 * Turn audio codec power and 32 kHz clock on/off. Use for
 * testing OMAP543X + TWL603X + TWL604X boards only.
 */
int twl603x_audio_power(u8 on)
{
	u8 cval = 0, vval = 0, c32k = 0;
	int err;

	if (on) {
		vval = SMPS_VOLT_2V1;
		cval = SMPS_MODE_SLP_AUTO | SMPS_MODE_ACT_AUTO;
		c32k = RSC_MODE_SLEEP | RSC_MODE_ACTIVE;
	}
	/* Set SMPS9 to 2.1 V (for TWL604x), or to 0 (off) */
	err = palmas_i2c_write_u8(TWL603X_CHIP_P1, SMPS9_VOLTAGE, vval);
	if (err) {
		printf("twl603x: could not set SMPS9 voltage: err = %d\n",
		       err);
		return err;
	}
	/* Turn on or off SMPS9 */
	err = palmas_i2c_write_u8(TWL603X_CHIP_P1, SMPS9_CTRL, cval);
	if (err) {
		printf("twl603x: could not turn SMPS9 %s: err = %d\n",
		       cval ? "on" : "off", err);
		return err;
	}
	/* Output 32 kHz clock on or off */
	err = palmas_i2c_write_u8(TWL603X_CHIP_P1, CLK32KGAUDIO_CTRL, c32k);
	if (err)
		printf("twl603x: could not turn CLK32KGAUDIO %s: err = %d\n",
		       c32k ? "on" : "off", err);
	return err;
}
#endif

/*
 * Enable/disable back-up battery (or super cap) charging on TWL6035/37.
 * Please use defined BB_xxx values.
 */
int twl603x_enable_bb_charge(u8 bb_fields)
{
	u8 val = bb_fields & 0x0f;
	int err;

	val |= (VRTC_EN_SLP | VRTC_EN_OFF | VRTC_PWEN);
	err = palmas_i2c_write_u8(TWL603X_CHIP_P1, BB_VRTC_CTRL, val);
	if (err)
		printf("twl603x: could not set BB_VRTC_CTRL to 0x%02x: err = %d\n",
		       val, err);
	return err;
}
