//==========================================================================
// Live555FFMPEG.cpp : Defines the entry point for the console application.
//==========================================================================
// Main Example for Live555 and FFMPEG
//
//==========================================================================

#include "FFMPEGClass.h"
#include "libavcodec/avcodec.h"
#include <csignal>

volatile bool quit = false;
void my_handler(int s){
	printf("Caught signal %d\n",s);
	quit = true;
}


int main(int argc, char* argv[])
{
	//Some local variables
	int mVidWidth = 640;
	int mVidHeight = 480;

	//Create Class
	FFMPEG * mFFMPEG = new FFMPEG();
	
	//Setup Encoder
	mFFMPEG->SetVideoResolution(mVidWidth, mVidHeight);	//Set Frame Resolution
	mFFMPEG->m_AVIMOV_BPS = 5000000;		//Set Bitrate (this is extremely important to get correct)

	/*
	//--------------------
	//Optional settings -- Uncomment to use
	//--------------------
	mFFMPEG->SetRTSPPort(8554);
	mFFMPEG->SetRTSPUserandPassword("admin", "admin");
	mFFMPEG->SetRTSPAddress("multicast");
	mFFMPEG->SetRTSPDescription("This is the stream description");
	mFFMPEG->SetRtpPortNum(20000);

	//Encoder options
	mFFMPEG->SetEncoder(AV_CODEC_ID_H264);
	//Other encoders
	// AV_CODEC_ID_H265
	// AV_CODEC_ID_H264
	// AV_CODEC_ID_MPEG4
	// AV_CODEC_ID_MPEG2VIDEO

	mFFMPEG->SetMulticast();
	mFFMPEG->SetUnicast();
	//--------------------
	*/

	//Start Encoding and Streaming
	mFFMPEG->SetRTSPPort(8555);
	mFFMPEG->Start();

	struct sigaction sigIntHandler{};

	sigIntHandler.sa_handler = my_handler;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;
	sigaction(SIGINT, &sigIntHandler, NULL);
	sigaction(SIGTERM, &sigIntHandler, NULL);

	//Temporary Frame
	unsigned char * TempRGBFrame = new unsigned char[mVidWidth * mVidHeight * 3];

	//Main Random Frame Generation loop

	int circleX = 50;
	int circleY = 50;
	int circleR = 40;
	int vx = 4;
	int vy = 3;
	while (!quit) {
		for (int i = 0; i < mVidWidth * mVidHeight; i++) {
		    int x = i % mVidWidth;
		    int y = i / mVidWidth;
            TempRGBFrame[i * 3 + 0] = (3 * x + y) % 256;  // R
            TempRGBFrame[i * 3 + 1] = (2 * x + 5 * y) % 256; // G
            TempRGBFrame[i * 3 + 2] = (4 * x + 3 * y) % 256; // B
            if ((x - circleX) * (x - circleX) + (y - circleY) * (y - circleY) <= circleR * circleR) {
				TempRGBFrame[i * 3 + 0] = 0;
				TempRGBFrame[i * 3 + 1] = 0;
				TempRGBFrame[i * 3 + 2] = 0;
            }
//			TempRGBFrame[i] = (i/3) * 5 / 2 % 256;
		}
		mFFMPEG->SendNewFrame((char*)TempRGBFrame);
		circleX+=vx;
		circleY+=vy;
		if (circleX + circleR > mVidWidth || circleX - circleR < 0 )
			vx *= -1;
		if (circleY + circleR > mVidHeight || circleY - circleR < 0 )
			vy *= -1;
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	printf("Exiting");

	//Clean up
	mFFMPEG->Stop();
	while (!mFFMPEG->IsDone()) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	delete mFFMPEG;
	delete[] TempRGBFrame;
	return 0;
}

