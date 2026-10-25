#include<bits/stdc++.h>
using namespace std;

int main()
{
   

    //1.选择排序
    int n;
    cin>>n;
    vector<int>arr(n);
     for(int i=0;i<arr.size();i++){
        cin>>arr[i];
    }
    int start=0,end=n-1;
    while(start<end){
        int max0=start;
        int min0=start;

        for(int i=start;i<=end;i++){
            if(arr[i]<arr[min0])min0=i;
            if(arr[i]>arr[max0])max0=i;
        }
        swap(arr[start],arr[min0]);
        swap(arr[end],arr[max0]);

        start++;
        end--;
    }
     for(int i=0;i<arr.size();i++){
        if(i>0)cout<<" ";
        cout<<arr[i];
    }
    cout<<endl;

    return 0;
}