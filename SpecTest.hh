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
	
	bool GetGain(double& output, int eventNum, int channelNum, double threshold);  // for gen2
	bool GetGain(double& output, int eventNum, int channelNum1, int channelNum2, double threshold); // for gen1
	void GetGainHist(TH1D* hist, int channelNum, double threshold); // for gen2
	void GetGainHist(TH1D* hist, int channelNum1, int channelNum2, double threshold); // for gen1
	bool GetTT(double& output, int eventNum, int channelNum, int trigChannelNum, double threshold);
	double GetTTS(TH1D* hist, int channelNum, int trigChannelNum, double threshold);
	double GetQE(int channelNum, double threshold);

	readDRS4* mReadDRS4;

private:
	static double FunctionForFitTTS(double* x, double* par);
	static double GetCenter_5Points(std::vector<double> Vlist, int peakIndex);

	TF1* mFitFunc;
	int mTotalEventNum;
	constexpr static double mDtDRS4 = 1 / 5.12; // ns
	constexpr static double mOhmDRS4 = 50; // Ohm
};
#endif