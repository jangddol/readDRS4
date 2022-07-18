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
auto drs4 = new readDRS4(path);
drs4 -> Initializing();
int eventNum = 8;
int channelNum = 1; // 1 ~ 8 available, 1 ~ 4 : first board, 5 ~ 8 : second board
auto data = drs4 -> GetData(eventNum, channelNum); // type of data : std::vector<double>(1024)

```

