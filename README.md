# readDRS4
DRS4 binary file reader for double board DRS4

This class need to be used with ROOT(CERN).

-----------------------
## How to download
> git clone https://github.com/jangddol/readDRS4.git


-----------------------
## How to use
```C++
TString path = "../07-11-2022 1126.dat"
int totalEventNum = 20000;

SpecTest* spectest = new SpecTest(path, totalEventNum);
    
TH1D* hGain = new TH1D("hGain", "", 100, 0.1, 3.5);
TH1D* hTTS = new TH1D("hTTS", "", 30, -6 - 30 / 5.12, -6);
    
double threshold = -4; // mV
int channelNum = 1; // DRS4 Channel Number : 1 ~ 8 (1 ~ 4 : first board, 5 ~ 8 : second board)
int channelNum1 = 1;
int channelNum2 = 2;
int trigChannelNum = 3;
    
spectest->GetGainHist(hGain, channelNum1, channelNum2, threshold);
double TTS = spectest->GetTTS(hTTS, channelNum, trigChannelNum, threshold);
double QE = spectest->GetQE(channelNum, threshold);



```

