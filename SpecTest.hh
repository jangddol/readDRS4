#pragma once

#ifndef SPECTEST_HH
#define SPECTEST_HH

#include "TH1D.h"
#include "TString.h"
#include "TF1.h"
#include "readDRS4.cc"
#include <vector>


class SpecTest
{
public:
	SpecTest(TString path, int totalEventNum);
	
	double GetGain(int eventNum, int channelNum, double threshold);
	double GetGain(int eventNum, int channelNum1, int channelNum2, double threshold);
	void GetGainHist(TH1D* hist, int channelNum, double threshold);
	void GetGainHist(TH1D* hist, int channelNum1, int channelNum2, double threshold);
	bool GetTT(double& output, int eventNum, int channelNum, int trigChannelNum, double threshold);
	double GetTTS(TH1D* hist, int channelNum, int trigChannelNum, double threshold);
	double GetQE(int channelNum, double threshold);

	double FunctionForFitTTS(double* x, double* par);

private:
	readDRS4* mReader;
	TF1* mFitFunc;
	int mTotalEventNum;
	const static double mDtDRS4 = 1 / 5.12; // ns
	const static double mOhmDRS4 = 50; // Ohm
};
#endif