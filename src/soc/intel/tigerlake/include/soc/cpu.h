/* SPDX-License-Identifier: GPL-2.0-only */
/* This file is part of the coreboot project. */

#ifndef _SOC_TIGERLAKE_CPU_H_
#define _SOC_TIGERLAKE_CPU_H_

#include <intelblocks/msr.h>

/* Latency times in us */
#define C1_LATENCY	1
#define C6_LATENCY	127
#define C7_LATENCY	253
#define C8_LATENCY	260
#define C9_LATENCY	487
#define C10_LATENCY	1048

/* Power in units of mW */
#define C1_POWER	0x3e8
#define C6_POWER	0x15e
#define C7_POWER	0xc8
#define C8_POWER	0xc8
#define C9_POWER	0xc8
#define C10_POWER	0xc8

/* Common Timer Copy (CTC) frequency - 38.4MHz. */
#define CTC_FREQ	38400000

/* Configure power limits for turbo mode */
void set_power_limits(u8 power_limit_1_time);

#endif
