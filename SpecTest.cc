#include "SpecTest.hh"


SpecTest::SpecTest(TString path, int totalEventNum)
{
    mReadDRS4 = new readDRS4(path);
    mTotalEventNum = totalEventNum;
    mFitFunc = new TF1("fit", FunctionForFitTTS, -20, 0, 6);
}


bool SpecTest::GetGain(double& output, int eventNum, int channelNum, double threshold)
{
    bool whetherFire = false;
    double result = 0;
    std::vector<double> Vlist = mReadDRS4->GetData(eventNum, channelNum);
    for (int i = 0; i < 1024; i++)
    {
        if (Vlist[i] < threshold)
        {
            whetherFire = true;
            result += Vlist[i] * mDtDRS4;
        }
    }
    output = -result / mOhmDRS4;
    return whetherFire;
}


bool SpecTest::GetGain(double& output, int eventNum, int channelNum1, int channelNum2, double threshold)
{
    bool whetherFire1 = false;
    bool whetherFire2 = false;
    double result = 0;

    std::vector<double> Vlist1 = mReadDRS4->GetData(eventNum, channelNum1);
    std::vector<double> Vlist2 = mReadDRS4->GetData(eventNum, channelNum2);

    for (int i = 0; i < 1024; i++)
    {
        if (Vlist1[i] < threshold)
        {
            whetherFire1 = true;
            result += Vlist1[i] * mDtDRS4;
        }
    }
    for (int i = 0; i < 1024; i++)
    {
        if (Vlist2[i] < threshold)
        {
            whetherFire2 = true;
            result += Vlist2[i] * mDtDRS4;
        }
    }
    if (whetherFire1 && whetherFire2)
    {
        output = -result / mOhmDRS4;
        return true;
    }
    else
    {
        output = 0;
        return false;
    }
}


void SpecTest::GetGainHist(TH1D* hist, int channelNum, double threshold)
{
    double temp;
    for (int i = 0; i < mTotalEventNum; i++)
    {
        GetGain(temp, i, channelNum, threshold);
        hist->Fill(temp);
    }
}


void SpecTest::GetGainHist(TH1D* hist, int channelNum1, int channelNum2, double threshold)
{
    double temp;
    for (int i = 0; i < mTotalEventNum; i++)
    {
        GetGain(temp, i, channelNum1, channelNum2, threshold);
        hist->Fill(temp);
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
        maxV = *min_element(Vlist.begin(), Vlist.end()); 
        for (int i = 0; i < 1024; i++)
        {
            if (maxV == Vlist[i])
            {
                maxindex = i;
                break;
            }
        }
        maxVTrig = *min_element(VlistTrig.begin(), VlistTrig.end());
        for (int i = 0; i < 1024; i++)
        {
            if (maxVTrig == VlistTrig[i])
            {
                maxindexTrig = i;
                break;
            }
        }
        output = mDtDRS4 * (maxindex - maxindexTrig) + GetCenter_5Points(Vlist, maxindex) - GetCenter_5Points(VlistTrig, maxindexTrig);
    }
    return whetherFire;
}


double SpecTest::GetTTS(TH1D* hist, int channelNum, int trigChannelNum, double threshold)
{
    double result = 0;
    double temp;
    bool tempFire;
    for (int i = 0; i < mTotalEventNum; i++)
    {
        tempFire = SpecTest::GetTT(temp, i, channelNum, trigChannelNum, threshold);
        if (tempFire) hist->Fill(temp);
    }
    mFitFunc->SetParameters(hist->GetEntries(), hist->GetMean(), hist->GetStdDev(),
                            hist->GetEntries() * 0.01, hist->GetMean() + hist->GetStdDev(), hist->GetStdDev());
    mFitFunc->SetParNames("Coef_Main", "Mean_Main", "Sigma_Main", "Coef_Tail", "Mean_Tail", "Sigma_Tail");
    hist->Fit("fit", "0");
    TF1* fit = hist->GetFunction("fit");
    result = TMath::Abs(fit->GetParameter("Sigma_Main"));
    return result;
}


double SpecTest::GetQE(int channelNum, double threshold)
{
    int fireNumber = 0;
    int whetherFire;
    double temp;
    for (int ev = 0; ev < mTotalEventNum; ev++)
    {
        if (GetGain(temp, ev, channelNum, threshold))
        {
            fireNumber += 1;
        }
    }
    return (double)fireNumber / (double)mTotalEventNum;
}


double SpecTest::FunctionForFitTTS(double* x, double* par)
{
    // gauss + gauss (two mean + two sigma + two amplitude)
    double mainPart = par[0] * TMath::Gaus(x[0], par[1], par[2], kTRUE);
    double tailPart = par[3] * TMath::Gaus(x[0], par[4], par[5], kTRUE);
    return mainPart + tailPart;
}


double SpecTest::GetCenter_5Points(std::vector<double> Vlist, int peakIndex)
{
    int i = peakIndex;
    double dt = mDtDRS4;
    double sum_y = Vlist[i - 2] + Vlist[i - 1] + Vlist[i] + Vlist[i + 1] + Vlist[i + 2];
    double sum_iy = -2 * Vlist[i - 2] - Vlist[i - 1] + Vlist[i + 1] + 2 * Vlist[i + 2];
    double sum_i2y = 4 * Vlist[i - 2] + Vlist[i - 1] + Vlist[i + 1] + 4 * Vlist[i + 2];
    double result = -dt * 7 * sum_iy / (sum_i2y - 2 * sum_y) / 10;
    return result;
}