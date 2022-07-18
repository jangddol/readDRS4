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
    std::vector<double> result(1024);
    
    int startbyte;
    if(channelNum == 1)
    {
        startbyte = 32856/2;
    }
    else if(channelNum == 2) startbyte = 34912/2;
    else if(channelNum == 3) startbyte = 36968/2;
    else if(channelNum == 4) startbyte = 39024/2;
    else if(channelNum == 5) startbyte = 41088/2;
    else if(channelNum == 6) startbyte = 43144/2;
    else if(channelNum == 7) startbyte = 45200/2;
    else if(channelNum == 8) startbyte = 47256/2;
    
    startbyte += 8244 * eventNum;
    
    double temp;
    for(int i=0; i<1024; i++)
    {
        cout << "04" << i << endl;
        temp = mData[i + startbyte]; // cast
        result[i] = temp / 65536 - 0.5;
    }
    
    return result;
}
