#include "readDRS4.cc"
#include "TString.h"
#include "TMath.h"
#include "TGraph.h"
#include "TH1D.h"

void readdat()
{
    gStyle -> SetOptStat(kFALSE);

    readDRS4 * drs4 = new readDRS4("../07-14-2022 1735.dat");
    drs4 -> Initializing();
    auto data = drs4 -> GetData(1, 1);

    TGraph * gTest = new TGraph();
    for(int i=0; i<1024; i++)
    {
        cout << data[i] << endl;
        gTest -> SetPoint(i, (double)i/5.12, 1000 * data[i]);
    }

    TH1D * hFrame = new TH1D("hFrame", "", 100, 0, 1024/5.12);
    hFrame->GetXaxis()->SetTitle(" time (ns) ");
    hFrame->GetYaxis()->SetTitle(" Voltage (mV)");
    hFrame->GetXaxis()->SetTitleSize(0.047);
    hFrame->GetYaxis()->SetTitleSize(0.047);
    hFrame->GetXaxis()->SetTitleOffset(1.1);
    hFrame->GetYaxis()->SetTitleOffset(1.1);
    hFrame->GetXaxis()->CenterTitle();
    hFrame->GetYaxis()->CenterTitle();
    hFrame->GetXaxis()->SetLabelSize(0.038);
    hFrame->GetYaxis()->SetLabelSize(0.038);
    hFrame->GetYaxis()->SetNdivisions(505);
    hFrame->GetYaxis()->SetRangeUser(-500, 500);
    hFrame->Draw();
    gTest -> Draw("l");
}

