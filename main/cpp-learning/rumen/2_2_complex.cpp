#include<bits/stdc++.h>
using namespace std;
//复数运算
class calculator
{
public:
    calculator():m_a(0),m_b(0){}
    calculator(int a,int b){
        m_a=a;
        m_b=b;
    }
    int m_a;
    int m_b;

};
calculator operator+(const calculator&p1,const calculator&p2){
    calculator temp;
    temp.m_a=p1.m_a+p2.m_a;
    temp.m_b=p1.m_b+p2.m_b;
    return temp;
}

calculator operator-(const calculator&p1,const calculator&p2){
    calculator temp;
    temp.m_a=p1.m_a-p2.m_a;
    temp.m_b=p1.m_b-p2.m_b;
    return temp;
}

calculator operator*(const calculator&p1,const calculator&p2){
    calculator temp;
    temp.m_a=p1.m_a*p2.m_a-p1.m_b*p2.m_b;
    temp.m_b=p1.m_b*p2.m_a+p1.m_a*p2.m_b;
    return temp;
}

calculator operator/(const calculator&p1,const calculator&p2){
    calculator temp;
    int chushu=p2.m_a*p2.m_a+p2.m_b*p2.m_b;
    if(chushu==0)return calculator(0,0);
    temp.m_a=(p1.m_a*p2.m_a+p1.m_b*p2.m_b)/chushu;
    temp.m_b=(p1.m_b*p2.m_a-p1.m_a*p2.m_b)/chushu;
    return temp;
}

ostream&operator<<(ostream&cout,const calculator&c){
    if(c.m_b==0)cout<<c.m_a<<endl;
    else if(c.m_a==0)
    {
        if(c.m_b==1)cout<<"i"<<endl;
        else if(c.m_b==-1)cout<<"-i"<<endl;
        else cout<<c.m_b<<"i"<<endl;
    }else{
        if(c.m_b>0)
        {
            if(c.m_b==1)cout<<c.m_a<<"+i"<<endl;
            else cout<<c.m_a<<"+"<<c.m_b<<"i"<<endl;
        }else{
            if(c.m_b==-1)cout<<c.m_a<<"-i"<<endl;
            else cout<<c.m_a<<c.m_b<<"i"<<endl;
        }
   }
    return cout;
}

void test()
{
    int a1,a2,b1,b2;
    cout<<"请输入第一个复数的实部和虚部：";
    cin>>a1>>b1;
    cout<<"请输入第二个复数的实部和虚部：";
    cin>>a2>>b2;

    calculator p1(a1,b1);
    calculator p2(a2,b2);

    calculator p3=p1+p2;
    calculator p4=p1-p2;
    calculator p5=p1*p2;
    calculator p6=p1/p2;

    cout<<"加法："<<p3<<endl;
    cout<<"减法："<<p4<<endl;
    cout<<"乘法："<<p5<<endl;
    cout<<"除法："<<p6<<endl;
}

int main(){
    test();
    return 0;
}
