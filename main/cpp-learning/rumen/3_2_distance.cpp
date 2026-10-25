#include<bits/stdc++.h>
using namespace std;
//改良四则运算器
bool isop(char c){
    return c=='+'||c=='-'||c=='*'||c=='/';
}

template<class T1,class T2>
T1 add(T1&a,T2&b){
    return a+b;
}

template<class T1,class T2>
T1 minus1(T1&a,T2&b){
    return a-b;
}

template<class T1,class T2>
T1 multiply(T1&a,T2&b){
   return a*b;
}

template<class T1,class T2>
T1 divide(T1&a,T2&b){
    if(b==0){
        cout<<"除数不能为0！"<<endl;
        return -1;
    }
    return a/b;
}

template<class T1,class T2>
T1 resolve(T1 a,T2 b,string s)
{
    if(s=="+") return add(a,b);
    if(s=="-")return minus1(a,b);
    if(s=="*")return multiply(a,b);
    if(s=="/")return divide(a,b);
    cout<<"不支持操作符！"<<endl;
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
   result=resolve(n1,n2,opstr);

   cout<<n1<<" "<<opstr<<" "<<n2<<"="<<result<<endl;

    return 0;
}