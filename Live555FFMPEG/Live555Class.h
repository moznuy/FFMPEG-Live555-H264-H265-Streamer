#pragma once

#include "Live555UsageEnv.h"
#include "FFMPEGClass.h"
#include <cstring>
#include <thread>

class Live555Class
{
public:

	Live555Class( FFMPEG * a_Encoder );
	~Live555Class();

	void Start();
	void Stop();

	void SetPriority( int aPriority );
	int GetPriority() const;

	bool IsDone();
	uint32_t GetReturnValue();

	void SetRTSPPort(int PortNo) {
		RTSPPort = PortNo;
	}

	void SetRTSPUserandPassword(char * Username, char * Password) {
		strcpy(RTSPUser, Username);
		strcpy(RTSPPass, Password);
	}

	void SetRTSPAddress(char * streamname) {
		strcpy(RTSP_Address, streamname);
	}

	void SetRTSPDescription(char * streamdescription) {
		strcpy(RTSP_Description, streamdescription);
	}

	void SetEncoder(int m_enc) {
		m_selected_encoder = m_enc;
	}
	void SetMulticast(void) {
		m_multicast = 1;
	}
	void SetUnicast(void) {
		m_multicast = 0;
	}
	void SetRtpPortNum(int m_rtpnum) {
		m_rtpPortNum = m_rtpnum;
	}
protected:

	static unsigned long Link( void *aParam );
	virtual uint32_t Function();
	bool IsStopping() const;

private:
	void SetDefaultValues();
	int RTSPPort;
	char RTSPUser[100];
	char RTSPPass[100];
	char RTSP_Address[1024];
	char RTSP_Description[1024];

	int m_selected_encoder;
	int m_multicast;
	int m_rtpPortNum;

	uint32_t LiveSingleStart();
//	HANDLE mHandle;
	std::thread mHandle;
	uint32_t mID;
	bool mStop;
	uint32_t mReturnValue;
	FFMPEG * m_Encoder;
	char Stop_RTSP_Loop;
};
