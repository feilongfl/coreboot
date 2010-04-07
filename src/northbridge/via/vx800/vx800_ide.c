/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2009 One Laptop per Child, Association, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 */

#include <device/device.h>
#include <device/pci.h>
#include <device/pci_ops.h>
#include <device/pci_ids.h>
#include <console/console.h>
#include "chip.h"
#include <arch/io.h>
#include "vx800.h"

static const u8 idedevicepcitable[16 * 12] = {
	/*
	0x02, 0x00, 0x00, 0x00, 0x00, 0x82, 0x00, 0x00,
	0x00, 0x00, 0xA8, 0xA8, 0xF0, 0x00, 0x00, 0xB6,
	0x00, 0x00, 0x01, 0x21, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x01, 0x09, 0xC4, 0x06, 0x11, 0x09, 0xC4,
	0x00, 0xC2, 0xF9, 0x01, 0x10, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x02, 0x01, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	*/

	0x02, 0x00, 0x00, 0x00, 0x00, 0x82, 0x00, 0x00,
	0x00, 0x00, 0x99, 0x20, 0xf0, 0x00, 0x00, 0x20,
	0x00, 0x00, 0x17, 0xF1, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x01, 0x09, 0xC4, 0x06, 0x11, 0x09, 0xC4,
	0x00, 0xc2, 0x09, 0x01, 0x10, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x02, 0x01, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

	/* Legacy BIOS XP PCI value */
	/*
	0x02, 0x00, 0x00, 0x00, 0x00, 0x82, 0x00, 0x00,
	0x00, 0x00, 0xa8, 0x20, 0x00, 0x00, 0x00, 0xb6,
	0x00, 0x00, 0x16, 0xF1, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x01, 0x09, 0xC4, 0x06, 0x11, 0x09, 0xC4,
	0x00, 0x02, 0x09, 0x00, 0x18, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x02, 0x01, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	*/

	/* ROM legacy BIOS on cn_8562b */
	/*
	0x03, 0x00, 0x00, 0x00, 0x00, 0x82, 0x00, 0x00,
	0x00, 0x00, 0x99, 0x20, 0x60, 0x00, 0x00, 0x20,
	0x00, 0x00, 0x1E, 0xF1, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x01, 0x09, 0xC4, 0x06, 0x11, 0x09, 0xC4,
	0x00, 0x02, 0x09, 0x01, 0x18, 0x0C, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x02, 0x01, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	*/

	/* From legacy BIOS on c7_8562b */
	/*
	0x03, 0x00, 0x00, 0x00, 0x00, 0x82, 0x00, 0x00,
	0x00, 0x00, 0x5E, 0x20, 0x60, 0x00, 0x00, 0xB6,
	0x00, 0x00, 0x1E, 0xF1, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x01, 0x09, 0xC4, 0x06, 0x11, 0x09, 0xC4,
	0x00, 0x02, 0x09, 0x01, 0x18, 0x0C, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x02, 0x01, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	*/
};

static void ide_init(struct device *dev)
{
	u8 i, data;
	printk(BIOS_INFO, "ide_init\n");

	/* these 3 lines help to keep interl back door for DID VID SUBID untouched */
	u16 data16_1, data16_2;
	data16_1 = pci_read_config16(dev, 0xba);
	data16_2 = pci_read_config16(dev, 0xbe);

	for (i = 0; i < (16 * 12); i++) {
		pci_write_config8(dev, 0x40 + i, idedevicepcitable[i]);
	}
	//pci_write_config8(dev, 0x0d, 0x20);
	data = pci_read_config8(dev, 0x0d);
	data &= 0x0f;
	data |= 0x40;
	pci_write_config8(dev, 0x0d, data);

	//these 2 lines help to keep interl back door for DID VID SUBID untouched
	pci_write_config16(dev, 0xba, data16_1);
	pci_write_config16(dev, 0xbe, data16_2);

	/* Force interrupts to use compat mode. */
	pci_write_config8(dev, PCI_INTERRUPT_PIN, 0x0);
	pci_write_config8(dev, PCI_INTERRUPT_LINE, 0xff);
#if 0

	struct southbridge_via_vt8237r_config *sb =
	    (struct southbridge_via_vt8237r_config *)dev->chip_info;

	u8 enables;
	u32 cablesel;

	pci_write_config16(dev, 0x04, 0x0007);

	enables = pci_read_config8(dev, IDE_CS) & ~0x3;
	enables |= 0x02;
	pci_write_config8(dev, IDE_CS, enables);
	enables = pci_read_config8(dev, IDE_CS);
	printk(BIOS_DEBUG, "Enables in reg 0x40 read back as 0x%x\n", enables);

	/* Enable only compatibility mode. */
	enables = pci_read_config8(dev, IDE_CONF_II);
	enables &= ~0xc0;
	pci_write_config8(dev, IDE_CONF_II, enables);
	enables = pci_read_config8(dev, IDE_CONF_II);
	printk(BIOS_DEBUG, "Enables in reg 0x42 read back as 0x%x\n", enables);

	/* Enable prefetch buffers. */
	enables = pci_read_config8(dev, IDE_CONF_I);
	enables |= 0xf0;
	pci_write_config8(dev, IDE_CONF_I, enables);

	/* Flush FIFOs at half. */
	enables = pci_read_config8(dev, IDE_CONF_FIFO);
	enables &= 0xf0;
	enables |= (1 << 2) | (1 << 0);
	pci_write_config8(dev, IDE_CONF_FIFO, enables);

	/* PIO read prefetch counter, Bus Master IDE Status Reg. Read Retry. */
	enables = pci_read_config8(dev, IDE_MISC_I);
	enables &= 0xe2;
	enables |= (1 << 4) | (1 << 3);
	pci_write_config8(dev, IDE_MISC_I, enables);

	/* Use memory read multiple, Memory-Write-and-Invalidate. */
	enables = pci_read_config8(dev, IDE_MISC_II);
	enables |= (1 << 2) | (1 << 3);
	pci_write_config8(dev, IDE_MISC_II, enables);

	/* Force interrupts to use compat mode. */
	pci_write_config8(dev, PCI_INTERRUPT_PIN, 0x0);
	pci_write_config8(dev, PCI_INTERRUPT_LINE, 0xff);

	/* Cable guy... */
	cablesel = pci_read_config32(dev, IDE_UDMA);
	cablesel &= ~((1 << 28) | (1 << 20) | (1 << 12) | (1 << 4));
	cablesel |= (sb->ide0_80pin_cable << 28) |
	    (sb->ide0_80pin_cable << 20) |
	    (sb->ide1_80pin_cable << 12) | (sb->ide1_80pin_cable << 4);
	pci_write_config32(dev, IDE_UDMA, cablesel);
#endif
}

static struct device_operations ide_ops = {
	.read_resources = pci_dev_read_resources,
	.set_resources = pci_dev_set_resources,
	.enable_resources = pci_dev_enable_resources,
	.init = ide_init,
	.enable = 0,
	.ops_pci = 0,
};

static const struct pci_driver via_ide_driver __pci_driver = {
	.ops = &ide_ops,
	.vendor = PCI_VENDOR_ID_VIA,
	.device = PCI_DEVICE_ID_VIA_VX855_IDE,
};
