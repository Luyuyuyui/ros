#include<bits/stdc++.h>
using namespace std;

int main()
{
    cout << "===欢迎来到猜数字游戏！===" << endl;
    cout << "我已经想好了一个1到99之间的数字,你能猜出来吗?" << endl;

    srand(time(NULL));
    int n = rand() % 99+1;
    int count = 0;
    while (1) {
        count++;
        cout << "请输入你的猜测：";
        int i;
        cin >> i;
        if (i == n) {
            cout << "猜对了！";
            break;
        }
        else if (i < n) {
            cout << "太小了！再试试！";
        }
        else  {
            cout << "太大了！再试试！";
        }
         cout << endl;
    }
    cout << "你总共猜了" << count << "次！" << endl;
    if (count == 1)cout << "封神！";
    else if (count < 7)cout << "太棒了!";
    else { cout << "再接再励！"; }

    return 0;
}