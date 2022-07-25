#pragma once

#ifndef READDRS4_H
#define READDRS4_H

#include "TH1D.h"
#include "TGraph.h"
#include "TString.h"

#include<vector>

class readDRS4
{
    public:
        readDRS4(TString path);
        ~readDRS4();
        void Initializing();
        std::vector<double> GetData(int eventNum, int channelNum);
        void ShowData(int eventNum, int channelNum=0);
        
        void SetDRS4Setting(int boardNum, int channelNum);
        void SetPath(TString path);
        TString GetPath() { return mPath; }

    private:
        TString mPath;
        unsigned short* mData;
        int mLength;
        int mBoardNum = 2;
        int mChannelNum = 4;

};
#endif
