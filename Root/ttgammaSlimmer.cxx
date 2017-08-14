//////////////////////////////
//Joshua.Wyatt.Smith@cern.ch//
//////////////////////////////
#include "ttgammaSlimmer.h"
#include <iostream>
#include <memory>

using namespace std;


void m_add_branches(
  TChain *fChain,
  TEntryList *entryList,
  TTree *newtree,
  string filename)
  {

  float total_events=0;
  float total_events_unweighted=0;

  activateBranches(fChain); 

  int nentries = entryList->GetN();

  for (Long64_t event =0;event<nentries;event++) {
    int entryNumber = fChain->GetEntryNumber(event);
    if (entryNumber < 0) break;
       fChain->GetEntry(entryNumber);
    loadBar(event, nentries, 100, 50);

    //if(selph_index1<0){continue;}


    total_events_unweighted=total_events_unweighted+1;
    if (filename.find("QCDfakes") != std::string::npos) {
         total_events = total_events+1*weights_mm_ejets->at(17) + 1*weights_mm_mujets->at(75);
    } else {
      total_events = total_events + 1 * (weight_mc * weight_pileup
          * weight_bTagSF_Continuous * ph_SF_eff->at(selph_index1)
          * ph_SF_iso->at(selph_index1)
          * weight_leptonSF * weight_jvt * event_norm * event_lumi);
    }
    newtree->Fill();

  }// end event loop
  std::cout << "Number of weighted events = "<< total_events << std::endl;
  std::cout << "Number of unweighted events = "<< total_events_unweighted << std::endl;


}// end add_nn loop

int main(int argc, char** argv)
{
  gROOT->ProcessLine( "gErrorIgnoreLevel = kFatal;");
  std::cout << "Found " << argc-1 << " files to run over:" << std::endl;

  //string inputPath = "/eos/user/c/caudron/TtGamma_ntuples/v009/CR1/";

  //string path = "/eos/atlas/user/j/jwsmith/reprocessedNtuples/v009_flattened/CR1/";
  string path = "root://eosuser//eos/user/j/jwsmith/reprocessedNtuples/v009/QE2_yichen/";
  string channels[] ={"ejets","mujets"};
  //string channels[] ={"emu","mumu","ee"};
  // Where we save to:
  // Remember to make the directory. I.e. mkdir ../SR1 ; cd ../SR1 ; mkdir emu mumu etc
  // I'm just too lazy.
  //string outputPath = "/eos/atlas/user/j/jwsmith/reprocessedNtuples/v007_training/SR1/";
  string myPath = "root://eosatlas//eos/atlas/user/b/bvolkel/ntuples/ttgamma_13TeV/v009/QE2/";

  TFile *newfile;
  TFile *oldFile;
  TTree *newtree;
  TChain *fChain;

  for (int i = 1; i < argc; ++i) {
    for(const string &c : channels){


      string filename = argv[i];
      string file = path+c+"/"+filename;
      string newpath = myPath + c+"/"+filename;
      std::cout<<c<<": "<< filename<< std::endl;
      std::cout<<c<<": Saving to "<<newpath<< std::endl;


      oldFile = new TFile((file.c_str()), "read");

      TList* list = oldFile->GetListOfKeys() ;
      if (!list) { printf("<E> No keys found in file\n") ; exit(1) ; }
      TIter next(list) ;
      TKey* key ;
      TObject* obj ;
          
      while ( key = (TKey*)next() ) {

          obj = key->ReadObj() ;
          if ( (strcmp(obj->IsA()->GetName(),"TTree")!=0) || (strcmp("sumWeights",obj->GetName()) == 0) ) {
            printf("Not running over: %s \n",obj->GetName()); continue; 
          }
          printf("#####################################\n");
          printf("Currently working on %s \n",obj->GetName());
          printf("#####################################\n");


          newfile = new TFile((newpath.c_str()), "update");
          fChain = new TChain(obj->GetName());
     
          fChain->Add((file).c_str());

        TCut cut;
        //Channel specific selection:
        if (c.find("ejets") != std::string::npos) {
          // Direct SR cuts
          cut="selph_index1 >=0 && event_ngoodphotons==1 && event_njets >= 4 && event_nbjets77 >= 1 && abs(ph_mgammalept[selph_index1] - 91188) > 5000 && ph_drlept[selph_index1] > 1.0 && ph_isoFCT[selph_index1]";
          // QE2 cuts
          //cut=" ((ejets_2015 && (HLT_e24_lhmedium_L1EM20VH || HLT_e60_lhmedium || HLT_e120_lhloose)) || (ejets_2016 && ((HLT_e26_lhtight_nod0_ivarloose && el_pt[0] < 61000.) || ((HLT_e60_lhmedium_nod0 || HLT_e140_lhloose_nod0) && el_pt[0] > 61000)))) && (selph_index1 >=0 && event_ngoodphotons==1 && event_njets >= 4 && event_nbjets77 >= 1 && abs(ph_mgammalept[selph_index1] - 91188) > 5000 && ph_drlept[selph_index1] > 1.0 && ph_isoFCT[selph_index1])";
          // QE2 cuts Benedikt
          cut=" ((ejets_2015 && (HLT_e24_lhmedium_L1EM20VH || HLT_e60_lhmedium || HLT_e120_lhloose)) || (ejets_2016 && ((HLT_e26_lhtight_nod0_ivarloose && el_pt[0] < 61000.) || ((HLT_e60_lhmedium_nod0 || HLT_e140_lhloose_nod0) && el_pt[0] > 61000))))";

        }
        if (c.find("mujets") != std::string::npos) {
          // Direct SR cuts
          //cut="selph_index1 >=0 && event_ngoodphotons==1 && event_njets >= 4 && event_nbjets77 >= 1 && ph_drlept[selph_index1] > 1.0 && ph_isoFCT[selph_index1]";
          // QE2 cuts
          //cut="((mujets_2015 && (HLT_mu20_iloose_L1MU15 || HLT_mu50)) || (mujets_2016 && ((HLT_mu24 && mu_pt[0] < 51000.) || (HLT_mu50 && mu_pt[0] > 51000.)))) && (selph_index1 >=0 && event_ngoodphotons==1 && event_njets >= 4 && event_nbjets77 >= 1 && ph_drlept[selph_index1] > 1.0 && ph_isoFCT[selph_index1])";
          // QE2 cuts Benedikt
          cut="((mujets_2015 && (HLT_mu20_iloose_L1MU15 || HLT_mu50)) || (mujets_2016 && ((HLT_mu24 && mu_pt[0] < 51000.) || (HLT_mu50 && mu_pt[0] > 51000.))))";
        }
        if (c.find("ee") != std::string::npos) {
          cut="selph_index1 >=0 && event_ngoodphotons == 1 && event_nbjets77 >= 1 && met_met > 30000 && (event_mll < 85000 || event_mll > 95000) && (ph_mgammaleptlept[selph_index1] < 85000 || ph_mgammaleptlept[selph_index1] > 95000) && ph_drlept[selph_index1] > 1.0 && ph_isoFCT[selph_index1]";
        }
        if (c.find("emu") != std::string::npos) {
          cut="event_ngoodphotons == 1 && event_nbjets77 >= 1 && ph_drlept[selph_index1] > 1.0 && ph_isoFCT[selph_index1]";
        }
        if (c.find("mumu") != std::string::npos) {
          cut="event_ngoodphotons == 1 && event_nbjets77 >= 1 && met_met > 30000 && (event_mll < 85000 || event_mll > 95000) && (ph_mgammaleptlept[selph_index1] < 85000 || ph_mgammaleptlept[selph_index1] > 95000) && ph_drlept[selph_index1] > 1.0 && ph_isoFCT[selph_index1]";
        }

        TCut overlapRemoval;
        // Signal
        if(filename.find("ttgamma") != std::string::npos) {
          overlapRemoval="event_photonorigin<10";
        } 
        // ttbar
        if(filename.find("ttbar") != std::string::npos) {
          overlapRemoval="event_photonorigin>=10";
        } 
        else{
          overlapRemoval="";
        }

        fChain->Draw(">>entrylist",cut&&overlapRemoval,"entrylist");
        TEntryList *elist = (TEntryList*)gDirectory->Get("entrylist");
   
        fChain->SetEntryList(elist);
        newtree = fChain->CloneTree(0);
        if(fChain->GetEntries() == 0){
          std::cout<<"No events, is this an error? Skipping..."<<std::endl;
          continue;
        }
        m_add_branches(fChain,elist,newtree, filename);
        newfile->cd();
        newtree->Write();
        newfile->Close();
      }
    }
  }

  return 0;
}
