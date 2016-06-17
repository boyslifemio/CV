#include"sub_test.hpp"


using namespace std;
using namespace cv;

int main(int argc, char* argv[]){
  if(argc != 2) cout << "usage: ./test.out data-place/";
  vector<Mat> MaskArray;
  MakeMask(argv,MaskArray);
}
