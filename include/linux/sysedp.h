/*
 * Copyright (c) 2013, NVIDIA CORPORATION.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _LINUX_SYSEDP_H
#define _LINUX_SYSEDP_H

#include <linux/kernel.h>
#include <linux/notifier.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>

#define SYSEDP_NAME_LEN 32

/*
 * @name: name of consumer
 * @states: EDP state array holding the max peak power for each state.
 * @num_states: length of the above array
 * @state: current power state of sysedp consumer
 */
struct sysedp_consumer {
	char name[SYSEDP_NAME_LEN];
	unsigned int *states;
	unsigned int num_states;

	unsigned int state;

	/* internal */
	int removable;
	struct list_head link;
	struct kobject kobj;

#ifdef CONFIG_DEBUG_FS
	/* public */
	struct dentry *dentry;
#endif
};

struct sysedp_consumer_data {
	char *name;
	unsigned int *states;
	unsigned int num_states;
};

struct sysedp_platform_data {
	struct sysedp_consumer_data *consumer_data;
	unsigned int consumer_data_size;
	int margin;
};

#define SYSEDP_CONSUMER_DATA(_name, _states)	  \
	{					  \
		.name = _name,			  \
		.states = _states,		  \
		.num_states = ARRAY_SIZE(_states) \
	}

/*
 * Temperature -> IBAT LUT
 * Should be descending wrt temp
 * { ..., .ibat = 0 } must be the last entry
 */
struct sysedp_batmon_ibat_lut {
	int temp;
	unsigned int ibat;
};

/*
 * Capacity -> RBAT LUT
 * Should be descending wrt capacity
 * { .capacity = 0, ... } must be the last entry
 */
struct sysedp_batmon_rbat_lut {
	unsigned int capacity;
	unsigned int rbat;
};

/*
 * Capacity -> OCV LUT
 * Should be descending wrt capacity
 * { .capacity = 0, ... } must be the last entry
 * @capacity: battery capacity in percents
 * @ocv: OCV in uV
 */
struct sysedp_batmon_ocv_lut {
	unsigned int capacity;
	unsigned int ocv;
};

/* Battery monitor data */
struct sysedp_batmon_calc_platform_data {
	char *power_supply;
	unsigned int r_const;
	unsigned int vsys_min;
	struct sysedp_batmon_ibat_lut *ibat_lut;
	struct sysedp_batmon_rbat_lut *rbat_lut;
	struct sysedp_batmon_ocv_lut *ocv_lut;
	unsigned int update_interval;
};

#ifdef CONFIG_SYSEDP_FRAMEWORK
extern struct dentry *edp_debugfs_dir;

void sysedp_set_state(struct sysedp_consumer *, unsigned int);
struct sysedp_consumer *sysedp_create_consumer(const char *, const char *);
int sysedp_register_consumer(struct sysedp_consumer *);
void sysedp_unregister_consumer(struct sysedp_consumer *);
void sysedp_free_consumer(struct sysedp_consumer *);

#else
static inline void sysedp_set_state(struct sysedp_consumer *c, unsigned int i)
{ return; }
static inline struct sysedp_consumer *sysedp_create_consumer(const char *c,
							     const char *s)
{ return NULL; }
static inline int sysedp_register_consumer(struct sysedp_consumer *c)
{ return -ENODEV; }
static inline void sysedp_unregister_consumer(struct sysedp_consumer *c)
{ return; }
static inline void sysedp_free_consumer(struct sysedp_consumer *c)
{ return; }
#endif

#endif