#include "readDRS4.hh"
#include <vector>


readDRS4::readDRS4(TString path)
{
    SetPath(path);
    Initializing();
}


void readDRS4::Initializing()
{
    int length;
    ifstream f;
    f.open(mPath, ios::binary);
    f.seekg(0, ios::end);
    length = f.tellg();
    f.seekg(0, ios::beg);

    unsigned short* buffer = new unsigned short[length];
    f.read( (char*)buffer, length);

    mData = buffer;
    mLength = length;

    f.close();
}


std::vector<double> readDRS4::GetData(int eventNum, int channelNum)
{
    int boardNum = (channelNum - 1) / mChannelNum + 1;
    channelNum = (channelNum - 1) % mChannelNum;
    if(boardNum > mBoardNum) cout << "Wrong Setting" << endl;

    std::vector<double> result(1024);

    int firstEventIndex;
    if(channelNum == 1)
    {
        firstEventIndex = 32856/2;
    }
    else if(channelNum == 2) firstEventIndex = 34912/2;
    else if(channelNum == 3) firstEventIndex = 36968/2;
    else if(channelNum == 4) firstEventIndex = 39024/2;
    else if(channelNum == 5) firstEventIndex = 41088/2;
    else if(channelNum == 6) firstEventIndex = 43144/2;
    else if(channelNum == 7) firstEventIndex = 45200/2;
    else if(channelNum == 8) firstEventIndex = 47256/2;

    int Event_DeltaByte = 1028 * mChannelNum * mBoardNum + 4 * (mBoardNum - 1) + 16;
    firstEventIndex = 2 * (2 + mBoardNum + mBoardNum * mChannelNum * 1025 + 10) 
              + (boardNum - 1) * (mChannelNum * 1028 + 4) + channelNum * 1028;
    firstEventIndex += eventNum * Event_DeltaByte;

    double temp;
    for(int i=0; i<1024; i++)
    {
        temp = mData[i + firstEventIndex]; // cast
        result[i] = (temp / 65536 - 0.5) * 1000;
    }

    return result;
}


void readDRS4::ShowData(int eventNum, int channelNum)
{
    gStyle->SetOptStat(0);

    double maxV;
    double minV;

    if (channelNum == 0)
    {
        TGraph* gVoltage[mBoardNum * mChannelNum];
        TH1D* hFrame[mBoardNum * mChannelNum];

        for (int hNum = 0; hNum < mBoardNum * mChannelNum; hNum++)
        {
            std::vector<double> Vlist = GetData(eventNum, hNum + 1);

            maxV = *max_element(Vlist.begin(), Vlist.end());
            minV = *min_element(Vlist.begin(), Vlist.end());

            hFrame[hNum] = new TH1D("", "", 100, 0, 1024);
            if ((hNum + 1) % mChannelNum == 0)
            {
                hFrame[hNum]->GetXaxis()->SetTitle("index");
                hFrame[hNum]->GetXaxis()->SetTitleOffset(1.1);
                hFrame[hNum]->GetXaxis()->CenterTitle();
                hFrame[hNum]->GetXaxis()->SetTitleSize(0.067);
                hFrame[hNum]->GetXaxis()->SetLabelSize(0.058);
                hFrame[hNum]->GetXaxis()->SetNdivisions(505);
            }
            else
            {
                hFrame[hNum]->GetXaxis()->SetTitleSize(0);
                hFrame[hNum]->GetXaxis()->SetLabelSize(0);
            }

            hFrame[hNum]->GetYaxis()->SetTitle("Voltage(mV)");
            hFrame[hNum]->GetYaxis()->SetTitleSize(0.067);
            hFrame[hNum]->GetYaxis()->SetTitleOffset(0.5);
            hFrame[hNum]->GetYaxis()->CenterTitle();
            hFrame[hNum]->GetYaxis()->SetLabelSize(0.058);
            hFrame[hNum]->GetYaxis()->SetNdivisions(505);

            hFrame[hNum]->GetYaxis()->SetRangeUser(minV - 0.1 * (maxV - minV), maxV + 0.1 * (maxV - minV));

            gVoltage[hNum] = new TGraph();
            for (int i = 0; i < 1024; i++)
            {
                gVoltage[hNum]->SetPoint(i, i, Vlist[i]);
            }

        }

        TCanvas* can = new TCanvas("showdata", "showdata", 1200, 800);

        can->Divide(mBoardNum, mChannelNum, 0.01, 1e-17);

        for (int hNum = 0; hNum < mBoardNum * mChannelNum; hNum++)
        {
            int rowNum, colNum;
            rowNum = hNum / mBoardNum;
            colNum = hNum % mBoardNum;
            int hNum_convert = colNum * mChannelNum + rowNum;
            can->cd(hNum + 1);
            
            if (rowNum == 0)
            {
                gPad->SetBottomMargin(0);
            }
            else if (rowNum == mChannelNum - 1)
            {
                gPad->SetTopMargin(0);
                gPad->SetBottomMargin(0.22);
            }
            else
            {
                gPad->SetBottomMargin(0);
                gPad->SetTopMargin(0);
            }

            hFrame[hNum_convert]->Draw();
            gVoltage[hNum_convert]->Draw("l");
        }
    }
    else
    {
        std::vector<double> Vlist = GetData(eventNum, channelNum);

        maxV = *max_element(Vlist.begin(), Vlist.end());
        minV = *min_element(Vlist.begin(), Vlist.end());

        TCanvas* can = new TCanvas("showdata", "showdata", 1200, 800);
        gPad->SetLeftMargin(0.12);
        gPad->SetBottomMargin(0.12);
        gPad->SetRightMargin(0.12);
        gPad->SetTopMargin(0.05);

        TGraph* gVoltage = new TGraph();
        TH1D* hFrame = new TH1D("", "", 100, 0, 1024);
        hFrame->GetXaxis()->SetTitle("index");
        hFrame->GetYaxis()->SetTitle("Voltage(mV)");
        hFrame->GetXaxis()->SetTitleSize(0.047);
        hFrame->GetYaxis()->SetTitleSize(0.047);
        hFrame->GetXaxis()->SetTitleOffset(1.1);
        hFrame->GetYaxis()->SetTitleOffset(1.1);
        hFrame->GetXaxis()->CenterTitle();
        hFrame->GetYaxis()->CenterTitle();
        hFrame->GetXaxis()->SetLabelSize(0.038);
        hFrame->GetYaxis()->SetLabelSize(0.038);
        hFrame->GetYaxis()->SetNdivisions(505);
        hFrame->GetYaxis()->SetRangeUser(minV - 0.1 * (maxV - minV), maxV + 0.1 * (maxV - minV));
        hFrame->Draw();

        for (int i = 0; i < 1024; i++)
        {
            gVoltage->SetPoint(i, i, Vlist[i]);
        }
        gVoltage->Draw("l");
    }
}


void readDRS4::SetDRS4Setting(int boardNum, int channelNum)
{
    mBoardNum = boardNum;
    mChannelNum = channelNum;
    Initializing();
}


void readDRS4::SetPath(TString path)
{
    mPath = path;
    Initializing();
}