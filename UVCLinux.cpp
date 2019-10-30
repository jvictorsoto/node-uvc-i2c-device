#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/hidraw.h>
#include <stdint.h>
#include <linux/version.h>
#include <string>
#include "UVCLinux.h"
#include "UVCLinuxControl.h"

UVC_QRESULT UVC_OPEN_DEVICE_EX( CHAR * strDevicePath /*IN*/, BOOL * bIsOurDevice /*OUT*/, UVC_DEVICE_HANDLE * ppDevice /*OUT*/ )
{
	UVCLinuxControl *pUVCLinuxControl = new UVCLinuxControl();

	if( pUVCLinuxControl == NULL ){
		delete pUVCLinuxControl;
		return UVC_RS_ERROR_GENERAL;
	}

	int qres = pUVCLinuxControl->OpenDeviceEx(strDevicePath);
	if(qres != UVC_RS_SUCCESSFUL) {
		delete pUVCLinuxControl;
		return UVC_RS_DEVICE_CREATE_FAILED;
	}

	pUVCLinuxControl->Init();
	*ppDevice = pUVCLinuxControl;

	// printf("CREATE SUCCESSFUL\n");

	return UVC_RS_SUCCESSFUL;
}

UVC_QRESULT UVC_CLOSE_DEVICE( UVC_DEVICE_HANDLE pDevice /*IN*/ )
{
	UVCLinuxControl *pUVCLinuxControl = (UVCLinuxControl *) (pDevice);

	if( pUVCLinuxControl == NULL ) {
		delete pUVCLinuxControl;
		return UVC_RS_ERROR_GENERAL;
	}

	if( pUVCLinuxControl->g_bIsValid != true ){
		delete pUVCLinuxControl;
		return UVC_RS_ERROR_GENERAL;
	}

	pUVCLinuxControl->Destroy();

	delete pUVCLinuxControl;

	// printf("UVC_CLOSE_DEVICE SUCCESSFUL\n");

	return UVC_RS_SUCCESSFUL;
}

UVC_EXT_API UVC_QRESULT UVC_SET_VIDEO_INPUT( UVC_DEVICE_HANDLE pDevice /*IN*/, ULONG nVideoInput /*IN*/ )
{
	UVCLinuxControl *pUVCLinuxControl = (UVCLinuxControl *) (pDevice);

	int set_input_type = 0x00;

	if( pUVCLinuxControl->g_bIsValid != true ){
		return UVC_RS_ERROR_GENERAL;
	}

	pUVCLinuxControl->WriteI2CData(nVideoInput);
	
	return UVC_RS_SUCCESSFUL;
}

UVC_EXT_API UVC_QRESULT UVC_GET_VIDEO_FORMAT_POLLING_READ( UVC_DEVICE_HANDLE pDevice, ULONG * pWidth, ULONG * pHeight, ULONG * pFPS, BOOL * pIs_1000_1001, BOOL * pIsInterleaved, BOOL * pIsSignalLocked, BOOL * pIsHDCP, BOOL * pIsHDMI )
{
	UVCLinuxControl *pUVCLinuxControl = (UVCLinuxControl *) (pDevice);

	if( pUVCLinuxControl->g_bIsValid != true ){
		return UVC_RS_ERROR_GENERAL;
	}

	pUVCLinuxControl->GetVideoFormatPollingRead(pWidth, pHeight, pFPS, pIs_1000_1001, pIsInterleaved, pIsSignalLocked, pIsHDCP, pIsHDMI);

	return UVC_RS_SUCCESSFUL;
}


