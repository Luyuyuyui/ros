## 排序算法：

```
//1.**冒泡排序**
    int n;
    cin>>n;
    vector<int>arr(n);
    for(int i=0;i<arr.size();i++){
        cin>>arr[i];
    }
    for(int i=0;i<arr.size()-1;i++){
        for(int j=0;j<arr.size()-i-1;j++){
            if(arr[j]>arr[j+1]){
                int temp=arr[j];
                arr[j]=arr[j+1];
                arr[j+1]=temp;
            }
        }

    }
    for(int i=0;i<arr.size();i++){
        if(i>0)cout<<" ";
        cout<<arr[i];
    }
    cout<<endl;

    //2.**选择排序**
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

    //3.**插入排序**
    //[1]直接插入
    int n;
    cin>>n;
    vector<int>arr(n);
     for(int i=0;i<arr.size();i++){
        cin>>arr[i];
    }
    for(int i=1;i<n;i++){
        int key=arr[i];
        int j=i-1;
        while(j>=0&&key<arr[j]){
            arr[j+1]=arr[j];
            j--;
        }
        arr[j+1]=key;
    }
    for(int i=0;i<arr.size();i++){
        if(i>0)cout<<" ";
        cout<<arr[i];
    }
    cout<<endl;

    
    //[2]**希尔排序**
    for(int gap=n/2;gap>0;gap/=2){
        for(int i=gap;i<n;i++){
            int temp=arr[i];
            int j=i;

            while(j>=gap&&arr[j-gap]>temp){
                arr[j]=arr[j-gap];
                j-=gap;
            }
            arr[j]=temp;
        }
    }

    //**归并排序**
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


//**快速排序**
//基准值排序
void quickSort(vector<int>& arr, int left, int right) {
    if (left >= right || left < 0 || right >= arr.size()) {
        return;
    }
    
    int p = arr[right];
    int i = left;
    
    for (int j = left; j < right; j++) {
        if (arr[j] < p) {
            if (i != j) {
                swap(arr[i], arr[j]);
            }
            i++;
        }
    }
    if (i != right) {
        swap(arr[i], arr[right]);
    }
    
    if (left < i - 1) {
        quickSort(arr, left, i - 1);
    }
    if (i + 1 < right) {
        quickSort(arr, i + 1, right);
    }
}

int main() {
    int n;
    cin >> n;
    if (n <= 0) {
        cout << endl;
        return 0;
    }
    vector<int> arr(n);
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }
    quickSort(arr, 0, n - 1);
    
    for (int i = 0; i < n; i++) {
        if (i > 0) cout << " ";
        cout << arr[i];
    }
    cout << endl;
    
    return 0;
}
//挖坑法
int PartSort2(vector<int>&a, int left, int right)
{
	int key = a[left];
	int hole = left;
	while (left < right)
	{
		while (left < right && a[right] >= key)
		{
			right--;
		}
		a[hole] = a[right];
		hole = right;
 
		while (left < right && a[left] <= key)
		{
			left++;
		}
		a[hole] = a[left];
		hole = left;
	}
	a[hole] = key;
	return hole;
}
 
void QuickSort(vector<int>&a, int left, int right)
{
	if (left >= right)
	{
		return;
	}
 
	
	int keyi = PartSort2(a, left, right);
	QuickSort(a, left, keyi - 1);
	QuickSort(a, keyi + 1, right);
	
}


```