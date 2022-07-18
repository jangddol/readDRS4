#pragma once

#ifndef READDRS4_H
#define READDRS4_H

#include<vector>

class readDRS4
{
    public:
        readDRS4(TString path){SetPath(path);}
        ~readDRS4();
        void Initializing();
        std::vector<double> GetData(int eventNum, int channelNum);
        
        void SetPath(TString path){mPath = path;}
        TString GetPath(){return mPath;}
    private:
        TString mPath;
        unsigned short* mData;
        int mLength;

};
#endif
