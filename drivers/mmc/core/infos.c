/*
 *  This file contains work-arounds for many known SD/MMC
 *  and SDIO hardware bugs.
 *
 *  Copyright (c) 2011 Andrei Warkentin <andreiw@motorola.com>
 *  Copyright (c) 2011 Pierre Tardy <tardyp@gmail.com>
 *  Inspired from pci fixup code:
 *  Copyright (c) 1999 Martin Mares <mj@ucw.cz>
 *
 */

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/export.h>
#include <linux/mmc/infos.h>

#define SD_CID_MANFID_SANDISK	0x03
#define SD_CID_MANFID_TOSHIBA	0x02
#define SD_CID_MANFID_KINGSTON	0x41

#define SD_CID_MANFID_ANY (-1u)
#define SD_CID_OEMID_ANY ((unsigned short) -1)
#define SD_CID_NAME_ANY (NULL)

#define END_INFO { NULL }

#define _INFO_EXT(_name, _manfid, _oemid, \
			_rev_start, _rev_end,	\
			_info, _data)				\
	{						   \
		.name = (_name),			   \
		.manfid = (_manfid),			   \
		.oemid = (_oemid),			   \
		.rev_start = (_rev_start),		   \
		.rev_end = (_rev_end),			   \
		.vendor_info = (_info),		   \
		.data = (_data),			   \
	 }

#define MMC_INFO_REV(_name, _manfid, _oemid, \
			_rev_start, _rev_end,	\
		      _info, _data)			\
	_INFO_EXT(_name, _manfid, _oemid, _rev_start, _rev_end, _info, _data)

#define MMC_INFO(_name, _manfid, _oemid, \
			_info, _data) \
	MMC_INFO_REV(_name, _manfid, _oemid, 0, -1ull, _info, _data)

#define cid_rev(hwrev, fwrev, year, month)	\
	(((u64) hwrev) << 40 |                  \
	 ((u64) fwrev) << 32 |                  \
	 ((u64) year) << 16 |                   \
	 ((u64) month))

#define cid_rev_card(card)		  \
	cid_rev(card->cid.hwrev,	  \
		    card->cid.fwrev,      \
		    card->cid.year,	  \
		    card->cid.month)

/*
 * This hook just print for all sd devices
 */
static void print_sd_info(struct mmc_card *card, const char *data)
{
	pr_info("%s: [0x%02x:0x%04x] %s, made at %02d/%04d",
					mmc_hostname(card->host),
					card->cid.manfid, card->cid.oemid,
					data, mmc_card_month(card), mmc_card_year(card));
}

static const struct mmc_info mmc_device_infos[] = {
	MMC_INFO("SU04G", SD_CID_MANFID_SANDISK, SD_CID_OEMID_ANY, print_sd_info,
		  "Sandisk 4GB"),
	MMC_INFO("SU08G", SD_CID_MANFID_SANDISK, SD_CID_OEMID_ANY, print_sd_info,
		  "Sandisk 8GB"),
	MMC_INFO("SU16G", SD_CID_MANFID_SANDISK, SD_CID_OEMID_ANY, print_sd_info,
		  "Sandisk 16GB"),
	MMC_INFO("SL64G", SD_CID_MANFID_SANDISK, SD_CID_OEMID_ANY, print_sd_info,
		  "Sandisk 64GB"),
	MMC_INFO("SL128", SD_CID_MANFID_SANDISK, SD_CID_OEMID_ANY, print_sd_info,
		  "Sandisk 128GB"),
	MMC_INFO("SL200", SD_CID_MANFID_SANDISK, SD_CID_OEMID_ANY, print_sd_info,
		  "Sandisk 200GB"),
	MMC_INFO(SD_CID_NAME_ANY, SD_CID_MANFID_SANDISK, SD_CID_OEMID_ANY, print_sd_info,
		  "Sandisk"),

	MMC_INFO("SD64G", 0x27, SD_CID_OEMID_ANY, print_sd_info,
		  "Kingston 64GB"),
	MMC_INFO("SD16G", SD_CID_MANFID_KINGSTON, SD_CID_OEMID_ANY, print_sd_info,
		  "Kingston 16GB"),
	MMC_INFO("SD32G", SD_CID_MANFID_KINGSTON, SD_CID_OEMID_ANY, print_sd_info,
		  "Kingston 32GB"),
	MMC_INFO("SD128", SD_CID_MANFID_KINGSTON, SD_CID_OEMID_ANY, print_sd_info,
		  "Kingston 128GB"),
	MMC_INFO(SD_CID_NAME_ANY, SD_CID_MANFID_KINGSTON, SD_CID_OEMID_ANY, print_sd_info,
		  "Kingston"),

	MMC_INFO("SD08G", SD_CID_MANFID_TOSHIBA, SD_CID_OEMID_ANY, print_sd_info,
		  "Toshiba 8GB"),
	MMC_INFO("SA16G", SD_CID_MANFID_TOSHIBA, SD_CID_OEMID_ANY, print_sd_info,
		  "Toshiba 16GB"),
	MMC_INFO("SD32G", SD_CID_MANFID_TOSHIBA, SD_CID_OEMID_ANY, print_sd_info,
		  "Toshiba 32GB"),
	MMC_INFO("SA64G", SD_CID_MANFID_TOSHIBA, SD_CID_OEMID_ANY, print_sd_info,
		  "Toshiba 64GB"),
	MMC_INFO(SD_CID_NAME_ANY, SD_CID_MANFID_TOSHIBA, SD_CID_OEMID_ANY, print_sd_info,
		  "Toshiba"),

	MMC_INFO(SD_CID_NAME_ANY, 0x1b, SD_CID_OEMID_ANY, print_sd_info,
		  "Samsung"),
	MMC_INFO(SD_CID_NAME_ANY, 0X28, SD_CID_OEMID_ANY, print_sd_info,
		  "Lexar"),
	MMC_INFO(SD_CID_NAME_ANY, 0x9c, SD_CID_OEMID_ANY, print_sd_info,
		  "ZTE"),
	MMC_INFO(SD_CID_NAME_ANY, 0x82, SD_CID_OEMID_ANY, print_sd_info,
		  "SONY"),
	MMC_INFO(SD_CID_NAME_ANY, 0x1d, SD_CID_OEMID_ANY, print_sd_info,
		  "ADATA"),
	MMC_INFO(SD_CID_NAME_ANY, 0x74, SD_CID_OEMID_ANY, print_sd_info,
		  "Transcend"),
	MMC_INFO(SD_CID_NAME_ANY, 0x13, SD_CID_OEMID_ANY, print_sd_info,
		  "KINGMAX"),

	END_INFO
};

void mmc_device_info(struct mmc_card *card, const struct mmc_info *table)
{
	const struct mmc_info *f;
	u64 rev = cid_rev_card(card);

	/*Only print SD card info*/
	if (!mmc_card_sd(card))
		return;

	/* Non-core specific workarounds. */
	if (!table)
		table = mmc_device_infos;

	for (f = table; f->vendor_info; f++) {
		if ((f->manfid == SD_CID_MANFID_ANY ||
		     f->manfid == card->cid.manfid) &&
		    (f->oemid == SD_CID_OEMID_ANY ||
		     f->oemid == card->cid.oemid) &&
		    (f->name == SD_CID_NAME_ANY ||
		     !strncmp(f->name, card->cid.prod_name,
			      sizeof(card->cid.prod_name))) &&
		    rev >= f->rev_start && rev <= f->rev_end) {
			f->vendor_info(card, f->data);
			return;
		}
	}

	pr_info("%s: [0x%02x:0x%04x] , made at %02d/%04d", mmc_hostname(card->host),
								card->cid.manfid, card->cid.oemid,
								mmc_card_month(card), mmc_card_year(card));
}
EXPORT_SYMBOL(mmc_device_info);
