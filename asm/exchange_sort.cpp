#include<iostream>


using namespace std;

int main() {
  int a[10] = {10, 3, 7, 2, 8, 9, 4, 5, 11, 1};
   cout << 10  << endl;
  int len = sizeof(a) / sizeof(a[0]);
 
  for(int i = 0; i < len - 1; i++) {
    for(int j = i + 1; j < len; j++) {
	if(a[i] > a[j]) {
	  int temp = a[i];
	  a[i] = a[j];
          a[j] = temp;
	}
    }
  }
  for(int i = 0; i < len; i++) {
    cout << a[i] << " ";
  }
  cout << "\n";
  return 0;
}
