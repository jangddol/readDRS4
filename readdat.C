#include "SpecTest.cc"
#include "TString.h"
#include "TMath.h"
#include "TGraph.h"
#include "TH1D.h"

void readdat()
{
    SpecTest* spectest = new SpecTest("../07-12-2022 1727.dat", 20000);
    
    TH1D* hGain = new TH1D("hGain", "", 100, 0.1, 3.5);
    TH1D* hTTS = new TH1D("hTTS", "", 100, -11, -7);
    
    // spectest->mReadDRS4->ShowData(19926, 0);

    double threshold = -4; // mV
    int channelNum = 1;
    int channelNum1 = 1;
    int channelNum2 = 2;
    int trigChannelNum = 3;
    
    double temp1, temp2, temp3;
    bool tempFire;
    // spectest->GetGainHist(hGain, channelNum1, channelNum2, threshold);
    for (int i = 0; i < 20000; i++)
    {
        tempFire = spectest->GetGain(temp1, i, 1, 2, threshold);
        if (tempFire)
        {
            spectest->GetGain(temp2, i, 5, 6, threshold);
            spectest->GetGain(temp3, i, 7, 8, threshold);
            hGain->Fill(temp1 + temp2 + temp3);
        }        
    }
    double TTS = spectest->GetTTS(hTTS, channelNum, trigChannelNum, threshold);
    double QE = spectest->GetQE(channelNum, threshold);

    cout << "TTS : " << TTS << endl;
    cout << "QE  : " << QE << endl;

    TCanvas* can = new TCanvas("can", "", 1200, 600);
    gPad->SetLeftMargin(0.12);
    gPad->SetBottomMargin(0.12);
    gPad->SetRightMargin(0.12);
    gPad->SetTopMargin(0.05);

    can->Divide(2, 1);
    can->cd(1);

    hGain->GetXaxis()->SetTitle(" Gain (pC) ");
    hGain->GetYaxis()->SetTitle(" Count");
    hGain->GetXaxis()->SetTitleSize(0.047);
    hGain->GetYaxis()->SetTitleSize(0.047);
    hGain->GetXaxis()->SetTitleOffset(1.1);
    hGain->GetYaxis()->SetTitleOffset(1.1);
    hGain->GetXaxis()->CenterTitle();
    hGain->GetYaxis()->CenterTitle();
    hGain->GetXaxis()->SetLabelSize(0.038);
    hGain->GetYaxis()->SetLabelSize(0.038);
    hGain->GetYaxis()->SetNdivisions(505);
    hGain->GetYaxis()->SetRangeUser(0, 500);
    hGain->Draw();

    can->cd(2);

    hTTS->GetXaxis()->SetTitle(" time (ns) ");
    hTTS->GetYaxis()->SetTitle(" Count");
    hTTS->GetXaxis()->SetTitleSize(0.047);
    hTTS->GetYaxis()->SetTitleSize(0.047);
    hTTS->GetXaxis()->SetTitleOffset(1.1);
    hTTS->GetYaxis()->SetTitleOffset(1.1);
    hTTS->GetXaxis()->CenterTitle();
    hTTS->GetYaxis()->CenterTitle();
    hTTS->GetXaxis()->SetLabelSize(0.038);
    hTTS->GetYaxis()->SetLabelSize(0.038);
    hTTS->GetYaxis()->SetNdivisions(505);
    hTTS->GetYaxis()->SetRangeUser(0, 800);
    hTTS->Draw();
    hTTS->GetFunction("fit")->Draw("Same");
}

