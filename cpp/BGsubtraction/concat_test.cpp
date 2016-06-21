#include"sub_test.hpp"


using namespace std;
using namespace cv;

int main(int argc, char* argv[]){
  if(argc != 2){ 
    cout << "usage: ./test.out data-place/" << endl;
    return 0;
  }
  vector<Mat> MaskArray;
  MakeMask(argv,MaskArray);
  return 0;
}
