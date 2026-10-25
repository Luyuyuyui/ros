#include<bits/stdc++.h>
using namespace std;

bool isop(char c){
    return c=='+'||c=='-'||c=='*'||c=='/';
}

double resolve(string s,double a,double b){
    if(s=="+")return a+b;
    if(s=="-")return a-b;
    if(s=="*")return a*b;
    if(s=="/"){
        if(b==0)return -1;
        else return a/b;
    }
    return -1;
}


int main(){
    string s;
    cout<<"请输入单项数学式：";
    getline(cin,s);

   int oppos=-1;
   char opchar=' ';
   for(int i=1;i<s.length()-1;i++){
    char c=s[i];
    if(isop(c)){
        if(i>0&&isdigit(s[i-1])){
            oppos=i;
            opchar=c;
            break;
        }
     }
   }
   string s1=s.substr(0,oppos);
   string s2=s.substr(oppos+1);
   string opstr(1,opchar);
   
   double n1=stod(s1);
   double n2=stod(s2);
   double result;
   result=resolve(opstr,n1,n2);

   cout<<n1<<" "<<opstr<<" "<<n2<<"="<<result<<endl;

    return 0;
}