#include<bits/stdc++.h>
using namespace std;

class matrix
{
public:
    matrix(){};
    matrix(vector<vector<int>>m1,vector<vector<int>>m2)
    {
        m_m1=m1;
        m_m2=m2;
    }

    //1.计算行列式的值
    int det(const vector<vector<int>>& m)
    {
        if (m.size() != m[0].size()) return 0;
        
        if (m.size() == 1) return m[0][0]; // 1x1矩阵
        
        // 使用double类型进行中间计算!!!!
        vector<vector<double>> temp(m.size(), vector<double>(m.size()));
        for (int i = 0; i < m.size(); i++) {
            for (int j = 0; j < m.size(); j++) {
                temp[i][j] = m[i][j];
            }
        }
    
        int count = 0; // 行交换次数
        double result = 1.0;
        //高斯消元获得上三角
        for (int i = 0; i < temp.size(); i++)
        {
            // 寻找主元
            int maxR = i;
            for (int j = i + 1; j < temp.size(); j++) {
                if (abs(temp[j][i]) > abs(temp[maxR][i])) {
                    maxR = j;
                }
            }
            // 如果主元为0，行列式为0
            if (abs(temp[maxR][i]) < 1e-10) {
                return 0;
            }
            
            // 交换行
            if (maxR != i) {
                swap(temp[i], temp[maxR]);
                count++;
            }
            
            // 累积对角线元素
            result *= temp[i][i];
            
            // 消元
            for (int j = i + 1; j < temp.size(); j++) {
                double factor = temp[j][i] / temp[i][i];//求倍数
                for (int k = i; k < temp.size(); k++) {
                    temp[j][k] -= factor * temp[i][k];
                }
            }
        }
        
        // 考虑行交换的符号
        if (count % 2 == 1) {
            result=-result;
        }
        
        return round(result); // 四舍五入到整数
    }
    
    
    
    
    //2.求代数余子式
    vector<vector<int>> dsyzs(const vector<vector<int>>& m)
    {
        //检查是否为方阵
        if(m.size() != m[0].size()) {
            return vector<vector<int>>();
        }

        vector<vector<int>>result(m.size(),vector<int>(m.size()));
        vector<vector<int>>yzs(m.size()-1,vector<int>(m.size()-1));

        for(int i=0;i<m.size();i++){
            for(int j=0;j<m[0].size();j++)
            {
                int xx=0;
                for(int x=0;x<m.size();x++)
                {
                    if(x==i)continue;//跳过当前行
                    int yy=0;
                    for(int y=0;y<m.size();y++)
                    {
                        if(y==j)continue;//跳过当前列
                        yzs[xx][yy]=m[x][y];
                        yy++;
                    }
                    xx++;
                }

                if((i+j)%2==0)
                {
                    result[i][j]=det(yzs);
                }
                else{
                    result[i][j]=-det(yzs);
                }
            }
        }
        return result;
    }



//3.求伴随矩阵
vector<vector<int>>bsjz(const vector<vector<int>>& m)
{
    vector<vector<int>>temp01=dsyzs(m);
    vector<vector<int>>result(m.size(),vector<int>(m.size()));

    for(int i=0;i<m.size();i++)
    {
        for(int j=0;j<m.size();j++)
        {
            result[j][i]=temp01[i][j];
        }
    }
    return result;
}



//4.求逆矩阵
vector<vector<double>>qnjz(const vector<vector<int>>&m)
{
    int w=det(m);
    if(w==0)return vector<vector<double>>();//不合条件行列式为0不能求逆

    vector<vector<int>>bs=bsjz(m);
    vector<vector<double>>result(m.size(),vector<double>(m.size()));

    double zhi=1.0/det(m);
    for(int i=0;i<m.size();i++)
    {
        for(int j=0;j<m[0].size();j++)
        {
            result[i][j]=zhi*bs[i][j];
        }
    }
    return result;
}


    vector<vector<int>> m_m1;
    vector<vector<int>> m_m2;
};


//1.计算加法
matrix operator+(const vector<vector<int>>&m1,const vector<vector<int>>&m2)
{
    matrix temp;
//先检查是否匹配
    if(m1.size()!=m2.size()||m1[0].size() != m2[0].size())
    {
        cout<<"矩阵维度不匹配！"<<endl;
        return matrix();
    }

    vector<vector<int>> result(m1.size(), vector<int>(m1[0].size()));
    for(int i=0;i<m1.size();i++)
    {
        for(int j=0;j<m1[0].size();j++)
        {
            result[i][j]=m1[i][j]+m2[i][j];
        }
    }
    return matrix(result,vector<vector<int>>());
}

//2.计算减法
matrix operator-(const vector<vector<int>>&m1,const vector<vector<int>>&m2)
{
    matrix temp;
//先检查是否匹配
    if(m1.size()!=m2.size()||m1[0].size() != m2[0].size())
    {
        cout<<"矩阵维度不匹配！"<<endl;
    }

    vector<vector<int>> result(m1.size(), vector<int>(m1[0].size()));

    for(int i=0;i<m1.size();i++)
    {
        for(int j=0;j<m1[0].size();j++)
        {
            result[i][j]=m1[i][j]-m2[i][j];
        }
    }
    return matrix(result,vector<vector<int>>());
}

//3.计算乘法
matrix operator*(const vector<vector<int>>&m1,const vector<vector<int>>&m2)
{
    matrix temp;
//先检查是否匹配
    if(m1[0].size()!=m2.size())
    {
        cout<<"矩阵维度不匹配！"<<endl;
    }

    vector<vector<int>> result(m1.size(), vector<int>(m1[0].size()));
    for(int i=0;i<m1.size();i++)
    {
        for(int j=0;j<m2[0].size();j++)
        {
            for(int k=0;k<m1[0].size();k++)
            {
                result[i][j]+=m1[i][k]*m2[k][j];
            }
        }
    }
    
    return matrix(result,vector<vector<int>>());
}


//重载输出
ostream&operator<<(ostream& cout,const matrix&result)
{
    for(const auto& r:result.m_m1){
        for(int num:r){
            if(num>0)cout<<" ";
            cout<<num;
        }
        cout<<endl;
    }
    return cout;
}

//重载输入
istream&operator>>(istream& cin,matrix&result)
{
    int r,c;
    cout<<"请输入第一个矩阵的维度：";
    cin>>r>>c;

    vector<vector<int>> m1(r, vector<int>(c));
    cout<<"请输入第一个矩阵的元素："<<endl;
    for(int i=0;i<r;i++)
    {
        for(int j=0;j<c;j++)
        {
            cin>>m1[i][j];
        }
    }

    cout<<"请输入第二个矩阵的维度：";
    cin>>r>>c;

    vector<vector<int>> m2(r, vector<int>(c));
    cout<<"请输入第二个矩阵的元素："<<endl;
    for(int i=0;i<r;i++)
    {
        for(int j=0;j<c;j++)
        {
            cin>>m2[i][j];
        }
    }

    result=matrix(m1,m2);
    return cin;
}

//重载使得能输出double矩阵
ostream&operator<<(ostream& cout,const vector<vector<double>>&m)
{
    for(const auto& r:m){
        for(double num:r){
            if(num>0)cout<<" ";
            cout<<setprecision(2)<<num;//设置保留两位小数
        }
        cout<<endl;
    }
    return cout;
}


void test()
{
    matrix m;
    cout<<"请输入矩阵："<<endl;
    cin>>m;

    matrix temp1=m.m_m1+m.m_m2;
    matrix temp2=m.m_m1-m.m_m2;
    matrix temp3=m.m_m1*m.m_m2;

    matrix detcal;
    int re1=detcal.det(m.m_m1);
    int re2=detcal.det(m.m_m2);
   
    vector<vector<double>>re3=detcal.qnjz(m.m_m1);
    vector<vector<double>>re4=detcal.qnjz(m.m_m2);

    

    cout<<"加法结果："<<endl<<temp1<<endl;
    cout<<"减法结果："<<endl<<temp2<<endl;
    cout<<"乘法结果："<<endl<<temp3<<endl;
    cout<<"矩阵1行列式的值："<<endl<<re1<<endl;
    cout<<"矩阵2行列式的值："<<endl<<re2<<endl;
    cout<<"矩阵1的求逆结果："<<endl<<re3<<endl;
    cout<<"矩阵2的求逆结果："<<endl<<re4<<endl;

}
int main()
{
    test();
    return 0;
}