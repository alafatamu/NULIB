//This program will handle viewing, processing, and plotting 
#include <iostream>
#include <fstream>

#include "analysis/detector.hpp"
#include "analysis/histos.hpp"

#include "io/baseroot.hpp"
#include "io/dataref.hpp"
#include "io/evthandler.hpp"

#include "proggy/config.hpp"
#include "proggy/tformat.hpp"

#include "TFile.h"

using namespace tformat;

int main(){
  bool working = true;
  INFOCON cfg = load_config("../info/config.cfg"); //read in the config from the given path
  detector texneut; 
  bool goodmapread = texneut.fill_maps(cfg); //load the detector object
  if(!goodmapread){
    std::cout<<RED<<"Detector map read failed."<<RESET<<std::endl;
    return 0;
  }

  while(working){
    int choice = 0;
    std::cout<<"View (1), Process (2), Plot (3), or Exit (0): "<<std::endl; std::cin>>choice;
    if(choice==0){
      working = false;
    }else if(choice==1){
      bool viewing = true;
      while(viewing){
        int runreq = 0;
        std::cout<<"Run to view: "<<std::endl; std::cin>>runreq;
        if(runreq==0) break;

        std::string InputEvtName = namebuilder::get_InputEvtName(cfg, runreq); //generate an input file name
        std::ifstream evtfile = grabfile(InputEvtName); //open the input file
        if(!evtfile) return 0; //no file loaded
        
        int result = inspect_evt_file(evtfile);
        if(result==0){
          std::cout<<"Successful read!"<<std::endl;
        }else{
          std::cout<<"Incomplete read..."<<std::endl;
        } 
        evtfile.close();
      }
    }else if(choice==2){
      bool processing = true;
      while(processing){
        int procreq = 0;
        std::cout<<"Process a run (1) or a set (2): "<<std::endl; std::cin>>procreq;
        if(procreq==0){
          processing = false;
        }else if(procreq==1){

          int runreq = 0;
          std::cout<<"Run to process: "<<std::endl; std::cin>>runreq;
          if(runreq==0) break;

          std::string InputEvtName = namebuilder::get_InputEvtName(cfg, runreq); //generate an input file name
          std::ifstream InputEvtFile = grabfile(InputEvtName); //open the input file
          if(!InputEvtFile){
            std::cout<<RED<<"Error: could not open evt file"<<RESET<<std::endl;
            return 0;
          }

          std::string OutputROOTFileName = namebuilder::get_OutputROOTName(cfg, runreq); //generate an output file name
          TFile OutputROOTFile(OutputROOTFileName.c_str(), "RECREATE");
          if(OutputROOTFile.IsZombie()){
            std::cout<<RED<<"Error: could not open ROOT file"<<RESET<<std::endl;
            return 0;
          }

          int result = Evt_to_ROOT(InputEvtFile, OutputROOTFile, texneut); //read through the file
          if(result==0){
            std::cout<<RED<<"Run "<<runreq<<" read failed."<<RESET<<std::endl;
          }else if(result==1){
            std::cout<<GREEN<<"Run "<<runreq<<" successfully read."<<RESET<<std::endl;
          }

          std::cout<<"Done!"<<std::endl;
          InputEvtFile.close();
          OutputROOTFile.Close();

        }else if(procreq==2){
          int setreq = 0;
          int runnum = 0;
          std::string setfilename = cfg.ExpInfoDir;
          std::ifstream setfile;

          std::cout<<"Set to convert (1 for 6He, 2 for 9Li): "<<std::endl; std::cin>>setreq;
          if(setreq==0){
            break; //exit code
          }else if(setreq==1){
            setfilename += "6hepn.txt";
            setfile.open(setfilename);
            if(!setfile){
              std::cout<<"Error: could not open set file"<<std::endl;
              break;
            }
          }else if(setreq==2){
            setfilename += "9lipn.txt";
            setfile.open(setfilename);
            if(!setfile){
              std::cout<<"Error: could not open set file"<<std::endl;
              break;
            }
          }else{
            std::cout<<"Error: invalid set number"<<std::endl;
            break;
          }

          std::string OutputSETFileName = namebuilder::get_OutputSETName(cfg, setreq); //generate an output file name
          TFile OutputSETFile(OutputSETFileName.c_str(), "RECREATE");
          if(OutputSETFile.IsZombie()){
            std::cout<<RED<<"Error: could not open ROOT file"<<RESET<<std::endl;
            break;
          }

          int tnrun,tarun;
          bool conask = true;
          int concheck = 0;

          while(!setfile.eof()){
            setfile>>tnrun>>tarun;
            std::string InputEvtName = namebuilder::get_InputEvtName(cfg, tnrun); //generate an input file name
            std::ifstream InputEvtFile = grabfile(InputEvtName); //open the input file
            if(!InputEvtFile) break; //no file loaded

            int result = Evt_to_ROOT(InputEvtFile, OutputSETFile, texneut, tnrun); //read through the file
            if(result==0){
              std::cout<<RED<<"Run "<<tnrun<<" read failed."<<RESET<<std::endl;
            }else if(result==1){
              std::cout<<GREEN<<"Run "<<tnrun<<" successfully read."<<RESET<<std::endl;
            }
            std::cout<<"Done!"<<std::endl;
            InputEvtFile.close();
            
            if(!conask) continue;
            std::cout<<"Continue? 0=no, 1=yes: ";std::cin>>concheck;
            if(concheck==0){
              break;
            }else if(concheck == 11){
              conask = false;
              concheck = 0;
            }
          }

          setfile.close();
          OutputSETFile.Close();
          std::cout<<GREEN<<"Set conversion complete!"<<RESET<<std::endl;
        }

      }
    }else if(choice==3){
      bool plotting = true;
      while(plotting){
        int plotreq = 0;
        std::cout<<"Plot a run (1) or a set (2): "<<std::endl; std::cin>>plotreq;
        if(plotreq==0){
          plotting = false;
        }else if(plotreq==1){

          int runreq = 0;
          int barreq = 1234;

          std::cout<<"Run to plot: "<<std::endl; std::cin>>runreq;
          if(runreq==0) break; //exit code
          std::cout<<"Bar to plot (1234 to plot all): "<<std::endl; std::cin>>barreq;

          std::string InputROOTFileName = namebuilder::get_OutputROOTName(cfg, runreq); //generate an output file name
          TFile DataFile(InputROOTFileName.c_str(), "READ");
          if(DataFile.IsZombie()){
            std::cout<<"Error: could not open "<<InputROOTFileName<<std::endl;
            break;
          }

          std::string modifier = "";
          if(barreq!=1234) modifier = "_bar"+std::to_string(barreq);
          modifier += "_histos";
          std::string HDumpName = namebuilder::get_OutputROOTName(cfg, runreq, modifier); //generate a histos file name
          TFile HDump(HDumpName.c_str(),"UPDATE");
          if(HDump.IsZombie()){
            std::cout<<"Error: could not open histos.root"<<std::endl;
            break;
          }

          std::cout<<"Dumping histograms to "<<HDumpName<<std::endl;
          base_histos(DataFile, HDump); //dump the histograms
          proc_histos(DataFile, HDump); //dump the histograms
          calc_histos(DataFile, HDump, -10, barreq); //dump the histograms

        }else if(plotreq==2){
          
          int setreq = 0;
          int barreq = 1234;

          std::string setfilename = cfg.ExpInfoDir;
          std::ifstream setfile;

          std::cout<<"Set to convert (1 for 6He, 2 for 9Li): "<<std::endl; std::cin>>setreq;
          if(setreq==0){
            break;
          }else if(setreq==1){
            setfilename += "6hepn.txt";
            setfile.open(setfilename);
            if(!setfile){
              std::cout<<"Error: could not open set file"<<std::endl;
              break;
            }
          }else if(setreq==2){
            setfilename += "9lipn.txt";
            setfile.open(setfilename);
            if(!setfile){
              std::cout<<"Error: could not open set file"<<std::endl;
              break;
            }
          }else{
            std::cout<<"Error: invalid set number"<<std::endl;
            break;
          }

          std::string DataFileName = namebuilder::get_OutputSETName(cfg, setreq); //generate an output file name
          TFile DataFile(DataFileName.c_str(), "READ");
          if(DataFile.IsZombie()){
            std::cout<<"Error: could not open "<<DataFileName<<std::endl;
            break;
          }

          std::cout<<"Bar to plot (1234 to plot all): "<<std::endl; std::cin>>barreq;

          std::string modifier = "";
          if(barreq!=1234) modifier += "_bar"+std::to_string(barreq);
          modifier += "_histos";
          std::string HDumpName = namebuilder::get_OutputSETName(cfg, setreq, modifier); //generate an output file name
          TFile HDump(HDumpName.c_str(), "RECREATE");
          if(HDump.IsZombie()){
            std::cout<<RED<<"Error: could not open ROOT file"<<RESET<<std::endl;
            break;
          }
          std::cout<<"Dumping histograms to "<<HDumpName<<"..."<<std::endl;

          int tnrun,tarun;
          bool conask = true;
          int concheck = 0;

          while(!setfile.eof()){
            setfile>>tnrun>>tarun;
            std::cout<<"Run "<<tnrun<<"..."<<std::endl;
            base_histos(DataFile, HDump, tnrun); //dump the histograms
            proc_histos(DataFile, HDump, tnrun); //dump the histograms
            calc_histos(DataFile, HDump, tnrun, barreq); //dump the histograms

            if(!conask) continue;
            std::cout<<"Continue? 0=no, 1=yes: ";std::cin>>concheck;
            if(concheck==0){
              break;
            }else if(concheck == 11){
              conask = false;
              concheck = 0;
            }
          }
          
          DataFile.Close();
          setfile.close();
          HDump.Close();
          std::cout<<GREEN<<"Histogram generation complete!"<<RESET<<std::endl;
        }
      }
    }
  }
  return 0;
}