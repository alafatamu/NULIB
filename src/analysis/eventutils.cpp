#include "analysis/eventutils.hpp"

#include <iostream>
#include <unordered_set>

//stuff used in event processing only
#include "analysis/detector.hpp"
#include "proggy/config.hpp"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

namespace eventutils{
  void display_raw(const raw_event& event){
    std::cout<<"Timestamp: "<<event.timestamp<<std::endl;
    for(int i=0;i<event.chip.size();i++){
      std::cout<<"Chip: "<<event.chip[i]<<std::endl;
      std::cout<<"Channel: "<<event.chan[i]<<std::endl;
      std::cout<<"Aint: "<<event.Aint[i]<<std::endl;
      std::cout<<"Bint: "<<event.Bint[i]<<std::endl;
      std::cout<<"Cint: "<<event.Cint[i]<<std::endl;
      std::cout<<"Tint: "<<event.Tint[i]<<std::endl;
    }
    for(int i=0;i<event.TDCchan.size();i++){
      std::cout<<"TDCchannel: "<<event.TDCchan[i]<<std::endl;
      std::cout<<"TDCvalue: "<<event.TDCval[i]<<std::endl;
    }
    std::cout<<"Flag: "<<event.unpackflag<<std::endl;
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  void display_event(const processed_event& event){
    std::cout<<"Good Hits: "<<event.hitcount<<std::endl;
    std::cout<<"Timestamp: "<<event.timestamp<<std::endl;
    for(int i=0;i<event.hitcount;i++){
      std::cout<<"Hit "<<i<<": "<<std::endl;
      std::cout<<"Chip: "<<event.chip[i]<<std::endl;
      std::cout<<"Channel: "<<event.chan[i]<<std::endl;
      std::cout<<"Aint: "<<event.Aint[i]<<std::endl;
      std::cout<<"Bint: "<<event.Bint[i]<<std::endl;
      std::cout<<"Cint: "<<event.Cint[i]<<std::endl;
      std::cout<<"Tint: "<<event.Tint[i]<<std::endl;
      std::cout<<"TDCchannel: "<<event.TDCchan[i]<<std::endl;
      std::cout<<"TDCvalue: "<<event.TDCval[i]<<std::endl;
    }
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  processed_event process_event(raw_event rawevent, detector texneut){
    processed_event outevent;
    outevent.keep = false; //default is toss
    outevent.timestamp = rawevent.timestamp;
    //Let's copy over the useful vectors.
    outevent.chip = std::move(rawevent.chip);
    outevent.chan = std::move(rawevent.chan);
    outevent.Aint = std::move(rawevent.Aint);
    outevent.Bint = std::move(rawevent.Bint);
    outevent.Cint = std::move(rawevent.Cint);
    outevent.Tint = std::move(rawevent.Tint);
    outevent.TDCchan = std::move(rawevent.TDCchan);
    outevent.TDCval = std::move(rawevent.TDCval);

    int totalhits = outevent.chip.size();
    int coupledhits = 0;

    std::vector<bool> used(totalhits, false); //keep track of which hits have been used
    for (int h=0;h<totalhits;h++){
      int h_chip = outevent.chip[h];
      int h_chan = outevent.chan[h];
      //check for the hit being used??
      for(int j=h+1;j<totalhits;j++){
        if(used[j]) continue; //skip if already used
        int j_chip = outevent.chip[j];
        int j_chan = outevent.chan[j];
        if(texneut.getbar(h_chip,h_chan)!=texneut.getbar(j_chip,j_chan))continue; //confirm they are in the same bar
        int barseen=texneut.getbar(h_chip,h_chan);
        used[h]=used[j]=true;//now we can do our full processing
        outevent.barshit.push_back(barseen);
        coupledhits++;

        //figure out top/bottom pmt ID confirmation
        //h,j -> t,b
        //for now, assume h on top, j on bottom
        int t = h;
        int t_chip = h_chip;
        int t_chan = h_chan;
        int b = j;
        int b_chip = j_chip;
        int b_chan = j_chan;

        double topgain,botgain,Atop,Abot,Btop,Bbot;
        topgain = texneut.get_gainfactors(barseen,0);
        botgain = texneut.get_gainfactors(barseen,1);
        Atop = (double)outevent.Aint[t]*topgain-texneut.get_offset(t_chip,t_chan,0);
        Abot = (double)outevent.Aint[b]*botgain-texneut.get_offset(b_chip,b_chan,1);
        Btop = (double)outevent.Bint[t]*topgain-texneut.get_offset(t_chip,t_chan,1);
        Bbot = (double)outevent.Bint[t]*botgain-texneut.get_offset(b_chip,b_chan,0);

        outevent.barshit.push_back(barseen);

        outevent.chip_top.push_back(t_chip);
        outevent.chan_top.push_back(t_chan);
        outevent.Aint_top.push_back(Atop);
        outevent.Bint_top.push_back(Btop);
        outevent.Cint_top.push_back(outevent.Cint[t]);
        outevent.Tint_top.push_back(outevent.Tint[t]);

        outevent.chip_bot.push_back(b_chip);
        outevent.chan_bot.push_back(b_chan);
        outevent.Aint_bot.push_back(Abot);
        outevent.Bint_bot.push_back(Bbot);
        outevent.Cint_bot.push_back(outevent.Cint[b]);
        outevent.Tint_bot.push_back(outevent.Tint[b]);
/*
        int tdcchan_top = texneut.get_tdcchan(outevent.chip[t],outevent.chan[t]);
        int tdcchan_bot = texneut.get_tdcchan(outevent.chip[b],outevent.chan[b]);
        outevent.TDCchan_top.push_back(tdcchan_top);
        outevent.TDCchan_bot.push_back(tdcchan_bot);
        double tdc_topval, tdc_botval;
        for(int m=0; m<outevent.TDCchan.size(); m++){
          for(int n=0; n<outevent.TDCchan.size(); n++){
            if(m==n || tdcchan_top!=outevent.TDCchan[m] || tdcchan_bot!=outevent.TDCchan[n])continue;
            tdc_topval=-outevent.TDCval[m]+4092;
            tdc_botval=-outevent.TDCval[n]+4092;
            //tdc_topval*=0.293;
            //tdc_botval*=0.293;
            outevent.TDCval_top.push_back(tdc_topval);
            outevent.TDCval_bot.push_back(tdc_botval);
          }
        }*/
      }
    }
    if(coupledhits==0) return outevent; //NOTICE NO KEEP=TRUE HERE
    //find bar multiplicity
    std::unordered_set<int> multfinder(outevent.barshit.begin(),outevent.barshit.end());
    outevent.barmult=multfinder.size();
    if(outevent.barmult==0) return outevent; //NOTICE NO KEEP=TRUE HERE

    //Once things look good, let's clean up the initial vectors and return a positive keep
    for(int i=totalhits-1;i>=0;--i){
      //std::cout<<"Looking at hit "<<i<<" // value: "<<used[i]<<std::endl;
      if(!used[i]){
        totalhits--;
        outevent.chip.erase(outevent.chip.begin()+i);
        outevent.chan.erase(outevent.chan.begin()+i);
        outevent.Aint.erase(outevent.Aint.begin()+i);
        outevent.Bint.erase(outevent.Bint.begin()+i);
        outevent.Cint.erase(outevent.Cint.begin()+i);
        outevent.Tint.erase(outevent.Tint.begin()+i);
      };
    }
    outevent.coupledhits = coupledhits; //can't forget this one!
    outevent.hitcount = outevent.chip.size(); //can't forget this one!

    //only keep the event if we can successfully process it and things look good
    outevent.keep=true;
    return outevent;
  }
}