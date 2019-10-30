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
#include <string>
#include <linux/version.h>
#include "UVCLinuxControl.h"

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

#if LINUX_VERSION_CODE <= KERNEL_VERSION(3,1,0)
/* The first uint8_t of SFEATURE and GFEATURE is the report number */
#define HIDIOCSFEATURE(len)    _IOC(_IOC_WRITE|_IOC_READ, 'H', 0x06, len)
#define HIDIOCGFEATURE(len)    _IOC(_IOC_WRITE|_IOC_READ, 'H', 0x07, len)
#endif

#define _W64 __attribute__((mode (__pointer__)))

typedef _W64 uintptr_t ULONG_PTR,DWORD_PTR;

#define LOBYTE(w)           ((uint8_t)(((DWORD_PTR)(w)) & 0xff))

struct PD570_HID_ID {
	static const int I2C_READ_SET_ID = 9;
	static const int I2C_READ_GET_ID = 10;
	static const int I2C_WRITE_ID = 11;
	static const int RESET_ID = 18;
	static const int GET_RES_ID = 19;
};

struct PD570_HID_CONSTS {
	static const int MAX_COMM_READ_BUFFER_SIZE = 32;
};

int hid_send_feature_report(int fd, const unsigned char* data, size_t length) {
	return ioctl(fd, HIDIOCSFEATURE(length), data);
}

int hid_get_feature_report(int fd, unsigned char* data, size_t length) {
	return ioctl(fd, HIDIOCGFEATURE(length), data);
}

int hid_report_read(int fd, void* p_buffer, size_t i_size) {
	return hid_get_feature_report(fd, (unsigned char*)p_buffer, i_size);
}

int hid_report_write(int fd, void* p_buffer, size_t i_size) {
	return hid_send_feature_report(fd, (const unsigned char*)p_buffer, i_size);
}

bool find_hid_dev_path( int devNum, ULONG pszDeviceID, char* outPath ){
	int err = 0;

	for(int i = 0;;++i) {
		char pszDeviceName[128];
		sprintf(pszDeviceName, "hidraw%d", i);
		std::string strDeviceName = pszDeviceName;
		std::string strDevicePath = "/dev/" + strDeviceName;

		struct stat status;
		if( -1 == stat(strDevicePath.c_str(), &status)) {
			break;
		}

		if( false == S_ISCHR(status.st_mode) ) {
			break;
		}

		int fd = open(strDevicePath.c_str(), O_RDWR, 0);
		// printf("[find_hid_dev_path] OPENING %s -> %d\n", strDevicePath.c_str(), fd);
		if(fd == -1) {
			err = errno;
			printf("%s(%d): open(\"%s\") failed, err=%d\n", __FUNCTION__, __LINE__, strDevicePath.c_str(), err);
			break;
		}

		int _vid = -1, _pid = -1;
		do {
			hidraw_devinfo devinfo;
			err = ioctl(fd, HIDIOCGRAWINFO, &devinfo);
			if (err < 0) {
				err = errno;
				printf("%s(%d): ioctl(HIDIOCGRAWINFO) failed, err=%d\n", __FUNCTION__, __LINE__, err);
				break;
			}

			_vid = devinfo.vendor & 0xFFFF;
			_pid = devinfo.product & 0xFFFF;
		} while(false);
		// printf("[find_hid_dev_path] CLOSING %d\n", fd);
		close(fd);

		// LOGI("!!!!!!!!!!!!!!!!!! %04X %04X ?= %04X:%04X", _vid, _pid, vid, pid);

		int vid = -1, pid = -1;
		bool found = false;
		for(int c = 0;;++c) {
			if(pszDeviceID == 0) break;

			vid = (pszDeviceID & 0xFFFF0000) >> 16;
			pid = (pszDeviceID & 0x0000FFFF);

			if(_vid == vid && _pid == pid) {
				found = true;
				break;
			}
		}

		if(devNum >= 0 && found) {
			if(devNum == 0) {
				strcpy(outPath, strDevicePath.c_str());

				return true;
			}

			--devNum;
		}
	}

	return false;
}

int read_i2c_data(int fd, uint32_t nDeviceAddress, uint32_t nRegisterAddress, uint8_t* pData, uint32_t nLength) {
	uint8_t buffer[5 + PD570_HID_CONSTS::MAX_COMM_READ_BUFFER_SIZE] = {
		PD570_HID_ID::I2C_READ_SET_ID,
		nDeviceAddress,
		LOBYTE(nRegisterAddress),
		nLength,
	};

	int err = hid_report_write(fd, buffer, 4);
	if(err < 0) {
		err = errno;
		printf("%s(%d): hid_report_write(I2C_READ_SET_ID) failed, err=%d\n", __FUNCTION__, __LINE__);

		return err;
	}

	buffer[0] = PD570_HID_ID::I2C_READ_GET_ID;
	err = hid_report_read(fd, buffer, 4 + PD570_HID_CONSTS::MAX_COMM_READ_BUFFER_SIZE);
	if(err < 0) {
		err = errno;
		printf("%s(%d): hid_report_read(I2C_READ_GET_ID) failed, err=%d\n", __FUNCTION__, __LINE__);

		return err;
	}

	memcpy(pData, buffer, nLength);

	return 0;
}

int write_i2c_data(int fd, uint32_t nDeviceAddress, uint32_t nRegisterAddress, uint8_t* pData, uint32_t nLength) {
	uint8_t buffer[4 + PD570_HID_CONSTS::MAX_COMM_READ_BUFFER_SIZE] = {
		PD570_HID_ID::I2C_WRITE_ID,
		nDeviceAddress,
		LOBYTE(nRegisterAddress),
		nLength,
	};
	memcpy(&buffer[4], pData, nLength);
	memset(&buffer[4 + nLength], 0, sizeof(buffer) - 4 - nLength);

#if 0
	for(int i = 0;i < 8;++i) {
		LOGD("!!!! write_i2c_data %d 0x%X", i, (int)buffer[i]);
	}
#endif
	
	int err = hid_report_write(fd, buffer, 4 + PD570_HID_CONSTS::MAX_COMM_READ_BUFFER_SIZE);
	if(err < 0) {
		err = errno;
		printf("%s(%d): hid_report_write(I2C_WRITE_ID) failed, err=%d\n", __FUNCTION__, __LINE__);

		return err;
	}
}

void UVCLinuxControl::Init()
{
	g_bIsValid = true;

	// g_fd = -1;
}

void UVCLinuxControl::Destroy()
{
	// printf("[Destroy] CLOSING %d", g_fd);
	close(g_fd);

	g_bIsValid = false;

	g_fd = -1;
}

UVCLinuxControl::UVCLinuxControl()
{
	g_bIsValid = false;

	g_fd = -1;
}

UVCLinuxControl::~UVCLinuxControl()
{

}

/*
int UVCLinuxControl::OpenDeviceEx(char* pszDeviceID)
{
	int nDeviceID = strtol(pszDeviceID, NULL, 16);

	char strDevicePath[256] = "";

	find_hid_dev_path(0, nDeviceID, &strDevicePath[0]);

	int fd = open(strDevicePath, O_RDWR, 0);
	printf("[OpenDeviceEx] OPENING %s -> %d\n", strDevicePath, fd);

	if(fd < 0){
		return 4;
	}

	g_fd = fd;

	return 0;
}*/
int UVCLinuxControl::OpenDeviceEx(char* strDevicePath)
{
	int fd = open(strDevicePath, O_RDWR, 0);
	// printf("[OpenDeviceEx] OPENING %s -> %d\n", strDevicePath, fd);

	if(fd < 0){
		return 4;
	}

	g_fd = fd;

	return 0;
}


int UVCLinuxControl::GetVideoFormatPollingRead(unsigned long * pWidth, unsigned long * pHeight, unsigned long * pFPS, int * pIs_1000_1001, int * pIsInterleaced, int * pIsSignalLocked, int * pIsHDCP, int * pIsHDMI)
{
	unsigned char pDevice_format[32] = "";

	unsigned long deivce_input_width, deivce_input_height, deivce_input_fps;

	BYTE detail_format = 0x00;

	int deivce_input_interleaved, deivce_input_is_1000_1001, deivce_input_lock, deivce_input_hdcp, deivce_input_hdmi;

	// printf("DEBUG: g_fd: %d\n", g_fd);

	read_i2c_data(g_fd, 0x55, 0x00, (BYTE *)pDevice_format, 7);

	deivce_input_width = (pDevice_format[1]&0xff)| ((pDevice_format[2]&0xff)<<8);
	deivce_input_height = (pDevice_format[3]&0xff)| ((pDevice_format[4]&0xff)<<8);
	deivce_input_fps = pDevice_format[5];

	detail_format = pDevice_format[6];

	if((detail_format & 0x01) == 0x01){deivce_input_interleaved=1;}else{deivce_input_interleaved=0;}
	if((detail_format & 0x02) == 0x02){deivce_input_is_1000_1001=1;}else{deivce_input_is_1000_1001=0;}
	if((detail_format & 0x04) == 0x04){deivce_input_lock=1;}else{deivce_input_lock=0;}
	if((detail_format & 0x08) == 0x08){deivce_input_hdcp=1;}else{deivce_input_hdcp=0;}
	if((detail_format & 0x10) == 0x10){deivce_input_hdmi=1;}else{deivce_input_hdmi=0;}

	// printf("GetVideoFormatPollingRead: %dx%d %dfps, interleaved:%d 1000_1001:%d lock:%d HDCP:%d HDMI:%d\n", deivce_input_width, deivce_input_height, deivce_input_fps, deivce_input_interleaved, deivce_input_is_1000_1001, deivce_input_lock, deivce_input_hdcp, deivce_input_hdmi);

	*pWidth = deivce_input_width;
	*pHeight = deivce_input_height;
	*pFPS = deivce_input_fps;
	*pIsInterleaced = deivce_input_interleaved;
	*pIs_1000_1001 = deivce_input_is_1000_1001;
	*pIsSignalLocked = deivce_input_lock;
	*pIsHDCP = deivce_input_hdcp;
	*pIsHDMI = deivce_input_hdmi;

	return 0;
}

int UVCLinuxControl::WriteI2CData(unsigned long nVideoInput)
{
	int set_input_type = 0x00;

	if(nVideoInput == 0)
	{
		set_input_type = 0x00;

		uint8_t bdata[] = {set_input_type};
		write_i2c_data(g_fd, 0x55, 0x09, bdata, sizeof(bdata));
	}

	if(nVideoInput == 1)
	{
		set_input_type = 0x01;

		uint8_t bdata[] = {set_input_type};
		write_i2c_data(g_fd, 0x55, 0x09, bdata, sizeof(bdata));
	}

	if(nVideoInput == 2)
	{
		set_input_type = 0x02;

		uint8_t bdata[] = {set_input_type};
		write_i2c_data(g_fd, 0x55, 0x09, bdata, sizeof(bdata));
	}

	if(nVideoInput == 3)
	{
		set_input_type = 0x03;

		uint8_t bdata[] = {set_input_type};
		write_i2c_data(g_fd, 0x55, 0x09, bdata, sizeof(bdata));
	}

	if(nVideoInput == 4)
	{
		set_input_type = 0x10;

		uint8_t bdata[] = {set_input_type};
		write_i2c_data(g_fd, 0x55, 0x09, bdata, sizeof(bdata));
	}

	if(nVideoInput == 5)
	{
		set_input_type = 0x11;

		uint8_t bdata[] = {set_input_type};
		write_i2c_data(g_fd, 0x55, 0x09, bdata, sizeof(bdata));
	}

	if(nVideoInput == 6)
	{
		set_input_type = 0x20;

		uint8_t bdata[] = {set_input_type};
		write_i2c_data(g_fd, 0x55, 0x09, bdata, sizeof(bdata));
	}

	if(nVideoInput == 7)
	{
		set_input_type = 0x21;

		uint8_t bdata[] = {set_input_type};
		write_i2c_data(g_fd, 0x55, 0x09, bdata, sizeof(bdata));
	}

	if(nVideoInput == 8)
	{
		set_input_type = 0x30;

		uint8_t bdata[] = {set_input_type};
		write_i2c_data(g_fd, 0x55, 0x09, bdata, sizeof(bdata));
	}

	if(nVideoInput == 9)
	{
		set_input_type = 0x31;

		uint8_t bdata[] = {set_input_type};
		write_i2c_data(g_fd, 0x55, 0x09, bdata, sizeof(bdata));
	}

	if(nVideoInput == 10)
	{
		set_input_type = 0x40;

		uint8_t bdata[] = {set_input_type};
		write_i2c_data(g_fd, 0x55, 0x09, bdata, sizeof(bdata));
	}

	if(nVideoInput == 11)
	{
		set_input_type = 0x41;

		uint8_t bdata[] = {set_input_type};
		write_i2c_data(g_fd, 0x55, 0x09, bdata, sizeof(bdata));
	}

	if(nVideoInput == 12)
	{
		set_input_type = 0x50;

		uint8_t bdata[] = {set_input_type};
		write_i2c_data(g_fd, 0x55, 0x09, bdata, sizeof(bdata));
	}

	if(nVideoInput == 13)
	{
		set_input_type = 0x51;

		uint8_t bdata[] = {set_input_type};
		write_i2c_data(g_fd, 0x55, 0x09, bdata, sizeof(bdata));
	}
	if(nVideoInput == 14)
	{
		set_input_type = 0x60;

		uint8_t bdata[] = {set_input_type};
		write_i2c_data(g_fd, 0x55, 0x09, bdata, sizeof(bdata));
	}

	if(nVideoInput == 15)
	{
		set_input_type = 0x61;

		uint8_t bdata[] = {set_input_type};
		write_i2c_data(g_fd, 0x55, 0x09, bdata, sizeof(bdata));
	}

	if(nVideoInput == 16)
	{
		set_input_type = 0x70;

		uint8_t bdata[] = {set_input_type};
		write_i2c_data(g_fd, 0x55, 0x09, bdata, sizeof(bdata));
	}

	return 0;
}
