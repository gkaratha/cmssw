// -*- C++ -*-
//
// Package:    SkimmingForB/LeptonSkimming
// Class:      LeptonSkimming
// 
/**\class LeptonSkimming LeptonSkimming.cc SkimmingForB/LeptonSkimming/plugins/LeptonSkimming.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Georgios Karathanasis georgios.karathanasis@cern.ch
//         Created:  Thu, 29 Nov 2018 15:23:09 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/PatCandidates/interface/VIDCutFlowResult.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/EgammaCandidates/interface/ConversionFwd.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "HLTrigger/HLTcore/interface/defaultModuleLabel.h"
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/RecoCandidate/interface/RecoEcalCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoEcalCandidateIsolation.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidateIsolation.h"
#include "DataFormats/Common/interface/AssociationMap.h"
#include "HLTrigger/HLTcore/interface/HLTFilter.h"
//#include "HLTrigger/Egamma/plugins/HLTGenericFilter.h"
#include "DataFormats/HLTReco/interface/TriggerFilterObjectWithRefs.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/ParametrizedEngine/src/OAEParametrizedMagneticField.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/IPTools/interface/IPTools.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"
#include "L1Trigger/L1TNtuples/interface/MuonID.h"
#include <vector>
#include "TTree.h"
#include <string>
#include <iostream>
#include "TMath.h"
#include "DataFormats/Common/interface/Ref.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "CommonTools/Statistics/interface/ChiSquaredProbability.h"
#include "DataFormats/Candidate/interface/VertexCompositeCandidate.h"
#include "TLorentzVector.h"
#include "RecoVertex/KinematicFitPrimitives/interface/ParticleMass.h"
#include "RecoVertex/KinematicFitPrimitives/interface/MultiTrackKinematicConstraint.h"
#include "RecoVertex/KinematicFit/interface/CombinedKinematicConstraint.h"
#include <RecoVertex/KinematicFitPrimitives/interface/KinematicParticleFactoryFromTransientTrack.h>
#include "RecoVertex/KinematicFit/interface/KinematicConstrainedVertexFitter.h"
#include "RecoVertex/KinematicFit/interface/TwoTrackMassKinematicConstraint.h"
#include "RecoVertex/KinematicFit/interface/KinematicParticleVertexFitter.h"
#include "RecoVertex/KinematicFit/interface/KinematicParticleFitter.h"
#include "RecoVertex/KinematicFit/interface/MassKinematicConstraint.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "RecoVertex/KinematicFit/interface/MultiTrackPointingKinematicConstraint.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"




//
// class declaration
//

class LeptonSkimming : public edm::stream::EDFilter<> {
   public:
      explicit LeptonSkimming(const edm::ParameterSet&);
      ~LeptonSkimming();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

       
   private:
      virtual void beginStream(edm::StreamID) override;
      virtual bool filter(edm::Event&, const edm::EventSetup&) override;
      virtual void endStream() override;
  
      float Dphi(float phi1,float phi2);
      float DR(float eta1,float phi1,float eta2, float phi2);
      std::pair<std::vector<float>,std::vector<std::vector<std::vector<float>>>> HLTAnalyze(const edm::Event& iEvent, const edm::EventSetup& iSetup,std::vector< string> HLTPath,std::vector< string> Seed );
      std::vector<std::vector<float>> genAnalyze(const edm::Event& iEvent, const edm::EventSetup& iSetup);
      std::vector<float> SelectTrg_Object(std::vector<std::vector<float>> &tr1,std::vector<std::vector<float>> &tr2, std::vector<std::vector<float>> &tr3,std::vector<std::vector<float>> &tr4,std::vector<std::vector<float>> &tr5,std::vector<std::vector<float>> &tr6);
      std::vector<float> SimulateTrigger(std::vector<float> & genMu_pt,std::vector<float> & genMu_eta,std::vector<float> & genMu_phi,std::vector<float> & genMu_ch);
  std::vector<std::vector<float>> genMuAnalyze(const edm::Event& iEvent, const edm::EventSetup& iSetup);

      edm::EDGetToken electronsToken_; edm::EDGetToken muonsToken_;
      edm::EDGetToken Tracks_; edm::EDGetTokenT<reco::VertexCollection> vtxToken_;
      edm::EDGetTokenT<reco::BeamSpot> beamSpotToken_;
      edm::EDGetTokenT<reco::ConversionCollection> conversionsToken_;
      edm::EDGetTokenT<edm::TriggerResults> trgresultsToken_; 
      edm::EDGetTokenT<trigger::TriggerEvent> trigobjectsToken_;
      std::vector<string> HLTFilter_,HLTPath_;
      edm::EDGetToken GenToken_;
  
      TTree * t1; edm::Service<TFileService> fs;
      std::vector<float> muon_pt,muon_eta,muon_phi,muon_qual,muon_charge,
      muon_dxy, muon_dz,muon_edxy,muon_edz,muon_d0,muon_ed0,muon_vx,muon_vz,
      muon_vy,muon_iso,muon_trkpt,muon_trketa,muon_trkphi;
      std::vector<bool> muon_medium,muon_loose,muon_tight,muon_soft;  
      std::vector<float> el_pt,el_eta,el_phi,el_charge,el_vx,el_vy,el_vz,el_dxy,
      el_dz,el_edxy,el_edz,el_mva_out,el_mva_iso,el_iso,el_mva_map_value,
      el_trkpt,el_trketa,el_trkphi;
      std::vector<bool>el_veto,el_soft,el_medium,el_tight; 

      std::vector<float> vertex_x,vertex_y,vertex_z; float beam_x,beam_y,beam_z; 
      std::vector<float> track_pt,track_eta,track_phi,track_norm_chi2,track_charge,track_dxy,track_dz,track_edxy,track_edz,track_mva;
      std::vector<bool> track_MuCleaned;
      std::vector<std::vector<float>> TTrack_PtEtaPhiM,TTrack_XYZ;
      std::vector<float> TTrack_chi_prob,TTrack_ObjIndex,TTrack_TrkIndex,TTrack_kid,TTrack_mll,TTrack_cos,TTrack_Lxy,TTrack_eLxy;
      std::vector<std::vector<float>> Epair_PtEtaPhiM,Epair_XYZ; 
      std::vector<float> Epair_cos,Epair_chi_prob,Epair_ObjIndex,Epair_TrkIndex,Epair_Lxy,Epair_eLxy;
 
     std::vector<float>  genpart_pt,genpart_phi,genpart_eta,genpart_pdgId,genpart_charge,genpart_Bindex,genpart_Daughtindex,genpart_mother_pt,genpart_mother_eta,genpart_mother_phi,genpart_mother_pdgId,genpart_mother_Bindex,genpart_mother_Daughtindex,genpart_grandmother_pt,genpart_grandmother_eta,genpart_grandmother_phi,genpart_grandmother_pdgId,genpart_grandmother_Bindex,genpart_grandmother_x,genpart_grandmother_y,genpart_grandmother_z;
     std::vector<float> genMu_pt,genMu_eta,genMu_phi,genMu_ch,genMu_motherId,genMu_gmotherId;
     std::vector<float> track_vx,track_vy,track_vz;
     std::vector<std::vector<float>> TrgObj1_PtEtaPhiCharge,TrgObj2_PtEtaPhiCharge,TrgObj3_PtEtaPhiCharge,TrgObj4_PtEtaPhiCharge,TrgObj5_PtEtaPhiCharge,TrgObj6_PtEtaPhiCharge;
     std::vector<float> SelectedTrgObj_PtEtaPhiCharge; int SelectedMu_index;
     std::vector<unsigned int> Epair_ObjId,TTrack_ObjId;
     float SelectedMu_DR;
     int trigger1=0,trigger2=0,trigger3=0,trigger4=0,trigger5=0,trigger6=0;
     unsigned int nel=0,nmuons=0,ntracks=0,event=0,run_number=0,ls=0;
     //options
  bool IsData=true,SaveHLT=true;
  double PtTrack_Cut=0; double EtaTrack_Cut=10; double MinChi2Track_Cut=-1000; 
  double MaxChi2Track_Cut=1000; double MuTrkMinDR_Cut=0; double MaxMee_Cut=1000;
  double PtKTrack_Cut=0; 
  double MaxMB_Cut=1000; double MinMB_Cut=0; double TrkTrkMinDR_Cut=10000;
  bool SaveOnlyTracks=false; bool SaveOnlyEPairTracks=false;
  double TrackSdxy_Cut=0; bool UseOnlyBKeeMCForTriplets=false;
  double MinMee_Cut=0; double  Probee_Cut=0; double Cosee_Cut=-1;
  bool EarlyStop=false; double MuTrgMatchCone=1000; bool SkipIfNoMuMatch=false;
  double EpairZvtx_Cut=10000000; double Ksdxy_Cut=1000; double ProbeeK_Cut=0;
  double CoseeK_Cut=0; double TrackMuDz_Cut=100000000; double MaxMVA_Cut=-1000;
  double MinMVA_Cut=-1000; double TrgExclusionCone=-1; double SLxy_Cut=0;
  double PtB_Cut=0; double PtMu_Cut=0; double QualMu_Cut=0; double PtEl_Cut=0;
  double MuTrgExclusionCone=0; double ElTrgExclusionCone=0; 
  double TrkObjExclusionCone=0; double MuTrgMuDz_Cut=1000; 
  double ElTrgMuDz_Cut=1000; bool SaveOutputRoot=true;  bool ObjPtLargerThanTrack=false;
  
  //  internal stuff
  float ZvertexTrg=-100000000; unsigned int trk_index=0;
  std::vector<std::vector<float>> genparts; std::vector<std::vector<float>> genmu;
  std::vector<reco::TransientTrack> tempTracks;  std::vector<float> tempPtEtaPhiM,tempXYZ;
  std::vector<std::shared_ptr<reco::Track>> cleanedObjTracks;
  std::vector<std::shared_ptr<reco::Track>> cleanedPairTracks;
  std::vector<unsigned int> trackObj_container,trackPair_container;
  int mutemp=0; 
 
      // ----------member data ---------------------------
};
