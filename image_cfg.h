//------------------------------------------------------------------------------
//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft end-user
// license agreement (EULA) under which you licensed this SOFTWARE PRODUCT.
// If you did not accept the terms of the EULA, you are not authorized to use
// this source code. For a copy of the EULA, please see the LICENSE.RTF on your
// install media.
//
//------------------------------------------------------------------------------
//
//  Copyright (C) 2007-2010, Freescale Semiconductor, Inc. All Rights Reserved.
//  THIS SOURCE CODE, AND ITS USE AND DISTRIBUTION, IS SUBJECT TO THE TERMS
//  AND CONDITIONS OF THE APPLICABLE LICENSE AGREEMENT
//
//------------------------------------------------------------------------------
//
//  File:  image_cfg.h
//
//  Defines configuration parameters used to create the NK and Bootloader
//  program images.
//
//------------------------------------------------------------------------------
#ifndef __IMAGE_CFG_H
#define __IMAGE_CFG_H

#include "mx28_nandlayout.h"

#if __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------
//  RESTRICTION
//
//  This file is a configuration file. It should ONLY contain simple #define
//  directives defining constants. This file is included by other files that
//  only support simple substitutions.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  NAMING CONVENTION
//
//  The IMAGE_ naming convention ...
//
//  IMAGE_<NAME>_<SECTION>_<MEMORY_DEVICE>_[OFFSET|SIZE|START|END]
//
//      <NAME>          - WINCE, BOOT, SHARE
//      <SECTION>       - section name: user defined
//      <MEMORY_DEVICE> - the memory device the block resides on
//      OFFSET          - number of bytes from memory device start address
//      SIZE            - maximum size of the block
//      START           - start address of block    (device address + offset)
//      END             - end address of block      (start address  + size - 1)
//
//------------------------------------------------------------------------------
#define IMAGE_BOOT_RAM_PA_START         CSP_BASE_MEM_PA_SDRAM


#define IMAGE_WINCE_CODE_RAM_CA_START   ((DWORD)OALPAtoVA(IMAGE_BOOT_RAM_PA_START, TRUE) + 0x00200000)


//------------------------------------------------------------------------------
//// Internal RAM Mapping (128 KB)
#define IMAGE_WINCE_IRAM_PA_START       CSP_BASE_MEM_PA_IRAM
#define IMAGE_WINCE_IRAM_SIZE           (128 * 1024)

// 4K bytes reserved for USB Transfer
#define IMAGE_WINCE_USB_IRAM_OFFSET     (0)
#define IMAGE_WINCE_USB_IRAM_PA_START   (IMAGE_WINCE_IRAM_PA_START+IMAGE_WINCE_USB_IRAM_OFFSET)
#define IMAGE_WINCE_USB_IRAM_SIZE       (4 * 1024)

// 4K bytes reserved for SD/MMC Transfer
#define IMAGE_WINCE_SDMMC_IRAM_OFFSET   (IMAGE_WINCE_USB_IRAM_OFFSET + IMAGE_WINCE_USB_IRAM_SIZE)
#define IMAGE_WINCE_SDMMC_IRAM_PA_START (IMAGE_WINCE_IRAM_PA_START + IMAGE_WINCE_SDMMC_IRAM_OFFSET)
#define IMAGE_WINCE_SDMMC_IRAM_SIZE     (4 * 1024)

// 12K bytes reserved for NAND Transfer
#define IMAGE_WINCE_NAND_IRAM_OFFSET   (IMAGE_WINCE_SDMMC_IRAM_OFFSET + IMAGE_WINCE_SDMMC_IRAM_SIZE)
#define IMAGE_WINCE_NAND_IRAM_PA_START (IMAGE_WINCE_IRAM_PA_START + IMAGE_WINCE_NAND_IRAM_OFFSET)
#define IMAGE_WINCE_NAND_IRAM_SIZE     (12 * 1024)

// 1K bytes reserved for POWER 
#define IMAGE_WINCE_POWEROFF_IRAM_OFFSET    (IMAGE_WINCE_NAND_IRAM_OFFSET + IMAGE_WINCE_NAND_IRAM_SIZE)
#define IMAGE_WINCE_POWEROFF_IRAM_PA_START  (IMAGE_WINCE_IRAM_PA_START + IMAGE_WINCE_POWEROFF_IRAM_OFFSET)
#define IMAGE_WINCE_POWEROFF_IRAM_SIZE      (1 * 1024)

// 1K bytes reserved for AUDIO DMA Descriptor
#define IMAGE_WINCE_AUDIO_DESCRIPTOR_IRAM_OFFSET    (IMAGE_WINCE_POWEROFF_IRAM_OFFSET + IMAGE_WINCE_POWEROFF_IRAM_SIZE)
#define IMAGE_WINCE_AUDIO_DESCRIPTOR_IRAM_PA_START  (IMAGE_WINCE_IRAM_PA_START + IMAGE_WINCE_AUDIO_DESCRIPTOR_IRAM_OFFSET)
#define IMAGE_WINCE_AUDIO_DESCRIPTOR_IRAM_SIZE      (1 * 1024)

// 8K bytes reserved for AUDIO DMA Buffer
#define IMAGE_WINCE_AUDIO_IRAM_OFFSET    (IMAGE_WINCE_AUDIO_DESCRIPTOR_IRAM_OFFSET + IMAGE_WINCE_AUDIO_DESCRIPTOR_IRAM_SIZE)
#define IMAGE_WINCE_AUDIO_IRAM_PA_START  (IMAGE_WINCE_IRAM_PA_START + IMAGE_WINCE_AUDIO_IRAM_OFFSET)
#define IMAGE_WINCE_AUDIO_IRAM_SIZE      (8 * 1024)

// 4 K bytes reserved for DVFC 
#define IMAGE_WINCE_DVFC_IRAM_OFFSET    (IMAGE_WINCE_AUDIO_IRAM_OFFSET + IMAGE_WINCE_AUDIO_IRAM_SIZE)
#define IMAGE_WINCE_DVFC_IRAM_PA_START  (IMAGE_WINCE_IRAM_PA_START + IMAGE_WINCE_DVFC_IRAM_OFFSET)
#define IMAGE_WINCE_DVFC_IRAM_DATA_SIZE   (2*1024)
#define IMAGE_WINCE_DVFC_IRAM_FUNC_SIZE   (2*1024)
#define IMAGE_WINCE_DVFC_IRAM_SIZE      (IMAGE_WINCE_DVFC_IRAM_FUNC_SIZE + IMAGE_WINCE_DVFC_IRAM_DATA_SIZE )

// 1 K bytes reserved for Debug 
#define IMAGE_WINCE_DEBUG_IRAM_OFFSET    (IMAGE_WINCE_DVFC_IRAM_OFFSET + IMAGE_WINCE_DVFC_IRAM_SIZE)
#define IMAGE_WINCE_DEBUG_IRAM_PA_START  (IMAGE_WINCE_IRAM_PA_START + IMAGE_WINCE_DEBUG_IRAM_OFFSET)
#define IMAGE_WINCE_DEBUG_IRAM_SIZE      (1 * 1024)



//------------------------------------------------------------------------------
// RAM image defines
#define IMAGE_BOOT_RAMDEV_RAM_PA_START      (IMAGE_BOOT_RAM_PA_START)
#define IMAGE_BOOT_RAMDEV_RAM_SIZE          (128*1024*1024) // 128 MB
#define IMAGE_BOOT_RAMDEV_RAM_PA_END        (IMAGE_BOOT_RAMDEV_RAM_PA_START+IMAGE_BOOT_RAMDEV_RAM_SIZE-1)

// Stack space for boot procedure
#define IMAGE_BOOT_STACK_RAM_OFFSET         (0x3000000)
#define IMAGE_BOOT_STACK_RAM_START          (IMAGE_BOOT_RAMDEV_RAM_PA_START+IMAGE_BOOT_STACK_RAM_OFFSET)
#define IMAGE_BOOT_STACK_RAM_SIZE           (64*1024)

// RAM used during boot process
#define IMAGE_BOOT_SPARE_RAM_OFFSET         (IMAGE_BOOT_STACK_RAM_OFFSET+IMAGE_BOOT_STACK_RAM_SIZE)
#define IMAGE_BOOT_SPARE_RAM_PA_START       (IMAGE_BOOT_RAMDEV_RAM_PA_START+IMAGE_BOOT_SPARE_RAM_OFFSET)
#define IMAGE_BOOT_SPARE_RAM_SIZE           (256*1024)

//EBOOT image
#define IMAGE_BOOT_BOOTIMAGE_RAM_OFFSET     (IMAGE_BOOT_SPARE_RAM_OFFSET+IMAGE_BOOT_SPARE_RAM_SIZE)
#define IMAGE_BOOT_BOOTIMAGE_RAM_PA_START   (IMAGE_BOOT_RAMDEV_RAM_PA_START+IMAGE_BOOT_BOOTIMAGE_RAM_OFFSET)
#define IMAGE_BOOT_BOOTIMAGE_RAM_SIZE       (256*1024)
#define IMAGE_BOOT_BOOTIMAGE_RAM_PA_END     (IMAGE_BOOT_BOOTIMAGE_RAM_PA_START+IMAGE_BOOT_BOOTIMAGE_RAM_SIZE-1)

//NAND cache region
#define IMAGE_BOOT_NANDCACHE_RAM_OFFSET     (IMAGE_BOOT_BOOTIMAGE_RAM_OFFSET)
#define IMAGE_BOOT_NANDCACHE_RAM_START      (IMAGE_BOOT_BOOTIMAGE_RAM_PA_START)
#define IMAGE_BOOT_NANDCACHE_RAM_SIZE       (IMAGE_BOOT_BOOTIMAGE_RAM_SIZE)

//BOOT Page table
#define IMAGE_BOOT_BOOTPT_RAM_OFFSET        (IMAGE_BOOT_NANDCACHE_RAM_OFFSET+IMAGE_BOOT_NANDCACHE_RAM_SIZE)
#define IMAGE_BOOT_BOOTPT_RAM_PA_START      (IMAGE_BOOT_RAMDEV_RAM_PA_START+IMAGE_BOOT_BOOTPT_RAM_OFFSET)
#define IMAGE_BOOT_BOOTPT_RAM_SIZE          (16*1024)
#define IMAGE_BOOT_BOOTPT_RAM_PA_END        (IMAGE_BOOT_BOOTPT_RAM_PA_START+IMAGE_BOOT_BOOTPT_RAM_SIZE-1)

#define IMAGE_BOOT_ENET_RAM_PA_START       (IMAGE_BOOT_BOOTPT_RAM_PA_START+IMAGE_BOOT_BOOTPT_RAM_SIZE)
#define IMAGE_BOOT_ENET_RAM_SIZE           (16*1024)

// DDK CLK
#define IMAGE_WINCE_DDKCLK_RAM_OFFSET       (0)
#define IMAGE_WINCE_DDKCLK_RAM_PA_START     (IMAGE_BOOT_RAMDEV_RAM_PA_START)
#define IMAGE_WINCE_DDKCLK_RAM_UA_START     (OALPAtoVA(IMAGE_WINCE_DDKCLK_RAM_PA_START, FALSE))
#define IMAGE_WINCE_DDKCLK_RAM_SIZE         (16*1024)



// Share args
#define IMAGE_SHARE_ARGS_RAM_OFFSET         (0x70000)
#define IMAGE_SHARE_ARGS_RAM_PA_START       (IMAGE_BOOT_RAMDEV_RAM_PA_START+IMAGE_SHARE_ARGS_RAM_OFFSET)
#define IMAGE_SHARE_ARGS_UA_START           (OALPAtoVA(IMAGE_SHARE_ARGS_RAM_PA_START, FALSE))
#define IMAGE_SHARE_ARGS_RAM_SIZE           (0x88000)


#define KEYPAD_DATA_BASE			(IMAGE_SHARE_ARGS_RAM_PA_START+0x80000)

// USB KITL transfer
#define IMAGE_USB_KITL_RAM_OFFSET           (IMAGE_SHARE_ARGS_RAM_OFFSET+IMAGE_SHARE_ARGS_RAM_SIZE)
#define IMAGE_USB_KITL_RAM_PA_START         (IMAGE_WINCE_USB_IRAM_PA_START)
#define IMAGE_USB_KITL_RAM_UA_START         (OALPAtoVA(IMAGE_USB_KITL_RAM_PA_START, FALSE))
#define IMAGE_USB_KITL_RAM_SIZE             (32*1024)

// Display Frame buffer
#define IMAGE_WINCE_DISPLAY_RAM_OFFSET      (IMAGE_USB_KITL_RAM_OFFSET+IMAGE_USB_KITL_RAM_SIZE)
#define IMAGE_WINCE_DISPLAY_RAM_PA_START    (IMAGE_BOOT_RAMDEV_RAM_PA_START+IMAGE_WINCE_DISPLAY_RAM_OFFSET)
#define IMAGE_WINCE_DISPLAY_RAM_SIZE        (1008*1024)

#define IMAGE_SHARE_ENET_RAM_PA_START       (IMAGE_WINCE_DISPLAY_RAM_PA_START+IMAGE_WINCE_DISPLAY_RAM_SIZE)
#define IMAGE_SHARE_ENET_RAM_SIZE           (16*1024)




// Run-time image memory
#define IMAGE_BOOT_NKIMAGE_RAM_OFFSET       (0x200000)
#define IMAGE_BOOT_NKIMAGE_RAM_PA_START     (IMAGE_BOOT_RAMDEV_RAM_PA_START+IMAGE_BOOT_NKIMAGE_RAM_OFFSET)
#define IMAGE_BOOT_NKIMAGE_RAM_SIZE         (IMAGE_BOOT_RAMDEV_RAM_SIZE-IMAGE_BOOT_NKIMAGE_RAM_OFFSET)
#define IMAGE_BOOT_NKIMAGE_RAM_PA_END       (IMAGE_BOOT_NKIMAGE_RAM_PA_START+IMAGE_BOOT_NKIMAGE_RAM_SIZE-1)



//
//
//------------------------------------------------------------------------------
//// NAND flash image defines
//
//// NOTE:  EBOOT assumes these NAND regions are block-aligned
//
//// Define a special unused SOC address range that can be used to detect when an
//// image is destined for NAND flash
#define IMAGE_BOOT_NANDDEV_NAND_PA_START    (IMAGE_BOOT_RAMDEV_RAM_PA_START+IMAGE_BOOT_NKIMAGE_RAM_OFFSET)
#define IMAGE_BOOT_NANDDEV_RESERVED_SIZE    (DWORD)(IMAGE_BOOT_FILESYS_NAND_OFFSET*128*1024)
#define IMAGE_BOOT_NANDDEV_NAND_PA_END      (IMAGE_BOOT_NANDDEV_NAND_PA_START+IMAGE_BOOT_NANDDEV_RESERVED_SIZE-1)

#define PLATFORM_BCB_SECTOR_SEARCH_RANGE    (256)
//
// NCB 0~3 
// Boot Control Block
#define IMAGE_BOOT_NCB_NAND_OFFSET          (CHIP_NCB_NAND_OFFSET)
#define IMAGE_BOOT_NCB_NAND_BLOCKS          (CHIP_NCB_NAND_BLOCKS)

// BAD block table
//BBT 4~9
//IMAGE_BBT_NAND_OFFSET  --4
#define IMAGE_BBT_NAND_OFFSET               (IMAGE_BOOT_NCB_NAND_OFFSET + IMAGE_BOOT_NCB_NAND_BLOCKS)
#define IMAGE_BBT_NAND_BLOCKS               (6)

// Bad block table used by rom
//DBBT  10~13
//IMAGE_BOOT_DBBT_NAND_OFFSET 8
#define IMAGE_BOOT_DBBT_NAND_OFFSET         (IMAGE_BBT_NAND_OFFSET + IMAGE_BBT_NAND_BLOCKS)
#define IMAGE_BOOT_DBBT_NAND_BLOCKS         (4)

// EBOOT image 14~21
#define IMAGE_BOOT_BOOTIMAGE_NAND_OFFSET    (IMAGE_BOOT_DBBT_NAND_OFFSET + IMAGE_BOOT_DBBT_NAND_BLOCKS)
#define IMAGE_BOOT_BOOTIMAGE_NAND_BLOCKS     (8)

// Boot configuration-TCB 22~27
#define IMAGE_BOOT_BOOTCFG_NAND_OFFSET      (IMAGE_BOOT_BOOTIMAGE_NAND_OFFSET + IMAGE_BOOT_BOOTIMAGE_NAND_BLOCKS)
#define IMAGE_BOOT_BOOTCFG_NAND_BLOCKS       (6)

// Boot configuration-VCB 28~30
#define IMAGE_BOOT_VECTCFG_NAND_OFFSET      (IMAGE_BOOT_BOOTCFG_NAND_OFFSET + IMAGE_BOOT_BOOTCFG_NAND_BLOCKS)
#define IMAGE_BOOT_VECTCFG_NAND_BLOCKS       (3)

// UUID 31~32
#define IMAGE_BOOT_UUID_NAND_OFFSET         (IMAGE_BOOT_VECTCFG_NAND_OFFSET+IMAGE_BOOT_VECTCFG_NAND_BLOCKS)
#define IMAGE_BOOT_UUID_NAND_BLOCKS          (2)

// Rescue OS 33~99
#define IMAGE_BOOT_RESCUEIMG_NAND_OFFSET      (IMAGE_BOOT_UUID_NAND_OFFSET+IMAGE_BOOT_UUID_NAND_BLOCKS)
#define IMAGE_BOOT_RESCUEIMG_NAND_BLOCKS      (67)

// NK image 100~339
#define IMAGE_BOOT_NKIMAGE_NAND_OFFSET      (IMAGE_BOOT_RESCUEIMG_NAND_OFFSET+IMAGE_BOOT_RESCUEIMG_NAND_BLOCKS)
#define IMAGE_BOOT_NKIMAGE_NAND_SIZE        (30 * 1024 * 1024)
#define IMAGE_BOOT_NKIMAGE_BLOCKS        	(240)

#define IMAGE_BOOT_FILESYS_NAND_OFFSET		(IMAGE_BOOT_NKIMAGE_NAND_OFFSET+IMAGE_BOOT_NKIMAGE_BLOCKS)
// IMG, the address is only for download memory verification
// this configuration should be aligned with g_oalAddressTable
// or OALVATOPA will fail to get the virtual address from physical address

// first 1M is reserved for RAM image
#define IMAGE_BOOT_BOOTIMAGE_NAND_PA_START  (IMAGE_BOOT_NANDDEV_NAND_PA_START + 1 * 1024 * 1024)
#define IMAGE_BOOT_BOOTIMAGE_NAND_PA_END    (IMAGE_BOOT_BOOTIMAGE_NAND_PA_START + 63 * 1024 * 1024 -1U)
#define IMAGE_BOOT_NKIMAGE_NAND_PA_START    (IMAGE_BOOT_BOOTIMAGE_NAND_PA_END + 1)
#define IMAGE_BOOT_NKIMAGE_NAND_PA_END      (IMAGE_BOOT_NKIMAGE_NAND_PA_START + 63 * 1024 * 1024 - 1U)

// NOR Flash BOOT reserved
#define IMAGE_BOOT_BOOTIMAGE_NOR_PA_START   (0x60000000)
#define IMAGE_BOOT_BOOTIMAGE_NOR_SIZE       (1 * 1024 * 1024)
#define IMAGE_BOOT_BOOTIMAGE_NOR_PA_END     (IMAGE_BOOT_BOOTIMAGE_NOR_PA_START + IMAGE_BOOT_BOOTIMAGE_NOR_SIZE - 1U)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// SDHC flash image defines

// Define a special unused SOC address range that can be used to detect when an
// image is destined for SD/MMC Memory
#define IMAGE_BOOT_BOOTIMAGE_SD_PA_START     (0x80000000)
#define IMAGE_BOOT_BOOTIMAGE_SD_SIZE         (64*1024*1024)
#define IMAGE_BOOT_BOOTIMAGE_SD_PA_END       (IMAGE_BOOT_BOOTIMAGE_SD_PA_START+IMAGE_BOOT_BOOTIMAGE_SD_SIZE-1)

#define IMAGE_BOOT_NKIMAGE_SD_OFFSET       (125*1024*1024) 
#define IMAGE_BOOT_NKIMAGE_SD_PA_START     (IMAGE_BOOT_RAMDEV_RAM_PA_START+IMAGE_BOOT_NKIMAGE_RAM_OFFSET+IMAGE_BOOT_NKIMAGE_SD_OFFSET)
#define IMAGE_BOOT_NKIMAGE_SD_SIZE         (32*1024*1024)
#define IMAGE_BOOT_NKIMAGE_SD_PA_END       (IMAGE_BOOT_NKIMAGE_SD_PA_START+IMAGE_BOOT_NKIMAGE_SD_SIZE-1)

#if __cplusplus
}
#endif

#endif
