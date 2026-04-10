#include <iostream>
#include "io/evthandler.hpp"

int main(){
  int request = 0;
  std::cout<<"Enter 1 to read the evt file. 0 to exit."<<std::endl; std::cin>>request;
  if(request==1){
    std::cout<<"Reading evt file..."<<std::endl;
    int result = read_evt_file("/home/alafa/run1221a/asics/run1822/run-1822-00.evt");
    if(result==0){
      std::cout<<"Successful read!"<<std::endl;
    }else{
      std::cout<<"Incomplete read..."<<std::endl;
    }
  }
  return 0;
}