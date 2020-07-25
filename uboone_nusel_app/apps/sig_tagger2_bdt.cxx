// cz: code modified from tutorials/tmva/TMVAClassification.C

#include <cstdlib>
#include <iostream>
#include <map>
#include <string>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"

#include "TMVA/Factory.h"
#include "TMVA/DataLoader.h"
#include "TMVA/Tools.h"
#include "TMVA/TMVAGui.h"
#include "TMVA/Reader.h"

using namespace std;

TFile *input = 0;
TFile *output = 0;
TMVA::Factory *factory = 0;
TMVA::DataLoader *dataloader = 0;


void Run_r1();
void InitBDT_r1();
void TestEvaluate(TString filename);


void Run_r2();
void InitBDT_r2();



void convert_file();


void convert_file(){
  TFile *file = new TFile("bdtfile_0718.root");
  TTree *sig = (TTree*)file->Get("sig");
  TTree *bkg = (TTree*)file->Get("bkg");

  int run, event;

  sig->SetBranchAddress("run",&run);
  sig->SetBranchAddress("event",&event);

  bkg->SetBranchAddress("run",&run);
  bkg->SetBranchAddress("event",&event);
  
  float trueEdep;
  float weight;
  float lowEweight;
  Int_t nueTag;

  sig->SetBranchAddress("trueEdep",&trueEdep);
  sig->SetBranchAddress("weight",&weight);
  sig->SetBranchAddress("lowEweight",&lowEweight);
  sig->SetBranchAddress("nueTag",&nueTag);
    
  bkg->SetBranchAddress("trueEdep",&trueEdep);
  bkg->SetBranchAddress("weight",&weight);
  bkg->SetBranchAddress("lowEweight",&lowEweight);
  bkg->SetBranchAddress("nueTag",&nueTag);

  int truth_inFV;
  int truth_CC;
  int truth_nue;
  int truth_cosmic;

  sig->SetBranchAddress("truth_inFV",&truth_inFV);
  sig->SetBranchAddress("truth_CC",&truth_CC);
  sig->SetBranchAddress("truth_nue",&truth_nue);
  sig->SetBranchAddress("truth_cosmic",&truth_cosmic);

  bkg->SetBranchAddress("truth_inFV",&truth_inFV);
  bkg->SetBranchAddress("truth_CC",&truth_CC);
  bkg->SetBranchAddress("truth_nue",&truth_nue);
  bkg->SetBranchAddress("truth_cosmic",&truth_cosmic);
  
  std::vector<double> *sig_2_v_energy= new std::vector<double>;
  std::vector<double> *sig_2_v_shower_angle= new std::vector<double>;
  std::vector<int> *sig_2_v_flag_single_shower= new std::vector<int>;
  std::vector<double> *sig_2_v_medium_dQ_dx= new std::vector<double>;
  std::vector<double> *sig_2_v_start_dQ_dx= new std::vector<double>;
  std::vector<int> *sig_2_v_flag= new std::vector<int>;

  sig->SetBranchAddress("sig_2_v_energy",&sig_2_v_energy);
  sig->SetBranchAddress("sig_2_v_shower_angle",&sig_2_v_shower_angle);
  sig->SetBranchAddress("sig_2_v_flag_single_shower",&sig_2_v_flag_single_shower);
  sig->SetBranchAddress("sig_2_v_medium_dQ_dx",&sig_2_v_medium_dQ_dx);
  sig->SetBranchAddress("sig_2_v_start_dQ_dx",&sig_2_v_start_dQ_dx);
  sig->SetBranchAddress("sig_2_v_flag",&sig_2_v_flag);

  bkg->SetBranchAddress("sig_2_v_energy",&sig_2_v_energy);
  bkg->SetBranchAddress("sig_2_v_shower_angle",&sig_2_v_shower_angle);
  bkg->SetBranchAddress("sig_2_v_flag_single_shower",&sig_2_v_flag_single_shower);
  bkg->SetBranchAddress("sig_2_v_medium_dQ_dx",&sig_2_v_medium_dQ_dx);
  bkg->SetBranchAddress("sig_2_v_start_dQ_dx",&sig_2_v_start_dQ_dx);
  bkg->SetBranchAddress("sig_2_v_flag",&sig_2_v_flag);
  
  TFile *new_file = new TFile("reduced.root","RECREATE");
  TTree *Tsig = new TTree("sig","sig");
  TTree *Tbkg = new TTree("bkg","bkg");
  Tsig->SetDirectory(new_file);
  Tbkg->SetDirectory(new_file);

  Tsig->Branch("truth_inFV",&truth_inFV,"data/I");
  Tsig->Branch("truth_CC",&truth_CC,"data/I");
  Tsig->Branch("truth_nue",&truth_nue,"data/I");
  Tsig->Branch("truth_cosmic",&truth_cosmic,"data/I");

  Tbkg->Branch("truth_inFV",&truth_inFV,"data/I");
  Tbkg->Branch("truth_CC",&truth_CC,"data/I");
  Tbkg->Branch("truth_nue",&truth_nue,"data/I");
  Tbkg->Branch("truth_cosmic",&truth_cosmic,"data/I");
  
  
  Tsig->Branch("run",&run,"data/I");
  Tsig->Branch("event",&event,"data/I");

  Tbkg->Branch("run",&run,"data/I");
  Tbkg->Branch("event",&event,"data/I");

  Tsig->Branch("trueEdep",&trueEdep,"data/F");
  Tsig->Branch("weight",&weight,"data/F");
  Tsig->Branch("lowEweight",&lowEweight,"data/F");
  Tsig->Branch("nueTag",&nueTag,"data/I");
    
  Tbkg->Branch("trueEdep",&trueEdep,"data/F");
  Tbkg->Branch("weight",&weight,"data/F");
  Tbkg->Branch("lowEweight",&lowEweight,"data/F");
  Tbkg->Branch("nueTag",&nueTag,"data/I");

  float sig_2_v_energy_f;
  float sig_2_v_shower_angle_f;
  float sig_2_v_flag_single_shower_f;
  float sig_2_v_medium_dQ_dx_f;
  float sig_2_v_start_dQ_dx_f;
  float sig_2_v_flag_f;
  
  Tsig->Branch("sig_2_v_energy",&sig_2_v_energy_f,"data/F");
  Tsig->Branch("sig_2_v_shower_angle",&sig_2_v_shower_angle_f,"data/F");
  Tsig->Branch("sig_2_v_flag_single_shower",&sig_2_v_flag_single_shower_f,"data/F");
  Tsig->Branch("sig_2_v_medium_dQ_dx",&sig_2_v_medium_dQ_dx_f,"data/F");
  Tsig->Branch("sig_2_v_start_dQ_dx",&sig_2_v_start_dQ_dx_f,"data/F");
  Tsig->Branch("sig_2_v_flag",&sig_2_v_flag_f,"data/F");

  Tbkg->Branch("sig_2_v_energy",&sig_2_v_energy_f,"data/F");
  Tbkg->Branch("sig_2_v_shower_angle",&sig_2_v_shower_angle_f,"data/F");
  Tbkg->Branch("sig_2_v_flag_single_shower",&sig_2_v_flag_single_shower_f,"data/F");
  Tbkg->Branch("sig_2_v_medium_dQ_dx",&sig_2_v_medium_dQ_dx_f,"data/F");
  Tbkg->Branch("sig_2_v_start_dQ_dx",&sig_2_v_start_dQ_dx_f,"data/F");
  Tbkg->Branch("sig_2_v_flag",&sig_2_v_flag_f,"data/F");

  
  for (Int_t i=0;i!=sig->GetEntries();i++){
    sig->GetEntry(i);
    
    
    for (size_t j=0;j!=sig_2_v_energy->size();j++){
      sig_2_v_energy_f = sig_2_v_energy->at(j);
      sig_2_v_shower_angle_f = sig_2_v_shower_angle->at(j);
      sig_2_v_flag_single_shower_f = sig_2_v_flag_single_shower->at(j);
      sig_2_v_medium_dQ_dx_f = sig_2_v_medium_dQ_dx->at(j);
      sig_2_v_start_dQ_dx_f = sig_2_v_start_dQ_dx->at(j);
      sig_2_v_flag_f = sig_2_v_flag->at(j);
      
      Tsig->Fill();
    }
    
  }

   for (Int_t i=0;i!=bkg->GetEntries();i++){
    bkg->GetEntry(i);
    
   

    for (size_t j=0;j!=sig_2_v_energy->size();j++){
      sig_2_v_energy_f = sig_2_v_energy->at(j);
      sig_2_v_shower_angle_f = sig_2_v_shower_angle->at(j);
      sig_2_v_flag_single_shower_f = sig_2_v_flag_single_shower->at(j);
      sig_2_v_medium_dQ_dx_f = sig_2_v_medium_dQ_dx->at(j);
      sig_2_v_start_dQ_dx_f = sig_2_v_start_dQ_dx->at(j);
      sig_2_v_flag_f = sig_2_v_flag->at(j);
      
     
      
      Tbkg->Fill();
    }
    
  }
  
  
  new_file->Write();
  new_file->Close();
  
  
}

void Run_r2(){
  TMVA::Tools::Instance();

  TString fname = "./round_1.root";
  input = TFile::Open( fname ); // check if file in local directory exists
  cout << "Using input file: " << input->GetName() << std::endl;

  TString outfileName( "TMVA_r2.root" );
  output = TFile::Open( outfileName, "RECREATE" );

  InitBDT_r2();

  // Train MVAs using the set of training events
    factory->TrainAllMethods();

    // Evaluate all MVAs using the set of test events
    factory->TestAllMethods();

    // Evaluate and compare performance of all configured MVAs
    factory->EvaluateAllMethods();
    
    output->Close();
    cout << "Results saved at: " << output->GetName() << std::endl;

    delete factory;
    delete dataloader;

    TestEvaluate("round_2.root");

    // Launch the GUI for the root macros
    if (!gROOT->IsBatch()) TMVA::TMVAGui( output->GetName() );
  
}

void Run_r1()
{
    TMVA::Tools::Instance();

    TString fname = "./reduced.root";
    input = TFile::Open( fname ); // check if file in local directory exists
    cout << "Using input file: " << input->GetName() << std::endl;

    TString outfileName( "TMVA.root" );
    output = TFile::Open( outfileName, "RECREATE" );


    InitBDT_r1();


    // Train MVAs using the set of training events
    factory->TrainAllMethods();

    // Evaluate all MVAs using the set of test events
    factory->TestAllMethods();

    // Evaluate and compare performance of all configured MVAs
    factory->EvaluateAllMethods();
    
    output->Close();
    cout << "Results saved at: " << output->GetName() << std::endl;

    delete factory;
    delete dataloader;

    TestEvaluate("round_1.root");

    // Launch the GUI for the root macros
    if (!gROOT->IsBatch()) TMVA::TMVAGui( output->GetName() );

}

void InitBDT_r2(){
   factory = new TMVA::Factory( "Test", output,
        "!V:!Silent:Color:DrawProgressBar:"
				 //    "Transformations=I;D;P;G,D:"
				 "AnalysisType=Classification" );
    			 


    dataloader = new TMVA::DataLoader("dataset");
    // Define the input variables that shall be used for the MVA training
    // note that you may also use variable expressions, such as: "3*var1/var2*abs(var3)"
    // [all types of expressions that can also be parsed by TTree::Draw( "expression" )]
   //  float sig_2_v_energy_f;
    // float sig_2_v_shower_angle_f;
    // float sig_2_v_flag_single_shower_f;
    // float sig_2_v_medium_dQ_dx_f;
    // float sig_2_v_start_dQ_dx_f;
    // float sig_2_v_flag_f;
    dataloader->AddVariable("sig_2_v_energy","sig_2_v_energy","MeV",'F');
    dataloader->AddVariable("sig_2_v_shower_angle","sig_2_v_shower_angle","MeV",'F');
    dataloader->AddVariable("sig_2_v_flag_single_shower","sig_2_v_flag_single_shower","",'F');
    dataloader->AddVariable("sig_2_v_medium_dQ_dx","sig_2_v_medium_dQ_dx","MeV/cm",'F');
    dataloader->AddVariable("sig_2_v_start_dQ_dx","sig_2_v_start_dQ_dx","MeV/cm",'F');


    
    TTree *signalTree     = (TTree*)input->Get("sig");
    TTree *backgroundTree = (TTree*)input->Get("bkg");
    dataloader->AddSignalTree(signalTree, 1.0); // can add the global event weight
    dataloader->AddBackgroundTree( backgroundTree, 1.0);
    // Set individual event weights (the variables must exist in the original TTree)
    dataloader->SetSignalWeightExpression( "weight *lowEweight" );
    dataloader->SetBackgroundWeightExpression( "weight " );

    // Apply additional cuts on the signal and background samples (can be different)
    TCut mycut_s = "1>0 "; // 106 / 44194,
    TCut mycut_b = "(sig_2_v_flag==0 || sig_2_v_bdt < 0.1) && (!(truth_nue==1 && truth_CC==1))"; // 1375/21070, adding mip_id 340
    
    dataloader->PrepareTrainingAndTestTree( mycut_s, mycut_b,
        "nTrain_Signal=20000:"
        "nTrain_Background=1200:"
	"nTest_Signal=10000:"
        "nTest_Background=175:"
        "SplitMode=Random:"
        "NormMode=NumEvents:"
        "!V" );

    // variations of BDTs
    // BDT: uses Adaptive Boost
    // BDTG: uses Gradient Boost
    // BDTB: uses Bagging
    // BDTD: decorrelation + Adaptive Boost
    // BDTF: allow usage of fisher discriminant for node splitting
    factory->BookMethod(dataloader, TMVA::Types::kBDT, "BDT",
        "!H:!V:"
       	"VarTransform=D,P,U,G,D,N:"
        "NTrees=850:"
        "MinNodeSize=2.5%:"
        "MaxDepth=3:"
        "BoostType=AdaBoost:"
        "AdaBoostBeta=0.5:"
        "UseBaggedBoost:"
        "BaggedSampleFraction=0.5:"
        "SeparationType=GiniIndex:"
        "nCuts=20");
}


void InitBDT_r1()
{
    factory = new TMVA::Factory( "Test", output,
        "!V:!Silent:Color:DrawProgressBar:"
				 //    "Transformations=I;D;P;G,D:"
				 "AnalysisType=Classification" );
    			 


    dataloader = new TMVA::DataLoader("dataset");
    // Define the input variables that shall be used for the MVA training
    // note that you may also use variable expressions, such as: "3*var1/var2*abs(var3)"
    // [all types of expressions that can also be parsed by TTree::Draw( "expression" )]
   //  float sig_2_v_energy_f;
    // float sig_2_v_shower_angle_f;
    // float sig_2_v_flag_single_shower_f;
    // float sig_2_v_medium_dQ_dx_f;
    // float sig_2_v_start_dQ_dx_f;
    // float sig_2_v_flag_f;
    dataloader->AddVariable("sig_2_v_energy","sig_2_v_energy","MeV",'F');
    dataloader->AddVariable("sig_2_v_shower_angle","sig_2_v_shower_angle","MeV",'F');
    dataloader->AddVariable("sig_2_v_flag_single_shower","sig_2_v_flag_single_shower","",'F');
    dataloader->AddVariable("sig_2_v_medium_dQ_dx","sig_2_v_medium_dQ_dx","MeV/cm",'F');
    dataloader->AddVariable("sig_2_v_start_dQ_dx","sig_2_v_start_dQ_dx","MeV/cm",'F');


    
    TTree *signalTree     = (TTree*)input->Get("sig");
    TTree *backgroundTree = (TTree*)input->Get("bkg");
    dataloader->AddSignalTree(signalTree, 1.0); // can add the global event weight
    dataloader->AddBackgroundTree( backgroundTree, 1.0);
    // Set individual event weights (the variables must exist in the original TTree)
    dataloader->SetSignalWeightExpression( "weight *lowEweight" );
    dataloader->SetBackgroundWeightExpression( "weight " );

    // Apply additional cuts on the signal and background samples (can be different)
    TCut mycut_s = "1>0 "; // 106 / 44194,
    TCut mycut_b = "sig_2_v_flag==0 && (!(truth_nue==1 && truth_CC==1))"; // 768/21070, adding mip_id 340
    
    dataloader->PrepareTrainingAndTestTree( mycut_s, mycut_b,
        "nTrain_Signal=20000:"
        "nTrain_Background=600:"
	"nTest_Signal=10000:"
        "nTest_Background=168:"
        "SplitMode=Random:"
        "NormMode=NumEvents:"
        "!V" );

    // variations of BDTs
    // BDT: uses Adaptive Boost
    // BDTG: uses Gradient Boost
    // BDTB: uses Bagging
    // BDTD: decorrelation + Adaptive Boost
    // BDTF: allow usage of fisher discriminant for node splitting
    factory->BookMethod(dataloader, TMVA::Types::kBDT, "BDT",
        "!H:!V:"
       	"VarTransform=D,P,U,G,D,N:"
        "NTrees=850:"
        "MinNodeSize=2.5%:"
        "MaxDepth=3:"
        "BoostType=AdaBoost:"
        "AdaBoostBeta=0.5:"
        "UseBaggedBoost:"
        "BaggedSampleFraction=0.5:"
        "SeparationType=GiniIndex:"
        "nCuts=20");

}


void TestEvaluate(TString filename)
{
 
  Float_t bdt_value = 0;
    
  TFile *file = new TFile("reduced.root");
  TTree *sig = (TTree*)file->Get("sig");
  TTree *bkg = (TTree*)file->Get("bkg");

  int run, event;
  sig->SetBranchAddress("run",&run);
  sig->SetBranchAddress("event",&event);

  bkg->SetBranchAddress("run",&run);
  bkg->SetBranchAddress("event",&event);
  
  float trueEdep;
  float weight;
  float lowEweight;
  Int_t nueTag;
  
  sig->SetBranchAddress("trueEdep",&trueEdep);
  sig->SetBranchAddress("weight",&weight);
  sig->SetBranchAddress("lowEweight",&lowEweight);
  sig->SetBranchAddress("nueTag",&nueTag);
  
  bkg->SetBranchAddress("trueEdep",&trueEdep);
  bkg->SetBranchAddress("weight",&weight);
  bkg->SetBranchAddress("lowEweight",&lowEweight);
  bkg->SetBranchAddress("nueTag",&nueTag);

   int truth_inFV;
  int truth_CC;
  int truth_nue;
  int truth_cosmic;

  sig->SetBranchAddress("truth_inFV",&truth_inFV);
  sig->SetBranchAddress("truth_CC",&truth_CC);
  sig->SetBranchAddress("truth_nue",&truth_nue);
  sig->SetBranchAddress("truth_cosmic",&truth_cosmic);

  bkg->SetBranchAddress("truth_inFV",&truth_inFV);
  bkg->SetBranchAddress("truth_CC",&truth_CC);
  bkg->SetBranchAddress("truth_nue",&truth_nue);
  bkg->SetBranchAddress("truth_cosmic",&truth_cosmic);


  
  float sig_2_v_energy;
  float sig_2_v_shower_angle;
  float sig_2_v_flag_single_shower;
  float sig_2_v_medium_dQ_dx;
  float sig_2_v_start_dQ_dx;
  float sig_2_v_flag;

  sig->SetBranchAddress("sig_2_v_energy",&sig_2_v_energy);
  sig->SetBranchAddress("sig_2_v_shower_angle",&sig_2_v_shower_angle);
  sig->SetBranchAddress("sig_2_v_flag_single_shower",&sig_2_v_flag_single_shower);
  sig->SetBranchAddress("sig_2_v_medium_dQ_dx",&sig_2_v_medium_dQ_dx);
  sig->SetBranchAddress("sig_2_v_start_dQ_dx",&sig_2_v_start_dQ_dx);
  sig->SetBranchAddress("sig_2_v_flag",&sig_2_v_flag);

  bkg->SetBranchAddress("sig_2_v_energy",&sig_2_v_energy);
  bkg->SetBranchAddress("sig_2_v_shower_angle",&sig_2_v_shower_angle);
  bkg->SetBranchAddress("sig_2_v_flag_single_shower",&sig_2_v_flag_single_shower);
  bkg->SetBranchAddress("sig_2_v_medium_dQ_dx",&sig_2_v_medium_dQ_dx);
  bkg->SetBranchAddress("sig_2_v_start_dQ_dx",&sig_2_v_start_dQ_dx);
  bkg->SetBranchAddress("sig_2_v_flag",&sig_2_v_flag);
  
  
  TFile *new_file = new TFile(filename,"RECREATE");
  TTree *Tsig = new TTree("sig","sig");
  TTree *Tbkg = new TTree("bkg","bkg");
  Tsig->SetDirectory(new_file);
  Tbkg->SetDirectory(new_file);
  
 
  Tsig->Branch("trueEdep",&trueEdep,"data/F");
  Tsig->Branch("weight",&weight,"data/F");
  Tsig->Branch("lowEweight",&lowEweight,"data/F");
  Tsig->Branch("nueTag",&nueTag,"data/I");
 
  Tbkg->Branch("trueEdep",&trueEdep,"data/F");
  Tbkg->Branch("weight",&weight,"data/F");
  Tbkg->Branch("lowEweight",&lowEweight,"data/F");
  Tbkg->Branch("nueTag",&nueTag,"data/I");
  
  Tsig->Branch("run",&run,"data/I");
  Tsig->Branch("event",&event,"data/I");

  Tbkg->Branch("run",&run,"data/I");
  Tbkg->Branch("event",&event,"data/I");

  Tsig->Branch("truth_inFV",&truth_inFV,"data/I");
  Tsig->Branch("truth_CC",&truth_CC,"data/I");
  Tsig->Branch("truth_nue",&truth_nue,"data/I");
  Tsig->Branch("truth_cosmic",&truth_cosmic,"data/I");

  Tbkg->Branch("truth_inFV",&truth_inFV,"data/I");
  Tbkg->Branch("truth_CC",&truth_CC,"data/I");
  Tbkg->Branch("truth_nue",&truth_nue,"data/I");
  Tbkg->Branch("truth_cosmic",&truth_cosmic,"data/I");

  
  // float sig_2_v_energy;
  // float sig_2_v_shower_angle;
  // float sig_2_v_flag_single_shower;
  // float sig_2_v_medium_dQ_dx;
  // float sig_2_v_start_dQ_dx;
  // float sig_2_v_flag;
  
  Tsig->Branch("sig_2_v_energy",&sig_2_v_energy,"data/F");
  Tsig->Branch("sig_2_v_flag_single_shower",&sig_2_v_flag_single_shower,"data/F");
  Tsig->Branch("sig_2_v_shower_angle",&sig_2_v_shower_angle,"data/F");
  Tsig->Branch("sig_2_v_medium_dQ_dx",&sig_2_v_medium_dQ_dx,"data/F");
  Tsig->Branch("sig_2_v_start_dQ_dx",&sig_2_v_start_dQ_dx,"data/F");
  Tsig->Branch("sig_2_v_flag",&sig_2_v_flag,"data/F");
  Tsig->Branch("sig_2_v_bdt",&bdt_value,"data/F");
  
  Tbkg->Branch("sig_2_v_energy",&sig_2_v_energy,"data/F");
  Tbkg->Branch("sig_2_v_flag_single_shower",&sig_2_v_flag_single_shower,"data/F");
  Tbkg->Branch("sig_2_v_shower_angle",&sig_2_v_shower_angle,"data/F");
  Tbkg->Branch("sig_2_v_medium_dQ_dx",&sig_2_v_medium_dQ_dx,"data/F");
  Tbkg->Branch("sig_2_v_start_dQ_dx",&sig_2_v_start_dQ_dx,"data/F");
  Tbkg->Branch("sig_2_v_flag",&sig_2_v_flag,"data/F");
  Tbkg->Branch("sig_2_v_bdt",&bdt_value,"data/F");
  
  TMVA::Reader *reader = new TMVA::Reader();
  reader->AddVariable("sig_2_v_energy",&sig_2_v_energy);
  reader->AddVariable("sig_2_v_shower_angle",&sig_2_v_shower_angle);
  reader->AddVariable("sig_2_v_flag_single_shower",&sig_2_v_flag_single_shower);
  reader->AddVariable("sig_2_v_medium_dQ_dx",&sig_2_v_medium_dQ_dx);  
  reader->AddVariable("sig_2_v_start_dQ_dx",&sig_2_v_start_dQ_dx);
  reader->BookMVA( "MyBDT", "dataset/weights/Test_BDT.weights.xml");


  for (Int_t i=0;i!=sig->GetEntries();i++){
    sig->GetEntry(i);
    bdt_value = reader->EvaluateMVA("MyBDT");
    Tsig->Fill();
  }
  
  for (Int_t i=0;i!=bkg->GetEntries();i++){
    bkg->GetEntry(i);
    bdt_value = reader->EvaluateMVA("MyBDT");
    Tbkg->Fill();
  }

  new_file->Write();
  new_file->Close();
  

  
}








int main( int argc, char** argv )
{
  int process = 1;

  for (Int_t i=1;i!=argc;i++){
    switch(argv[i][1]){
    case 'p':
      process = atoi(&argv[i][2]);
      break;
    }
  }

  if (process==1){
    std::cout << "Prepare the initial rootfile" << std::endl;
    convert_file();
  }else if (process == 2){
    std::cout << "testing BDT in ROOT TMVA (1st round)" << std::endl;
    Run_r1();
  }else if (process == 3){
    std::cout << "testing BDT in ROOT TMVA (2nd round)" << std::endl;
    Run_r2();
  }
  
  return 1;
    
}

