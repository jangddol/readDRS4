#include "SpecTest.hh"

SpecTest::SpecTest(TString path, int totalEventNum)
{
    mReader = new readDRS4(path);
    mtotalEventNum = totalEventNum;
    mFitFunc = new TF1("fit", FunctionForFitTTS, -20, 0, 6);
}


double SpecTest::GetGain(int eventNum, int channelNum, double threshold)
{
    double result = 0;
    std::vector<double> Vlist = mReadDRS4->GetData(eventNum, channelNum);
    for (int i = 0; i < 1024; i++)
    {
        if (Vlist[i] < threshold)
        {
            result += Vlist[i] * mDtDRS4;
        }
    }
    return -result / mOhmDRS4;
}


double SpecTest::GetGain(int eventNum, int channelNum1, int channelNum2, double threshold)
{
    double result = 0;
    std::vector<double> Vlist1 = mReadDRS4->GetData(eventNum, channelNum1);
    std::vector<double> Vlist2 = mReadDRS4->GetData(eventNum, channelNum2);
    for (int i = 0; i < 1024; i++)
    {
        if (Vlist1[i] < threshold)
        {
            result += Vlist1[i] * mDtDRS4;
        }
        if (Vlist2[i] < threshold)
        {
            result += Vlist2[i] * mDtDRS4;
        }
    }
    return -result / mOhmDRS4;
}


void SpecTest::GetGainHist(TH1D* hist, int channelNum, double threshold)
{
    for (int i = 0; i < mTotalEventNum; i++)
    {
        hist->Fill(GetGain(i, channelNum, threshold));
    }
}


void SpecTest::GetGainHist(TH1D* hist, int channelNum1, int channelNum2, double threshold)
{
    for (int i = 0; i < mTotalEventNum; i++)
    {
        hist->Fill(GetGain(i, channelNum1, channelNum2, threshold));
    }
}


bool SpecTest::GetTT(double& output, int eventNum, int channelNum, int trigChannelNum, double threshold)
{
    bool whetherFire = false;
    std::vector<double> Vlist = mReadDRS4->GetData(eventNum, channelNum);
    std::vector<double> VlistTrig = mReadDRS4->GetData(eventNum, trigChannelNum);
    for (int i = 0; i < 1024; i++)
    {
        if (Vlist[i] < threshold)
        {
            whetherFire = true;
            break;
        }
    }

    double maxV, maxVTrig;
    int maxindex = 0;
    int maxindexTrig = 0;
    if (whetherFire)
    {
        maxV = min(Vlist); // minimum of std::vector
        for (int i = 0; i < 1024; i++)
        {
            if (maxV == Vlist[i])
            {
                maxindex = i;
                break;
            }
        }
        maxVTrig = min(VlistTrig);
        for (int i = 0; i < 1024; i++)
        {
            if (maxVTrig == VlistTrig[i])
            {
                maxindexTrig = i;
                break;
            }
        }
        output = (maxindex - maxindexTrig) * mDtDRS4;
    }
    return whetherFire;
}


double SpecTest::GetTTS(TH1D* hist, int channelNum, int trigChannelNum, double threshold)
{
    double temp;
    for (int i = 0; i < mTotalEventNum; i++)
    {
        SpecTest::GetTT(temp, i, channelNum, trigChannelNum, threshold);
        hist->Fill(temp);
    }
    return hist->GetStdDev();
}


double SpecTest::FunctionForFitTTS(double* x, double* par)
{
	// gauss + poisson (two mean + two sigma + two amplitude)
	double mainPart = par[0] * TMath::Gaus(x[0], par[1], par[2], kTRUE);
	double tailPart = par[3] * TMath::Gaus(x[0], par[4], par[5], kTRUE);
	return mainPart + tailPart;
}