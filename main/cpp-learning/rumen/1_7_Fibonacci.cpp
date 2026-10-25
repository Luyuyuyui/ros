#include<bits/stdc++.h>
using namespace std;
int f(int n){
    int result=0;
    if (n==1)result=1;
    if(n==2)result=1;
    if(n>2){
        result=f(n-1)+f(n-2);
    }
    return result;
}
int main(){
    int n;
    cin>>n;
for(int i=1;i<=n;i++){
    cout<<f(i)<<endl;
}
    return 0;
}