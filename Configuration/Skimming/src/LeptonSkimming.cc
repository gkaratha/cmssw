

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
#include "Configuration/Skimming/interface/LeptonSkimming.h"


using namespace edm;
using namespace reco;
using namespace std;

LeptonSkimming::LeptonSkimming(const edm::ParameterSet& iConfig):
  electronsToken_(consumes<edm::View<reco::GsfElectron> >(iConfig.getParameter<edm::InputTag>  ("electrons"))),
  muonsToken_(consumes<std::vector<reco::Muon>>(iConfig.getParameter<edm::InputTag>("muons"))),
  Tracks_(consumes<std::vector<reco::Track> >(iConfig.getParameter<edm::InputTag>("tracks"))),
       vtxToken_(consumes<reco::VertexCollection>(iConfig.getParameter<edm::InputTag>("vertices"))),
  beamSpotToken_(consumes<reco::BeamSpot>(iConfig.getParameter<edm::InputTag>("beamSpot"))),
  conversionsToken_(consumes< reco::ConversionCollection > (iConfig.getParameter<edm::InputTag> ("conversions"))),

   trgresultsToken_(consumes<edm::TriggerResults>(iConfig.getParameter<edm::InputTag> ("triggerresults"))),
  trigobjectsToken_(consumes<trigger::TriggerEvent>(iConfig.getParameter<edm::InputTag> ("triggerobjects"))),
  HLTFilter_(iConfig.getParameter<vector<string> >("HLTFilter")),
  HLTPath_(iConfig.getParameter<vector<string> >("HLTPath")),
  GenToken_(consumes<edm::View<reco::GenParticle>>(iConfig.getParameter<edm::InputTag>("gen")))
  {
  edm::ParameterSet runParameters=iConfig.getParameter<edm::ParameterSet>("RunParameters");     
 IsData=runParameters.getParameter<bool>("Data");
 SaveHLT=runParameters.getParameter<bool>("SaveHLT");
 PtTrack_Cut=runParameters.getParameter<double>("PtTrack_Cut");
 EtaTrack_Cut=runParameters.getParameter<double>("EtaTrack_Cut");
 //cout<<PtTrack_Cut<<endl;
 MinChi2Track_Cut=runParameters.getParameter<double>("MinChi2Track_Cut");
 MaxChi2Track_Cut=runParameters.getParameter<double>("MaxChi2Track_Cut");
 MuTrkMinDR_Cut=runParameters.getParameter<double>("MuTrkMinDR_Cut");
 MaxMee_Cut=runParameters.getParameter<double>("MaxMee_Cut");
 MinMee_Cut=runParameters.getParameter<double>("MinMee_Cut");
 Probee_Cut=runParameters.getParameter<double>("Probee_Cut");
 Cosee_Cut=runParameters.getParameter<double>("Cosee_Cut");
 
 PtKTrack_Cut=runParameters.getParameter<double>("PtKTrack_Cut");
 MaxMB_Cut=runParameters.getParameter<double>("MaxMB_Cut");
 MinMB_Cut=runParameters.getParameter<double>("MinMB_Cut");
 TrkTrkMinDR_Cut=runParameters.getParameter<double>("TrkTrkMinDR_Cut");
 SaveOnlyTracks=runParameters.getParameter<bool>("SaveOnlyTracks");
 SaveOnlyEPairTracks=runParameters.getParameter<bool>("SaveOnlyEPairTracks");
 UseOnlyBKeeMCForTriplets=runParameters.getParameter<bool>("UseOnlyBKeeMCForTriplets");
 TrackSdxy_Cut=runParameters.getParameter<double>("TrackSdxy_Cut");
 EarlyStop=runParameters.getParameter<bool>("EarlyStop");
 MuTrgMatchCone=runParameters.getParameter<double>("MuTrgMatchCone");
 SkipIfNoMuMatch=runParameters.getParameter<bool>("SkipIfNoMuMatch");
 EpairZvtx_Cut=runParameters.getParameter<double>("EpairZvtx_Cut");
 Ksdxy_Cut=runParameters.getParameter<double>("Ksdxy_Cut");
 ProbeeK_Cut=runParameters.getParameter<double>("ProbeeK_Cut");
 CoseeK_Cut=runParameters.getParameter<double>("CoseeK_Cut");
 TrackMuDz_Cut=runParameters.getParameter<double>("TrackMuDz_Cut");
 MaxMVA_Cut=runParameters.getParameter<double>("MaxMVA_Cut");
 MinMVA_Cut=runParameters.getParameter<double>("MinMVA_Cut");
 TrgExclusionCone=runParameters.getParameter<double>("TrgExclusionCone");
 SLxy_Cut=runParameters.getParameter<double>("SLxy_Cut");
 PtB_Cut=runParameters.getParameter<double>("PtB_Cut");
 PtMu_Cut=runParameters.getParameter<double>("PtMu_Cut");
  PtEl_Cut=runParameters.getParameter<double>("PtEl_Cut");
 QualMu_Cut=runParameters.getParameter<double>("QualMu_Cut");
 MuTrgExclusionCone=runParameters.getParameter<double>("MuTrgExclusionCone");
 ElTrgExclusionCone=runParameters.getParameter<double>("ElTrgExclusionCone");
 TrkObjExclusionCone=runParameters.getParameter<double>("TrkObjExclusionCone");
 MuTrgMuDz_Cut=runParameters.getParameter<double>("MuTrgMuDz_Cut");
 ElTrgMuDz_Cut=runParameters.getParameter<double>("ElTrgMuDz_Cut");
 ObjPtLargerThanTrack=runParameters.getParameter<bool>("ObjPtLargerThanTrack");
 SaveOutputRoot=runParameters.getParameter<bool>("SaveOutputRoot");
  t1=fs->make<TTree>("mytree","mytree"); 
  t1->Branch("event",&event);
  t1->Branch("run_number",&run_number);
  t1->Branch("ls",&ls);
  
  t1->Branch("vertex_x",&vertex_x); t1->Branch("vertex_y",&vertex_y);
  t1->Branch("vertex_z",&vertex_z); t1->Branch("beam_x",&beam_x);
  t1->Branch("beam_y",&beam_y); t1->Branch("beam_z",&beam_z);

  t1->Branch("HLT_path1",&trigger1); t1->Branch("HLT_path2",&trigger2);
  t1->Branch("HLT_path3",&trigger3); t1->Branch("HLT_path4",&trigger4);
  t1->Branch("HLT_path5",&trigger5); t1->Branch("HLT_path6",&trigger6);
  t1->Branch("TrgObj1_PtEtaPhiCharge",&TrgObj1_PtEtaPhiCharge);
  t1->Branch("TrgObj2_PtEtaPhiCharge",&TrgObj2_PtEtaPhiCharge);
  t1->Branch("TrgObj3_PtEtaPhiCharge",&TrgObj3_PtEtaPhiCharge);
  t1->Branch("TrgObj4_PtEtaPhiCharge",&TrgObj4_PtEtaPhiCharge);
  t1->Branch("TrgObj5_PtEtaPhiCharge",&TrgObj5_PtEtaPhiCharge);
  t1->Branch("TrgObj6_PtEtaPhiCharge",&TrgObj6_PtEtaPhiCharge);
  t1->Branch("SelectedTrgObj_PtEtaPhiCharge",&SelectedTrgObj_PtEtaPhiCharge);
  t1->Branch("SelectedMu_index",&SelectedMu_index);
  t1->Branch("SelectedMu_DR",&SelectedMu_DR);

   
  t1->Branch("nmuon",&nmuons); t1->Branch("muon_pt",&muon_pt);
  t1->Branch("muon_eta",&muon_eta); t1->Branch("muon_phi",&muon_phi);
  t1->Branch("muon_charge",&muon_charge); t1->Branch("muon_dxy",&muon_dxy);
  t1->Branch("muon_dz",&muon_dz); t1->Branch("muon_edxy",&muon_edxy);
  t1->Branch("muon_edz",&muon_edz); t1->Branch("muon_d0",&muon_d0);
  t1->Branch("muon_ed0",&muon_ed0); t1->Branch("muon_vx",&muon_vx);
  t1->Branch("muon_vy",&muon_vy); t1->Branch("muon_vz",&muon_vz);
  t1->Branch("muon_iso",&muon_iso); t1->Branch("muon_soft",&muon_soft);
  t1->Branch("muon_loose",&muon_loose); t1->Branch("muon_medium",&muon_medium);
  t1->Branch("muon_tight",&muon_tight);t1->Branch("muon_trkpt",&muon_trkpt); 
  t1->Branch("muon_trketa",&muon_trketa);t1->Branch("muon_trkphi",&muon_trkphi);
  
  t1->Branch("nelectron",&nel); t1->Branch("el_pt",&el_pt);
  t1->Branch("el_eta",&el_eta); t1->Branch("el_phi",&el_phi);
  t1->Branch("el_charge",&el_charge); t1->Branch("el_dxy",&el_dxy);
  t1->Branch("el_dz",&el_dz); t1->Branch("el_edxy",&el_edxy);
  t1->Branch("el_edz",&el_edz); t1->Branch("el_vx",&el_vx);
  t1->Branch("el_vy",&el_vy); t1->Branch("el_vz",&el_vz);
  t1->Branch("el_mva_out",&el_mva_out); t1->Branch("el_mva_iso",&el_mva_iso);
  t1->Branch("el_iso",&el_iso); t1->Branch("el_veto",&el_veto);
  t1->Branch("el_soft",&el_soft); t1->Branch("el_medium",&el_medium);
  t1->Branch("el_tight",&el_tight); t1->Branch("el_mva_map_value",&el_mva_map_value);
   t1->Branch("el_trkpt",&el_trkpt); t1->Branch("el_trketa",&el_trketa); 
   t1->Branch("el_trkphi",&el_trkphi);


  t1->Branch("genpart_pt",&genpart_pt); t1->Branch("genpart_phi",&genpart_phi);
  t1->Branch("genpart_eta",&genpart_eta); t1->Branch("genpart_pdgId",&genpart_pdgId);
  t1->Branch("genpart_Bindex",&genpart_Bindex);
  t1->Branch("genpart_Daughtindex",&genpart_Daughtindex);
  t1->Branch("genpart_charge",&genpart_charge);
  t1->Branch("genpart_mother_pdgId",&genpart_mother_pdgId);
  t1->Branch("genpart_mother_pt",&genpart_mother_pt);
  t1->Branch("genpart_mother_phi",&genpart_mother_phi);
  t1->Branch("genpart_mother_eta",&genpart_mother_eta);
  t1->Branch("genpart_mother_Bindex",&genpart_mother_Bindex);
  t1->Branch("genpart_mother_Daughtindex",&genpart_mother_Daughtindex);
  t1->Branch("genpart_grandmother_pdgId",&genpart_grandmother_pdgId);
  t1->Branch("genpart_grandmother_pt",&genpart_grandmother_pt);
  t1->Branch("genpart_grandmother_phi",&genpart_grandmother_phi);
  t1->Branch("genpart_grandmother_eta",&genpart_grandmother_eta);
  t1->Branch("genpart_grandmother_Bindex",&genpart_grandmother_Bindex);
  t1->Branch("genpart_grandmother_x",&genpart_grandmother_x);
  t1->Branch("genpart_grandmother_y",&genpart_grandmother_y);
  t1->Branch("genpart_grandmother_z",&genpart_grandmother_z);
  t1->Branch("genMu_pt",&genMu_pt);  t1->Branch("genMu_eta",&genMu_eta);
  t1->Branch("genMu_phi",&genMu_phi);  t1->Branch("genMu_ch",&genMu_ch);
  t1->Branch("genMu_motherId",&genMu_motherId);  t1->Branch("genMu_gmotherId",&genMu_gmotherId);
   
  t1->Branch("ntracks",&ntracks); t1->Branch("track_pt",&track_pt);
  t1->Branch("track_eta",&track_eta); t1->Branch("track_phi",&track_phi);
  t1->Branch("track_norm_chi2",&track_norm_chi2);
  t1->Branch("track_charge",&track_charge);
  t1->Branch("track_dxy",&track_dxy); t1->Branch("track_dz",&track_dz);
  t1->Branch("track_edxy",&track_edxy); t1->Branch("track_edz",&track_edz);
  t1->Branch("track_vx",&track_vx); t1->Branch("track_vy",&track_vy);
  t1->Branch("track_vz",&track_vz); t1->Branch("track_mva",&track_mva);

  t1->Branch("Epair_PtEtaPhiM",&Epair_PtEtaPhiM);
  t1->Branch("Epair_XYZ",&Epair_XYZ); t1->Branch("Epair_cos",&Epair_cos);
  t1->Branch("Epair_chi_prob",&Epair_chi_prob);
  t1->Branch("Epair_ObjIndex",&Epair_ObjIndex); t1->Branch("Epair_TrkIndex",&Epair_TrkIndex);
  t1->Branch("Epair_Lxy",&Epair_Lxy); t1->Branch("Epair_eLxy",&Epair_eLxy);
  t1->Branch("Epair_ObjId",&Epair_ObjId);

  t1->Branch("TTrack_PtEtaPhiM",&TTrack_PtEtaPhiM);
  t1->Branch("TTrack_chi_prob",&TTrack_chi_prob);
  // t1->Branch("TTrack_min2trk_prob",&TTrack_min2trk_prob);
  t1->Branch("TTrack_XYZ",&TTrack_XYZ); t1->Branch("TTrack_ObjIndex",&TTrack_ObjIndex);
  t1->Branch("TTrack_TrkIndex",&TTrack_TrkIndex); t1->Branch("TTrack_kid",&TTrack_kid);
  t1->Branch("TTrack_mll",&TTrack_mll); t1->Branch("TTrack_cos",&TTrack_cos);
  t1->Branch("TTrack_Lxy",&TTrack_Lxy); t1->Branch("TTrack_eLxy",&TTrack_eLxy);
  t1->Branch("TTrack_ObjId",&TTrack_ObjId);

}


LeptonSkimming::~LeptonSkimming()
{
 
   // do anything here that needs to be done at destruction time
   // (e.g. close files, deallocate resources etc.)

}

std::vector<std::vector<float>> LeptonSkimming::genAnalyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
  using namespace std;
  using namespace edm;
  using namespace reco;
  using namespace trigger;
//  cout<<" new event"<<endl;
  edm::Handle<edm::View<reco::GenParticle>> genPart;
  iEvent.getByToken(GenToken_,genPart);
  std::vector<float> temp_genpart_pt,temp_genpart_phi,temp_genpart_eta,
  temp_genpart_pdgId,temp_genpart_charge,temp_genpart_Bindex,
  temp_genpart_Daughtindex,temp_genpart_mother_pt,temp_genpart_mother_eta,
  temp_genpart_mother_phi,temp_genpart_mother_pdgId,temp_genpart_mother_Bindex,
  temp_genpart_mother_Daughtindex,temp_genpart_grandmother_pt,
  temp_genpart_grandmother_eta,temp_genpart_grandmother_phi,
  temp_genpart_grandmother_pdgId,temp_genpart_grandmother_Bindex,
  temp_genpart_grandmother_x,temp_genpart_grandmother_y,temp_genpart_grandmother_z;
  int c=0;
  for(typename edm::View<reco::GenParticle>::const_iterator gen=genPart->begin(); gen !=genPart->end(); gen++){
    if (abs(gen->pdgId())!=11 && abs(gen->pdgId())!=13) continue;
    if (gen->status()!=1) continue;
    if (gen->numberOfMothers()==0) continue;
    const Candidate* mother2 =gen->mother();
    if (mother2->numberOfMothers()==0) continue;
    const Candidate * grandmother2= mother2->mother();
    if ( (mother2->pdgId() % 1000)/100!=5 && (mother2->pdgId() % 10000)/1000!=5 &&  (mother2->pdgId() % 1000)/100!= -5  && (mother2->pdgId() % 10000)/1000!=-5  && (grandmother2->pdgId() % 1000)/100!=5 && (grandmother2->pdgId() % 10000)/1000!=5 &&  (grandmother2->pdgId() % 1000)/100!= -5  && (grandmother2->pdgId() % 10000)/1000!=-5 ) continue;    
    //cout<<"new "<<endl;
    c++;
    const Candidate * temp_gmom;
    if ( (mother2->pdgId() % 1000)/100==5 || (mother2->pdgId() % 10000)/1000==5 || (mother2->pdgId() % 1000)/100== -5 || (mother2->pdgId() % 10000)/1000==-5) 
       temp_gmom=gen->mother();
    else
       temp_gmom=mother2->mother();
     //check for double B due to two e or mu  
    bool already_saved=false;
    for(unsigned int doubleB=0; doubleB<temp_genpart_grandmother_pdgId.size(); doubleB++){
        if (temp_gmom->pdgId()==temp_genpart_grandmother_pdgId.at(doubleB) && temp_gmom->pt()==temp_genpart_grandmother_pt.at(doubleB) && temp_gmom->eta()==temp_genpart_grandmother_eta.at(doubleB) && temp_gmom->phi()==temp_genpart_grandmother_phi.at(doubleB)) already_saved=true; 
     }
    if (already_saved){ /*cout<<"skip,no saving"<<endl;*/ continue;}
    //else cout<<"saving"<<endl;
    
    temp_genpart_grandmother_pdgId.push_back( temp_gmom->pdgId());
    temp_genpart_grandmother_pt.push_back( temp_gmom->pt());
    temp_genpart_grandmother_eta.push_back( temp_gmom->eta());
    temp_genpart_grandmother_phi.push_back( temp_gmom->phi());
    temp_genpart_grandmother_Bindex.push_back(c);
    temp_genpart_grandmother_x.push_back(temp_gmom->vx());
    temp_genpart_grandmother_y.push_back(temp_gmom->vy());
    temp_genpart_grandmother_z.push_back(temp_gmom->vz());
   
     for (unsigned int igmu=0; igmu<temp_gmom->numberOfDaughters(); igmu++){
       const Candidate * temp_mom=temp_gmom->daughter(igmu);
       temp_genpart_mother_pt.push_back(temp_mom->pt());
       temp_genpart_mother_eta.push_back(temp_mom->eta());
       temp_genpart_mother_phi.push_back(temp_mom->phi());
       temp_genpart_mother_pdgId.push_back(temp_mom->pdgId());
       temp_genpart_mother_Bindex.push_back(c);
       temp_genpart_mother_Daughtindex.push_back(igmu);
 
       for (unsigned int imu=0; imu<temp_mom->numberOfDaughters(); imu++){
	   const Candidate * temp=temp_mom->daughter(imu);
           temp_genpart_pt.push_back(temp->pt());
           temp_genpart_eta.push_back(temp->eta());
           temp_genpart_phi.push_back(temp->phi());
           temp_genpart_pdgId.push_back(temp->pdgId());
           temp_genpart_charge.push_back(temp->charge());
           temp_genpart_Bindex.push_back(c);
           temp_genpart_Daughtindex.push_back(igmu);
//           cout<<" gdaughter "<<temp->pdgId()<<endl;   
       }
     }   
   }
 std::vector<std::vector<float>> tempv;
tempv.push_back(temp_genpart_pt);  tempv.push_back(temp_genpart_eta);
tempv.push_back(temp_genpart_phi); tempv.push_back(temp_genpart_pdgId);
 tempv.push_back(temp_genpart_charge);  tempv.push_back(temp_genpart_Bindex); 
tempv.push_back(temp_genpart_Daughtindex); tempv.push_back(temp_genpart_mother_pt);
  tempv.push_back(temp_genpart_mother_eta); tempv.push_back(temp_genpart_mother_phi);
 tempv.push_back(temp_genpart_mother_pdgId);
 tempv.push_back(temp_genpart_mother_Bindex); tempv.push_back(temp_genpart_mother_Daughtindex);
tempv.push_back(temp_genpart_grandmother_pt);  tempv.push_back(temp_genpart_grandmother_eta);
 tempv.push_back(temp_genpart_grandmother_phi); tempv.push_back(temp_genpart_grandmother_pdgId); 
 tempv.push_back(temp_genpart_grandmother_Bindex); tempv.push_back(temp_genpart_grandmother_x); tempv.push_back(temp_genpart_grandmother_y); tempv.push_back(temp_genpart_grandmother_z);
 return tempv;
	    }

std::vector<std::vector<float>> LeptonSkimming::genMuAnalyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){ 
   using namespace std;
  using namespace edm;
  using namespace reco;
  edm::Handle<edm::View<reco::GenParticle>> genPart;
  iEvent.getByToken(GenToken_,genPart);
  std::vector<float> genMu_pt,genMu_eta,genMu_phi,genMu_ch,genMu_motherId,genMu_gmotherId;
  std::vector<std::vector<float>> genMu;
  for(typename edm::View<reco::GenParticle>::const_iterator gen=genPart->begin(); gen !=genPart->end(); gen++){ 
    if (fabs(gen->pdgId())!=13) continue;
     const Candidate * mo= gen->mother();
     if (fabs(mo->pdgId())==13) continue; 
     const Candidate * mo2= mo->mother(); //const Candidate * mo3= mo2->mother();
    genMu_pt.push_back(gen->pt()); genMu_eta.push_back(gen->eta()); 
    genMu_phi.push_back(gen->phi()); genMu_ch.push_back(gen->charge()); 
    genMu_motherId.push_back(mo->pdgId()); genMu_gmotherId.push_back(mo2->pdgId());
  }
  genMu.push_back(genMu_pt); genMu.push_back(genMu_eta); genMu.push_back(genMu_phi);
   genMu.push_back(genMu_ch); genMu.push_back(genMu_motherId); genMu.push_back(genMu_gmotherId);
  return genMu;
  }



std::pair<std::vector<float>,std::vector<std::vector<std::vector<float>>>> LeptonSkimming::HLTAnalyze(const edm::Event& iEvent, const edm::EventSetup& iSetup,std::vector< string> HLTPath,std::vector< string> Seed ){
   using namespace std;  using namespace edm;  using namespace reco;
  using namespace trigger;
 
  edm::Handle<trigger::TriggerEvent> triggerObjectsSummary;
  iEvent.getByToken(trigobjectsToken_ ,triggerObjectsSummary);
  edm::Handle<edm::TriggerResults> trigResults;
  iEvent.getByToken(trgresultsToken_, trigResults);
  trigger::TriggerObjectCollection selectedObjects;
  std::vector<float> fires;
  std::vector<std::vector<std::vector<float>>> trg_event; 

for (unsigned int ipath=0; ipath<Seed.size(); ipath++){ 
  std::vector<std::vector<float>> tot_tr_obj_pt_eta_phi;
  if (triggerObjectsSummary.isValid()) {  
      size_t filterIndex = (*triggerObjectsSummary).filterIndex(InputTag(Seed[ipath],"","HLT"));
      trigger::TriggerObjectCollection allTriggerObjects = triggerObjectsSummary->getObjects();     
      if (filterIndex < (*triggerObjectsSummary).sizeFilters()) { 
      const trigger::Keys &keys = (*triggerObjectsSummary).filterKeys(filterIndex);
      for (size_t j = 0; j < keys.size(); j++) {
	trigger::TriggerObject foundObject = (allTriggerObjects)[keys[j]];
        std::vector<float> tr_obj_pt_eta_phi;
        if (fabs(foundObject.id())!=13) continue;
        tr_obj_pt_eta_phi.push_back(foundObject.pt());
        tr_obj_pt_eta_phi.push_back(foundObject.eta());
        tr_obj_pt_eta_phi.push_back(foundObject.phi());
        tr_obj_pt_eta_phi.push_back(foundObject.id()/fabs(foundObject.id()));
        tot_tr_obj_pt_eta_phi.push_back( tr_obj_pt_eta_phi);
      }
       
       }   
    }    
     trg_event.push_back(tot_tr_obj_pt_eta_phi);
  }
  //paths
  float fire0=0,fire1=0,fire2=0,fire3=0,fire4=0,fire5=0;
    if( !trigResults.failedToGet() ) {
    int N_Triggers = trigResults->size();
    const edm::TriggerNames & trigName = iEvent.triggerNames(*trigResults);    
    //cout << "new" << endl;
    for( int i_Trig = 0; i_Trig < N_Triggers; ++i_Trig ) {
      TString TrigPath =trigName.triggerName(i_Trig);    
      if (TrigPath.Contains(HLTPath[0]) && trigResults->accept(i_Trig)){
          fire0=1;}
      if(TrigPath.Contains(HLTPath[1])  && trigResults->accept(i_Trig) ){
          fire1=1;}
      if (TrigPath.Contains(HLTPath[2]) && trigResults->accept(i_Trig)){ 
          fire2=1;}
      if (TrigPath.Contains(HLTPath[3]) && trigResults->accept(i_Trig)){
	  fire3=1;}
      if (TrigPath.Contains(HLTPath[4]) && trigResults->accept(i_Trig)){
          fire4=1;}
      if (TrigPath.Contains(HLTPath[5]) && trigResults->accept(i_Trig)){ 
	  fire5=1;}
       } 
      }
								   	
    fires.push_back(fire0);  fires.push_back(fire1);   fires.push_back(fire2);
    fires.push_back(fire3); fires.push_back(fire4); fires.push_back(fire5);
    return std::make_pair(fires,trg_event);
}




std::vector<float> LeptonSkimming::SelectTrg_Object(std::vector<std::vector<float>> &tr1,std::vector<std::vector<float>> &tr2, std::vector<std::vector<float>> &tr3,std::vector<std::vector<float>> &tr4,std::vector<std::vector<float>> &tr5,std::vector<std::vector<float>> &tr6){
  //  std::vector<float> kolos; kolos.push_back(1);
  
  std::vector<std::vector<float>> max1;
  for (auto & vec: tr1) max1.push_back(vec);
  for (auto & vec: tr2) max1.push_back(vec);
  for (auto & vec: tr3) max1.push_back(vec);
  for (auto & vec: tr4) max1.push_back(vec);
  for (auto & vec: tr5) max1.push_back(vec);
  for (auto & vec: tr6) max1.push_back(vec);
 
  std::sort(max1.begin(), max1.end(),
          [](const std::vector<float>& a, const std::vector<float>& b) {
  return a[0] > b[0];
	    });
    return max1[0];
}

 
std::vector<float> LeptonSkimming::SimulateTrigger(std::vector<float> & genMu_pt,std::vector<float> & genMu_eta,std::vector<float> & genMu_phi,std::vector<float> & genMu_ch){
  std::vector<std::vector<float>> max1;
  for(unsigned int igen=0; igen<genMu_pt.size(); igen++){
    if (genMu_pt.at(igen)<9 || fabs(genMu_eta.at(igen))>1.5) continue;
    std::vector<float> temp; temp.push_back(genMu_pt.at(igen));
    temp.push_back(genMu_eta.at(igen));  temp.push_back(genMu_phi.at(igen));
    temp.push_back(genMu_ch.at(igen));
    max1.push_back(temp);
  }
  if (max1.size()>0)
     std::sort(max1.begin(), max1.end(),
          [](const std::vector<float>& a, const std::vector<float>& b) {
      return a[0] > b[0];
	    });
   else {
     std::vector<float> temp; temp.push_back(-99); temp.push_back(-99); temp.push_back(-99); temp.push_back(-99);
     max1.push_back(temp);
   }
   return max1[0];
}

 
float LeptonSkimming::Dphi(float phi1,float phi2){
    float result = phi1 - phi2;
    while (result > float(M_PI)) result -= float(2*M_PI);
    while (result <= -float(M_PI)) result += float(2*M_PI);
    return result;
}


float LeptonSkimming::DR(float eta1,float phi1,float eta2, float phi2){
  return TMath::Sqrt((eta1-eta2)*(eta1-eta2)+Dphi(phi1,phi2)*Dphi(phi1,phi2));
}


// ------------ method called on each new Event  ------------
bool
LeptonSkimming::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
 
  using namespace std;
  using namespace edm;
  using namespace reco;
  using namespace trigger;
  // using namespace PhysicsTools;


  //Get a few collections to apply basic electron ID
  //Get electrons
  edm::Handle<edm::View<reco::GsfElectron> > electrons;
  iEvent.getByToken(electronsToken_, electrons);

 
  edm::Handle<std::vector<reco::Muon>> muons;
   iEvent.getByToken(muonsToken_,muons);

  //Get conversions
  edm::Handle<reco::ConversionCollection> conversions;
  iEvent.getByToken(conversionsToken_, conversions);    
  // Get the beam spot
  edm::Handle<reco::BeamSpot> theBeamSpot;
  iEvent.getByToken(beamSpotToken_,theBeamSpot);  
  //Get vertices 
  edm::Handle<reco::VertexCollection> vertices;
  iEvent.getByToken(vtxToken_, vertices);
  //continue if there are no vertices
  if (vertices->empty()) return false;
  edm::Handle<vector<reco::Track>> tracks;
  iEvent.getByToken(Tracks_, tracks);
  edm::Handle<edm::TriggerResults> trigResults;
  iEvent.getByToken(trgresultsToken_, trigResults);
  edm::ESHandle<MagneticField> bFieldHandle;
  iSetup.get<IdealMagneticFieldRecord>().get(bFieldHandle);
  KalmanVertexFitter theKalmanFitter(false);
  TransientVertex LLvertex;
  

  vertex_x.clear();  vertex_y.clear();  vertex_z.clear();  
  beam_x=-9999,beam_y=-9999,beam_z=-99999;
 
  //run stuff
  run_number=iEvent.id().run();
  ls=iEvent.luminosityBlock();
 
  // trigger1=0; trigger2=0; trigger3=0; trigger4=0; trigger5=0; trigger6=0;
  event++; nmuons=0; nel=0; ntracks=0;
  muon_pt.clear(); muon_eta.clear(); muon_phi.clear(); muon_charge.clear();
  muon_dz.clear(); muon_dxy.clear(); muon_edz.clear(); muon_edxy.clear();
  muon_d0.clear(); muon_ed0.clear(); muon_vx.clear(); muon_vy.clear();
  muon_vz.clear(); muon_iso.clear(); muon_soft.clear(); muon_loose.clear();
  muon_medium.clear(); muon_tight.clear(); muon_trkpt.clear();
  muon_trketa.clear(); muon_trkphi.clear();
 
  el_pt.clear(); el_eta.clear(); el_phi.clear(); el_charge.clear();
  el_vx.clear(); el_vy.clear(); el_vz.clear(); el_dxy.clear(); el_mva_out.clear();
  el_dz.clear();el_edxy.clear(); el_edz.clear(); el_mva_out.clear(); 
  el_mva_iso.clear(); el_iso.clear();   el_mva_map_value.clear();
  el_veto.clear(); el_soft.clear(); el_medium.clear(); el_tight.clear();
  el_trkpt.clear(); el_trketa.clear(); el_trkphi.clear();
     
   TrgObj1_PtEtaPhiCharge.clear(); TrgObj2_PtEtaPhiCharge.clear();
  TrgObj3_PtEtaPhiCharge.clear(); TrgObj4_PtEtaPhiCharge.clear();
  TrgObj5_PtEtaPhiCharge.clear(); TrgObj6_PtEtaPhiCharge.clear();
  trigger1=0,trigger2=0,trigger3=0,trigger4=0,trigger5=0,trigger6=0;
  SelectedTrgObj_PtEtaPhiCharge.clear(); SelectedMu_index=-1;
  SelectedMu_DR=1000;

  genpart_pt.clear(),genpart_phi.clear(),genpart_eta.clear(),genpart_pdgId.clear(),genpart_charge.clear(),genpart_Bindex.clear(),genpart_Daughtindex.clear(),genpart_mother_pt.clear(),genpart_mother_eta.clear(),genpart_mother_phi.clear(),genpart_mother_pdgId.clear(),genpart_mother_Bindex.clear(),genpart_mother_Daughtindex.clear(),genpart_grandmother_pt.clear(),genpart_grandmother_eta.clear(),genpart_grandmother_phi.clear(),genpart_grandmother_pdgId.clear(),genpart_grandmother_Bindex.clear(); genpart_grandmother_x.clear(); genpart_grandmother_y.clear(); genpart_grandmother_z.clear();
 
  track_pt.clear(); track_eta.clear(); track_phi.clear(); track_dxy.clear();
  track_norm_chi2.clear(); track_charge.clear();  track_edz.clear(); 
  track_dz.clear(); track_MuCleaned.clear(); track_edxy.clear();
  track_vx.clear(); track_vz.clear(); track_vy.clear(); track_mva.clear();
  
  TTrack_PtEtaPhiM.clear(); TTrack_XYZ.clear();  TTrack_cos.clear();
  TTrack_chi_prob.clear(); TTrack_ObjIndex.clear(); TTrack_TrkIndex.clear(); 
  TTrack_kid.clear(); TTrack_mll.clear(); TTrack_Lxy.clear();
  TTrack_eLxy.clear(); TTrack_ObjId.clear();

  Epair_PtEtaPhiM.clear(); Epair_XYZ.clear();  Epair_cos.clear();
  Epair_chi_prob.clear(); Epair_ObjIndex.clear(); Epair_TrkIndex.clear();
  Epair_Lxy.clear(); Epair_eLxy.clear(); Epair_ObjId.clear();

  genMu_pt.clear(); genMu_eta.clear(); genMu_phi.clear(); genMu_ch.clear(); 
  genMu_motherId.clear(); genMu_gmotherId.clear();

  //internal stuff
   ZvertexTrg=-100000000;

for (VertexCollection::const_iterator vtx = vertices->begin();   vtx != vertices->end(); ++vtx) {
    bool isFake = vtx->isFake();
    if ( isFake) continue;
        vertex_x.push_back(vtx->x());  vertex_y.push_back(vtx->y());  vertex_z.push_back(vtx->z()); 
    }
  if (vertex_x.size()==0)
         return false;
  reco::TrackBase::Point  vertex_point;
  vertex_point.SetCoordinates(vertex_x[0],vertex_y[0],vertex_z[0]);
  beam_x= theBeamSpot->x0(); beam_y= theBeamSpot->y0(); beam_z= theBeamSpot->z0();   
  genparts.clear(); genmu.clear();
if(!IsData){
   genparts=genAnalyze(iEvent,iSetup);
   genpart_pt=genparts[0]; genpart_eta=genparts[1]; genpart_phi=genparts[2];
   genpart_pdgId=genparts[3]; genpart_charge=genparts[4]; 
   genpart_Bindex=genparts[5]; genpart_Daughtindex=genparts[6]; 
   genpart_mother_pt=genparts[7]; genpart_mother_eta=genparts[8]; 
   genpart_mother_phi=genparts[9]; genpart_mother_pdgId=genparts[10]; 
   genpart_mother_Bindex=genparts[11]; genpart_mother_Daughtindex=genparts[12];
   genpart_grandmother_pt=genparts[13]; genpart_grandmother_eta=genparts[14]; 
   genpart_grandmother_phi=genparts[15]; genpart_grandmother_pdgId=genparts[16];
   genpart_grandmother_Bindex=genparts[17]; genpart_grandmother_x=genparts[18];
   genpart_grandmother_y=genparts[19]; genpart_grandmother_z=genparts[20];
   genmu=genMuAnalyze(iEvent,iSetup);
   genMu_pt=genmu[0]; genMu_eta=genmu[1]; genMu_phi=genmu[2]; 
   genMu_ch=genmu[3];  genMu_motherId=genmu[4]; genMu_gmotherId=genmu[5]; 
   SelectedTrgObj_PtEtaPhiCharge=SimulateTrigger(genMu_pt,genMu_eta,genMu_phi,genMu_ch); 
 }

 if (IsData){    
    std::pair<std::vector<float>,std::vector<std::vector<std::vector<float>>>> trgresult=HLTAnalyze(iEvent,iSetup,HLTPath_,HLTFilter_);
    trigger1=trgresult.first[0]; trigger2=trgresult.first[1]; trigger3=trgresult.first[2];  
    trigger4=trgresult.first[3]; trigger5=trgresult.first[4]; trigger6=trgresult.first[5];   
    if(trigger1+trigger2+trigger3+trigger4+trigger5+trigger6==0) return false;
    TrgObj1_PtEtaPhiCharge=trgresult.second[0]; TrgObj2_PtEtaPhiCharge=trgresult.second[1];  
    TrgObj3_PtEtaPhiCharge=trgresult.second[2]; TrgObj4_PtEtaPhiCharge=trgresult.second[3]; 
    TrgObj5_PtEtaPhiCharge=trgresult.second[4]; TrgObj6_PtEtaPhiCharge=trgresult.second[5];      
    SelectedTrgObj_PtEtaPhiCharge=SelectTrg_Object(TrgObj1_PtEtaPhiCharge,TrgObj2_PtEtaPhiCharge,TrgObj3_PtEtaPhiCharge,TrgObj4_PtEtaPhiCharge,TrgObj5_PtEtaPhiCharge,TrgObj6_PtEtaPhiCharge);
 }
 
  SelectedMu_DR=1000; SelectedMu_index=-1;
  std::vector<std::shared_ptr<reco::Track>> MuTracks; 
  std::vector<unsigned int> object_container;
  std::vector<unsigned int> object_id;
  mutemp=0;
  for (std::vector<reco::Muon>::const_iterator mu=muons->begin(); mu!=muons->end(); mu++){
    if (fabs(mu->eta())>EtaTrack_Cut) continue;
    if (mu->pt()<PtMu_Cut) continue;
    bool tight=false,soft=false;
    if(vertices.isValid()){
      tight=isTightMuonCustom(*mu,(*vertices)[0]);
      soft=muon::isSoftMuon(*mu,(*vertices)[0]);
     }
    if (QualMu_Cut==1 && !soft) continue;
    if (QualMu_Cut==2 && !isMediumMuonCustom(*mu)) continue; 
    if (QualMu_Cut==3 && !tight) continue;
    nmuons++;muon_pt.push_back(mu->pt()); muon_phi.push_back(mu->phi());
    muon_eta.push_back(mu->eta()); muon_charge.push_back(mu->charge());
    const Track * mutrack= mu->bestTrack(); 
    muon_trkpt.push_back(mutrack->pt()); muon_trketa.push_back(mutrack->eta());
    muon_trkphi.push_back(mutrack->phi());
    muon_dz.push_back(mutrack->dz(vertex_point));
    muon_dxy.push_back(mutrack->dxy(vertex_point));
    muon_vx.push_back(mu->vx()); muon_vy.push_back(mu->vy());
    muon_vz.push_back(mu->vz()); muon_edz.push_back(mutrack->dzError());
    muon_edxy.push_back(mutrack->dxyError()); muon_d0.push_back(mutrack->d0());
    muon_ed0.push_back(mutrack->d0Error());
    muon_medium.push_back(isMediumMuonCustom(*mu));
    muon_loose.push_back(isLooseMuonCustom(*mu));
    
    muon_tight.push_back(tight); muon_soft.push_back(soft);
    auto muTrack=std::make_shared<reco::Track>(*mutrack);
    MuTracks.push_back(muTrack);
    object_container.push_back(mutemp);
    object_id.push_back(13);
    const MuonPFIsolation&  isol=mu->pfIsolationR04();
    double mu_iso=(isol.sumChargedHadronPt+max(0.,isol.sumNeutralHadronEt+isol.sumPhotonEt-0.5*isol.sumPUPt))/mu->pt();
    muon_iso.push_back(mu_iso);
    if ( DR(mu->eta(),mu->phi(), SelectedTrgObj_PtEtaPhiCharge[1], SelectedTrgObj_PtEtaPhiCharge[2])<MuTrgMatchCone &&  SelectedMu_DR>DR(mu->eta(),mu->phi(), SelectedTrgObj_PtEtaPhiCharge[1], SelectedTrgObj_PtEtaPhiCharge[2]) ){
      SelectedMu_DR=DR(mu->eta(),mu->phi(), SelectedTrgObj_PtEtaPhiCharge[1], SelectedTrgObj_PtEtaPhiCharge[2]);
      SelectedMu_index=mutemp;  ZvertexTrg=mu->vz();}
    mutemp++;
    //delete mutrack;
  }
  
  if (SelectedMu_DR==1000 && SkipIfNoMuMatch){
      muon_pt.clear(); muon_eta.clear(); muon_phi.clear(); muon_charge.clear();
      muon_dz.clear(); muon_dxy.clear(); muon_edz.clear(); muon_edxy.clear();
      muon_d0.clear(); muon_ed0.clear(); muon_vx.clear(); muon_vy.clear();
      muon_vz.clear(); muon_iso.clear(); muon_soft.clear(); muon_loose.clear();
      muon_medium.clear(); muon_tight.clear();
      el_pt.clear(); el_eta.clear(); el_phi.clear(); el_charge.clear();
      el_vx.clear(); el_vy.clear(); el_vz.clear(); el_dxy.clear(); el_mva_out.clear();
      el_dz.clear();el_edxy.clear(); el_edz.clear(); el_mva_out.clear(); 
      el_mva_iso.clear(); el_iso.clear();   el_mva_map_value.clear();
      el_veto.clear(); el_soft.clear(); el_medium.clear(); el_tight.clear();
      track_pt.clear(); track_eta.clear(); track_phi.clear(); track_dxy.clear();
      track_norm_chi2.clear(); track_charge.clear();  track_edz.clear(); 
      track_dz.clear(); track_MuCleaned.clear(); track_edxy.clear(); track_mva.clear();
      ntracks=0; nel=0; nmuons=0; SelectedMu_index=-1;
 if (SaveOutputRoot){ t1->Fill();}
      return false;
   }
std::vector<std::shared_ptr<reco::Track>> ElTracks;
for(size_t e=0; e<electrons->size(); e++){
      const auto el = electrons->ptrAt(e);  
      bool passConvVeto = !ConversionTools::hasMatchedConversion(*el, conversions, theBeamSpot->position());
      if (!passConvVeto) continue;
      if (fabs(el->eta())>EtaTrack_Cut) continue;
      if (el->pt()<PtEl_Cut) continue;
      el_pt.push_back(el->pt()); el_eta.push_back(el->eta());
      el_phi.push_back(el->phi()); el_charge.push_back(el->charge());
      const Track * eltrack= el->bestTrack();
      el_trkpt.push_back(eltrack->pt()); el_trketa.push_back(eltrack->eta());
      el_trkphi.push_back(eltrack->phi());
      el_dz.push_back(eltrack->dz(vertex_point));
      el_dxy.push_back(eltrack->dxy(vertex_point));
      el_edxy.push_back(eltrack->dxyError());
      el_edz.push_back(eltrack->dzError());
      el_vx.push_back(el->vx()); el_vy.push_back(el->vy());
      el_vz.push_back(el->vz()); el_mva_out.push_back(el->mva_e_pi());
      el_mva_iso.push_back(el->mva_Isolated());
      double iso= el->pfIsolationVariables().sumChargedHadronPt+max(0.0,el->pfIsolationVariables().sumNeutralHadronEt+el->pfIsolationVariables().sumPhotonEt-0.5*el->pfIsolationVariables().sumPUPt)/el->pt();
       el_iso.push_back(iso);// el_veto.push_back((*ele_veto_id)[el]);
       //el_soft.push_back((*ele_soft_id)[el]);
       //el_medium.push_back((*ele_medium_id)[el]);
       //el_tight.push_back((*ele_tight_id)[el]);
      // el_mva_map_value.push_back((*ele_mva_id_value)[el]);
      auto ElTrack=std::make_shared<reco::Track>(*eltrack);
      ElTracks.push_back(ElTrack); object_container.push_back(nel);
  //    cout<<"e "<<nel<<" pt "<<el->pt()<<" trkpt "<<eltrack->pt()<<endl;
      nel++; object_id.push_back(11);
      //  delete eltrack;
    }
 
std::vector<std::shared_ptr<reco::Track>> cleanedTracks; 
std::vector<unsigned int> track_container;
trk_index=0;
if (!UseOnlyBKeeMCForTriplets){
 for (typename vector<reco::Track>::const_iterator trk=tracks->begin(); trk!=tracks->end(); trk++){
   if (!trk->quality(Track::highPurity)) continue;
   if (trk->pt()<PtTrack_Cut) continue;
   if (fabs(trk->eta())>EtaTrack_Cut) continue;
   if(trk->charge()==0) continue;
   if(trk->normalizedChi2()>MaxChi2Track_Cut || trk->normalizedChi2()<MinChi2Track_Cut) continue;
   if (fabs(trk->dxy())/trk->dxyError()<TrackSdxy_Cut) continue;
   double minDR=1000;
   for (typename vector<reco::Muon>::const_iterator mu=muons->begin(); mu!=muons->end(); mu++){
      double tempDR=DR(mu->eta(),mu->phi(),trk->eta(),trk->phi());
      if (minDR<tempDR) continue;
      minDR=tempDR;
   }
   if (minDR<MuTrkMinDR_Cut) continue;
   if (SelectedMu_DR<1000 ){
     if (fabs(ZvertexTrg-trk->vz())>TrackMuDz_Cut ) continue;
     if ( DR(trk->eta(),trk->phi(),SelectedTrgObj_PtEtaPhiCharge[1],SelectedTrgObj_PtEtaPhiCharge[2])<TrgExclusionCone) continue;}

   //assignments
   track_pt.push_back(trk->pt()); track_eta.push_back(trk->eta());
   track_phi.push_back(trk->phi()); track_charge.push_back(trk->charge());
   track_norm_chi2.push_back(trk->normalizedChi2());
   track_dxy.push_back(trk->dxy(vertex_point)); track_dz.push_back(trk->dz(vertex_point));
   track_edxy.push_back(trk->dxyError()); track_edz.push_back(trk->dzError());  
   auto cleanTrack=std::make_shared<reco::Track>(*trk);
   cleanedTracks.push_back(cleanTrack);
   track_vx.push_back(trk->vx()); track_vy.push_back(trk->vy()); track_vz.push_back(trk->vz());
   track_container.push_back(trk_index);
   trk_index++; ntracks++;  
 }
 } 

 if (SaveOnlyTracks) {
     if (SaveOutputRoot) {t1->Fill();}
     return true;}
    //create mother ee combination
 
 // fit track pairs
    cleanedObjTracks.clear(); cleanedPairTracks.clear(); 
    
    TLorentzVector vel1,vel2;
    std::vector<std::shared_ptr<reco::Track>> cleanedObjects; 
    //objects    
    for(auto & vec: MuTracks) cleanedObjects.push_back(vec);
    for(auto & vec: ElTracks) cleanedObjects.push_back(vec);  
    if (cleanedObjects.size()==0)
             return false;
    for(unsigned int iobj=0; iobj<cleanedObjects.size(); iobj++){
      auto obj=cleanedObjects.at(iobj);
      auto tranobj=std::make_shared<reco::TransientTrack>(reco::TransientTrack(*obj,&(*bFieldHandle)));
      //int ctrk=0;
      for(unsigned int itrk2=0; itrk2<cleanedTracks.size(); itrk2++){
         auto trk2=cleanedTracks.at(itrk2);
         if (obj->charge()*trk2->charge()==1) continue;
         if (ObjPtLargerThanTrack && obj->pt()<trk2->pt()) continue;
         //supposed to be like that not take e 
         vel1.SetPtEtaPhiM(obj->pt(),obj->eta(),obj->phi(),0.0005);
         vel2.SetPtEtaPhiM(trk2->pt(),trk2->eta(),trk2->phi(),0.0005);
         if (DR(obj->eta(),obj->phi(),trk2->eta(),trk2->phi())<TrkObjExclusionCone) continue;
         if (object_id.at(iobj)==13 && DR(obj->eta(),obj->phi(),SelectedTrgObj_PtEtaPhiCharge[1],SelectedTrgObj_PtEtaPhiCharge[2])<MuTrgExclusionCone) continue;
         if (object_id.at(iobj)==11 && DR(obj->eta(),obj->phi(),SelectedTrgObj_PtEtaPhiCharge[1],SelectedTrgObj_PtEtaPhiCharge[2])<ElTrgExclusionCone) continue;
         if (SelectedMu_DR<1000 ){
	   if (object_id.at(iobj)==13 && fabs(ZvertexTrg- obj->vz())>MuTrgMuDz_Cut ) continue;
           if (object_id.at(iobj)==11 && fabs(ZvertexTrg-obj->vz())>ElTrgMuDz_Cut ) continue;
         }
         if ((vel1+vel2).M()>MaxMee_Cut || (vel1+vel2).M()<MinMee_Cut ) continue;   
	 auto trantrk2=std::make_shared<reco::TransientTrack>(reco::TransientTrack(*trk2,&(*bFieldHandle)));
	 tempTracks.clear(); 
         tempTracks.push_back(*tranobj); tempTracks.push_back(*trantrk2);
         LLvertex = theKalmanFitter.vertex(tempTracks);
         if (!LLvertex.isValid()) continue;
	 if (ChiSquaredProbability(LLvertex.totalChiSquared(),LLvertex.degreesOfFreedom())<Probee_Cut)  continue;
         if (ZvertexTrg>-1000000 && fabs(ZvertexTrg-LLvertex.position().z())>EpairZvtx_Cut ) continue;
	 GlobalError err =LLvertex.positionError();
	 GlobalPoint Dispbeamspot(-1*((theBeamSpot->x0()-LLvertex.position().x())+(LLvertex.position().z()-theBeamSpot->z0()) * theBeamSpot->dxdz()),-1*((theBeamSpot->y0()-LLvertex.position().y())+ (LLvertex.position().z()-theBeamSpot->z0()) * theBeamSpot->dydz()), 0);

        math::XYZVector pperp((vel1+vel2).Px(),(vel1+vel2).Py(),0);
        math::XYZVector vperp(Dispbeamspot.x(),Dispbeamspot.y(),0.);
        float tempCos=vperp.Dot(pperp)/(vperp.R()*pperp.R());
        if (tempCos<Cosee_Cut) continue;
	cleanedObjTracks.push_back(obj);
        cleanedPairTracks.push_back(trk2);
        Epair_ObjIndex.push_back(object_container.at(iobj));
        Epair_TrkIndex.push_back(track_container.at(itrk2));
        Epair_ObjId.push_back(object_id.at(iobj));   
	tempPtEtaPhiM.clear(); tempXYZ.clear();
        tempPtEtaPhiM.push_back((vel1+vel2).Pt()); tempPtEtaPhiM.push_back((vel1+vel2).Eta());
        tempPtEtaPhiM.push_back((vel1+vel2).Phi()); tempPtEtaPhiM.push_back((vel1+vel2).M());
	tempXYZ.push_back(LLvertex.position().x());  tempXYZ.push_back(LLvertex.position().y()); 
	tempXYZ.push_back(LLvertex.position().z());
	Epair_PtEtaPhiM.push_back(tempPtEtaPhiM); Epair_XYZ.push_back(tempXYZ); 
        Epair_cos.push_back(tempCos);
	Epair_chi_prob.push_back(ChiSquaredProbability(LLvertex.totalChiSquared(),LLvertex.degreesOfFreedom())); 	
	Epair_Lxy.push_back(Dispbeamspot.perp());
	Epair_eLxy.push_back(err.rerr(Dispbeamspot));
       
	}
     
    }
     
  if (SaveOnlyEPairTracks) {
     if (SaveOutputRoot) {t1->Fill();}
     return true;}
    // B recontrsuvtion
    TLorentzVector vK; 
    for(unsigned int iobj=0; iobj<cleanedObjTracks.size(); iobj++){
      auto objtrk=cleanedObjTracks.at(iobj);
      auto pairtrk=cleanedPairTracks.at(iobj);
      auto tranobj=std::make_shared<reco::TransientTrack>(reco::TransientTrack(*objtrk,&(*bFieldHandle)));
      auto tranpair=std::make_shared<reco::TransientTrack>(reco::TransientTrack(*pairtrk,&(*bFieldHandle)));
      for(unsigned int itrk=0; itrk<cleanedTracks.size(); itrk++){
	 auto trk=cleanedTracks.at(itrk);
         if(DR(objtrk->eta(),objtrk->phi(),trk->eta(),trk->phi())<TrkObjExclusionCone) continue;
	 if (trk->pt()<PtKTrack_Cut) continue;
         if (fabs(trk->dxy(vertex_point))/trk->dxyError()<Ksdxy_Cut) continue;
         if (trk->charge()==pairtrk->charge() && DR(pairtrk->eta(),pairtrk->phi(),trk->eta(),trk->phi())<TrkTrkMinDR_Cut) continue;
         
         vel1.SetPtEtaPhiM(objtrk->pt(),objtrk->eta(),objtrk->phi(),0.0005);
         vel2.SetPtEtaPhiM(pairtrk->pt(),pairtrk->eta(),pairtrk->phi(),0.0005);
         vK.SetPtEtaPhiM(trk->pt(),trk->eta(),trk->phi(),0.493);
         if ((vel1+vel2+vK).M()> MaxMB_Cut || (vel1+vel2+vK).M()< MinMB_Cut) continue;
         if ((vel1+vel2+vK).Pt()<PtB_Cut) continue;
         auto trantrk=std::make_shared<reco::TransientTrack>(reco::TransientTrack(*trk,&(*bFieldHandle)));
	 tempTracks.clear();
         tempTracks.push_back(*tranobj); tempTracks.push_back(*tranpair);
         tempTracks.push_back(*trantrk);
	 
         LLvertex = theKalmanFitter.vertex(tempTracks);
	 if (!LLvertex.isValid()) continue;
	  if (ChiSquaredProbability(LLvertex.totalChiSquared(),LLvertex.degreesOfFreedom())<ProbeeK_Cut) continue;
	 GlobalError err =LLvertex.positionError();
         GlobalPoint Dispbeamspot(-1*((theBeamSpot->x0()-LLvertex.position().x())+(LLvertex.position().z()-theBeamSpot->z0()) * theBeamSpot->dxdz()),-1*((theBeamSpot->y0()-LLvertex.position().y())+ (LLvertex.position().z()-theBeamSpot->z0()) * theBeamSpot->dydz()), 0);
      
         math::XYZVector pperp((vel1+vel2+vK).Px(),(vel1+vel2+vK).Py(),0);
         math::XYZVector vperp(Dispbeamspot.x(),Dispbeamspot.y(),0.);
         float tempCos=vperp.Dot(pperp)/(vperp.R()*pperp.R());
         if (tempCos<CoseeK_Cut) continue;
         if (SLxy_Cut>Dispbeamspot.perp()/TMath::Sqrt(err.rerr(Dispbeamspot))) continue;
 
         tempPtEtaPhiM.clear(); tempXYZ.clear();
         tempPtEtaPhiM.push_back((vel1+vel2+vK).Pt());
         tempPtEtaPhiM.push_back((vel1+vel2+vK).Eta()); 
         tempPtEtaPhiM.push_back((vel1+vel2+vK).Phi());
         tempPtEtaPhiM.push_back((vel1+vel2+vK).M());       
	 TTrack_PtEtaPhiM.push_back(tempPtEtaPhiM);
	 tempXYZ.push_back(LLvertex.position().x());
	 tempXYZ.push_back(LLvertex.position().y());
	 tempXYZ.push_back(LLvertex.position().z());
         TTrack_ObjId.push_back(Epair_ObjId.at(iobj));
         TTrack_XYZ.push_back(tempXYZ); TTrack_mll.push_back((vel1+vel2).M());
         TTrack_ObjIndex.push_back(Epair_ObjIndex.at(iobj)); 
         TTrack_TrkIndex.push_back(Epair_TrkIndex.at(iobj)); 
         TTrack_kid.push_back(track_container.at(itrk));
         TTrack_chi_prob.push_back(ChiSquaredProbability(LLvertex.totalChiSquared(),LLvertex.degreesOfFreedom()));
         TTrack_cos.push_back(tempCos);
         TTrack_Lxy.push_back(Dispbeamspot.perp());
	 TTrack_eLxy.push_back(err.rerr(Dispbeamspot));
         if (EarlyStop) break;
     }
    }
   if (TTrack_chi_prob.size()==0){
     muon_pt.clear(); muon_eta.clear(); muon_phi.clear(); muon_charge.clear();
     muon_dz.clear(); muon_dxy.clear(); muon_edz.clear(); muon_edxy.clear();
     muon_d0.clear(); muon_ed0.clear(); muon_vx.clear(); muon_vy.clear();
     muon_vz.clear(); muon_iso.clear(); muon_soft.clear(); muon_loose.clear();
     muon_medium.clear(); muon_tight.clear();
 
     el_pt.clear(); el_eta.clear(); el_phi.clear(); el_charge.clear();
     el_vx.clear(); el_vy.clear(); el_vz.clear(); el_dxy.clear(); el_mva_out.clear();
     el_dz.clear();el_edxy.clear(); el_edz.clear(); el_mva_out.clear(); 
     el_mva_iso.clear(); el_iso.clear();   el_mva_map_value.clear();
     el_veto.clear(); el_soft.clear(); el_medium.clear(); el_tight.clear();
     track_pt.clear(); track_eta.clear(); track_phi.clear(); track_dxy.clear();
     track_norm_chi2.clear(); track_charge.clear();  track_edz.clear(); 
     track_dz.clear(); track_MuCleaned.clear(); track_edxy.clear();
     ntracks=0; nel=0; nmuons=0; SelectedMu_index=-1; track_mva.clear();
     return false;
  }

   else{ 
     if (SaveOutputRoot) { t1->Fill();}
      return true;}
}

// ------------ method called once each stream before processing any runs, lumis or events  ------------
void
LeptonSkimming::beginStream(edm::StreamID)
{
}

// ------------ method called once each stream after processing all runs, lumis and events  ------------
void
LeptonSkimming::endStream() {
}

// ------------ method called when starting to processes a run  ------------
/*
void
LeptonSkimming::beginRun(edm::Run const&, edm::EventSetup const&)
{ 
}
*/
 
// ------------ method called when ending the processing of a run  ------------
/*
void
LeptonSkimming::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when starting to processes a luminosity block  ------------
/*
void
LeptonSkimming::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
LeptonSkimming::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
LeptonSkimming::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(LeptonSkimming);
