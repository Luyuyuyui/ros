#include<bits/stdc++.h>
using namespace std;
//归并排序
void hebin(vector<int>& a, int low, int mid, int high) {  
    int i = low, j = mid + 1, k = 0;
    vector<int> b(high - low + 1);
    while(i <= mid && j <= high) {
        if(a[i] <= a[j]) {
            b[k++] = a[i++];
        } else {
            b[k++] = a[j++];
        }
    }
    while(i <= mid) b[k++] = a[i++];
    while(j <= high) b[k++] = a[j++];

    k = 0;
    for(int i = low; i <= high; i++) {
        a[i] = b[k++];  
    }
}

void gbsort(vector<int>& a, int low, int high) {  
    if(low < high) {
        int mid = (high + low) / 2;
        gbsort(a, low, mid);
        gbsort(a, mid + 1, high);
        hebin(a, low, mid, high);
    }
}

int main() {
    int n;
    cin >> n;
    vector<int> a(n);
    for(int i = 0; i < n; i++) {
        cin >> a[i];
    }
    gbsort(a, 0, n - 1);
    for(int i = 0; i < n; i++) {
        if(i > 0) cout << " ";
        cout << a[i];
    }
    cout << endl;
    return 0;
}

