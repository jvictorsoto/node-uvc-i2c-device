class UVCLinuxControl
{
public:
	UVCLinuxControl();
	~UVCLinuxControl();

	void Init();

	void Destroy();

	int OpenDeviceEx(char* pszDeviceID);

	int GetVideoFormatPollingRead(unsigned long * pWidth, unsigned long * pHeight, unsigned long * pFPS, int * pIs_1000_1001, int * pIsInterleaced, int * pIsSignalLocked, int * pIsHDCP, int * pIsHDMI);

	int WriteI2CData(unsigned long nVideoInput);

public:

	bool g_bIsValid;

	int g_fd;
	
};
