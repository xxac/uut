//-----------------------------------------------------------------------------
//
//  Copyright (c) Microsoft Corporation.  All rights reserved.
//
//  Use of this source code is subject to the terms of the Microsoft end-user
//  license agreement (EULA) under which you licensed this SOFTWARE PRODUCT.
//  If you did not accept the terms of the EULA, you are not authorized to use
//  this source code. For a copy of the EULA, please see the LICENSE.RTF on
//  your install media.
//
//-----------------------------------------------------------------------------
//
//  Copyright (C) 2007-2010, Freescale Semiconductor, Inc. All Rights Reserved.
//  THIS SOURCE CODE, AND ITS USE AND DISTRIBUTION, IS SUBJECT TO THE TERMS
//  AND CONDITIONS OF THE APPLICABLE LICENSE AGREEMENT
//
//-----------------------------------------------------------------------------
//
//  File:  nand.c
//
//  Contains BOOT NAND flash support functions.
//
//-----------------------------------------------------------------------------
#pragma warning(push)
#pragma warning(disable: 4115 4201 4204 4214)
#include "bsp.h"
#include "loader.h"
#include "nandboot.h"
#include "bcb.h"
#include "otp.h"
#include "saio.h"
#pragma warning(pop)
//-----------------------------------------------------------------------------
// External Functions

//-----------------------------------------------------------------------------
// External Variables
extern FlashInfoExt g_CSPfi;

static NANDImgInfo g_ImgInfo;

//-----------------------------------------------------------------------------
// Defines
#define NANDFC_BOOT_SIZE (4096*8)

//-----------------------------------------------------------------------------
// Types

//-----------------------------------------------------------------------------
// Global Variables
NAND_IMAGE_CFG NANDImageCfg;
SECTOR_BUFFER SectorBuffer;
//-----------------------------------------------------------------------------
// Local Variables
BYTE sectorBuf[NANDFC_BOOT_SIZE];

//-----------------------------------------------------------------------------
// Local Functions
VOID NANDGetImgInfo(PNANDImgInfo pInfo);

//------------------------------------------------------------------------------
//
// Function: BSP_GetNANDBufferPointer
//
//    Get the Sector Buffer pointer
//
// Parameters:
//        pSectorBuffer[out] - buffer parameters
//
// Returns:
//        the buffer parameters
//
//------------------------------------------------------------------------------
void BSP_GetNANDBufferPointer(PSECTOR_BUFFER pSectorBuffer)
{   
    pSectorBuffer->pSectorBuf = sectorBuf;
    pSectorBuffer->dwSectorBufSize = NANDFC_BOOT_SIZE;
}

//------------------------------------------------------------------------------
//
// Function: BSP_GetNANDImageCfg
//
//    Get the image parameters
//
// Parameters:
//        pNANDImageCfg[out] - image parameters
//
// Returns:
//        the image parameters
//
//------------------------------------------------------------------------------
void BSP_GetNANDImageCfg(PNAND_IMAGE_CFG pNANDImageCfg)
{
    FlashInfo flashInfo;
    
    if (!FMD_GetInfo(&flashInfo))
    {
        RETAILMSG(TRUE, (_T("ERROR: Unable to get NAND flash information.\r\n")));
        return;
    }
    
    pNANDImageCfg->dwXldrOffset = 0xFFFFFFFF;
    pNANDImageCfg->dwXldrSize = 0;

    pNANDImageCfg->dwBootOffset = IMAGE_BOOT_BOOTIMAGE_NAND_OFFSET * flashInfo.dwBytesPerBlock;
    pNANDImageCfg->dwBootSize = IMAGE_BOOT_BOOTIMAGE_NAND_BLOCKS * flashInfo.dwBytesPerBlock;

    pNANDImageCfg->dwIplOffset = 0xFFFFFFFF;
    pNANDImageCfg->dwIplSize = 0;
    
    pNANDImageCfg->dwNkOffset = IMAGE_BOOT_NKIMAGE_NAND_OFFSET * flashInfo.dwBytesPerBlock;
    pNANDImageCfg->dwNkSize = IMAGE_BOOT_NKIMAGE_NAND_SIZE;

    pNANDImageCfg->dwDioOffset = 0xFFFFFFFF;
    pNANDImageCfg->dwDioSize = 0;
    
    pNANDImageCfg->dwCfgOffset = IMAGE_BOOT_BOOTCFG_NAND_OFFSET * flashInfo.dwBytesPerBlock;
    pNANDImageCfg->dwCfgSize = IMAGE_BOOT_BOOTCFG_NAND_BLOCKS * flashInfo.dwBytesPerBlock;

    pNANDImageCfg->dwIplRamStart = 0xFFFFFFFF;
    //pNANDImageCfg->dwNkRamStart = IMAGE_BOOT_NKIMAGE_RAM_PA_START;
    pNANDImageCfg->dwNandSize = IMAGE_BOOT_NANDDEV_RESERVED_SIZE;
}

//-----------------------------------------------------------------------------
//
//  Function:  NANDBootInit
//
//      This function reads the image configuration and sector buffer.
//
//  Parameters:
//      None.        
//
//  Returns:
//      None.
//
//-----------------------------------------------------------------------------
static BOOL NANDBootInit()
{ 
    // Get the Image Cfg
    BSP_GetNANDImageCfg(&NANDImageCfg);
    // Get the Sector Buffer
    BSP_GetNANDBufferPointer(&SectorBuffer);
    if(SectorBuffer.pSectorBuf == NULL)
    {
        return FALSE;
    }

    if(SectorBuffer.dwSectorBufSize == 0)
    {
        return FALSE;
    }
    
    memset(SectorBuffer.pSectorBuf, 0xFF, SectorBuffer.dwSectorBufSize);
    NANDGetImgInfo(&g_ImgInfo);
    return TRUE;
}

//-----------------------------------------------------------------------------
//
//  Function:  NANDBootReserved
//
//      This function reserves the blocks of the image area.
//
//  Parameters:
//      None.        
//
//  Returns:
//      None.
//
//-----------------------------------------------------------------------------
void NANDBootReserved()
{
    DWORD dwResult;
    FlashInfo flashInfo;
    BLOCK_ID blockID, startBlockID, endBlockID;
    
    if(!FMD_GetInfo(&flashInfo))
    {
        RETAILMSG(TRUE, (_T("ERROR: Unable to get NAND flash information.\r\n")));
        return;
    }
    
    // Boot Configuration and Buffer
    if(!NANDBootInit())
    {
        RETAILMSG(TRUE, (_T("WARNING: NANDBootInit failed.\r\n")));
        return ;
    }
    
    startBlockID = 0;
    endBlockID = (DWORD)(NANDImageCfg.dwNandSize + flashInfo.dwBytesPerBlock - 1) / (DWORD)flashInfo.dwBytesPerBlock;
    RETAILMSG(TRUE, (_T("INFO: Set NAND flash blocks [0x%x ~ 0x%x] as reserved.\r\n"), startBlockID, endBlockID-1));        

    for(blockID = startBlockID; blockID < endBlockID; blockID++)
    {
        dwResult = FMD_GetBlockStatus(blockID);
        
        // Skip bad blocks
        if(dwResult & BLOCK_STATUS_BAD)
        {
            //RETAILMSG(TRUE, (_T("INFO: Found bad NAND flash block [0x%x].\r\n", blockID);
            continue;
        }

        // Skip reserved blocks
        if(dwResult & BLOCK_STATUS_RESERVED)
        {
            continue;
        }
        
        // Erase the block...
        if(!FMD_EraseBlock(blockID))
        {
            //RETAILMSG(TRUE, (_T("ERROR: Unable to erase NAND flash block [0x%x].\r\n", blockID);
            FMD_SetBlockStatus(blockID, BLOCK_STATUS_BAD);
            continue;
        }
        
        //RETAILMSG(TRUE, (_T("Set Block #%d to be reserved.\r\n"), blockID));
        if(!FMD_SetBlockStatus(blockID, BLOCK_STATUS_RESERVED))
        {
            //RETAILMSG(TRUE, (_T("ERROR: Unable to set block status [0x%x].\r\n", blockID);
            continue;
        }
    }
}

//-----------------------------------------------------------------------------
//
//  Function:  NANDLowLevelFormat
//
//  This function formats (erases) the entire NAND flash memory without
//  checking the NAND flash block status.
//
//  Parameters:
//      None.     
//
//  Returns:
//      TRUE indicates success. FALSE indicates failure.
//
//-----------------------------------------------------------------------------
BOOL NANDLowLevelFormat(void)
{
    UINT32 blockID, startBlockID, endBlockID;
    UINT32 percentComplete, lastPercentComplete;
    FlashInfo flashInfo;

    if(!FMD_GetInfo(&flashInfo))
    {
        RETAILMSG(TRUE, (_T("ERROR: Unable to get NAND flash information.\r\n")));
        return FALSE;
    }

    // Calculate the physical block range for the enrire NAND device
    startBlockID = 0;
    endBlockID = flashInfo.dwNumBlocks;

    RETAILMSG(TRUE, (_T("INFO: Start erasing whole NAND space!\r\n")));

    lastPercentComplete = 0;

    for (blockID = startBlockID; blockID < endBlockID ; blockID++)
    {
        // Erase the block...
        //
        if (!FMD_EraseBlock(blockID))
        {
            //RETAILMSG(TRUE, (_T("ERROR: Unable to erase NAND flash block 0x%x.\r\n"), blockID));
            continue;
        }
        else
        {
            percentComplete = 100 * (blockID - startBlockID + 1) / (endBlockID - startBlockID);

            // If percentage complete has changed, show the progress
            if (lastPercentComplete != percentComplete)
            {
                lastPercentComplete = percentComplete;
                RETAILMSG(TRUE, (_T("\rINFO: Erasing is %d%% complete."), percentComplete));
            }
        }
    }
    
    RETAILMSG(TRUE, (_T("\r\nINFO: Erasing whole NAND space completed successfully.\r\n"))); 
    RETAILMSG(TRUE, (_T("INFO: Please power off the board.\r\n")));  
    RETAILMSG(TRUE, (_T("INFO: Spin Forever...\r\n"))); 
    //Spin forever
    while(endBlockID!=0)
    {
        endBlockID = lastPercentComplete;         
    }

    return(TRUE);
}


static BOOL GetGoodPhyBlock(DWORD LogBlockAddr, DWORD *pGoodBlkAddr)
{
    DWORD StartPhyBlockAddr = 0;
    DWORD i;
    FlashInfo flashInfo;
    
    if (!FMD_GetInfo(&flashInfo))
    {
        DEBUGMSG(TRUE, (_T("ERROR: Unable to get NAND flash information.\r\n")));
        return(FALSE);
    }
    
    if(LogBlockAddr >= flashInfo.dwNumBlocks)
    {
        DEBUGMSG(TRUE, (_T("ERROR: LogBlockAddr exceed flashInfo.dwNumBlocks.\r\n")));
        return(FALSE);
    }
    
    for(i = 0; i < LogBlockAddr + 1;)
    {
        if(StartPhyBlockAddr >= flashInfo.dwNumBlocks)
        {
            return FALSE;   
        }
        //check block status
        if(FMD_GetBlockStatus(StartPhyBlockAddr) != BLOCK_STATUS_BAD)
        {
            i++;
            if(i == LogBlockAddr + 1)
            {
                *pGoodBlkAddr = StartPhyBlockAddr;
                return TRUE;
            }
        }
        else
        {
            DEBUGMSG(TRUE, (_T(" skip bad block 0x%x\r\n"),StartPhyBlockAddr));
        }
        StartPhyBlockAddr++;
    }

    return FALSE;
}


BOOL GetNextGoodBlock(DWORD StartPhyBlockAddr, DWORD *pNxtGoodBlkAddr)
{
    DWORD BlockStatus;
    FlashInfo flashInfo;
    
    if (!FMD_GetInfo(&flashInfo))
    {
        RETAILMSG(TRUE, (_T("ERROR: Unable to get NAND flash information.\r\n")));
        return FALSE;
    }
    
    do
    {
        //We start with next block of StartPhyBlockAddr
        StartPhyBlockAddr++;

        //check border validation
        if(StartPhyBlockAddr >= flashInfo.dwNumBlocks)
        return FALSE;

        //check block status
        BlockStatus = FMD_GetBlockStatus(StartPhyBlockAddr);

    }while(BlockStatus == BLOCK_STATUS_BAD);

    //return good block address
    *pNxtGoodBlkAddr = StartPhyBlockAddr;

    return TRUE;
}


BOOL GetPhyBlkAddr(DWORD LogicalBlkAddr, DWORD *pNxtGoodBlkAddr)
{
    DWORD i;

    *pNxtGoodBlkAddr = 0;
    for(i=0; i < LogicalBlkAddr+1; i++)
    {
        if(i==0)
        {
            if(FMD_GetBlockStatus(*pNxtGoodBlkAddr) != BLOCK_STATUS_BAD)
            {
                continue;
            }
        }
        
        if(!GetNextGoodBlock(*pNxtGoodBlkAddr, pNxtGoodBlkAddr))
        {
            return FALSE;
        }
    }
    
    return TRUE;
}

void CalculateParity_New(BYTE d, BYTE * p)
{
    BYTE Bit0  = (d & (1 << 0))  ? 1 : 0; 
    BYTE Bit1  = (d & (1 << 1))  ? 1 : 0;
    BYTE Bit2  = (d & (1 << 2))  ? 1 : 0;
    BYTE Bit3  = (d & (1 << 3))  ? 1 : 0;
    BYTE Bit4  = (d & (1 << 4))  ? 1 : 0;
    BYTE Bit5  = (d & (1 << 5))  ? 1 : 0;
    BYTE Bit6  = (d & (1 << 6))  ? 1 : 0;
    BYTE Bit7  = (d & (1 << 7))  ? 1 : 0;

    *p = 0;

    *p |= ((Bit6 ^ Bit5 ^ Bit3 ^ Bit2)               << 0);
    *p |= ((Bit7 ^ Bit5 ^ Bit4 ^ Bit2 ^ Bit1)        << 1);
    *p |= ((Bit7 ^ Bit6 ^ Bit5 ^ Bit1 ^ Bit0)        << 2);
    *p |= ((Bit7 ^ Bit4 ^ Bit3 ^ Bit0)               << 3);
    *p |= ((Bit6 ^ Bit4 ^ Bit3 ^ Bit2 ^ Bit1 ^ Bit0) << 4);
}

void CalculateHammingForNCB_New(unsigned char* pSector, BYTE *pOutBuffer, DWORD dwLen)
{
    DWORD i;
    BYTE *pui8SectorData = (BYTE*)pSector;
 
    for(i=0; i<dwLen; i++ )
    {
        BYTE NP=0;
 
        CalculateParity_New(pui8SectorData[i], &NP);
 
        pOutBuffer[i] = (NP & 0x1F);
    }
}

VOID NANDClearNCB(SBPosition Pos)
{
    DWORD i;
    
    UNREFERENCED_PARAMETER(Pos);
    
    for(i = CHIP_NCB_NAND_OFFSET; i < CHIP_NCB_SEARCH_RANGE; i++)
    {
        FMD_EraseBlock(i);
    }
}

UINT32 GetBCBChecksum(void * pBuffer, int u32Size)
{
    BYTE *pu8Buf = (BYTE *)pBuffer;
    UINT32 crc=0;
    int i;
    
    for(i=0;i < u32Size;i++)
    {
        crc += pu8Buf[i];
    }
    return (crc ^ 0xFFFFFFFF);
}

BOOL NANDWriteNCB(PFlashInfoExt pFlashInfo, SBPosition Pos)
{
    BootBlockStruct_t NCB;
    DWORD i, j;
    SectorInfo si;
    BYTE NCBNum = 0;
    DWORD startBlk1, startBlk2, endBlk2, bbtBlk;
    
    UNREFERENCED_PARAMETER(Pos);
    
    NCB.m_u32FingerPrint = 0x20424346;
    NCB.m_u32Version = 0x01000000;
    NCB.FCB_Block.m_NANDTiming.initializer = 0x60f0f0f;
    NCB.FCB_Block.m_u32DataPageSize = pFlashInfo->fi.wDataBytesPerSector;
    NCB.FCB_Block.m_u32TotalPageSize = pFlashInfo->fi.wDataBytesPerSector + pFlashInfo->SpareDataLength;
    NCB.FCB_Block.m_u32SectorsPerBlock = pFlashInfo->fi.wSectorsPerBlock;
    
    NCB.FCB_Block.m_u32NumberOfNANDs = pFlashInfo->NumberOfChip;
    NCB.FCB_Block.m_u32TotalInternalDie = 1;
    NCB.FCB_Block.m_u32CellType = 1;
    
    if(pFlashInfo->fi.wDataBytesPerSector == 2048)
    {
        NCB.FCB_Block.m_u32EccBlockNEccType = ECCN_2K64_PAGE;
    }
    else if(pFlashInfo->fi.wDataBytesPerSector == 4096 && pFlashInfo->SpareDataLength == 128)
    {
        NCB.FCB_Block.m_u32EccBlockNEccType = ECCN_4K128_PAGE;
    }
    else if(pFlashInfo->fi.wDataBytesPerSector == 4096 && pFlashInfo->SpareDataLength == 218)
    {
        NCB.FCB_Block.m_u32EccBlockNEccType = ECCN_4K218_PAGE;
    }
    else
    {
        ERRORMSG(1, (_T("Fatal Error!\r\n")));
        return FALSE;
    }
    NCB.FCB_Block.m_u32EccBlock0EccType = ECC0_2K4K_PAGE;     //!< Ecc level for Block 0 - BCH
    
    NCB.FCB_Block.m_u32EccBlock0Size = 0;         //!< Number of bytes for Block0 - BCH
    NCB.FCB_Block.m_u32EccBlockNSize = 512;         //!< Block size in bytes for all blocks other than Block0 - BCH
    NCB.FCB_Block.m_u32NumEccBlocksPerPage = pFlashInfo->fi.wDataBytesPerSector / 512;   //!< Number of blocks per page - BCH
    NCB.FCB_Block.m_u32MetadataBytes = METADATA_SIZE;         //!< Metadata size - BCH
    NCB.FCB_Block.m_u32EraseThreshold = NCB.FCB_Block.m_u32EccBlockNEccType;

    if(!GetGoodPhyBlock(IMAGE_BOOT_BOOTIMAGE_NAND_OFFSET, &startBlk1) || \
    !GetGoodPhyBlock(IMAGE_BOOT_BOOTIMAGE_NAND_OFFSET + IMAGE_BOOT_BOOTIMAGE_NAND_BLOCKS / 2, &startBlk2) || \
    !GetGoodPhyBlock(IMAGE_BOOT_BOOTIMAGE_NAND_OFFSET + IMAGE_BOOT_BOOTIMAGE_NAND_BLOCKS, &endBlk2) || \
    !GetGoodPhyBlock(IMAGE_BOOT_DBBT_NAND_OFFSET, &bbtBlk))
    {
        ERRORMSG(TRUE, (_T(" get good block failed\r\n")));
        return FALSE;
    }

    NCB.FCB_Block.m_u32Firmware1_startingSector = startBlk1 * pFlashInfo->fi.wSectorsPerBlock;   //!< Firmware image starts on this sector.
    NCB.FCB_Block.m_u32SectorsInFirmware1 = (startBlk2 - startBlk1) * pFlashInfo->fi.wSectorsPerBlock; //!< Number of sectors in firmware image.
    NCB.FCB_Block.m_u32Firmware2_startingSector = startBlk2 * pFlashInfo->fi.wSectorsPerBlock;  //!< Secondary FW Image starting Sector.
    NCB.FCB_Block.m_u32SectorsInFirmware2 = (endBlk2 - startBlk2) * pFlashInfo->fi.wSectorsPerBlock; //!< Number of sector in secondary FW image.
    NCB.FCB_Block.m_u32DBBTSearchAreaStartAddress = bbtBlk * pFlashInfo->fi.wSectorsPerBlock;
    
    if(pFlashInfo->fi.wDataBytesPerSector == 2048)
    {
        NCB.FCB_Block.m_u32BadBlockMarkerByte = BBMarkByteOffsetInPageData_2K64;
        NCB.FCB_Block.m_u32BadBlockMarkerStartBit = BBMarkBitOffset_2K64;
    }
    else if(pFlashInfo->fi.wDataBytesPerSector == 4096 && pFlashInfo->SpareDataLength == 128)
    {
        NCB.FCB_Block.m_u32BadBlockMarkerByte = BBMarkByteOffsetInPageData_4K128;
        NCB.FCB_Block.m_u32BadBlockMarkerStartBit = BBMarkBitOffset_4K128;
    }
    else if(pFlashInfo->fi.wDataBytesPerSector == 4096 && pFlashInfo->SpareDataLength == 218)
    {
        NCB.FCB_Block.m_u32BadBlockMarkerByte = BBMarkByteOffsetInPageData_4K218;
        NCB.FCB_Block.m_u32BadBlockMarkerStartBit = BBMarkBitOffset_4K218;
    }
    
    NCB.FCB_Block.m_u32BBMarkerPhysicalOffset = pFlashInfo->fi.wDataBytesPerSector;
    
    memcpy(sectorBuf + NAND_HC_ECC_OFFSET_DATA_COPY, &NCB, sizeof(NCB));
    NCB.m_u32Checksum = GetBCBChecksum(sectorBuf + NAND_HC_ECC_OFFSET_DATA_COPY + 4, 512 - 4);
    memcpy(sectorBuf + NAND_HC_ECC_OFFSET_DATA_COPY, &NCB, sizeof(NCB));
    
    CalculateHammingForNCB_New((unsigned char *)sectorBuf + NAND_HC_ECC_OFFSET_DATA_COPY, sectorBuf + NAND_HC_ECC_OFFSET_PARITY_COPY, NAND_HC_ECC_SIZEOF_DATA_BLOCK_IN_BYTES);
    
    memset(&si, 0xff, sizeof(SectorInfo));
    si.bOEMReserved = (BYTE)(~OEM_BLOCK_RESERVED);
    
    for(i = CHIP_NCB_NAND_OFFSET; i < CHIP_NCB_SEARCH_RANGE; i++)
    {
        if(!FMD_EraseBlock(i))
        {
            ERRORMSG(TRUE, (_T("erase block %d failed, set to bad\r\n"),i));
            FMD_SetBlockStatus(i, BLOCK_STATUS_BAD);
        }
        else
        {
            for(j = 0; j < pFlashInfo->fi.wSectorsPerBlock; j += CHIP_BCB_SECTOR_STRIDE)
            {
                if(!FMD_WriteSector(i * pFlashInfo->fi.wSectorsPerBlock + j, sectorBuf, &si, 1))
                {
                    ERRORMSG(TRUE, (_T("failed write sector 0x%x\r\n"), i * pFlashInfo->fi.wSectorsPerBlock));   
                }
                else
                {
                    NCBNum++;
                }
            }
        }
    }
    
    return NCBNum > 0;
}

BOOL NANDWriteDBBT(PFlashInfoExt pFlashInfo, SBPosition Pos)
{
    BootBlockStruct_t NCB;
    DWORD badBlockNum = 0;
    WORD  badBlockBuf[CHIP_BAD_BLOCK_RANGE], i;
    SectorInfo si;
    DWORD bbtBlk;
    
    UNREFERENCED_PARAMETER(Pos);
    
    memset(&NCB, 0, sizeof(NCB));
    memset(badBlockBuf, 0xff, sizeof(badBlockBuf));
    
    for(i = 0; i < CHIP_BAD_BLOCK_RANGE; i++)
    {
        if(FMD_GetBlockStatus(i) & BLOCK_STATUS_BAD)
        {
            badBlockBuf[badBlockNum] = i;
            badBlockNum++;
        }
    }
    
    NCB.m_u32FingerPrint = 0x54424244;
    NCB.m_u32Version = 0x01000000;
    
    NCB.DBBT_Block.m_u32NumberBB = badBlockNum;
    NCB.DBBT_Block.m_u32Number2KPagesBB = 1;
    
    if(!GetGoodPhyBlock(IMAGE_BOOT_DBBT_NAND_OFFSET, &bbtBlk))
    {
        ERRORMSG(TRUE, (_T(" get good block failed\r\n")));
        return FALSE;
    }
    
    memset(sectorBuf, 0, NANDFC_BOOT_SIZE);
    memcpy(sectorBuf, &NCB, sizeof(NCB));
    memset(&si, 0xff, sizeof(SectorInfo));
    si.bOEMReserved = (BYTE)~OEM_BLOCK_RESERVED;
    
    if(!FMD_EraseBlock(bbtBlk))
    {
        ERRORMSG(TRUE, (_T("erase block %d failed, set to bad\r\n"),i));
        FMD_SetBlockStatus(bbtBlk, BLOCK_STATUS_BAD);
        return FALSE;
    }
    else
    {
        if(!FMD_WriteSector(bbtBlk * pFlashInfo->fi.wSectorsPerBlock, sectorBuf, &si, 1))
        {
            ERRORMSG(TRUE, (_T("failed write sector 0x%x\r\n"), i * pFlashInfo->fi.wSectorsPerBlock));  
            FMD_EraseBlock(bbtBlk);
            FMD_SetBlockStatus(bbtBlk, BLOCK_STATUS_BAD);
            return FALSE; 
        }
    }
    memset(sectorBuf, 0xff, NANDFC_BOOT_SIZE);
    
    *(DWORD*)sectorBuf = 0;
    *((DWORD*)sectorBuf + 1) = badBlockNum;  
        
    memcpy((DWORD*)sectorBuf + 2, badBlockBuf, sizeof(badBlockBuf));
    if(!FMD_WriteSector(bbtBlk * pFlashInfo->fi.wSectorsPerBlock + CHIP_BBT_SECTOR_OFFSET, sectorBuf, &si, 1))
    {
        ERRORMSG(TRUE, (_T("failed write sector 0x%x\r\n"), i * pFlashInfo->fi.wSectorsPerBlock));  
        FMD_EraseBlock(bbtBlk);
        FMD_SetBlockStatus(bbtBlk, BLOCK_STATUS_BAD);
        return FALSE; 
    }
    
    return TRUE;
}

VOID NANDGetImgInfo(PNANDImgInfo pInfo)
{
    FlashInfo flashInfo;

    FMD_GetInfo(&flashInfo);
    
    pInfo->dwSBLen = IMAGE_BOOT_BOOTIMAGE_NAND_BLOCKS * flashInfo.dwBytesPerBlock / 2;
    pInfo->dwSBLenUnit = flashInfo.dwBytesPerBlock;
    pInfo->dwNB0Len = NANDImageCfg.dwNkSize;
    pInfo->dwNB0LenUnit = flashInfo.dwBytesPerBlock;
}

//-----------------------------------------------------------------------------
//
//  Function:  NANDReadSB
//
//  This function reads the Boot image stored in NAND flash memory to RAM
//
//  Parameters:
//      pRequest 
//          [in] transfer infomation, including SB position and index
//
//      dwLength 
//          [in] Length of the RAM, in bytes, to be read from flash
//          memory.            
//      Pos
//          [in] specify which boot stream caller wants to read.
//  Returns:
//      TRUE indicates success. FALSE indicates failure.
//
//-----------------------------------------------------------------------------
BOOL NANDReadSB(PNANDIORequest pRequest, LPBYTE pImage, DWORD dwLength)
{
    FlashInfo flashInfo;
    SectorInfo sectorInfo;
    SECTOR_ADDR sectorAddr, startSectorAddr, endSectorAddr;
    DWORD StartLogBlkAddr,EndLogBlkAddr,PhyBlockAddr;
    PBYTE pSectorBuf, pBuf;
    DWORD dwReadSectorLen;
    
    if(!FMD_GetInfo(&flashInfo))
    {
        RETAILMSG(TRUE, (_T("ERROR: Unable to get NAND flash information.\r\n")));
        return FALSE;
    }
    dwReadSectorLen = flashInfo.wDataBytesPerSector;
    pSectorBuf = (PBYTE)LocalAlloc(LPTR, flashInfo.wDataBytesPerSector);
    if(!pSectorBuf)
    {
        ERRORMSG(TRUE, (_T("alloc memory for pSectorBuf failed\r\n")));   
    }
    
    // Check parameters
    if(dwLength < g_ImgInfo.dwSBLenUnit)
    {
        ERRORMSG(TRUE, (_T("length size(0x%x) is smaller than expected (0x%x)\r\n"), dwLength, g_ImgInfo.dwSBLenUnit));
        return FALSE;
    }
    if(pRequest->dwIndex > g_ImgInfo.dwSBLen / g_ImgInfo.dwSBLenUnit)
    {
        ERRORMSG(TRUE, (_T("dwIndex (0x%x) exceeds expected (0x%x)\r\n"), pRequest->dwIndex, g_ImgInfo.dwSBLen / g_ImgInfo.dwSBLenUnit));
        return FALSE;
    }
    
    // Fill the sectorInfo
    memset(&sectorInfo, 0xFF, sizeof(sectorInfo));

    // Calculate block range for the EBOOT image
    StartLogBlkAddr = IMAGE_BOOT_BOOTIMAGE_NAND_OFFSET;     
    EndLogBlkAddr = IMAGE_BOOT_BOOTIMAGE_NAND_OFFSET + IMAGE_BOOT_BOOTIMAGE_NAND_BLOCKS / 2;

    if(pRequest->SBPos == SecondBoot)
    {
        StartLogBlkAddr = IMAGE_BOOT_BOOTIMAGE_NAND_OFFSET + IMAGE_BOOT_BOOTIMAGE_NAND_BLOCKS / 2;
        EndLogBlkAddr = IMAGE_BOOT_BOOTIMAGE_NAND_OFFSET + IMAGE_BOOT_BOOTIMAGE_NAND_BLOCKS;
    }
    StartLogBlkAddr += pRequest->dwIndex;
    
    DEBUGMSG(TRUE, (_T("INFO: reading NAND flash blocks 0x%x\r\n"), StartLogBlkAddr));
    
    if(!GetGoodPhyBlock(StartLogBlkAddr, &PhyBlockAddr))
    {
        ERRORMSG(TRUE, (_T("GetGoodPhyBlock failed: 0x%x\r\n"),StartLogBlkAddr));  
        return FALSE; 
    }
    // Compute sector address based on current physical block
    startSectorAddr = PhyBlockAddr * flashInfo.wSectorsPerBlock;
    endSectorAddr = startSectorAddr + flashInfo.wSectorsPerBlock;
    
    for (sectorAddr = startSectorAddr; sectorAddr < endSectorAddr; sectorAddr++)
    {
        if(dwLength < dwReadSectorLen)
        {
            pBuf = pSectorBuf;
        }
        else
        {
            pBuf = pImage;
        }
    
        if (!FMD_ReadSector(sectorAddr, pBuf, &sectorInfo, 1))
        {
            RETAILMSG(TRUE, (_T("ERROR: Failed to verify image.\r\n")));
            return FALSE;
        }
        
        if(dwLength < dwReadSectorLen)
        {
            memcpy(pImage, pSectorBuf, dwLength);
        }
        pImage += flashInfo.wDataBytesPerSector;      
        dwLength -= flashInfo.wDataBytesPerSector;
    }
    
    LocalFree(pSectorBuf);
    
    return(TRUE);
}
//-----------------------------------------------------------------------------
//
//  Function:  NANDWriteSB
//
//  This function writes to NAND flash memory the Boot image stored 
//  in the RAM file cache area.
//
//  Parameters:
//      pRequest 
//          [in] transfer infomation, including SB position and index
//
//      dwLength 
//          [in] Length of the Boot image, in bytes, to be written to flash
//          memory.            
//      Pos
//          [in] specify which boot stream caller wants to write.
//  Returns:
//      TRUE indicates success. FALSE indicates failure.
//
//-----------------------------------------------------------------------------
BOOL NANDWriteSB(PNANDIORequest pRequest, LPBYTE pImage, DWORD dwLength)
{
    FlashInfo flashInfo;
    LPBYTE pBuf, pVerSectorBuf,pExtraBuf;
    SectorInfo sectorInfo,VersectorInfo;
    SECTOR_ADDR sectorAddr, startSectorAddr, endSectorAddr;
    DWORD StartLogBlkAddr,PhyBlockAddr;
    DWORD dwPhySectorSize;
    LPBYTE pOriginalImage = pImage;
    DWORD dwOriginalLength = dwLength;
RETAILMSG(TRUE, (_T("common nandbootbunner NANDWriteSB.\r\n")));
    if(!FMD_GetInfo(&flashInfo))
    {
        RETAILMSG(TRUE, (_T("ERROR: Unable to get NAND flash information.\r\n")));
        return FALSE;
    }
    
    dwPhySectorSize = flashInfo.wDataBytesPerSector;
    pExtraBuf = (PBYTE) LocalAlloc(LPTR, dwPhySectorSize); 
    if(pExtraBuf == NULL){
        ERRORMSG(TRUE, (_T("Allocate memory buffer with size of 0x%x failed!\r\n"),flashInfo.dwBytesPerBlock+dwPhySectorSize));
        return FALSE;
    }
        
    // Check parameters
    if(dwLength < g_ImgInfo.dwSBLenUnit)
    {
        ERRORMSG(TRUE, (_T("length size(0x%x) is smaller than expected (0x%x)\r\n"), dwLength, g_ImgInfo.dwSBLenUnit));
        return FALSE;
    }
    if(pRequest->dwIndex > g_ImgInfo.dwSBLen / g_ImgInfo.dwSBLenUnit)
    {
        ERRORMSG(TRUE, (_T("dwIndex (0x%x) exceeds expected (0x%x)\r\n"), pRequest->dwIndex, g_ImgInfo.dwSBLen / g_ImgInfo.dwSBLenUnit));
        return FALSE;
    }
    
    memset(pExtraBuf, 0xFF, dwPhySectorSize);    

    // Write sb to NAND flash
    pVerSectorBuf = (PBYTE) LocalAlloc(LPTR, dwPhySectorSize);
    if (pVerSectorBuf == NULL) {
        ERRORMSG(1, (_T("Failed to alloc enough memory space! Memory required: 0x%x bytes.\r\n"), flashInfo.wDataBytesPerSector));    
        return FALSE;
    }    
    
    // Fill the sectorInfo
    memset(&sectorInfo, 0xFF, sizeof(sectorInfo));

    // Set Reserved and image flag
    sectorInfo.bOEMReserved &= ~OEM_BLOCK_RESERVED;   
        
    StartLogBlkAddr = IMAGE_BOOT_BOOTIMAGE_NAND_OFFSET;     
    
    if(pRequest->SBPos == SecondBoot)
    {
        StartLogBlkAddr = IMAGE_BOOT_BOOTIMAGE_NAND_OFFSET + IMAGE_BOOT_BOOTIMAGE_NAND_BLOCKS / 2;
    }
begin_program:    
    StartLogBlkAddr += pRequest->dwIndex;
    
    RETAILMSG(TRUE, (_T("INFO: Programming NAND flash blocks 0x%x\r\n"), StartLogBlkAddr));
    
    if(pRequest->dwIndex == 0)
    {
        NANDClearNCB(pRequest->SBPos);   
    }
    if(!GetGoodPhyBlock(StartLogBlkAddr, &PhyBlockAddr))
    {
        ERRORMSG(TRUE, (_T("GetGoodPhyBlock failed: 0x%x\r\n"),StartLogBlkAddr));  
        return FALSE; 
    }
    
    // Erase the block...
    if (!FMD_EraseBlock(PhyBlockAddr))
    {
        FMD_SetBlockStatus(PhyBlockAddr, BLOCK_STATUS_BAD);
        if(!(FMD_GetBlockStatus(PhyBlockAddr) & BLOCK_STATUS_BAD))
        {
            RETAILMSG(TRUE, (_T("ERROR: Unable to erase NAND flash block [0x%x].\r\n"), PhyBlockAddr));
            return FALSE;
        }
    }
    
    // Compute sector address based on current physical block
    startSectorAddr = PhyBlockAddr * flashInfo.wSectorsPerBlock;
    endSectorAddr = startSectorAddr + flashInfo.wSectorsPerBlock;

    for (sectorAddr = startSectorAddr; sectorAddr < endSectorAddr; sectorAddr++)
    {
        if(dwLength < dwPhySectorSize)
        {
            pBuf = pExtraBuf;
            memcpy(pBuf, pImage, dwLength);
        }
        else
        {
            pBuf = pImage;
        }
        
        if (!FMD_WriteSector(sectorAddr, pBuf, &sectorInfo, 1))
        {
            RETAILMSG(TRUE, (_T("ERROR: Failed to update EBOOT/SBOOT.\r\n")));
            return FALSE;
        }

        if (!FMD_ReadSector(sectorAddr, pVerSectorBuf, &VersectorInfo, 1))
        {
            RETAILMSG(TRUE, (_T("ERROR: Failed to verify image.\r\n")));
            return FALSE;
        }

        if (memcmp(pBuf, pVerSectorBuf, flashInfo.wDataBytesPerSector) != 0)
        {
            RETAILMSG(TRUE, (_T("ERROR: Failed to verify image.\r\n")));
            return FALSE;
        }
        
        pImage += flashInfo.wDataBytesPerSector; 
        dwLength -= flashInfo.wDataBytesPerSector;
    }
    
    if(pRequest->SBPos == BothBoot && StartLogBlkAddr < IMAGE_BOOT_BOOTIMAGE_NAND_OFFSET + IMAGE_BOOT_BOOTIMAGE_NAND_BLOCKS / 2)
    {
        StartLogBlkAddr = IMAGE_BOOT_BOOTIMAGE_NAND_OFFSET + IMAGE_BOOT_BOOTIMAGE_NAND_BLOCKS / 2;
        pImage = pOriginalImage;
        dwLength = dwOriginalLength;
        goto begin_program;   
    }
    
    if(pExtraBuf)
    {
        LocalFree(pExtraBuf);
    }   
    if(pVerSectorBuf)
    { 
        LocalFree(pVerSectorBuf);
    }
    return(TRUE);
}

BOOL NANDEndWriteSB(PNANDIORequest pRequest)
{
    if(!NANDWriteNCB(&g_CSPfi, pRequest->SBPos) || !NANDWriteDBBT(&g_CSPfi, pRequest->SBPos))
    {
        ERRORMSG(TRUE, (_T("write NCB&LDLB failed\r\n")));
        return FALSE;
    }
   
    NANDBootReserved();
    return(TRUE);
}
// add 2015-09-16
BOOL NANDEndWriteNCB(PNANDIORequest pRequest)
{
	if(!NANDWriteNCB(&g_CSPfi, pRequest->SBPos))
	{
		ERRORMSG(TRUE, (_T("write NCB failed\r\n")));
		return FALSE;
	}
    NANDBootReserved();
    return(TRUE);
}
//-----------------------------------------------------------------------------
//
//  Function:  NANDWriteNK
//
//  This function writes to NAND flash memory the OS image stored 
//  in the RAM file cache area.
//
//  Parameters:
//      dwStartAddr 
//          [in] Address in flash memory where the start of the downloaded 
//          OS image is to be written.
//
//      dwLength 
//          [in] Length of the OS image, in bytes, to be written to flash
//          memory.            
//
//  Returns:
//      TRUE indicates success. FALSE indicates failure.
//
//-----------------------------------------------------------------------------
BOOL NANDWriteNK(DWORD dwIndex, LPBYTE pImage, DWORD dwLength, BYTE flag)
{
    FlashInfo flashInfo;
    LPBYTE pSectorBuf, pVerSectorBuf, pVerifyBuf;
    SectorInfo sectorInfo,VersectorInfo;
    SECTOR_ADDR sectorAddr, startSectorAddr, endSectorAddr;
    DWORD StartLogBlkAddr,PhyBlockAddr;
    
    // Boot Configuration and Buffer
    if(!NANDBootInit())
    {
        RETAILMSG(TRUE, (_T("WARNING: NANDBootInit fail\r\n")));
        return FALSE;
    }

    if(!FMD_GetInfo(&flashInfo))
    {
        RETAILMSG(TRUE, (_T("ERROR: Unable to get NAND flash information.\r\n")));
        return FALSE;
    }

    // Check parameters
    if(dwLength < g_ImgInfo.dwNB0LenUnit)
    {
        ERRORMSG(TRUE, (_T("length size(0x%x) is smaller than expected (0x%x)\r\n"), dwLength, g_ImgInfo.dwNB0LenUnit));
        return FALSE;
    }
    if(dwIndex > g_ImgInfo.dwNB0Len / g_ImgInfo.dwNB0LenUnit)
    {
        ERRORMSG(TRUE, (_T("dwIndex (0x%x) exceeds expected (0x%x)\r\n"), dwIndex, g_ImgInfo.dwNB0Len / g_ImgInfo.dwNB0LenUnit));
        return FALSE;
    }
    
    // Write EBOOT to NAND flash
    pSectorBuf = pImage;    
    pVerSectorBuf = pVerifyBuf = (PBYTE) LocalAlloc(LPTR, flashInfo.dwBytesPerBlock);
    if (!pVerifyBuf) {
        ERRORMSG(1, (_T("Failed to alloc enough memory space!\r\n"))); 
        goto exit;   
    }

    // Fill the sectorInfo
    memset(&sectorInfo, 0xFF, sizeof(sectorInfo));

    // Set Reserved and image flag
    sectorInfo.bOEMReserved &= ~OEM_BLOCK_RESERVED;   
    sectorInfo.bOEMReserved &= ~OEM_BLOCK_READONLY;

    // Calculate block range for the NK image
    if (flag != IMAGE_RSNK)
    	StartLogBlkAddr = NANDImageCfg.dwNkOffset / flashInfo.dwBytesPerBlock + dwIndex;
    else
		StartLogBlkAddr = IMAGE_BOOT_RESCUEIMG_NAND_OFFSET + dwIndex; // rescue OS
	
    DEBUGMSG(TRUE, (_T("INFO: Programming NAND flash blocks [0x%x].\r\n"), StartLogBlkAddr));        

retry:
    if(!GetPhyBlkAddr(StartLogBlkAddr, &PhyBlockAddr))
    {
        RETAILMSG(TRUE, (_T("Error: No good block found - unable to store image!\r\n")));
        goto exit;
    }

    // Erase the block...
    if (!FMD_EraseBlock(PhyBlockAddr))
    {
        FMD_SetBlockStatus(PhyBlockAddr, BLOCK_STATUS_BAD);
        if(!(FMD_GetBlockStatus(PhyBlockAddr) & BLOCK_STATUS_BAD))
        {
            RETAILMSG(TRUE, (_T("ERROR: Unable to erase NAND flash block [0x%x].\r\n"), PhyBlockAddr));
            goto exit;
        }
        else
        {
            goto retry;
        }
    }
    
    // Compute sector address based on current physical block
    startSectorAddr = PhyBlockAddr * flashInfo.wSectorsPerBlock;
    endSectorAddr = startSectorAddr + flashInfo.wSectorsPerBlock;
    
    for (sectorAddr = startSectorAddr; sectorAddr < endSectorAddr; sectorAddr++)
    {
        if (!FMD_WriteSector(sectorAddr, pSectorBuf, &sectorInfo, 1))
        {
            RETAILMSG(TRUE, (_T("ERROR: Failed to update image.\r\n")));
            goto exit;
        }

        pSectorBuf += flashInfo.wDataBytesPerSector;
    }
    
    // Compute sector address based on current physical block
    startSectorAddr = PhyBlockAddr * flashInfo.wSectorsPerBlock;
    endSectorAddr = startSectorAddr + flashInfo.wSectorsPerBlock;    
    for (sectorAddr = startSectorAddr; sectorAddr < endSectorAddr; sectorAddr++)
    {
        if (!FMD_ReadSector(sectorAddr, pVerSectorBuf, &VersectorInfo, 1))
        {
            RETAILMSG(TRUE, (_T("ERROR: Failed to verify image.\r\n")));
            goto exit;
        }
        pVerSectorBuf += flashInfo.wDataBytesPerSector;
    }
    pVerSectorBuf = pVerifyBuf;

    if (memcmp(pSectorBuf - flashInfo.dwBytesPerBlock, pVerSectorBuf, flashInfo.dwBytesPerBlock) != 0)
    {
        RETAILMSG(TRUE, (_T("ERROR: Failed to verify image.\r\n")));
        goto exit;
    }
    
exit:
    if(pVerifyBuf)
    {  
        LocalFree(pVerifyBuf);
    }
    return(TRUE);

}

// TCB VCB
BOOL NANDWriteTCBVCB(DWORD dwIndex, LPBYTE pImage, DWORD dwLength, BYTE tv)
{
    FlashInfo flashInfo;
    LPBYTE pBuf, pVerSectorBuf,pExtraBuf;
    SectorInfo sectorInfo,VersectorInfo;
    SECTOR_ADDR sectorAddr, startSectorAddr, endSectorAddr;
    DWORD StartLogBlkAddr,PhyBlockAddr;
    DWORD dwPhySectorSize;
	struct saio_tcb *cfg = NULL;
	int i,ct = 0;

    if(!FMD_GetInfo(&flashInfo))
    {
        RETAILMSG(TRUE, (_T("ERROR: Unable to get NAND flash information.\r\n")));
        return FALSE;
    }
    if( tv == IMAGE_TCB && (dwIndex > IMAGE_BOOT_BOOTCFG_NAND_BLOCKS-1) )
    {
        RETAILMSG(TRUE, (_T("dwIndex (0x%x) exceeds expected (0x%x)\r\n"), dwIndex, (IMAGE_BOOT_BOOTCFG_NAND_BLOCKS-1)));
        return FALSE;
    }
	if( tv == IMAGE_VCB && (dwIndex > IMAGE_BOOT_VECTCFG_NAND_BLOCKS-1) )
	{
		RETAILMSG(TRUE, (_T("dwIndex (0x%x) exceeds expected (0x%x)\r\n"), dwIndex, (IMAGE_BOOT_VECTCFG_NAND_BLOCKS-1)));
		return FALSE;
	}

    dwPhySectorSize = flashInfo.wDataBytesPerSector;
    pExtraBuf = (PBYTE) LocalAlloc(LPTR, dwPhySectorSize); 
    if(pExtraBuf == NULL){
        RETAILMSG(TRUE, (_T("Allocate memory buffer with size of 0x%x failed!\r\n"),flashInfo.dwBytesPerBlock+dwPhySectorSize));
        return FALSE;
    }
		memset(pExtraBuf, 0xFF, dwPhySectorSize); 
    // Write tcb to NAND flash
    pVerSectorBuf = (PBYTE) LocalAlloc(LPTR, dwPhySectorSize);
    if (pVerSectorBuf == NULL) {
        RETAILMSG(1, (_T("Failed to alloc enough memory space! Memory required: 0x%x bytes.\r\n"), flashInfo.wDataBytesPerSector));    
        goto exit;
    } 
    // Fill the sectorInfo
    memset(&sectorInfo, 0xFF, sizeof(sectorInfo));

    // Set Reserved and image flag
    sectorInfo.bOEMReserved &= ~OEM_BLOCK_RESERVED;   
    sectorInfo.bOEMReserved &= ~OEM_BLOCK_READONLY;
    
	if (tv == IMAGE_TCB)
		StartLogBlkAddr = IMAGE_BOOT_BOOTCFG_NAND_OFFSET;  
	else
		StartLogBlkAddr = IMAGE_BOOT_VECTCFG_NAND_OFFSET; // VCB start block
    StartLogBlkAddr += dwIndex; // +2 for test


	if (tv == IMAGE_TCB)
	{
		// dont erase sn/mac
		if(!GetGoodPhyBlock(StartLogBlkAddr, &PhyBlockAddr))
		{
			RETAILMSG(TRUE, (_T("GetGoodPhyBlock failed: 0x%x\r\n"),StartLogBlkAddr));  
			goto exit;
		}
		// Read one section
		if (!FMD_ReadSector(PhyBlockAddr * flashInfo.wSectorsPerBlock, pVerSectorBuf, &VersectorInfo, 1))
		{
			RETAILMSG(TRUE, (_T("ERROR: Failed to read image.\r\n")));
			goto exit;
		}
		cfg = (struct saio_tcb *)pImage;

		for (i=0;i<4;i++){
			cfg->PVersion[i] = pVerSectorBuf[i+36]; // product version
		}
		for (i=0;i<20;i++){
			cfg->SerialNum[i] = pVerSectorBuf[i+40]; // serial num
		}

		for (i=0;i<6;i++){
			if (pVerSectorBuf[i+68] == 0xFF) ct++;
		}
		if (ct!=6) // for first in PL
		{
			for (i=0;i<6;i++){
				cfg->MACBuf[i] = pVerSectorBuf[i+68]; // MAC
			}
		}
	}



retry:
    if(!GetGoodPhyBlock(StartLogBlkAddr, &PhyBlockAddr))
    {
        RETAILMSG(TRUE, (_T("GetGoodPhyBlock failed: 0x%x\r\n"),StartLogBlkAddr));  
        goto exit;
    }
    // Erase the block...
    if (!FMD_EraseBlock(PhyBlockAddr))
    {
        FMD_SetBlockStatus(PhyBlockAddr, BLOCK_STATUS_BAD);
        if(!(FMD_GetBlockStatus(PhyBlockAddr) & BLOCK_STATUS_BAD))
        {
            RETAILMSG(TRUE, (_T("ERROR: Unable to erase NAND flash block [0x%x].\r\n"), PhyBlockAddr));
            goto exit;
        }
        else
        {
            goto retry;
        }
    }
    
    // Compute sector address based on current physical block
    startSectorAddr = PhyBlockAddr * flashInfo.wSectorsPerBlock;
    endSectorAddr = startSectorAddr + flashInfo.wSectorsPerBlock;
    for (sectorAddr = startSectorAddr; sectorAddr < endSectorAddr; sectorAddr++)
    {
        if(dwLength < dwPhySectorSize)
        {
            pBuf = pExtraBuf;
            memcpy(pBuf, pImage, dwLength);
        }
        else
        {
            pBuf = pImage;
        }
        
        if (!FMD_WriteSector(sectorAddr, pBuf, &sectorInfo, 1))
        {
            RETAILMSG(TRUE, (_T("ERROR: Failed to update EBOOT/SBOOT.\r\n")));
            goto exit;
        }

        if (!FMD_ReadSector(sectorAddr, pVerSectorBuf, &VersectorInfo, 1))
        {
            RETAILMSG(TRUE, (_T("ERROR: Failed to verify image.\r\n")));
            goto exit;
        }

        if (memcmp(pBuf, pVerSectorBuf, flashInfo.wDataBytesPerSector) != 0)
        {
            RETAILMSG(TRUE, (_T("ERROR: Failed to verify image.\r\n")));
            goto exit;
        }
        if(dwLength <= ((sectorAddr-startSectorAddr+1)*dwPhySectorSize)) break;
        pImage += flashInfo.wDataBytesPerSector; 
        dwLength -= flashInfo.wDataBytesPerSector;
    }
    
exit:
    if(pExtraBuf)
    {
        LocalFree(pExtraBuf);
    }   
    if(pVerSectorBuf)
    { 
        LocalFree(pVerSectorBuf);
    }
    return(TRUE);
}
BOOL NANDCopyBack(SBPosition SrcPos, SBPosition DestPos)
{
    FlashInfo flashInfo;
    SectorInfo sectorInfo;
    PBYTE pBuf, pBuf2;
    SECTOR_ADDR sectorAddr, startSectorAddr, endSectorAddr;
    DWORD rdStartLogBlkAddr,rdEndLogBlkAddr,rdPhyBlockAddr;
    DWORD wtStartLogBlkAddr,wtEndLogBlkAddr,wtPhyBlockAddr;
    
    if(SrcPos == DestPos)
    {
        return FALSE;   
    }
    if(SrcPos != FirstBoot && SrcPos != SecondBoot)
    {
        return FALSE;   
    }
    if(DestPos != FirstBoot && DestPos != SecondBoot)
    {
        return FALSE;   
    }
    
    if(!FMD_GetInfo(&flashInfo))
    {
        RETAILMSG(TRUE, (_T("ERROR: Unable to get NAND flash information.\r\n")));
        return FALSE;
    }
    pBuf2 = pBuf = (PBYTE)LocalAlloc(LPTR, flashInfo.dwBytesPerBlock);
    if(!pBuf)
    {
        ERRORMSG(TRUE, (_T("Alloc memory fails\r\n")));
        return FALSE;   
    }
    
    // Fill the sectorInfo
    memset(&sectorInfo, 0xFF, sizeof(sectorInfo));

    // Calculate block range for the EBOOT image
    rdStartLogBlkAddr = IMAGE_BOOT_BOOTIMAGE_NAND_OFFSET;     
    rdEndLogBlkAddr = IMAGE_BOOT_BOOTIMAGE_NAND_OFFSET + IMAGE_BOOT_BOOTIMAGE_NAND_BLOCKS / 2;
    
    wtStartLogBlkAddr = IMAGE_BOOT_BOOTIMAGE_NAND_OFFSET + IMAGE_BOOT_BOOTIMAGE_NAND_BLOCKS / 2; 
    wtEndLogBlkAddr = IMAGE_BOOT_BOOTIMAGE_NAND_OFFSET + IMAGE_BOOT_BOOTIMAGE_NAND_BLOCKS;
    
    if(SrcPos == SecondBoot)
    {
        rdStartLogBlkAddr = IMAGE_BOOT_BOOTIMAGE_NAND_OFFSET + IMAGE_BOOT_BOOTIMAGE_NAND_BLOCKS / 2;
        rdEndLogBlkAddr = IMAGE_BOOT_BOOTIMAGE_NAND_OFFSET + IMAGE_BOOT_BOOTIMAGE_NAND_BLOCKS;
        
        wtStartLogBlkAddr = IMAGE_BOOT_BOOTIMAGE_NAND_OFFSET;
        wtEndLogBlkAddr = IMAGE_BOOT_BOOTIMAGE_NAND_OFFSET + IMAGE_BOOT_BOOTIMAGE_NAND_BLOCKS / 2;
    }
    if(!GetGoodPhyBlock(rdStartLogBlkAddr, &rdPhyBlockAddr))
    {
        ERRORMSG(TRUE, (_T("GetGoodPhyBlock failed: 0x%x\r\n"),rdStartLogBlkAddr));  
        return FALSE; 
    }
    if(!GetGoodPhyBlock(wtStartLogBlkAddr, &wtPhyBlockAddr))
    {
        ERRORMSG(TRUE, (_T("GetGoodPhyBlock failed: 0x%x\r\n"),wtStartLogBlkAddr));  
        return FALSE; 
    }
        
    do
    {
        // read
        pBuf = pBuf2;
        startSectorAddr = rdPhyBlockAddr * flashInfo.wSectorsPerBlock;
        endSectorAddr = startSectorAddr + flashInfo.wSectorsPerBlock;
        
        for (sectorAddr = startSectorAddr; sectorAddr < endSectorAddr; sectorAddr++)
        {
            if (!FMD_ReadSector(sectorAddr, pBuf, &sectorInfo, 1))
            {
                RETAILMSG(TRUE, (_T("ERROR: Failed to read image.\r\n")));
                return FALSE;
            }

            pBuf += flashInfo.wDataBytesPerSector;            
        }
        // write
        
        if (!FMD_EraseBlock(wtPhyBlockAddr))
        {
            FMD_SetBlockStatus(wtPhyBlockAddr, BLOCK_STATUS_BAD);
            RETAILMSG(TRUE, (_T("ERROR: Failed to erase block.\r\n")));
            return FALSE;
        }
        
        pBuf = pBuf2;
        startSectorAddr = wtPhyBlockAddr * flashInfo.wSectorsPerBlock;
        endSectorAddr = startSectorAddr + flashInfo.wSectorsPerBlock;
        
        for (sectorAddr = startSectorAddr; sectorAddr < endSectorAddr; sectorAddr++)
        {
            if (!FMD_WriteSector(sectorAddr, pBuf, &sectorInfo, 1))
            {
                RETAILMSG(TRUE, (_T("ERROR: Failed to write image.\r\n")));
                return FALSE;
            }

            pBuf += flashInfo.wDataBytesPerSector;            
        }
        
        
        rdStartLogBlkAddr++;
        wtStartLogBlkAddr++;
        
        GetNextGoodBlock(rdPhyBlockAddr, &rdPhyBlockAddr);
        GetNextGoodBlock(wtPhyBlockAddr, &wtPhyBlockAddr);
        
    }while(rdStartLogBlkAddr < rdEndLogBlkAddr);   

    if(!NANDWriteNCB(&g_CSPfi, DestPos) || !NANDWriteDBBT(&g_CSPfi, DestPos))
    {
        ERRORMSG(TRUE, (_T("write NCB&LDLB failed\r\n")));
        return FALSE;
    }   
    
    return TRUE;
}

//-----------------------------------------------------------------------------
//
//  Function: FMD_OEMIoControl
//
//  This function implements user-defined commands for the flash memory device. 
//
//  Parameters:
//      dwIoControlCode 
//          [in] The control code specifying the command to execute.
//
//      pInBuf
//          [in] Long pointer to a buffer that contains the data required to 
//          perform the operation. Set to NULL if the dwIoControlCode parameter 
//          specifies an operation that does not require input data.
//                        
//      nInBufSize
//          [in] Size, in bytes, of the buffer pointed to by pInBuf. 
//
//      pOutBuf
//          [out] Long pointer to a buffer that receives the output data for 
//          the operation. Set to NULL if the dwIoControlCode parameter 
//          specifies an operation that does not produce output data.
//
//      nOutBufSize
//          [in] Size, in bytes, of the buffer pointed to by pOutBuf.
//
//      pBytesReturned
//          [out] Long pointer to a variable that receives the size, in bytes, 
//          of the data stored into the buffer pointed to by pOutBuf. Even 
//          when an operation produces no output data and pOutBuf is set to 
//          NULL, the DeviceIoControl function uses the variable pointed to 
//          by pBytesReturned. After such an operation, the value of the 
//          variable has no meaning. 
//
//  Returns:  
//      Returns TRUE on success. Returns FALSE on failure.
//
//-----------------------------------------------------------------------------
BOOL BSP_OEMIoControl(DWORD dwIoControlCode, PBYTE pInBuf, DWORD nInBufSize, 
    PBYTE pOutBuf, DWORD nOutBufSize, PDWORD pBytesReturned)
{
    BOOL rc = FALSE;
    
    UNREFERENCED_PARAMETER(pBytesReturned);
    
    switch (dwIoControlCode){
        case IOCTL_DISK_COPYBACK:
            if(!pOutBuf || nOutBufSize < sizeof(SBPosition))
            {
                ERRORMSG(TRUE, (L"para error in IOCTL_DISK_COPYBACK. \r\n"));
                break;
            }
            if(!pInBuf || nInBufSize < sizeof(SBPosition))
            {
                ERRORMSG(TRUE, (L"para error in IOCTL_DISK_COPYBACK. \r\n"));
                break;
            }
            BSPNAND_SetClock(TRUE);
            rc = NANDCopyBack(*(PSBPosition)pInBuf, *(PSBPosition)pOutBuf);
            BSPNAND_SetClock(FALSE);
            break;
        case IOCTL_DISK_VENDOR_GET_UPDATE_SIG:
        case IOCTL_DISK_VENDOR_SET_UPDATE_SIG:
        case IOCTL_DISK_GET_NANDBOOT_MODE:
        case IOCTL_DISK_SET_NANDBOOT_MODE:
            SetLastError(ERROR_NOT_SUPPORTED);
            break;
        case IOCTL_DISK_VENDOR_GET_SBIMGINFO:
            {
                DEBUGMSG(TRUE, (L"IOCTL_DISK_VENDOR_GET_IMGLENTH received. \r\n"));    
                if(!pOutBuf || nOutBufSize < sizeof(NANDImgInfo))
                {
                    break;
                }
                memcpy(pOutBuf, &g_ImgInfo, sizeof(NANDImgInfo));
                //NANDGetImgInfo((PNANDImgInfo)pOutBuf);
                rc = TRUE;
                break;       
            }
        
        case IOCTL_DISK_VENDOR_GET_IMGINFO:
            {
                PNANDWrtImgInfo pNANDWrtImgInfo;

                DEBUGMSG(TRUE, (L"IOCTL_DISK_VENDOR_GET_IMGLENTH received. \r\n"));   
            
                if(!pOutBuf || nOutBufSize < sizeof(NANDWrtImgInfo))
                {
                    ERRORMSG(TRUE, (_T("invalid parameters\r\n")));
                    break;
                }
                
                pNANDWrtImgInfo = (PNANDWrtImgInfo)pOutBuf;
                  
                if(pNANDWrtImgInfo->dwImgType == IMAGE_NK || pNANDWrtImgInfo->dwImgType == IMAGE_TCB \
					|| pNANDWrtImgInfo->dwImgType == IMAGE_VCB || pNANDWrtImgInfo->dwImgType == IMAGE_RSNK)
                {
                    pNANDWrtImgInfo->dwImgSizeUnit = g_ImgInfo.dwNB0LenUnit;
                }
                else
                {
                    pNANDWrtImgInfo->dwImgSizeUnit = g_ImgInfo.dwSBLenUnit;                  
                }
                rc = TRUE;
                break;       
            }
                    

        case IOCTL_DISK_VENDOR_READ_EBOOT:    
            DEBUGMSG(TRUE, (L"IOCTL_DISK_VENDOR_READ_EBOOT received. \r\n"));
            if(!pOutBuf || nOutBufSize < g_ImgInfo.dwSBLenUnit)
            {
                ERRORMSG(TRUE, (_T("invalid parameters\r\n")));
                break;
            }
            if(!pInBuf || nInBufSize < sizeof(NANDIORequest))
            {
                ERRORMSG(TRUE, (_T("invalid parameters\r\n")));
                break;
            }
            if(((PNANDIORequest)pInBuf)->SBPos != FirstBoot && ((PNANDIORequest)pInBuf)->SBPos != SecondBoot)
            {
                ERRORMSG(TRUE, (_T("invalid parameters\r\n")));
                break;
            }
            BSPNAND_SetClock(TRUE);
            rc = NANDReadSB((PNANDIORequest)pInBuf, pOutBuf, nOutBufSize);
            BSPNAND_SetClock(FALSE);
            break;
          
          case IOCTL_DISK_VENDOR_WRITE_SB:
              DEBUGMSG(TRUE, (L"IOCTL_FLASH_VENDOR_WRITE_SB received. \r\n"));
              if(!pOutBuf || nOutBufSize < g_ImgInfo.dwSBLenUnit)
              {
                  ERRORMSG(TRUE, (_T("invalid parameters\r\n")));
                  break;
              }
              if(!pInBuf || nInBufSize < sizeof(NANDIORequest))
              {
                  ERRORMSG(TRUE, (_T("invalid parameters\r\n")));
                  break;
              }
              BSPNAND_SetClock(TRUE);
              rc = NANDWriteSB((PNANDIORequest)pInBuf, pOutBuf, nOutBufSize);
              BSPNAND_SetClock(FALSE);
              break;
              
          case IOCTL_DISK_VENDOR_END_WRITE_SB:
              DEBUGMSG(TRUE, (L"IOCTL_DISK_VENDOR_END_WRITE_SB received. \r\n"));
              if(!pInBuf || nInBufSize < sizeof(NANDIORequest))
              {
                  ERRORMSG(TRUE, (_T("invalid parameters\r\n")));
                  break;
              }
              BSPNAND_SetClock(TRUE);
              rc = NANDEndWriteSB((PNANDIORequest)pInBuf);
              BSPNAND_SetClock(FALSE);
              break; 

        case IOCTL_DISK_VENDOR_WRITE_IMAGE:
            {
                PNANDWrtImgInfo pNANDWrtImgInfo;

                if(!pInBuf || nInBufSize < sizeof(NANDWrtImgInfo))
                {
                    ERRORMSG(TRUE, (_T("invalid parameters\r\n")));
                    break;
                }                
                if(!pOutBuf || nOutBufSize < sizeof(DWORD))
                {
                    ERRORMSG(TRUE, (_T("invalid parameters\r\n")));
                    break;
                }
                
                BSPNAND_SetClock(TRUE);
                pNANDWrtImgInfo = (PNANDWrtImgInfo)pInBuf;
                
                if(pNANDWrtImgInfo->dwImgType == IMAGE_NK || pNANDWrtImgInfo->dwImgType == IMAGE_RSNK)
                {
                    rc = NANDWriteNK(pNANDWrtImgInfo->dwIndex, pOutBuf,nOutBufSize, pNANDWrtImgInfo->dwImgType);
                }
				else if(pNANDWrtImgInfo->dwImgType == IMAGE_TCB || pNANDWrtImgInfo->dwImgType == IMAGE_VCB)
				{
					rc = NANDWriteTCBVCB(pNANDWrtImgInfo->dwIndex, pOutBuf,nOutBufSize, pNANDWrtImgInfo->dwImgType);
				}
                else
                {
                    NANDIORequest NANDIORequest;  
                    NANDIORequest.SBPos = BothBoot;
                    NANDIORequest.dwIndex = pNANDWrtImgInfo->dwIndex;
                    rc = NANDWriteSB(&NANDIORequest, pOutBuf, nOutBufSize);                    
                }

                BSPNAND_SetClock(FALSE);
                break;
            }            
        case IOCTL_DISK_VENDOR_END_WRITE_IMAGE:
            {
                PNANDWrtImgInfo pNANDWrtImgInfo;

                if(!pInBuf || nInBufSize < sizeof(NANDWrtImgInfo))
                {
                    ERRORMSG(TRUE, (_T("invalid parameters\r\n")));
                    break;
                }             
                pNANDWrtImgInfo = (PNANDWrtImgInfo)pInBuf;

                BSPNAND_SetClock(TRUE);
/*                
                if(pNANDWrtImgInfo->dwImgType == IMAGE_NK)
                {
                    NANDBootReserved();
                }
                else if(pNANDWrtImgInfo->dwImgType != IMAGE_TCB && pNANDWrtImgInfo->dwImgType != IMAGE_VCB)
                {
                    NANDIORequest NANDIORequest;            

                    NANDIORequest.SBPos = BothBoot;           
                    rc = NANDEndWriteSB(&NANDIORequest);                  
                }       
*/
				if(pNANDWrtImgInfo->dwImgType != IMAGE_NK && pNANDWrtImgInfo->dwImgType != IMAGE_TCB \
					&& pNANDWrtImgInfo->dwImgType != IMAGE_VCB && pNANDWrtImgInfo->dwImgType != IMAGE_RSNK)
				{
                    NANDIORequest NANDIORequest;            

                    NANDIORequest.SBPos = BothBoot;           
                    rc = NANDEndWriteSB(&NANDIORequest);                  
                }
                BSPNAND_SetClock(FALSE);

                break;    
            }
		case IOCTL_DISK_VENDOR_END_WRITE_NCB:
			{
                PNANDWrtImgInfo pNANDWrtImgInfo;

                if(!pInBuf || nInBufSize < sizeof(NANDWrtImgInfo))
                {
                    ERRORMSG(TRUE, (_T("invalid parameters\r\n")));
                    break;
                }             
                pNANDWrtImgInfo = (PNANDWrtImgInfo)pInBuf;

				BSPNAND_SetClock(TRUE);
				if(pNANDWrtImgInfo->dwImgType == IMAGE_NK || pNANDWrtImgInfo->dwImgType == IMAGE_TCB \
					|| pNANDWrtImgInfo->dwImgType == IMAGE_VCB || pNANDWrtImgInfo->dwImgType == IMAGE_RSNK)
				{
				    NANDIORequest NANDIORequest;    

	                NANDIORequest.SBPos = BothBoot;           
	                rc = NANDEndWriteNCB(&NANDIORequest); 
				}
				BSPNAND_SetClock(FALSE);
			}
			break;
        
        default:
            break;
    }

    return(rc);
}
