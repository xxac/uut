//
// 	Copyright (c) 2009 -2013 Zakus Inc.  All rights reserved.
//
//
#ifndef SAIO_SYS_H
#define SAIO_SYS_H
#ifndef SAIO_DEBUG
#define	SAIO_DEBUG		0
#endif
#define SAIO_MIN_KLEN		148
#define SAIO_MAX_KLEN		532
#define	SAIO_MAX_CAID		10
#define	SAIO_PEFLAG		"SAIO--"
#define SAIO_PKLEN		512
#define SAIO_SYS_HV		L"\\System\\system.hv"
#define SAIO_USR_HV		L"\\System\\default\\user.hv"
#define SAIO_DEF_MK		L"\\System\\default.mky"
#define UCE_VER			L"A2.3"


#ifndef	SAIO_BOOT_LOADER
#include "pkfuncs.h"
#include "Oal_kitl.h"
#define PROC_CREATE	(-2)
//
//	Ioctl codes
//
#define IOCTL_SAIO_SET_PUBKEY	CTL_CODE(FILE_DEVICE_HAL,200,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SAIO_GET_PUBKEY	CTL_CODE(FILE_DEVICE_HAL,201,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SAIO_SET_ID	CTL_CODE(FILE_DEVICE_HAL,202,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SAIO_GET_ID	CTL_CODE(FILE_DEVICE_HAL,203,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SAIO_NF_WRITE	CTL_CODE(FILE_DEVICE_HAL,204,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SAIO_NF_READ	CTL_CODE(FILE_DEVICE_HAL,205,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SAIO_GET_STATIS	CTL_CODE(FILE_DEVICE_HAL,206,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SAIO_SET_STATIS	CTL_CODE(FILE_DEVICE_HAL,207,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SAIO_GET_ADC	CTL_CODE(FILE_DEVICE_HAL,208,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SAIO_GET_TCB	CTL_CODE(FILE_DEVICE_HAL,209,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SAIO_GET_MATE	CTL_CODE(FILE_DEVICE_HAL,210,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SAIO_SET_MATE	CTL_CODE(FILE_DEVICE_HAL,211,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SAIO_GET_TOUCH	CTL_CODE(FILE_DEVICE_HAL,212,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SAIO_SET_READY	CTL_CODE(FILE_DEVICE_HAL,213,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SAIO_GET_READY	CTL_CODE(FILE_DEVICE_HAL,214,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SAIO_GPCMD	CTL_CODE(FILE_DEVICE_HAL,216,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SAIO_HIDEVENT	CTL_CODE(FILE_DEVICE_HAL,217,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SAIO_DEVOPEN	CTL_CODE(FILE_DEVICE_HAL,218,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SAIO_GETOWNER	CTL_CODE(FILE_DEVICE_HAL,219,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SAIO_SETOWNER	CTL_CODE(FILE_DEVICE_HAL,220,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SAIO_NF_OVERWRITE	CTL_CODE(FILE_DEVICE_HAL,221,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SAIO_GET_ADCREG	CTL_CODE(FILE_DEVICE_HAL,222,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SAIO_GET_GPIOREG	CTL_CODE(FILE_DEVICE_HAL,223,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SAIO_GET_CELSIUS	CTL_CODE(FILE_DEVICE_HAL,224,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SAIO_GET_RTC_VL	CTL_CODE(FILE_DEVICE_HAL,225,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SAIO_USR_ACTIVITY	CTL_CODE(FILE_DEVICE_HAL,226,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SAIO_DRV_ACTIVITY	CTL_CODE(FILE_DEVICE_HAL,227,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SAIO_RTC_INIT	CTL_CODE(FILE_DEVICE_HAL,228,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SAIO_CHARGEABLE	CTL_CODE(FILE_DEVICE_HAL,229,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SAIO_WAN		CTL_CODE(FILE_DEVICE_HAL,230,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SAIO_WAKEUP_ALARM	CTL_CODE(FILE_DEVICE_HAL,231,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SAIO_SET_PROCESS	CTL_CODE(FILE_DEVICE_HAL,232,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SAIO_CRITICAL	CTL_CODE(FILE_DEVICE_HAL,234,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SAIO_VSTANDBY	CTL_CODE(FILE_DEVICE_HAL,236,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SAIO_MUTEX	CTL_CODE(FILE_DEVICE_HAL,238,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SAIO_SHUTOFF	CTL_CODE(FILE_DEVICE_HAL,240,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SAIO_USB_STUFF	CTL_CODE(FILE_DEVICE_HAL,242,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SAIO_RTC_SYNC	CTL_CODE(FILE_DEVICE_HAL,244,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SAIO_IMG_BLOCK	CTL_CODE(FILE_DEVICE_HAL,246,METHOD_BUFFERED,FILE_ANY_ACCESS)

#define IOCTL_SAIO_GET_MAC	CTL_CODE(FILE_DEVICE_HAL,600,METHOD_BUFFERED,FILE_ANY_ACCESS)

//
//	Macro
//
#define OS_IMAGE_BLK_IDX	1
#define POST_IMAGE_BLK_IDX	2
#define BOOTVECT_BLK_IDX	3

#define	SAIO_ACTIVITY(x)	OAL_AioIoctl(IOCTL_SAIO_DRV_ACTIVITY,0,x,0,0,0)
//
//	Structures
//
typedef struct {
	unsigned char		flag[6];
	unsigned short		ca_id;
	unsigned short		vendor_id;
	unsigned short		ap_id;
	unsigned char		signature[SAIO_PKLEN];
} SAIO_ID_INFO;

typedef struct {
    	LPWSTR 			fname;
	unsigned int		len;
    	unsigned char 		LCR;
} KNOWN_FILE_DESC;
#endif
//
//	Process structure
//
typedef struct {
	unsigned short		vendor_id;
	unsigned short		ap_id;
	unsigned short		superuser;
} SAIO_PROCESS;
//
//	Structure of SAIO certificate
//
struct saio_cert
{
	DWORD	version;		// Version information
	CHAR	comment[48];		// Comment about this update task
	CHAR	effect_date[4];		// Activate date in YYYYMMDD
	CHAR	expire_date[4];		// Deactivate date in YYYYMMDD
	UCHAR	type;			// Specify the type of the Certificate
	//
	//	Define Types of certificate
	//
	#define	UPD_PUBLIC_KEY_CERT	0x00
	#define	UPD_CATALOG_CERT	0x01
	//
	UCHAR	CA_ID;			// CA's ID who issues this certificate	
	UCHAR	CA_ID2;			// Ignored if type > 0	
	UCHAR	reserved[12];		// Reserved for future use
	UCHAR	algorithm;		// Fingerprint Algorithm
	//
	//	Algoritm for HASH
	//
	#define UPD_ALGORITHM_SHA1	0x01
	#define UPD_ALGORITHM_SHA256	0x02
	#define UPD_ALGORITHM_SHA348	0x03
	#define UPD_ALGORITHM_SHA512	0x04
	#define UPD_ALGORITHM_MD2	0x10
	#define UPD_ALGORITHM_MD5	0x11
	#define UPD_ALGORITHM_MAC	0x20
	#define UPD_ALGORITHM_HMAC	0x21
	#define	MAX_SIGN_LEN		512
	//
	UCHAR	sign[MAX_SIGN_LEN];	// Digital signature
};
//
//	SAIO public key structure 
//
struct 	SPubKey 
{
	unsigned int		Len;			// Length of the key BLOB
	unsigned char		Blob[SAIO_MAX_KLEN+1];	// key BLOB
	struct saio_cert	Cert;
};
//
//	SAIO program ID structure
//
struct 	SaioID
{
	unsigned short		vid;			// Vendor ID
	unsigned short		pid;			// Program/application ID
	unsigned short		su;			// Superuser ?
};
//
//	Define NAND mapping  (x indicates if the Large Block is used)
//
#define	SAIO_SYSCONF_BLOCK(x)	((x)? 22:22)
#define SAIO_BOOTVECT_BLOCK(x)	((x)? 28:28)
#define SAIO_POST_BLOCK(x)	((x)? 14:14)
#define SAIO_POST_SIZE(x)	((x)? 8:8)
#define SAIO_RESCUE_OS_BLOCK(x)	((x)? 33:33)
#define SAIO_RESCUE_OS_SIZE(x)	((x)? 67:67)
#define SAIO_SYSTEM_OS_BLOCK(x)	((x)? 100:100)
//
//	Magic number
//
#define SYSCONF_MAGIC_NUM 	0x5341494F		// "SAIO"
#define BOOTVECTOR_MAGIC_NUM	0x56454354 		// "VECT"
//
//	BOOT vector structure
//
struct bootvect 
{
	unsigned int		MagicNum;
	unsigned int		PostExist;
	unsigned int 		OSVector;
	//
	#define	RESCUE_OS	0
	#define SYSTEM_OS	1
	//
	unsigned int		SysSize;	
	unsigned int		UsbDevice;
	//
	#define	USBDEV_DISABLE	0
	#define VENUS_STYLE	1
	#define MS_VIRTUAL_COM	2
};
//
//	TCB structure
//
typedef struct saio_tcb
{
	unsigned int	MagicNum;		// It should be 'SAIO'
	char			Signature[16];			
	char			ProdName[14];		// Product Name
	char			TcbVersion[2];		// This TCB version
	char			PVersion[4];		// Product Version
	char			SerialNum[20];		// Serial Number
	char			MFG_date[8];		 
	unsigned char		MACBuf[6];
	unsigned char		PubKey[SAIO_MIN_KLEN+1];
	//
	//	Data for LCD configuration
	//
	unsigned char		LCD_Cfg;
	//
	#define	HORZONTAL_SWAP		0x01		// For Mono LCD only 
	#define	COLOR_REVERSED		0x02		// For Mono LCD only
	#define CONTRAST_CONFIG		0x04		// For Mono LCD only
	#define DISPLAY_ROTATE		0x04		// For TFT LCD only
	#define VERTICAL_SWAP		0x08		// For Mono LCD only
	#define	LCD_MONO(x)		((x>>4)&7)	
	#define LCD128x64MONO		1
	#define LCD128x64MONO2		2
	#define LCD128x32MONO		3
	#define LCD_TFT			0x80
	//
	unsigned char		LCD_default_contrast;
	unsigned char		LCD_max_contrast;	// Maximum contrast percentage
	unsigned char		LCD_min_contrast;	// Minimum contrast percentage
	//
	//	Data for Touch panel configuration
	//
	unsigned char		TCH_Cfg;		
	//
	#define	TCH_ON_COM		0x01
	#define TCH_GET_PORTNUM		0x06	
	#define	TCH_BUILD_IN		0x08			
	#define TCH_X_SWAP		0x10
	#define TCH_Y_SWAP		0x20
	#define	TCH_ROTATE		0x40
	//
	//	Data for Keypad configuration
	//
	unsigned char		Keypad_Exist;		// Indicate if small keypad exists
	//
	#define	KBD_STANDARD		0x01
	#define	KBD_EPP_MODULE		0x02
	#define	KBD_MINI		0x04
	#define	KBD_MINI_EXT		0x08
	#define	KBD_MAP_INDEX(x)	((x>>6)&3)
	//
	unsigned char		ASIC_Device;		// Indicate if device powered by ASIC
	unsigned char		FinalVersion;		// Indicate if device is in release moda
	unsigned char		Nand_TotalBlock_H;	// High byte of Nand Flash size
	unsigned char		Nand_TotalBlock_L;	// Low byte of Nand Flash size
	unsigned char		FS_StartBlock_H;	// High byte of starting block for FS
	unsigned char		FS_StartBlock_L;	// Low byte of starting block for FS
	//
	//	TFT LCD configuration
	//
	unsigned char		LCD_Cfg2;		
	//
	#define	LCD_VIRTUAL_TFT		0x01			
	#define	LCD_320x240_TFT		0x02		// WinTek			

	//
	unsigned char		WP_VEE_WorkAround;	// For WallEye Plue only
	unsigned char		No_Temperature_Sensor;	// Indicate if temperature sensor exists
	//
	//	The below "Slope" is the rate at which the AIN6 changes with
	//	respect to a changes in termperature (every 10 Celsius) and 
	//	the "Intercept" is the AIN6 value at zero degree.
	//
	//	AIN6 = (Slope * celsius)/10 + Intercept
	//	
	unsigned char		Slope_H;		// High byte of Slope		
	unsigned char		Slope_L;		// Low byte of Slope		
	unsigned char		Intercept_H;		// High byte of Intercept
	unsigned char		Intercept_L;		// Low byte of Intercept
	unsigned char    	Printer_Cfg;           	// Build-in printer configuration
	//
	#define	TP48_ROHM		0x01
	#define	TP48_KYOCERA		0x02
	#define	TP72			0x03
	#define	TP48_ROHM_PCI		0x04
	#define	TP72_PCI		0x05
	#define	TP48_KYOCERA_PCI	0x06
	#define	TP48_HYBIRD		0x07
	#define TP48_2G_KPB        	0x10        
	#define TP48_2G_J245        	0x11 
	#define TP48_2G_5V_3A_E100    	0x12	
	#define TP48_2G_5V_4A_T202    	0x13
	#define TP48_2G_7V_5A_100GT2  	0x14
	#define TP48_2G_7V_2A_T103  	0x15
	
	#define TP_ON_USB		0x0F
	//
	//	Default Card transporter 
	//
	unsigned char		CT_Cfg;
	//
	#define CT_BUILD_IN		0x1		// Build in motorize CT
	#define CT_VENUS_COM		0x2		// Motorize CT in Venus attached on COM
	#define CT_VNG_COM		0x3		// Motorize CT in VNG attached on COM
	#define CT_VENUS_USB		0x4		// Motorize CT in Venus attached on USB
	#define CT_VNG_USB		0x5		// Motorize CT in Venus attached on USB
	#define CT_ON_EPP			0x0A		// Motorize CT in EPP

	#define	XPD_TYPE_MASK		0x0F
	#define	XPD_COM_MASK		0xF0
	//
	//	Default MSR reader
	//
	unsigned char		MSR_Cfg;
	//
	#define MSR_BUILD_IN		0x1		// Build in MSR
	#define MSR_VENUS_COM		0x2		// MSR in Venus attached on COM
	#define MSR_VNG_COM		0x3		// MSR in VNG attached on COM
	#define MSR_VENUS_USB		0x4		// MSR in Venus attached on USB
	#define MSR_VNG_USB		0x5		// MSR in VNG attached on USB
	#define MSR_CRADLE_USB		0x6		// MSR in Cradle attached on USB
	#define MSR_ON_EPP		0xA		// MSR in EPP device
       	//
	//	Default ICC and SAM reader
	//
	unsigned char		SCR_Cfg;
	//
	#define ICC_BUILD_IN		0x1		// Build in SCR
	#define ICC_VENUS_COM		0x2		// SCR in Venus attached on COM
	#define ICC_VNG_COM		0x3		// SCR in VNG attached on COM
	#define ICC_VENUS_USB		0x4		// SCR in Venus attached on USB
	#define ICC_VNG_USB		0x5		// SCR in VNG attached on USB
	#define ICC_CCID_USB		0x6		// SCR in USB CCID reader
	#define ICC_ON_EPP		0xA		// SCR in EPP device
       	//
	unsigned char		passwd[9];		// Password (null-terminated string)
	unsigned char		MiscFlag;		// Misc device flag #1
	//
	#define SAIO_BBT_DEV		0x01
	#define SAIO_PCI		0x02
	#define SAIO_PORTABLE		0x04
	#define	SAIO_ISL1219_RTC	0x08		// The ISL1219 RTC with tamper detection
	#define SAIO_OUTDOOR_DEV	0x10
	#define SAIO_ALT_GPIO_INF	0x20		// Alternate GPIO for LCD & keypad
	#define SAIO_NO_USBHOST		0x40		// There is no USB host
	//
	unsigned char		MiscFlag2;		// Misc device flag #2
	//
	#define SAIO_VSTANDBY		0x01
	#define SAIO_SWITCHED_COM(x) 	((x>>1)&7)	// Bit1-3 for switchable com number (1-6)
	#define SAIO_DIS_MEMCHECK	0x10
	//
	unsigned char		MiscFlag3;		// Misc device flag #3
	//
	#define SAIO_FS_ENCRYPT		0x01
	#define SAIO_SCREENSAVER	0x02
	#define SAIO_FLOATSCREEN_BLON	0x04
	#define SAIO_FLOATSCREEN_BLOFF	0x06
	#define SAIO_OS_LINUX		0x80
	//
	unsigned char		MiscFlag4;		// Misc device flag #4
	//
	#define SAIO_MODEM_PATCH	0x01
	//
	//	EPP config
	//
	unsigned char		EPP_Cfg;
	//
	#define EPP_ON_COM		0x1			
	#define LC_EPP_ON_COM		0x2
	#define LC_EPP_ON_USB		0x3
	//
	//	WAN config
	//
	unsigned char		WAN_Cfg;
	//
	#define WAN_ON_USB		0x1			
	#define WAN_ON_COM		0x2
	#define WAN_CMUX_ON_COM		0x3
	#define WAN_CMUX_ON_USB		0x4
	#define WAN_MULTI_MASK	0x70
	//	
	//	Default SAM reader
	//
	unsigned char		SAM_Cfg;
	//
	//	WAN secondary interface 
	//
	unsigned char		WAN_Cfg2;
	//
	#define SIEMENS_MC55I_ON_COM	0x01
	#define SIEMENS_TC63I_ON_COM	0x02
	#define WAN_AUX_ON_USB		0x08
	//
	//	The RFID device
	//
	unsigned char		RFID_Cfg;
	//
	#define RFID_BUILD_IN		0x1		// Build in RFID
	#define RFID_VNG_COM		0x3		// RFID in VNG attached on COM
	#define RFID_VNG_USB		0x5		// RFID in VNG attached on USB
	#define RFID_ON_EPP		0xA		// RFID in EPP device
	//
	//	The extended Keypad config
	//
	unsigned char		KBD_ExtCfg;	
	//
	//	Customer related OS version
	//
	unsigned char		OS_version[3];
	//
	//	The default 3300mV Ref Volt adjustment in mV 
	//	for battery level ADC
	//
	signed char		RefVoltAdjust;	
	//
	//	The default Warning/shutdown temperature
	//
	unsigned char		Warning_Temp;
	unsigned char		Shutdown_Temp;
	//
	//Wireless module configuration
	//Bit7~Bit4:WIFI,Bit3~Bit0:Bluetooth module
	//
	#define USB_WIFI_MODULE		0x80
	#define IVT_BT_MODULE       0x01
	#define ISSC_BT_MODULE      0x02
	unsigned char		WirelessModule_Cfg1;
	
	unsigned char		autoDownloadImage;
	unsigned char		EtherDevice;
	unsigned char		delay;
	unsigned char		dwConfigFlags;
	unsigned char 	DHCPEnable;
	unsigned char		numBootMe;
	unsigned char		RTCSrc;
	#define RTC_EXMASK	0x3
	#define RTC_EPP		0x1
	#define RTC_PCF		0x2
	unsigned char		DeviceFlag;
	#define Enable_TTL_MSR 0x1
	unsigned int		IP;
  unsigned int		subnetMask;
	unsigned char		Reserved3[30];
	unsigned char		ReservedAP[14];//
} SAIO_TCB_CFG;
//
//	Statistic stuff
//
struct saio_statistic
{
	unsigned long		MSR_swipe;		// Total MSR swipping#
	unsigned long		MSR_error;		// Total error number occured on MSR
	unsigned long		MSR_reserved[2];
	//
	unsigned long		PWR_on;			// Total power on time (sec)
	unsigned long		PWR_reset;		// Total number of power reset
	unsigned long		PWR_fail;		// Total number of power failure
	unsigned long		PWR_reserved[1];
	//
	unsigned long		KEY_hitted;		// Total number keypad has been hitted
	unsigned long		KEY_reserved[3];
	//
	unsigned long		TCH_down;		// Total touch down number of touch
	unsigned long		TCH_reserved[3];
	//
	unsigned long		PRN_paperlenght;	// Total length of paper has been printed
	unsigned long		PRN_dotline;		// Total line number of dot being printed
	unsigned long		PRN_coveropen;		// Total number of printer cover was opened
	unsigned long		PRN_reserved[1];
	//
	unsigned long		ICC_insert;		// Total number of IC slot was inserted
	unsigned long		ICC_error;		// Total error number occured on ICC
	unsigned long		ICC_reserved[2];
	//
	unsigned long		LCD_duration;		// Total LCD backlight on duration
	unsigned long		LCD_times;		// Total number backlight was turned on
	unsigned long		LCD_reserved[2];
	//
	unsigned long		DWN_times;		// Total number of download 
	unsigned long		DWN_error;		// Total errpr number of download
	unsigned long		DWN_reserved[2];
	//
	unsigned long		AIO_update;		// Total number of SAIO update
	unsigned long		AIO_update_fail;	// Total number of update failure
	unsigned long		AIO_reserved[2];
	//
	unsigned long		BAT_power;		// Total battery power duration time (sec)
	unsigned long 		BAT_discharge_cycle;	// Total number of battery discharge_cycle
	unsigned long 		BAT_critical;		// Total number of battery reach critical
	unsigned long		BAT_force_shutdown;	// Total number of battery force poweroff
	//
	unsigned long		TFS_format;		// Total number of TFAT was formatted
	unsigned long		TFS_format_fail;	// Total number of formatting failure
	unsigned long		TFS_format_Lowlevel;	// Total number of low level formatting 
	unsigned long		TFS_reserved[1];
	//
	unsigned long		PM_power_btn;		// Total number power button was hitted
	unsigned long		PM_suspend;		// Total number of system standby
	unsigned long		PM_suspend_PBtn;	// Total number of suspend by power button
	unsigned long		PM_reserved[1];
	//
	unsigned long		PIN_key_loaded;		// Total number of pin key loaded
	unsigned long		PIN_key_fail;		// Total number of pin key loading failure
	unsigned long		PIN_entry;		// Total number of pin entry	
	unsigned long		PIN_entry_fail;		// Total number of pin entry failure
	//
	unsigned long		CHR_micr_read;		// Total number of MICR read
	unsigned long		CHR_micr_error;		// Total number of MICR read error
	unsigned long		CHR_doc_scan;		// Total number of document scan	
	unsigned long		CHR_dot_fired;		// Total number of Ink jet dot fired
	//
	unsigned long 		TAMPER_count;		// Total number of physical tamper
	unsigned long		TAMPER_reserved[1];
	unsigned long		TAMPER_date;		// The 1st tamper date
	unsigned long		TAMPER_time;		// The 1st tamper time
	//
	unsigned long 		RFID_tapped;		// Total number of RFID tapped
	unsigned long 		RFID_error;		// Total number of RFID error
	unsigned long		RFID_reserved[2];
	//
	unsigned long 		TEMP_high_warning;	// Total number of temperature high warning
	unsigned long 		TEMP_force_shutdown;	// Total number of temperature force posweroff
	unsigned long		TEMP_reserved[2];
	//
	unsigned long		Reserved[(20-17)*4];
};
//
//	SAIO HouseKeeping structure
//	
struct 	saio_mate 
{
	unsigned int		MagicNum;		
	#define MATE_MAGIC	0x8888888E
	unsigned short		BL_saver;
	unsigned char		BL_keypad;
	unsigned char		BL_touch;
	unsigned char		BL_display;
	unsigned char		BL_status;
	unsigned char		LCD_contrast;
	unsigned char		LCD_init;
	unsigned short		SYS_idle_AC;
	unsigned short		SYS_suspend_AC;
	unsigned short		SYS_idle_BATT;
	unsigned short		SYS_suspend_BATT;
	unsigned short		SYS_turn_off;
	unsigned int		Reset_src;
	//
	//	Timer reset source
	//
	//	Must be defined as the same as in "SaioBase.h"
	//
	#define	SAIO_DEV_COM1_6		(1<<0)
	#define SAIO_DEV_SCR		(1<<5)
	#define SAIO_DEV_MSR		(1<<6)
	#define SAIO_DEV_USBD		(1<<7)
	#define SAIO_DEV_NAND		(1<<8)
	#define SAIO_DEV_AUDIO		(1<<9)
	#define SAIO_DEV_BEEPER		(1<<10)
	#define SAIO_DEV_XPD		(1<<11)
	#define SAIO_DEV_PRINTER	(1<<12)
	#define SAIO_DEV_ETHERNET	(1<<13)
	#define SAIO_DEV_WAN		(1<<14)
	#define SAIO_DEV_USBMODEM	(1<<15)
	#define SAIO_DEV_RFID		(1<<16)
	#define SAIO_DEV_QRDEV		(1<<17)
	//
	unsigned char		BATT_warning;	
	unsigned char		BATT_critical;
	unsigned char		BATT_poweroff;
	unsigned char		TEMP_warning;	
	unsigned char		TEMP_poweroff;

	unsigned char		BrightNess;
	unsigned char		DarkNess;
	unsigned char		Dialer_ctrl;
	unsigned char		KBD_ctrl;
	unsigned char		Reserved[10];
};
//
//	SAIO small touch calibration
//
typedef struct saio_calibration
{
	unsigned short		TCH_Max_X;
	unsigned short		TCH_Min_X;
	unsigned short		TCH_Max_Y;
	unsigned short		TCH_Min_Y;
} CALIBRATOR;

#ifndef	SAIO_BOOT_LOADER
//
//	SAIO power manager
//
typedef struct saio_pm
{
	unsigned char		PWR_Fail	:1;
	unsigned char		PWR_LcdOff	:1;
	unsigned char		PWR_ShutDown	:1;
	unsigned char		PWR_Boot	:3;
	//
	//	Define boot stages
	//
	#define SAIO_LOADER		0x1
	#define SAIO_POST		0x2
	#define SAIO_KERNEL		0x4
	//
	unsigned char		PWR_chargeable	:1;
	unsigned char		PWR_Reserved	:1;
	unsigned char		PWR_state;
	//
	//	Define power state
	//
	#define	SAIO_POWER_ON		0x01
	#define	SAIO_POWER_USER_IDLE	0x02
	#define	SAIO_POWER_SYS_IDLE	0x04
	#define	SAIO_POWER_SYS_SUSPEND	0x08
	#define SAIO_POWER_MASK		0xF0
	#define	SAIO_POWER_OFF		0x10
	#define	SAIO_POWER_CRITICAL	0x20
	#define SAIO_POWER_AC_POWER	0x40
	#define	SAIO_POWER_CHARGE	0x80
	//
	unsigned short		SYS_Idle;
	unsigned short		SYS_Suspend;
} SAIO_PM;
//
//	SAIO CFG Area
//
typedef struct saio_cfg 
{
	SAIO_TCB_CFG		TCB;
	unsigned short		LCD_Width;
	unsigned short		LCD_Height;
	unsigned int		LCD_Color;
	unsigned char		LCD_Rotate;
	unsigned char		LCD_Activity;
	unsigned char		Reg_Unlocked;
	unsigned char		Int_Trigger;
	unsigned short		BL_Timer;
	unsigned short		TCH_RawX;
	unsigned short		TCH_RawY;

	unsigned int		NFBlock_Size;
	unsigned int		UsbDevice;
	unsigned char		Registry;
	unsigned char		MA_Flag;
	unsigned char		HashSize;
	unsigned char		DevOpenFlag;
	//
	//	Define for Open Devices
	//
	#define	SAIO_USB_DEVICE		0x01
	#define SAIO_VENUS_USB		0x02
	#define SAIO_MSR_READER		0x04
	#define SAIO_ICC_READER		0x08
	#define SAIO_CCID_USB		0x10
	#define SAIO_CDMA_USB		0x20
	//
	unsigned int		TLPStatus;
	unsigned int		CardTPStatus;
	SAIO_PM			PM;
	CALIBRATOR		Calibator;	
	struct SPubKey		*SaioPubKey;
	unsigned char		SaioOpt;
	//
	//	Define for SAIO operation
	//
	#define	SAIO_RUNNING		0x00
	#define SAIO_STANDBY		0x01
	#define SAIO_RTC_RESUME		0x02
	#define SAIO_BTN_RESUME		0x04
	#define SAIO_PFI_RESUME		0x08
	//
	struct saio_statistic	Statistic;
	struct saio_mate	Mate;
	//
	unsigned int		KeyMatrix;
	unsigned char		NumInputOnly;
	unsigned char		PinMode;
	unsigned char		UsbHid;
	unsigned char		EppFlag;
	unsigned char		WanDev;
	unsigned char		Reserved[4];
	//
	//	Define for WAN device
	//
	#define	WAN_CDMA		0x01
	#define	WAN_GPRS		0x02
	#define	WAN_COM			0x04
	#define	WAN_COM_CMUX		0x08
	#define	WAN_CONNECT		0x10
	#define	WAN_HANGUP		0x20
	//	
	unsigned char		Security;
	//
	//	Scurity setting
	//
	#define	SECURITY_ENABLE		0x80
	#define	BOOTUP_CHECKED		0x40
	#define	EPP_TAMPED_CHECKED	0x20

	#define TAMPER_MASK		0x1F
	#define	TCB_TAMPERED		0x01
	#define	PUBKEY_TAMPERED		0x02
	#define	OS_TAMPERED		0x04
	#define	EPP_TAMPERED		0x08
	//
	unsigned char		PMDevNum;
	//
	//	Defines for PM
	//
	#define	MAX_PMDEV		0x10
	#define	INSERT_PMDEV		0x01
	#define	DELETE_PM_DEV		0x00
	#define	RESUME_PMDEV		0x01
	#define	SUSPEND_PMDEV		0x00
	//
	//	Routines
	//
	void			(*LcdBackLight)(BOOL On);
	void			(*LcdContrast)(DWORD percent);
	void			(*SetPower[MAX_PMDEV])(BYTE On);
	void			(*FISRHandle[6])(BYTE index);
	//
	//	Define ISR 
	//
	#define	PRINTER_ISR		0
	#define	MSR_ISR			1
	#define	UART_ISR		2
	#define TOUCH_INITIALIZE	3
	//
	int			(*AccessRTSCTS)(BYTE port,BYTE ctrl,BYTE *cts);
	unsigned char		(*GetRawKey)(WORD timeout,WORD delay,WORD rate);
	void			(*KernelDelay)(DWORD time);
	void			(*Cipher)(BYTE idx, BYTE *data,WORD len,WORD mode);
	//
	//	Atomic 
	//
	unsigned long		Mutex_Flag;
	//
	//	Define for mutex
	//
	#define	SAIO_NAND_MUTEX		(1<<0)
	#define	SAIO_ADC_MUTEX		(1<<1)
	#define	SAIO_EPP_MUTEX		(1<<2)
	#define	SAIO_CMUX_MUTEX		(1<<3)
	//
	// Define for sync EPP RTC
	//
	unsigned char EppRtc;
	//
	// Define for Secure OS&PK verified flag
	// B0: OS&PK Verified.
	unsigned char OSVerified;
	OAL_KITL_ARGS kitl;
	unsigned int dbgSerPhysAddr;
	unsigned char deviceId[16]; // Device identification
	unsigned char uuid[16];
	unsigned char  eUsbDevice;
	BOOL updateMode;
	BOOL bHiveCleanFlag;                // TRUE = Clean hive at boot
  	BOOL bCleanBootFlag;                // TRUE = Clear RAM, hive, user store at boot
} SAIO_CONF;
//
//	External common routine
//
#ifdef __cplusplus
extern "C" {
#endif
extern void 	OAL_Delay(int time);
extern PVOID 	OAL_MemMap(PVOID addr, INT sz);
extern VOID 	OAL_MemUnmap(PVOID addr);
extern DWORD 	OAL_IrqMaping(DWORD irq_num);
extern BOOL 	OAL_AioIoctl(DWORD,PBYTE,DWORD,PBYTE,DWORD,PDWORD);
extern VOID		OAL_Critital(DWORD mode, BOOL user);
extern BOOL 	OAL_PmDevice(SAIO_CONF *,BOOL,void (*setpower)(BYTE on));
#ifdef __cplusplus
}
#endif
//
//	Inline routine
//
#ifdef __cplusplus
extern "C" {
#endif
__inline DWORD
OAL_Atomic_Opt(DWORD *ptr,DWORD val, BYTE op) 
{ 										
	DWORD 	Old, New;
	#define	ATOMIC_XOR	0
	#define	ATOMIC_OR	1
	#define	ATOMIC_AND	2
	#define	ATOMIC_SET	3
        do {
		Old = *ptr;
		switch (op)
		{
			case ATOMIC_XOR: 
				New = Old ^ val; break;
			case ATOMIC_OR:  
				New = Old | val; break;
			case ATOMIC_AND: 
				New = Old & val; break;
			case ATOMIC_SET: 
				New = val; break;
			default: 
				New = Old; break;
		}
	} while (InterlockedCompareExchange((LONG *)ptr,New,Old)!=(long)Old); 
	return Old;
}

__inline DWORD
OAL_Mutex(DWORD *ptr, DWORD mutex, DWORD lock)
{
    	DWORD 	retries = 10000, flag;

	if (lock)
	{
		while (TRUE) 
		{
			flag = OAL_Atomic_Opt(ptr,mutex,ATOMIC_OR); 
			if (!(flag & mutex) || !retries) break;
			Sleep(1);
			retries--;
		}
		if (flag & mutex) return FALSE;
	} else {
		OAL_Atomic_Opt(ptr,~mutex,ATOMIC_AND);
	}
	return TRUE;
}
#ifdef __cplusplus
}
#endif
#endif
#endif
