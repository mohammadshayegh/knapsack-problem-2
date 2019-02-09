#include <iostream>
#include <windows.h>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include <fstream>

using namespace std;
struct data{
    int maxTime;
    int* weight;
    int* prof;
    vector <int>* indexes;
    int seed;
    int maxWeight;
};
int cpuCores(){
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    return sysinfo.dwNumberOfProcessors;
}
bool isSmaller(float curr,float max)
{
    return curr <= max;
}
bool findIt(vector<int> &in1,int &val)
{
    for(unsigned int i=0; i<in1.size();i++){
        if(in1[i] == val)
            return 1;
    }
    return 0;
}

vector<int>* knapsack(int* knap_max_weight_arr,int *knap_max_pro_arr,int maxTime,int seed,int maxWeight)
{
    srand(seed);

    int currW = 0,startTime = time(0);
    int spentTime = time(0) - startTime,
            index = 0,
            wight = 0,
            prof  = 0;

    vector<int> *in1 = new vector<int>,*in2 = new vector<int>;
    while(isSmaller(spentTime,maxTime))
    {
        do{
            index = rand()%100;
        }while(findIt(*in1,index));

        wight += knap_max_weight_arr[index];
        prof  += knap_max_pro_arr[index];
        in1->push_back(index);

        if(isSmaller(wight,maxWeight))
        {

            if(isSmaller(currW,prof))
            {
                currW = prof;
                in2->clear();
                for(unsigned int i=0; i<in1->size();i++){
                    in2->push_back(in1->at(i));
                }

            }
        }
        else
        {
            wight = 0;
            prof = 0;
            in1->clear();
        }

        spentTime = time(0) - startTime;
    }

    return in2;
}
DWORD WINAPI knapsack_thread(LPVOID param)
{
    data *n = (data*) param;
    n->indexes = knapsack(n->weight,n->prof,n->maxTime,n->seed,n->maxWeight);

    return 0;
}



template <class T>
void initializer(T* b,T* p)
{
    ifstream data;
    data.open("C:\\Users\\Mohammad Sh\\Documents\\knapsack\\dta.txt");

    for(int i=0 ; data >> p [i] >> b[i] ;i++);
    data.close();

}
int main()
{
    int timer,maxW,p[100],w[100],cores = cpuCores();
    initializer(p,w);
    srand(time(0));

    cout << " enter time in secods : ";
    cin >> timer;
    cout << " enter knapsack maxWeight : ";
    cin >> maxW ;
    cout << " pls wait...";

    data *s1 = new data[cores];

    for(int i=0; i<cores; i++)
    {
        s1[i].maxTime = timer;
        s1[i].prof = p;
        s1[i].weight = w;
        s1[i].seed = rand();
        s1[i].maxWeight = maxW;
    }

    HANDLE *h = new HANDLE [cores];

    for(int i=0; i<cores;i++){
        h[i] = CreateThread(NULL, 0, knapsack_thread, &s1[i], 0, NULL);
    }

    for(int i=0; i<cores;i++)
        WaitForSingleObject(h[i],INFINITE);

    for(int i=0; i<cores;i++)
        CloseHandle(h[i]);

    int mProf = 0,mWeight=0,mIndex = 0,mmProf= 0;

    for(int j=0; j<cores; j++)
    {
        mProf=0; mWeight=0;

        for(unsigned int i=0; i<s1[j].indexes->size() ; i++)
        {
            int x = s1[j].indexes->at(i)  ;
            mProf+=p[x];
            mWeight+=w[x];
        }

        if(mmProf < mProf)
        {
            mmProf = mProf;
            mIndex = j;
        }
        cout << "\n max-profit : " << mProf << "\n" << " max-weight : " <<  mWeight << endl ;
    }

    cout << endl << " best core : " << mIndex+1 <<  " \n max-profit indexes : "  << endl ;
    for(unsigned int i=0; i<s1[mIndex].indexes->size() ; i++)
    {
        int x = s1[mIndex].indexes->at(i)  ;
        cout << x << "\t";
    }
    cout << endl << endl;

    cout << "shared Array result  " << endl;

    return 0;
}
