//////////////////////////////
//Joshua.Wyatt.Smith@cern.ch//
//////////////////////////////
#include "ttgammaSlimmer.h"
#include <iostream>
#include <memory>

using namespace std;


void m_add_branches(
  TChain *fChain_func,
  TEntryList *entryList,
  TTree *newtree_func,
  string filename)
  {

  float total_events=0;
  float total_events_unweighted=0;

  activateBranches(fChain_func); 

  int nentries = entryList->GetN();

  for (Long64_t event =0;event<nentries;event++) {
    int entryNumber = fChain_func->GetEntryNumber(event);
    if (entryNumber < 0) break;
       fChain_func->GetEntry(entryNumber);
    loadBar(event, nentries, 100, 50);

    //if(selph_index1<0){continue;}


    total_events_unweighted=total_events_unweighted+1;
    if (filename.find("fake_CR1") != std::string::npos) {
         total_events = total_events+1*weights_mm_ejets->at(17) + 1*weights_mm_mujets->at(75);
    } else {
      total_events = total_events + 1 * (weight_mc * weight_pileup
          * weight_bTagSF_Continuous * ph_SF_eff->at(selph_index1)
          * ph_SF_iso->at(selph_index1)
          * weight_leptonSF * weight_jvt * event_norm * event_lumi
          * ph_kfactor->at(selph_index1));
    }
    newtree_func->Fill();

  }// end event loop
  std::cout << "Number of unweighted events = "<< total_events_unweighted << std::endl;
  std::cout << "Number of weighted events = "<< total_events << std::endl;


}// end add_nn loop

int main(int argc, char** argv)
{
  //gROOT->ProcessLine( "gErrorIgnoreLevel = kFatal;");
  std::cout << "Found " << argc-1 << " files to run over:" << std::endl;

  string path = "root://eosatlas//eos/atlas/atlascerngroupdisk/phys-top/toproperties/ttgamma/v010_production/QE2/";

  string channels[] ={"mujets"};
  // Where we save to:
  // Remember to make the directory. I.e. mkdir ../SR1 ; cd ../SR1 ; mkdir emu mumu etc
  // I'm just too lazy.
  string outputPath = "root://eosatlas//eos/atlas/atlascerngroupdisk/phys-top/toproperties/ttgamma/v010_production/QE2_cut/";


  for (int i = 1; i < argc; ++i) {
    for(const string &c : channels){


      TFile *newfile=nullptr;
      TFile *oldFile=nullptr;

      string filename = argv[i];
      string file = path+c+"/"+filename;
      std::cout<<c<<": Reading from "<<path<< std::endl;
      string newpath = outputPath + c+"/"+filename;
      std::cout<<c<<": "<< filename<< std::endl;
      std::cout<<c<<": Saving to "<<newpath<< std::endl;

      TCut cut;
      //Channel specific selection:
      if (c.find("ejets") != std::string::npos) {
        // Direct SR cuts
        cut="selph_index1 >=0 && event_ngoodphotons==1 && event_njets >= 4 && event_nbjets77 >= 1 && abs(ph_mgammalept[selph_index1] - 91188) > 5000 && ph_drlept[selph_index1] > 1.0 && ph_isoFCT[selph_index1]";
      }
      if (c.find("mujets") != std::string::npos) {
        // Direct SR cuts
        cut="selph_index1 >=0 && event_ngoodphotons==1 && event_njets >= 4 && event_nbjets77 >= 1 && ph_drlept[selph_index1] > 1.0 && ph_isoFCT[selph_index1]";
      }
      if (c.find("ee") != std::string::npos) {
        cut="selph_index1 >=0 && event_ngoodphotons == 1 && event_njets >= 2 && event_nbjets77 >= 1 && met_met > 30000 && (event_mll < 85000 || event_mll > 95000) && (ph_mgammaleptlept[selph_index1] < 85000 || ph_mgammaleptlept[selph_index1] > 95000) && ph_drlept[selph_index1] > 1.0 && ph_isoFCT[selph_index1]";
      }
      if (c.find("emu") != std::string::npos) {
        cut="selph_index1 >=0 && event_ngoodphotons == 1 && event_njets >= 2 && event_nbjets77 >= 1 && ph_drlept[selph_index1] > 1.0 && ph_isoFCT[selph_index1]";
      }
      if (c.find("mumu") != std::string::npos) {
        cut="selph_index1 >=0 && event_ngoodphotons == 1 && event_njets >= 2 && event_nbjets77 >= 1 && met_met > 30000 && (event_mll < 85000 || event_mll > 95000) && (ph_mgammaleptlept[selph_index1] < 85000 || ph_mgammaleptlept[selph_index1] > 95000) && ph_drlept[selph_index1] > 1.0 && ph_isoFCT[selph_index1]";
      }

      TCut overlapRemoval;
      // Signal
      if(filename.find("ttgamma") != std::string::npos) {
        overlapRemoval="event_photonorigin<10";
      } 
      // ttbar
      else if(filename.find("ttbar") != std::string::npos) {
        overlapRemoval="event_photonorigin>=10";
      } 
      else{
        overlapRemoval="";
      }

      newfile = new TFile((newpath.c_str()), "recreate");
      oldFile = new TFile((file.c_str()), "read");

      TList* list = oldFile->GetListOfKeys() ;
      if (!list) { printf("<E> No keys found in file\n") ; exit(1) ; }
      TIter next(list) ;
      TKey* key ;
      TObject* obj ;
          
      std::string oldkeyname;

      while ( (key = (TKey*)next() )) {

        bool alreadyseen = (oldkeyname == key->GetName());

        if (alreadyseen){
          continue;
        }
        oldkeyname = key->GetName();

        TChain *fChain=nullptr;
        TTree *newtree=nullptr;

        obj = key->ReadObj() ;
        if ( (strcmp(obj->IsA()->GetName(),"TTree")!=0) || (strcmp("sumWeights",obj->GetName()) == 0)) {
          printf("Not running over: %s \n",obj->GetName()); continue; 
        }
        printf("#####################################\n");
        printf("Currently working on %s \n",obj->GetName());
        printf("#####################################\n");

        fChain = new TChain(obj->GetName());
        fChain->Add((file).c_str());
        newfile->cd(); // Make sure we are in new file otherwise shit breaks.
    
        fChain->Draw(">>entrylist",cut&&overlapRemoval,"entrylist");
        TEntryList *elist = (TEntryList*)gDirectory->Get("entrylist");
   
        fChain->SetEntryList(elist);
        newtree = fChain->CloneTree(0);
        if(fChain->GetEntries() == 0){
          std::cout<<"No events, is this an error? Skipping..."<<std::endl;
          delete newtree;
          delete fChain;
          continue;
        }
        m_add_branches(fChain,elist,newtree, filename);
        newfile->cd();
        newtree->Write();
        delete newtree;
        delete fChain;
      } // end loop over trees
      delete newfile;
      delete oldFile;
    } // end loop over channels
  }  // end loop over files

  return 0;
}
