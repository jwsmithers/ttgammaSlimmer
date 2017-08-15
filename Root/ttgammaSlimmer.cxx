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
    if (filename.find("QCDfakes") != std::string::npos) {
         total_events = total_events+1*weights_mm_ejets->at(17) + 1*weights_mm_mujets->at(75);
    } else {
      total_events = total_events + 1 * (weight_mc * weight_pileup
          * weight_bTagSF_Continuous * ph_SF_eff->at(selph_index1)
          * ph_SF_iso->at(selph_index1)
          * weight_leptonSF * weight_jvt * event_norm * event_lumi);
    }
    newtree_func->Fill();

  }// end event loop
  std::cout << "Number of unweighted events = "<< total_events_unweighted << std::endl;
  std::cout << "Number of weighted events = "<< total_events << std::endl;


}// end add_nn loop

int main(int argc, char** argv)
{
  gROOT->ProcessLine( "gErrorIgnoreLevel = kFatal;");
  std::cout << "Found " << argc-1 << " files to run over:" << std::endl;

  // string path = "root://eosuser//eos/user/j/jwsmith2/reprocessedNtuples/v009_flattened/CR1S/";
  string path = "/eos/user/j/jwsmith2/reprocessedNtuples/v009_flattened/CR1S/";

  string channels[] ={"mujets"};
  //string channels[] ={"emu","mumu","ee"};
  // Where we save to:
  // Remember to make the directory. I.e. mkdir ../SR1 ; cd ../SR1 ; mkdir emu mumu etc
  // I'm just too lazy.
  // string outputPath = "root://eosatlas//eos/atlas/user/j/jwsmith/reprocessedNtuples/v009_flattened/SR1S/";
  string outputPath = "/eos/atlas/user/j/jwsmith/reprocessedNtuples/v009_flattened/SR1S/";


  for (int i = 1; i < argc; ++i) {
    for(const string &c : channels){


      TFile *newfile=nullptr;
      TFile *oldFile=nullptr;
      TTree *newtree_nominal=nullptr;
      TChain *fChain_nominal=nullptr;

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
        // QE2 cuts
        //cut=" ((ejets_2015 && (HLT_e24_lhmedium_L1EM20VH || HLT_e60_lhmedium || HLT_e120_lhloose)) || (ejets_2016 && ((HLT_e26_lhtight_nod0_ivarloose && el_pt[0] < 61000.) || ((HLT_e60_lhmedium_nod0 || HLT_e140_lhloose_nod0) && el_pt[0] > 61000)))) && (selph_index1 >=0 && event_ngoodphotons==1 && event_njets >= 4 && event_nbjets77 >= 1 && abs(ph_mgammalept[selph_index1] - 91188) > 5000 && ph_drlept[selph_index1] > 1.0 && ph_isoFCT[selph_index1])";
        // QE2 cuts Benedikt
        // cut=" ((ejets_2015 && (HLT_e24_lhmedium_L1EM20VH || HLT_e60_lhmedium || HLT_e120_lhloose)) || (ejets_2016 && ((HLT_e26_lhtight_nod0_ivarloose && el_pt[0] < 61000.) || ((HLT_e60_lhmedium_nod0 || HLT_e140_lhloose_nod0) && el_pt[0] > 61000))))";

      }
      if (c.find("mujets") != std::string::npos) {
        // Direct SR cuts
        cut="selph_index1 >=0 && event_ngoodphotons==1 && event_njets >= 4 && event_nbjets77 >= 1 && ph_drlept[selph_index1] > 1.0 && ph_isoFCT[selph_index1]";
        // QE2 cuts
        //cut="((mujets_2015 && (HLT_mu20_iloose_L1MU15 || HLT_mu50)) || (mujets_2016 && ((HLT_mu24 && mu_pt[0] < 51000.) || (HLT_mu50 && mu_pt[0] > 51000.)))) && (selph_index1 >=0 && event_ngoodphotons==1 && event_njets >= 4 && event_nbjets77 >= 1 && ph_drlept[selph_index1] > 1.0 && ph_isoFCT[selph_index1])";
        // QE2 cuts Benedikt
        // cut="((mujets_2015 && (HLT_mu20_iloose_L1MU15 || HLT_mu50)) || (mujets_2016 && ((HLT_mu24 && mu_pt[0] < 51000.) || (HLT_mu50 && mu_pt[0] > 51000.))))";
      }
      if (c.find("ee") != std::string::npos) {
        cut="selph_index1 >=0 && event_ngoodphotons == 1 && event_nbjets77 >= 1 && met_met > 30000 && (event_mll < 85000 || event_mll > 95000) && (ph_mgammaleptlept[selph_index1] < 85000 || ph_mgammaleptlept[selph_index1] > 95000) && ph_drlept[selph_index1] > 1.0 && ph_isoFCT[selph_index1]";
      }
      if (c.find("emu") != std::string::npos) {
        cut="selph_index1 >=0 && event_ngoodphotons == 1 && event_nbjets77 >= 1 && ph_drlept[selph_index1] > 1.0 && ph_isoFCT[selph_index1]";
      }
      if (c.find("mumu") != std::string::npos) {
        cut="selph_index1 >=0 && event_ngoodphotons == 1 && event_nbjets77 >= 1 && met_met > 30000 && (event_mll < 85000 || event_mll > 95000) && (ph_mgammaleptlept[selph_index1] < 85000 || ph_mgammaleptlept[selph_index1] > 95000) && ph_drlept[selph_index1] > 1.0 && ph_isoFCT[selph_index1]";
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

      newfile = new TFile((newpath.c_str()), "update");
      // Nominal tree
      if (filename.find("QCDfakes") != std::string::npos) {
        fChain_nominal = new TChain("nominal_Loose");
      }
      else {
        fChain_nominal = new TChain("nominal");
      }
 
      fChain_nominal->Add((file).c_str());

      fChain_nominal->Draw(">>entrylist_nominal",cut&&overlapRemoval,"entrylist_nominal");
      TEntryList *elist_nominal = (TEntryList*)gDirectory->Get("entrylist_nominal");
 
      fChain_nominal->SetEntryList(elist_nominal);
      newtree_nominal = fChain_nominal->CloneTree(0);
      if(fChain_nominal->GetEntries() == 0){
        std::cout<<"No events, is this an error? Skipping..."<<std::endl;
        continue;
      }

      printf("#################################################\n");
      printf("Currently working on nominal tree (outside loop) \n");
      printf("#################################################\n");
      m_add_branches(fChain_nominal,elist_nominal,newtree_nominal, filename);

      newfile->cd();
      newtree_nominal->Write();
      delete newtree_nominal;
      delete fChain_nominal;
      delete elist_nominal;


      oldFile = new TFile((file.c_str()), "read");

      TList* list = oldFile->GetListOfKeys() ;
      if (!list) { printf("<E> No keys found in file\n") ; exit(1) ; }
      TIter next(list) ;
      TKey* key ;
      TObject* obj ;
          
      while ( key = (TKey*)next() ) {

        TChain *fChain=nullptr;
        TTree *newtree=nullptr;

        obj = key->ReadObj() ;
        if ( (strcmp(obj->IsA()->GetName(),"TTree")!=0) || (strcmp("sumWeights",obj->GetName()) == 0) 
          || (strcmp("nominal",obj->GetName()) == 0) ) {
          printf("Not running over: %s \n",obj->GetName()); continue; 
        }
        printf("#####################################\n");
        printf("Currently working on %s \n",obj->GetName());
        printf("#####################################\n");

        fChain = new TChain(obj->GetName());
        fChain->Add((file).c_str());

    
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
        delete newtree;
        delete fChain;
      } // end loop over trees
      delete newfile;
      delete oldFile;
    } // end loop over channels
  }  // end loop over files

  return 0;
}
