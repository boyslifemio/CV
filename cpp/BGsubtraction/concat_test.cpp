#include"sub_test.hpp"
#include<time.h>


using namespace std;
using namespace cv;

int main(int argc, char* argv[]){
  clock_t t1, t2;
  t1 = clock();
  if(argc != 2){ 
    cout << "usage: ./test.out data-place/ " << endl;
    return 0;
  }
  vector<Mat> MaskArray;
  MakeMask(argv,MaskArray);
  t2 = clock();
  cout << "time -> " << t1 - t2 << endl;
  return 0;
}
