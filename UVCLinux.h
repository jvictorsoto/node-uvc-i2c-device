#ifndef UVCLINUX_H
#define UVCLINUX_H

#define UVC_EXT_API
#define UVC_EXPORT

#ifdef __cplusplus
extern "C"
{
#define DEFVAL(x) = x
#else
#define DEFVAL(x)
#endif

#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include <pthread.h>
#include <netinet/in.h>

#define MAX_PATH 260
#define VOID void


typedef int BOOLEAN;
typedef int BOOL;
typedef int INT;
typedef unsigned int UINT;
typedef long long REFERENCE_TIME;
typedef double DOUBLE;

typedef uint8_t BYTE;
typedef char CHAR, *PCHAR;
typedef unsigned char UCHAR, *PUCHAR;
typedef short SHORT, *PSHORT;
typedef unsigned short USHORT, *PUSHORT;
typedef unsigned short WORD, *PWORD;
typedef long LONG, *PLONG;
typedef unsigned long ULONG, *PULONG;
typedef unsigned long DWORD, *PDWORD, *LPDWORD;
typedef unsigned int UINT;
typedef long long LONGLONG;
typedef unsigned long long ULONGLONG;

typedef wchar_t WCHAR, *PWCHAR, *WSTRING;

typedef void *PVOID;
typedef PVOID HWND;
typedef PVOID HANDLE;



// RESULT OF FUNCTION
//
typedef enum {

	UVC_RS_SUCCESSFUL					= 0x00000000,

	UVC_RS_ERROR_GENERAL				= 0x00000001,

	UVC_RS_ERROR_INVALID_MEDIA_TYPE		= 0x00000002,

	UVC_RS_INVALID_FW_SIGNATURE			= 0x00000003,

	UVC_RS_DEVICE_CREATE_FAILED			= 0x00000004,

	UVC_RS_INCORRECT_IMAGE_LENGTH		= 0x00000005,

	UVC_RS_INVALID_FILE					= 0x00000006,

	UVC_RS_SPI_FLASH_ERASE_FAILED		= 0x00000007,

	UVC_RS_CORRUPT_FIRMWARE_IMAGE_FILE	= 0x00000008,

	UVC_RS_I2C_E2PROM_UNKNOWN_I2C_SIZE	= 0x00000009,

	UVC_RS_BOOT_LOADER_IS_NOT_RUNNING	= 0x0000000A,

	}UVC_QRESULT;

// UVC VIDEO INPUT
//
typedef enum {

	UVC_AUTO							= 0,	// AUTO Detect

	UVC_VGA								= 1,    // VGA

	UVC_HDMI							= 2,    // HDMI

	UVC_DISPLAY_PORT					= 3,    // DisplayPort

	UVC_HDMI1							= 4,	// PORT1 of MST3367 or IT6802 (HDMI or DVI depend on EDID)

	UVC_HDMI2							= 5,	// PORT2 of MST3367 or IT6802 (HDMI or DVI depend on EDID)

	UVC_RGB1							= 6,	// PORT1 of MST3367, RGB input

	UVC_RGB2							= 7,	// PORT2 of MST3367, RGB input

	UVC_YUV1							= 8,	// PORT1 of MST3367,   input

	UVC_YUV2							= 9,	// PORT2 of MST3367, YCbCr input

	UVC_SDI1							= 10,

	UVC_SDI2							= 11,

	UVC_AV1								= 12,	// AV PORT1 of TW9910

	UVC_AV2								= 13,	// AV PORT2 of TW9910

	UVC_SVHS1							= 14,	// SVHS PORT1 of TW9910

	UVC_SVHS2							= 15,	// SVHS PORT2 of TW9910

	UVC_END								= 16

} UVC_VIDEO_INPUT;

// UVC AUDIO INPUT
//
typedef enum {

	UVC_I2S_ONLY						= 0x00000000,   // HDMI ONLY

	UVC_LINE_IN_ONLY					= 0x00000001,   // LINE IN ONLY

	UVC_I2S_LINE_IN_MIX					= 0x00000002,   // MIX HDMI LINE IN

	UVC_MIC_IN_ONLY						= 0x00000003,	// MIC IN ONLY

	UVC_I2S_MIC_IN_MIX                	= 0x00000004,   // MIX HDMI MIC IN

} UVC_AUDIO_INPUT;

// UVC VIDEO EDID INPUT
//
typedef enum {

	UVC_ALL_EDID						= 0x00000000,

	UVC_HDMI_EDID						= 0x00000001,

	UVC_VGA_EDID						= 0x00000002,

	UVC_DVI_EDID						= 0x00000003,

} UVC_EDID_INPUT;

typedef struct _UVC_DEVICE_INFO
{
	ULONG	m_nVenderID;

	ULONG	m_nProductID;

	CHAR	pszDeviceID[ MAX_PATH ];

	CHAR	pszDevicePath[ MAX_PATH ];

} UVC_DEVICE_INFORMATION;

typedef struct _UVC_HDR_PACKET
{
	BYTE	HB0_2[ 3 ];		// HB0_2[0] : Type code , HB0_2[1] : Version number, HB0_2[2] : Length of InfoFrame.

	BYTE	PB0_26[ 27 ];   //

} UVC_HDR_PACKET;

typedef VOID* UVC_DEVICE_HANDLE;

typedef VOID* UVC_DEVICE_INFO;

// ##############################################################################################################################################################################
// #
// # GENERAL UVC DEVICE FUNCTIONS
// #
// ##############################################################################################################################################################################

UVC_EXT_API UVC_QRESULT UVC_ENUM_AVAILABLE_DEVICE_INFO( UVC_DEVICE_INFO * ppUvcDeviceInfo /*OUT*/, BOOL bNext = false /*IN*/ );

// EXAMPLE : UVC_ENUM_AVAILABLE_DEVICE_INFO( &pUVCDeviceInfo, TRUE );

UVC_EXT_API UVC_QRESULT UVC_OPEN_DEVICE( ULONG nVendorID /*IN*/, ULONG nProductID /*IN*/, BOOL * bIsOurDevice /*OUT*/, UVC_DEVICE_HANDLE * ppDevice /*OUT*/ );

// EXAMPLE : UVC_OPEN_DEVICE( 0x0FD9, 0x0061, &bIsOurDevice , &pDevice );

// EXAMPLE : UVC_OPEN_DEVICE( 0x1164, 0xD57A, &bIsOurDevice , &pDevice );

UVC_EXT_API UVC_QRESULT UVC_OPEN_DEVICE_EX( CHAR * pszDeviceID /*IN*/, BOOL * bIsOurDevice /*OUT*/, UVC_DEVICE_HANDLE * ppDevice /*OUT*/ );

// EXAMPLE : UVC_OPEN_DEVICE_EX( "0x1164F57A", &bIsOurDevice , &pDevice );

UVC_EXT_API UVC_QRESULT UVC_OPEN_DEVICE_EX2( ULONG * pDeviceTable /*IN*/, ULONG nDeviceTableSize /*IN*/, ULONG nDeviceIndex /*IN*/, CHAR * pszDevicePath /*IN*/, BOOL * bIsOurDevice /*OUT*/, UVC_DEVICE_HANDLE * ppDevice /*OUT*/, ULONG * pVendorID /*OUT*/, ULONG * pProductID /*OUT*/ );

// EXAMPLE : UVC_OPEN_DEVICE_EX2( p_hid_search_table, 28, 0, "\\?\usb#vid_1164&pid_f57a&mi_00#6&37c71c43&0&0000", &is_our_products, &pDevice, &pid, &vid );

UVC_EXT_API UVC_QRESULT UVC_CLOSE_DEVICE( UVC_DEVICE_HANDLE pDevice /*IN*/ );

// EXAMPLE : UVC_CLOSE_DEVICE( pDevice );

UVC_EXT_API UVC_QRESULT UVC_SET_VIDEO_CAPTURE_FILTER_NAME( UVC_DEVICE_HANDLE pDevice /*IN*/, CHAR * pszCaptureFilterName /*IN*/ );

// EXAMPLE : UVC_SET_VIDEO_CAPTURE_FILTER_NAME( pDevice, "UB3300 Video" );

UVC_EXT_API UVC_QRESULT UVC_CLEAR_VIDEO_CAPTURE_FILTER_NAME( UVC_DEVICE_HANDLE pDevice /*IN*/ );

// EXAMPLE : UVC_CLEAR_VIDEO_CAPTURE_FILTER_NAME( pDevice );

UVC_EXT_API UVC_QRESULT UVC_SET_AUDIO_CAPTURE_FILTER_NAME( UVC_DEVICE_HANDLE pDevice /*IN*/, CHAR * pszCaptureFilterName /*IN*/ );

// EXAMPLE : UVC_SET_AUDIO_CAPTURE_FILTER_NAME( pDevice, "UB3300 Audio" );

UVC_EXT_API UVC_QRESULT UVC_CLEAR_AUDIO_CAPTURE_FILTER_NAME( UVC_DEVICE_HANDLE pDevice /*IN*/ );

// EXAMPLE : UVC_CLEAR_AUDIO_CAPTURE_FILTER_NAME( pDevice );

UVC_EXT_API UVC_QRESULT UVC_UPDATE_CAPTURE_FILTER_NAME_TO_FLASH( UVC_DEVICE_HANDLE pDevice /*IN*/ );

// EXAMPLE : UVC_UPDATE_CAPTURE_FILTER_NAME_TO_FLASH( pDevice );

UVC_EXT_API UVC_QRESULT UVC_SET_SERIAL_NUMBER( UVC_DEVICE_HANDLE pDevice /*IN*/, CHAR * pszSerialNumer /*IN*/ );

// EXAMPLE : UVC_SET_SERIAL_NUMBER( pDevice, "FFFFFFFF" );

UVC_EXT_API UVC_QRESULT UVC_CLEAR_SERIAL_NUMBER( UVC_DEVICE_HANDLE pDevice /*IN*/ );

// EXAMPLE : UVC_CLEAR_SERIAL_NUMBER( pDevice );

UVC_EXT_API UVC_QRESULT UVC_GET_VIDEO_OFFSET( UVC_DEVICE_HANDLE pDevice /*IN*/, ULONG * pDisplayMode /*OUT*/, LONG * pHShift /*OUT*/, LONG * pVShift /*OUT*/ );

// EXAMPLE : UVC_GET_VIDEO_OFFSET( pDevice, &nDisplayMode, &nHShift, &nVShift );

UVC_EXT_API UVC_QRESULT UVC_SET_VIDEO_OFFSET( UVC_DEVICE_HANDLE pDevice /*IN*/, LONG nHShift /*IN*/, LONG nVShift /*IN*/ );

// EXAMPLE : UVC_SET_VIDEO_OFFSET( pDevice, &nHShift, &nVShift );

UVC_EXT_API UVC_QRESULT UVC_SET_VIDEO_INPUT( UVC_DEVICE_HANDLE pDevice /*IN*/, ULONG nVideoInput /*IN*/ );

// EXAMPLE : UVC_SET_VIDEO_INPUT( pDevice, UVC_VGA );

// EXAMPLE : UVC_SET_VIDEO_INPUT( pDevice, UVC_HDMI );

UVC_EXT_API UVC_QRESULT UVC_GET_VIDEO_INPUT( UVC_DEVICE_HANDLE pDevice /*IN*/, ULONG * pGetVideoInput /*OUT*/ );

// EXAMPLE : UVC_GET_VIDEO_INPUT( pDevice, &nGetVideoInput );

UVC_EXT_API UVC_QRESULT UVC_SET_AUDIO_INPUT( UVC_DEVICE_HANDLE pDevice /*IN*/, ULONG nAudioInput /*IN*/ );

// EXAMPLE : UVC_SET_AUDIO_INPUT( pDevice, UVC_I2S_ONLY );

// EXAMPLE : UVC_SET_AUDIO_INPUT( pDevice, UVC_LINE_IN_ONLY );

UVC_EXT_API UVC_QRESULT UVC_SET_AUDIO_VOLUME( UVC_DEVICE_HANDLE pDevice /*IN*/, ULONG nAudioInput /*IN*/, ULONG nVolume /*IN*/ );

// EXAMPLE : UVC_SET_AUDIO_VOLUME( pDevice, UVC_I2S_ONLY, 123 ); // range from (0-156), 123 is 0 db.

UVC_EXT_API UVC_QRESULT UVC_SET_MUTE( UVC_DEVICE_HANDLE pDevice /*IN*/ );

// EXAMPLE : UVC_SET_MUTE( pDevice );

UVC_EXT_API UVC_QRESULT UVC_SET_UNMUTE( UVC_DEVICE_HANDLE pDevice /*IN*/ );

// EXAMPLE : UVC_SET_UNMUTE( pDevice );

// SWITCH USB TRANSFER MODE - ISOCHRONOUS OR BULK

UVC_EXT_API UVC_QRESULT UVC_SET_TRANSFER_MODE( UVC_DEVICE_HANDLE pDevice /*IN*/, ULONG nTransferMode /*IN*/ );

// EXAMPLE : UVC_SET_TRANSFER_MODE( pDevice, UVC_ISOCHRONOUS );

UVC_EXT_API UVC_QRESULT UVC_GET_TRANSFER_MODE( UVC_DEVICE_HANDLE pDevice /*IN*/, ULONG * pTransferMode /*OUT*/ );

// EXAMPLE : UVC_GET_TRANSFER_MODE( pDevice, &nTransferMode );

UVC_EXT_API UVC_QRESULT UVC_SET_STARTUP_TRANSFER_MODE( UVC_DEVICE_HANDLE pDevice /*IN*/, ULONG nTransferMode /*IN*/ );

// EXAMPLE : UVC_SET_STARTUP_TRANSFER_MODE( pDevice, UVC_BULK );

UVC_EXT_API UVC_QRESULT UVC_GET_STARTUP_TRANSFER_MODE( UVC_DEVICE_HANDLE pDevice /*IN*/, ULONG * pTransferMode /*OUT*/ );

// EXAMPLE : UVC_GET_STARTUP_TRANSFER_MODE( pDevice, &nTransferMode );

UVC_EXT_API UVC_QRESULT UVC_GET_CONNECTED_PORT( UVC_DEVICE_HANDLE pDevice /*IN*/, ULONG * pConnectedPort /*OUT*/ );

// EXAMPLE : UVC_GET_CONNECTED_PORT( pDevice, &nConnectedPort );

UVC_EXT_API UVC_QRESULT UVC_GET_FIRMWARE_VERSION( UVC_DEVICE_HANDLE pDevice /*IN*/, ULONG * pVersion /*OUT*/ );

// EXAMPLE : UVC_GET_FIRMWARE_VERSION( pDevice, &nVersion );

UVC_EXT_API UVC_QRESULT UVC_SET_HDCP( UVC_DEVICE_HANDLE pDevice /*IN*/, ULONG nSituation /*IN*/ );

// EXAMPLE : UVC_SET_HDCP( pDevice, 0 ); // HDCP_OFF (Disable HDCP)

// EXAMPLE : UVC_SET_HDCP( pDevice, 1 ); // HDCP_ON_VIDEO (Enable HDCP with Normal Video)

// EXAMPLE : UVC_SET_HDCP( pDevice, 2 ); // HDCP_ON_PATTERN (Enable HDCP with MDIN black Pattern)

UVC_EXT_API UVC_QRESULT UVC_GET_HDCP( UVC_DEVICE_HANDLE pDevice /*IN*/, ULONG * pSituation /*OUT*/ );

// EXAMPLE : UVC_GET_HDCP( pDevice, &nSituation );

UVC_EXT_API UVC_QRESULT UVC_ENABLE_OSD( UVC_DEVICE_HANDLE pDevice /*IN*/ );

// EXAMPLE : UVC_ENABLE_OSD( pDevice );

UVC_EXT_API UVC_QRESULT UVC_DISABLE_OSD( UVC_DEVICE_HANDLE pDevice /*IN*/ );

// EXAMPLE : UVC_DISABLE_OSD( pDevice );

UVC_EXT_API UVC_QRESULT UVC_WRITE_KEY_PROTECT_DATA( UVC_DEVICE_HANDLE pDevice /*IN*/, BYTE * pKeyData /*IN*/, ULONG nLength /*IN*/ );

// EXAMPLE : UVC_WRITE_KEY_PROTECT_DATA( pDevice, pKeyData, 16 );

UVC_EXT_API UVC_QRESULT UVC_READ_KEY_PROTECT_DATA( UVC_DEVICE_HANDLE pDevice /*IN*/, BYTE * pKeyData /*IN*/, ULONG nLength /*IN*/ );

// EXAMPLE : UVC_READ_KEY_PROTECT_DATA( pDevice, pKeyData, 16 );

UVC_EXT_API UVC_QRESULT UVC_UPDATE_EDID_DATA( UVC_DEVICE_HANDLE pDevice /*IN*/, BYTE * pEdidData /*IN*/, ULONG nLength /*IN*/, ULONG nInput = UVC_ALL_EDID /*IN*/ );

// EXAMPLE : UVC_UPDATE_EDID_DATA( pDevice, pEdidData, 256 );

UVC_EXT_API UVC_QRESULT UVC_RAED_EDID_DATA( UVC_DEVICE_HANDLE pDevice /*IN*/, BYTE * pEdidData /*IN*/, ULONG nLength /*IN*/, ULONG nInput = UVC_ALL_EDID /*IN*/ );

// EXAMPLE : UVC_RAED_EDID_DATA( pDevice, pEdidData, 256 );

UVC_EXT_API UVC_QRESULT UVC_GET_CABLE_CONNECTED( UVC_DEVICE_HANDLE pDevice /*IN*/, BOOL * pbIsVGAConnected /*OUT*/, BOOL * pbIsHDMIConnected /*OUT*/, BOOL * pbIsDPConnected /*OUT*/ );

// EXAMPLE : UVC_GET_CABLE_CONNECTED( pDevice, &bIsVGAConnected, &bIsHDMIConnected, &bIsDPConnected );

// FORMAT DETECTION - POLLING SPEED PER SECOND
//
UVC_EXT_API UVC_QRESULT UVC_GET_VIDEO_FORMAT_POLLING_READ( UVC_DEVICE_HANDLE pDevice /*IN*/, ULONG * pWidth /*OUT*/, ULONG * pHeight /*OUT*/, ULONG * pFPS /*OUT*/, BOOL * pIs_1000_1001 /*OUT*/, BOOL * pIsInterleaved /*OUT*/, BOOL * pIsSignalLocked /*OUT*/, BOOL * pIsHDCP /*OUT*/, BOOL * pIsHDMI /*OUT*/ );

// EXAMPLE : UVC_GET_VIDEO_FORMAT_POLLING_READ( pDevice, &nWidth, &nHeight, &nFPS, &pIs_1000_1001, &bIsInterleaved, &bIsSignalLocked, &bIsHDCP, &bIsHDMI );

UVC_EXT_API UVC_QRESULT UVC_GET_VIDEO_FORMAT_POLLING_READ_EX( UVC_DEVICE_HANDLE pDevice /*IN*/, ULONG nVideoInput /*IN*/, ULONG * pWidth /*OUT*/, ULONG * pHeight /*OUT*/, ULONG * pFPS /*OUT*/, BOOL * pIs_1000_1001 /*OUT*/, BOOL * pIsInterleaved /*OUT*/, BOOL * pIsSignalLocked /*OUT*/, BOOL * pIsHDCP /*OUT*/ );

// EXAMPLE : UVC_GET_VIDEO_FORMAT_POLLING_READ_EX( pDevice, UVC_HDMI, &nWidth, &nHeight, &nFPS, &pIs_1000_1001, &bIsInterleaved, &bIsSignalLocked, &bIsHDCP );

// EXAMPLE : UVC_GET_VIDEO_FORMAT_POLLING_READ_EX( pDevice, UVC_DISPLAY_PORT, &nWidth, &nHeight, &nFPS, &pIs_1000_1001, &bIsInterleaved, &bIsSignalLocked, &bIsHDCP );

UVC_EXT_API UVC_QRESULT UVC_DOWNLOAD_OUR_FIRMWARE( UVC_DEVICE_HANDLE pDevice /*IN*/, CHAR * pszFileName /*IN*/, DOUBLE * dDownloadProgress = NULL /*OUT*/ );

// EXAMPLE : UVC_DOWNLOAD_OUR_FIRMWARE( pDevice, "D:\\USB_VIDEO_BRIDGE_SCALER_BULK.IMG" );

// EXAMPLE : UVC_DOWNLOAD_OUR_FIRMWARE( pDevice, "D:\\USB_VIDEO_BRIDGE_SCALER_ISO.IMG" );

// EXAMPLE : UVC_DOWNLOAD_OUR_FIRMWARE( pDevice, "D:\\UVC_YUAN_HD_PRO_0123.BIT" );

UVC_EXT_API UVC_QRESULT UVC_DOWNLOAD_CYPRESSS_FIRMWARE( UVC_DEVICE_HANDLE pDevice /*IN*/, CHAR * pszFileName /*IN*/, ULONG nMediaType /*IN*/, DOUBLE * dDownloadProgress = NULL /*OUT*/ );

// EXAMPLE : UVC_DOWNLOAD_CYPRESSS_FIRMWARE( pDevice, "D:\\CYFX_BOOT_PROG.IMG", UVC_RAM );

// EXAMPLE : UVC_DOWNLOAD_CYPRESSS_FIRMWARE( pDevice, "D:\\CYFX_I2C_E2PROM.IMG", UVC_I2C_E2PROM );

// EXAMPLE : UVC_DOWNLOAD_CYPRESSS_FIRMWARE( pDevice, "D:\\CYFX_FLASH_PROG.IMG", UVC_SPI_FLASH );

UVC_EXT_API UVC_QRESULT UVC_READ_I2C_DATA( UVC_DEVICE_HANDLE pDevice /*IN*/, ULONG nDeviceAddress /*IN*/, ULONG nRegisterAddress /*IN*/, BYTE * pData /*IN*/, ULONG nLength /*IN*/ );

// EXAMPLE : UVC_READ_I2C_DATA( pDevice, 0x00, 0x00, pData, 32 );

UVC_EXT_API UVC_QRESULT UVC_WRITE_I2C_DATA( UVC_DEVICE_HANDLE pDevice /*IN*/, ULONG nDeviceAddress /*IN*/, ULONG nRegisterAddress /*IN*/, BYTE * pData /*IN*/, ULONG nLength /*IN*/ );

// EXAMPLE : UVC_WRITE_I2C_DATA( pDevice, 0x00, 0x00, pData, 32 );

UVC_EXT_API UVC_QRESULT UVC_GET_MANUFACTURER_STRING( UVC_DEVICE_HANDLE pDevice /*IN*/, CHAR * pszManufacturer /*OUT*/ );

// EXAMPLE : UVC_GET_MANUFACTURER_STRING( pDevice, pszManufacturer );

UVC_EXT_API UVC_QRESULT UVC_GET_PRODUCT_STRING( UVC_DEVICE_HANDLE pDevice /*IN*/, CHAR * pszProductName /*OUT*/ );

// EXAMPLE : UVC_GET_PRODUCT_STRING( pDevice, pszProductName );

UVC_EXT_API UVC_QRESULT UVC_GET_VIDEO_BRIGHTNESS( UVC_DEVICE_HANDLE pDevice /*IN*/, ULONG * pValue /*OUT*/ );

// EXAMPLE : UVC_GET_VIDEO_BRIGHTNESS( pDevice, &nValue );  // SUPPORT RANGES: 0 ~ 255

UVC_EXT_API UVC_QRESULT UVC_SET_VIDEO_BRIGHTNESS( UVC_DEVICE_HANDLE pDevice /*IN*/, ULONG nValue /*IN*/ );

// EXAMPLE : UVC_SET_VIDEO_BRIGHTNESS( pDevice, 100 );  // SUPPORT RANGES: 0 ~ 255

UVC_EXT_API UVC_QRESULT UVC_GET_VIDEO_CONTRAST( UVC_DEVICE_HANDLE pDevice /*IN*/, ULONG * pValue /*OUT*/ );

// EXAMPLE : UVC_GET_VIDEO_CONTRAST( pDevice, &nValue );  // SUPPORT RANGES: 0 ~ 255

UVC_EXT_API UVC_QRESULT UVC_SET_VIDEO_CONTRAST( UVC_DEVICE_HANDLE pDevice /*IN*/, ULONG nValue /*IN*/ );

// EXAMPLE : UVC_SET_VIDEO_CONTRAST( pDevice, 100 );  // SUPPORT RANGES: 0 ~ 255

UVC_EXT_API UVC_QRESULT UVC_GET_VIDEO_HUE( UVC_DEVICE_HANDLE pDevice /*IN*/, ULONG * pValue /*OUT*/ );

// EXAMPLE : UVC_GET_VIDEO_HUE( pDevice, &nValue );  // SUPPORT RANGES: 0 ~ 255

UVC_EXT_API UVC_QRESULT UVC_SET_VIDEO_HUE( UVC_DEVICE_HANDLE pDevice /*IN*/, ULONG nValue /*IN*/ );

// EXAMPLE : UVC_SET_VIDEO_HUE( pDevice, 100 );  // SUPPORT RANGES: 0 ~ 255

UVC_EXT_API UVC_QRESULT UVC_GET_VIDEO_SATURATION( UVC_DEVICE_HANDLE pDevice /*IN*/, ULONG * pValue /*OUT*/ );

// EXAMPLE : UVC_GET_VIDEO_SATURATION( pDevice, &nValue );  // SUPPORT RANGES: 0 ~ 255

UVC_EXT_API UVC_QRESULT UVC_SET_VIDEO_SATURATION( UVC_DEVICE_HANDLE pDevice /*IN*/, ULONG nValue /*IN*/ );

// EXAMPLE : UVC_SET_VIDEO_SATURATION( pDevice, 100 );  // SUPPORT RANGES: 0 ~ 255

UVC_EXT_API UVC_QRESULT UVC_GET_VIDEO_SHARPNESS( UVC_DEVICE_HANDLE pDevice /*IN*/, ULONG * pValue /*OUT*/ );

// EXAMPLE : UVC_GET_VIDEO_SHARPNESS( pDevice, &nValue );  // SUPPORT RANGES: 0 ~ 255

UVC_EXT_API UVC_QRESULT UVC_SET_VIDEO_SHARPNESS( UVC_DEVICE_HANDLE pDevice /*IN*/, ULONG nValue /*IN*/ );

// EXAMPLE : UVC_SET_VIDEO_SHARPNESS( pDevice, 100 );  // SUPPORT RANGES: 0 ~ 255

UVC_EXT_API UVC_QRESULT UVC_GET_TIME_CODE( UVC_DEVICE_HANDLE pDevice /*IN*/, ULONG * pHour /*OUT*/, ULONG * pMinute /*OUT*/, ULONG * pSecond /*OUT*/, ULONG * pFrame /*OUT*/ );

// EXAMPLE : UVC_GET_TIME_CODE( pDevice, &nHour, &nMinute, &nSecond, &nFrame );  // SUPPORT RANGES: Hour(0 -23), Minute(0 - 59), Second(0 - 59), Frame (0 - 29)

UVC_EXT_API UVC_QRESULT UVC_UPDATE_FIRMWARE( UVC_DEVICE_HANDLE pDevice /*IN*/, CHAR * pszFileName /*IN*/, DOUBLE * dUpdateProgress = NULL /*OUT*/ );

// EXAMPLE : UVC_UPDATE_FIRMWARE( pDevice, "D:\\uvc_yuan_pro_181218_1.IMG", &dUpdateProgress );

// ##############################################################################################################################################################################
// #
// # RESET DEVICE FUNCTIONS
// #
// ##############################################################################################################################################################################

UVC_EXT_API UVC_QRESULT UVC_ENABLE_DEVICE( CHAR * pszHardwareId /*IN*/ );

// EXAMPLE : UVC_ENABLE_DEVICE( "PCI\\VEN_8086&DEV_8C31&SUBSYS_8C318086&REV_05" );

UVC_EXT_API UVC_QRESULT UVC_DISABLE_DEVICE( CHAR * pszHardwareId /*IN*/ );

// EXAMPLE : UVC_DISABLE_DEVICE( "PCI\\VEN_8086&DEV_8C31&SUBSYS_8C318086&REV_05" );

UVC_EXT_API UVC_QRESULT UVC_ENABLE_UVC_DEVICE( ULONG nVendorID /*IN*/, ULONG nProductID /*IN*/ );

// EXAMPLE : UVC_ENABLE_UVC_DEVICE( 0x1164, 0xF57A );

UVC_EXT_API UVC_QRESULT UVC_DISABLE_UVC_DEVICE( ULONG nVendorID /*IN*/, ULONG nProductID /*IN*/ );

// EXAMPLE : UVC_DISABLE_UVC_DEVICE( 0x1164, 0xF57A );

// ##############################################################################################################################################################################
// #
// # OTHER DEVICE FUNCTIONS
// #
// ##############################################################################################################################################################################

UVC_EXT_API UVC_QRESULT UVC_ENABLE_QCAP_CONTROL_FLAG( UVC_DEVICE_HANDLE pDevice /*IN*/ );

// EXAMPLE : UVC_ENABLE_QCAP_CONTROL_FLAG( pDevice );

UVC_EXT_API UVC_QRESULT UVC_DISABLE_QCAP_CONTROL_FLAG( UVC_DEVICE_HANDLE pDevice /*IN*/ );

// EXAMPLE : UVC_DISABLE_QCAP_CONTROL_FLAG( pDevice );

UVC_EXT_API BOOL UVC_DEVICE_IS_READY( UVC_DEVICE_HANDLE pDevice /*IN*/ );

// EXAMPLE : UVC_DEVICE_IS_READY( pDevice );

UVC_EXT_API UVC_QRESULT UVC_GET_OUTPUT_FRAME_COUNT( UVC_DEVICE_HANDLE pDevice /*IN*/, ULONG * pFrameCount /*OUT*/ );

// EXAMPLE : UVC_GET_OUTPUT_FRAME_COUNT( pDevice, &nFrameCount );

UVC_EXT_API UVC_QRESULT UVC_GET_HDR_PACKET( UVC_DEVICE_HANDLE pDevice /*IN*/, UVC_HDR_PACKET * pUvcHdrPacket /*OUT*/, ULONG * pReceiveCount /*OUT*/ );

// EXAMPLE : UVC_GET_HDR_PACKET( pDevice, &pUvcHdrPacket, &nReceiveCount );

UVC_EXT_API UVC_QRESULT UVC_SET_TONE_MAPPING_TABLE( UVC_DEVICE_HANDLE pDevice /*IN*/, SHORT * pToneMappingData /*IN*/, ULONG nLength /*IN*/ );

// EXAMPLE : UVC_SET_TONE_MAPPING_TABLE( pDevice, &pToneMappingTable, 1024 );

UVC_EXT_API UVC_QRESULT UVC_RAED_PASSTHROUFG_MONITOR_EDID_DATA( UVC_DEVICE_HANDLE pDevice /*IN*/, BYTE * pEdidData /*IN*/, ULONG nLength /*IN*/, ULONG nInput = UVC_ALL_EDID /*IN*/ );

// EXAMPLE : UVC_RAED_PASSTHROUFG_MONITOR_EDID_DATA( pDevice, pEdidData, 256 );

#ifdef __cplusplus
}
#endif

#undef DEFVAL

#endif // UVCLINUX_H