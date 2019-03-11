/*
 *  linux/include/linux/mmc/card.h
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Card driver specific definitions.
 */
#ifndef LINUX_MMC_INFOS_H
#define LINUX_MMC_INFOS_H

#include <linux/mmc/card.h>
#include <linux/mmc/host.h>

struct mmc_info {
	/* CID-specific fields. */
	const char *name;

	/* Valid revision range */
	u64 rev_start, rev_end;

	unsigned int manfid;
	unsigned short oemid;

	void (*vendor_info)(struct mmc_card *card, const char *data);
	void *data;
};

extern void mmc_device_info(struct mmc_card *card, const struct mmc_info *table);
#endif /* LINUX_MMC_CARD_H */
