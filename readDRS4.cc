#include "readDRS4.hh"
#include <vector>


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
        cout << "04" << i << endl;
        temp = mData[i + firstEventIndex]; // cast
        result[i] = temp / 65536 - 0.5;
    }
    
    return result;
}
