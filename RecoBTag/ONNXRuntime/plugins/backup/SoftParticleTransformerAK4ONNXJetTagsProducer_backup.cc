//inference code for soft jets
// author: G Karathanasis (CERN)
// based on the examples by BTV

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/makeRefToBaseProdFrom.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"

#include "DataFormats/BTauReco/interface/JetTag.h"

#include "DataFormats/BTauReco/interface/SoftParticleTransformerAK4TagInfo.h"
#include "DataFormats/BTauReco/interface/SoftParticleTransformerAK4Features.h"

#include "PhysicsTools/ONNXRuntime/interface/ONNXRuntime.h"

using namespace cms::Ort;

class SoftParticleTransformerAK4ONNXJetTagsProducer : public edm::stream::EDProducer<edm::GlobalCache<ONNXRuntime>> {
public:
  explicit SoftParticleTransformerAK4ONNXJetTagsProducer(const edm::ParameterSet&, const ONNXRuntime*);
  ~SoftParticleTransformerAK4ONNXJetTagsProducer() override = default;

  static void fillDescriptions(edm::ConfigurationDescriptions&);

  static std::unique_ptr<ONNXRuntime> initializeGlobalCache(const edm::ParameterSet&);
  static void globalEndJob(const ONNXRuntime*);

private:
  typedef std::vector<reco::SoftParticleTransformerAK4TagInfo> TagInfoCollection;
  typedef reco::JetTagCollection JetTagCollection;

  void produce(edm::Event&, const edm::EventSetup&) override;

  void make_inputs(btagbtvdeep::SoftParticleTransformerAK4Features features);
  void get_input_sizes(const reco::FeaturesTagInfo<btagbtvdeep::SoftParticleTransformerAK4Features> taginfo);

  const edm::EDGetTokenT<TagInfoCollection> src_;
  std::vector<std::string> flav_names_;
  std::vector<std::string> input_names_;
  bool use_dynamic_axes_ = false;
  std::vector<std::string> output_names_;

  enum InputIndexes {
  //  kGlobalFeatures = 0,
    kChargedCandidates = 0,
  //  kNeutralCandidates = 2,
    kVertices = 1,
    kLostTracks = 2,
    kEdges=3
  };
  unsigned n_global_;
  constexpr static unsigned n_features_global_ = 0;
  unsigned n_cpf_;
  constexpr static unsigned n_features_cpf_ =  22; //18;
  constexpr static unsigned n_pairwise_features_cpf_ = 0; // 4;
  unsigned n_lt_;
  constexpr static unsigned n_features_lt_ = 18; //14;
  constexpr static unsigned n_pairwise_features_lt_ = 0;//4;
  unsigned n_npf_;
  constexpr static unsigned n_features_npf_ = 0;
  constexpr static unsigned n_pairwise_features_npf_ = 0;
  unsigned n_sv_;
  constexpr static unsigned n_features_sv_ = 17; //12;
  constexpr static unsigned n_pairwise_features_sv_ = 0; //4;
  unsigned n_edge_;
  constexpr static unsigned n_features_edge_ = 10;
  std::vector<unsigned> input_sizes_;
  std::vector<std::vector<int64_t>> input_shapes_;  // shapes of each input group (-1 for dynamic axis)

  // hold the input data
  FloatArrays data_;
};

SoftParticleTransformerAK4ONNXJetTagsProducer::SoftParticleTransformerAK4ONNXJetTagsProducer(
    const edm::ParameterSet& iConfig, const ONNXRuntime* cache)
    : src_(consumes<TagInfoCollection>(iConfig.getParameter<edm::InputTag>("src"))),
      flav_names_(iConfig.getParameter<std::vector<std::string>>("flav_names")),
      input_names_(iConfig.getParameter<std::vector<std::string>>("input_names")),
      use_dynamic_axes_(iConfig.getParameter<edm::FileInPath>("model_path").fullPath().find("v2.onnx") !=
                        std::string::npos),
      output_names_(iConfig.getParameter<std::vector<std::string>>("output_names")) {
  // get output names from flav_names
  for (const auto& flav_name : flav_names_) {
    produces<JetTagCollection>(flav_name);
  }
}

void SoftParticleTransformerAK4ONNXJetTagsProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  // pfUnifiedParticleTransformerAK4JetTags
  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag>("src", edm::InputTag("pfSoftParticleTransformerAK4TagInfos"));
  desc.add<std::vector<std::string>>(
//      "input_names", {"input_1", "input_2", "input_3", "input_4", "input_5", "input_6", "input_7", "input_8", "input_9", "input_10"});
     // "input_names", {"global_features","cpf_candidates","npf_candidates","vtx_features","lt_candidates","pair_candidates"});
       "input_names", {"cpf_candidates", "vtx_features","lt_candidates","pair_candidates"});
  desc.add<edm::FileInPath>("model_path",
                            edm::FileInPath("RecoBTag/data/SoftJetTagger/UParTAK4_v2.onnx"));
  desc.add<std::vector<std::string>>("output_names", {"output"});
  desc.add<std::vector<std::string>>(
      "flav_names",
      std::vector<std::string>{"probb",  "probc", "probuds",  "probg"});

  descriptions.add("pfSoftParticleTransformerAK4JetTags", desc);
}

std::unique_ptr<ONNXRuntime> SoftParticleTransformerAK4ONNXJetTagsProducer::initializeGlobalCache(
    const edm::ParameterSet& iConfig) {
  return std::make_unique<ONNXRuntime>(iConfig.getParameter<edm::FileInPath>("model_path").fullPath());
}

void SoftParticleTransformerAK4ONNXJetTagsProducer::globalEndJob(const ONNXRuntime* cache) {}

void SoftParticleTransformerAK4ONNXJetTagsProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  edm::Handle<TagInfoCollection> tag_infos;
  iEvent.getByToken(src_, tag_infos);
///  std::cout<<" flag1 "<<std::endl;
  // initialize output collection
  std::vector<std::unique_ptr<JetTagCollection>> output_tags;
  if (!tag_infos->empty()) {
   // std::cout<<"skata1"<<std::endl;
    auto jet_ref = tag_infos->begin()->jet();
    auto ref2prod = edm::makeRefToBaseProdFrom(jet_ref, iEvent);
    for (std::size_t i = 0; i < flav_names_.size(); i++) {
      output_tags.emplace_back(std::make_unique<JetTagCollection>(ref2prod));
    }
  } else {
   // std::cout<<"skata2"<<std::endl;
    for (std::size_t i = 0; i < flav_names_.size(); i++) {
      output_tags.emplace_back(std::make_unique<JetTagCollection>());
    }
  }
//  std::cout<<" flag2 "<<std::endl;

  for (unsigned jet_n = 0; jet_n < tag_infos->size(); ++jet_n) {
    const auto& taginfo = (*tag_infos)[jet_n];
    std::vector<float> outputs(flav_names_.size(), -1.0);

    const auto& jet_ref = tag_infos->at(jet_n).jet();
    bool Print = fabs(jet_ref->eta())<2;
   // if (Print){
   //    std::cout<<" flag jets "<<jet_n<<std::endl;
   //    std::cout<<" jetPt "<<jet_ref->pt()<<" Eta "<<jet_ref->eta()<<" Phi "<<jet_ref->phi()<<std::endl;
   // }

    if (taginfo.features().is_filled) {
   //   std::cout<<"   --taginfo "<<std::endl;
      get_input_sizes(taginfo);
    //  std::cout<<"   --input "<<std::endl;
      // run prediction with dynamic batch size per event
      input_shapes_ = { //{(int64_t)1, (int64_t)n_global_, (int64_t)n_features_global_},
                       {(int64_t)1, (int64_t)n_cpf_, (int64_t)n_features_cpf_},
                      // {(int64_t)1, (int64_t)n_npf_, (int64_t)n_features_npf_},
                       {(int64_t)1, (int64_t)n_sv_, (int64_t)n_features_sv_},
                       {(int64_t)1, (int64_t)n_lt_, (int64_t)n_features_lt_},
                       {(int64_t)1, (int64_t)n_edge_, (int64_t)n_features_edge_}};
      //std::cout<<" - before mva "<<std::endl;
      std::cout<<" input_shapes_ "<< input_shapes_[1][1]<<std::endl;
      outputs = globalCache()->run(input_names_, data_, input_shapes_, output_names_, 1)[0];
//      std::cout<<"outputs.size() "<<outputs.size()<<std::endl;
      assert(outputs.size() == flav_names_.size());
    }
    
    for (std::size_t flav_n = 0; flav_n < flav_names_.size(); flav_n++) {
      std::cout<<" flav_n "<<flav_n<<" tag "<<flav_names_[flav_n]<<" output "<<outputs[flav_n]<<std::endl;
      (*(output_tags[flav_n]))[jet_ref] = outputs[flav_n];
    }
  }

  // put into the event
  for (std::size_t flav_n = 0; flav_n < flav_names_.size(); ++flav_n) {
    iEvent.put(std::move(output_tags[flav_n]), flav_names_[flav_n]);
  }
}

void SoftParticleTransformerAK4ONNXJetTagsProducer::get_input_sizes(
    const reco::FeaturesTagInfo<btagbtvdeep::SoftParticleTransformerAK4Features> taginfo) {
  const auto& features = taginfo.features();

  if (use_dynamic_axes_) {
    // Use actual sizes for dynamic axes version
//    n_global_ = std::clamp((unsigned int)features.global_features.size(), (unsigned int)1, (unsigned int)0);
    n_cpf_ = std::clamp((unsigned int)features.c_pf_features.size(), (unsigned int)1, (unsigned int)50);
  //  n_npf_ = std::clamp((unsigned int)features.n_pf_features.size(), (unsigned int)1, (unsigned int)0);
    n_sv_ = std::clamp((unsigned int)features.sv_features.size(), (unsigned int)1, (unsigned int)5);
    n_lt_ = std::clamp((unsigned int)features.lt_features.size(), (unsigned int)1, (unsigned int)50);
    n_edge_ = std::clamp((unsigned int)features.pair_features.size(), (unsigned int)1, (unsigned int)300);    

  } else {
    // Use fixed sizes for original version
   // n_global_ = (unsigned int)0;
    n_cpf_ = (unsigned int)50;
   // n_npf_ = (unsigned int)0;
    n_sv_ = (unsigned int)5;
    n_lt_ = (unsigned int)50;
    n_edge_ = (unsigned int)300;
  }

  input_sizes_ = {
    //  n_global_ * n_features_global_,
      n_cpf_ * n_features_cpf_,
     // n_npf_ * n_features_npf_,
      n_sv_  * n_features_sv_,
      n_lt_  * n_features_lt_,
      n_edge_ * n_features_edge_,
//      n_cpf_ * n_pairwise_features_cpf_,
 //     n_lt_  * n_pairwise_features_lt_,
  //    n_npf_ * n_pairwise_features_npf_,
   //   n_sv_  * n_pairwise_features_sv_,
  };
  // init data storage
  data_.clear();
  for (const auto& len : input_sizes_) {
      data_.emplace_back(1 * len, 0);
  }

  make_inputs(features);
}

void SoftParticleTransformerAK4ONNXJetTagsProducer::make_inputs(
  btagbtvdeep::SoftParticleTransformerAK4Features features) {
//  std::cout<<" skata 1"<<std::endl;
  float* ptr = nullptr;
  const float* start = nullptr;
  unsigned offset = 0;

  // global candidates
/*  auto max_global_n = std::min(features.global_features.size(), (std::size_t)n_global_);
  for (std::size_t global_n = 0; global_n < max_global_n; global_n++) {
    const auto& global_features = features.global_features.at(global_n);
    ptr = &data_[kGlobalFeatures][offset + global_n * n_features_global_];
    start = ptr;
    assert(start + n_features_global_ - 1 == ptr);
  }
  //  std::cout<<" skata 2"<<std::endl;
*/

  // c_pf candidates
  auto max_c_pf_n = std::min(features.c_pf_features.size(), (std::size_t)n_cpf_);
  std::cout<<" max_c_pf_n "<<max_c_pf_n<<std::endl;
  max_c_pf_n=0;
  for (std::size_t c_pf_n = 0; c_pf_n < max_c_pf_n; c_pf_n++) {
    const auto& c_pf_features = features.c_pf_features.at(c_pf_n);
    ptr = &data_[kChargedCandidates][offset + c_pf_n * n_features_cpf_];
    start = ptr;
    *ptr = c_pf_features.etarel;
    *(++ptr) = c_pf_features.phirel;
    *(++ptr) = c_pf_features.quality;
    *(++ptr) = c_pf_features.vtx_ass;
    *(++ptr) = c_pf_features.chi2;
    *(++ptr) = c_pf_features.puppiw;
    *(++ptr) = c_pf_features.drminsv;
    *(++ptr) = c_pf_features.ptrel;
    *(++ptr) = c_pf_features.btagPf_trackEtaRel;
    *(++ptr) = c_pf_features.btagPf_trackPtRel;
    *(++ptr) = c_pf_features.btagPf_trackPPar;
    *(++ptr) = c_pf_features.btagPf_trackDeltaR;
    *(++ptr) = c_pf_features.btagPf_trackPParRatio;
    *(++ptr) = c_pf_features.btagPf_trackSip2dVal;
    *(++ptr) = c_pf_features.btagPf_trackSip2dSig;
    *(++ptr) = c_pf_features.btagPf_trackSip3dVal;
    *(++ptr) = c_pf_features.btagPf_trackSip3dSig;
    *(++ptr) = c_pf_features.btagPf_trackJetDistVal;
    *(++ptr) = c_pf_features.pt;
    *(++ptr) = c_pf_features.eta;
    *(++ptr) = c_pf_features.phi;
    *(++ptr) = c_pf_features.e;
    assert(start + n_features_cpf_ - 1 == ptr);
    /*         std::cout<<" cpfcand: "
             " - etarel "<< c_pf_features.etarel<<
             " - phirel "<< c_pf_features.phirel<<
             " - quality "<<c_pf_features.quality<<
             " - VTX_ass "<<c_pf_features.vtx_ass<<
             " - chi2  "<< c_pf_features.chi2<<
             " - puppiw "<<c_pf_features.puppiw<<
             " - drminsv "<<c_pf_features.drminsv<<
             " - ptrel "<<c_pf_features.ptrel<<
             " - BtagPf_trackEtaRel "<<c_pf_features.btagPf_trackEtaRel<<
             " - BtagPf_trackPtRel "<<c_pf_features.btagPf_trackPtRel<<
             " - BtagPf_trackPPar "<<c_pf_features.btagPf_trackPPar<<
             " - BtagPf_trackDeltaR "<<c_pf_features.btagPf_trackDeltaR<<
             " - BtagPf_trackPParRatio "<<c_pf_features.btagPf_trackPParRatio<<
             " - BtagPf_trackSip2dVal "<<c_pf_features.btagPf_trackSip2dVal<<
             " - BtagPf_trackSip2dSig "<<c_pf_features.btagPf_trackSip2dSig<<
             " - BtagPf_trackSip3dVal "<<c_pf_features.btagPf_trackSip3dVal<<
             " - BtagPf_trackSip3dSig "<<c_pf_features.btagPf_trackSip3dSig<<
             " - BtagPf_trackJetDistVal "<<c_pf_features.btagPf_trackJetDistVal<<
             " - pt "<<c_pf_features.pt<<
             " - eta "<<c_pf_features.eta<<
             " - phi "<<c_pf_features.phi<<
             " - e "<<c_pf_features.e<<
             std::endl;*/

  }
//  for (std::size_t c_pf_n = max_c_pf_n; c_pf_n < 50; c_pf_n++) {
//    ptr = &data_[kChargedCandidates][offset + c_pf_n * n_features_cpf_];
//    std::fill(ptr, ptr + n_features_cpf_, 0.0f);
 // }
 // std::cout<<" skata 3"<<std::endl;

/*
 // n_pf candidates
  auto max_n_pf_n = std::min(features.n_pf_features.size(), (std::size_t)n_npf_);

  for (std::size_t n_pf_n = 0; n_pf_n < max_n_pf_n; n_pf_n++) {
    const auto& n_pf_features = features.n_pf_features.at(n_pf_n);
    ptr = &data_[kNeutralCandidates][offset + n_pf_n * n_features_npf_];
    start = ptr;
    assert(start + n_features_npf_ - 1 == ptr);
  }
*/
 // std::cout<<" skata 3a"<<std::endl;
  // sv candidates
  auto max_sv_n = std::min(features.sv_features.size(), (std::size_t)n_sv_);
 // std::cout<<" skata 3b"<<std::endl;
 // auto max_sv_n = std::min(std::size_t(1), (std::size_t)n_sv_);
  for (std::size_t sv_n = 0; sv_n < max_sv_n; sv_n++) {
    const auto& sv_features = features.sv_features.at(sv_n);
    ptr = &data_[kVertices][offset + sv_n * n_features_sv_];
    start = ptr;
    *ptr     = 10*-0.34988176822662354;
    *(++ptr) = 10*-0.13795413076877594;
    *(++ptr) = 10*-4.549334526062012;
    *(++ptr) = 10*0.9975794553756714;
    *(++ptr) = 10*3.3894805908203125;
    *(++ptr) = 10*12.094277381896973;
    *(++ptr) = 10*2.1580896377563477;
    *(++ptr) = 10*12.118206024169922;
    *(++ptr) = 10*0.18568703532218933;
    *(++ptr) = 10*0.17420688271522522;
    *(++ptr) = 10*0.6081762909889221;
    *(++ptr) = 10*2.0;
    *(++ptr) = 10*-0.10806544125080109;
    *(++ptr) = 10*2.817603349685669;
    *(++ptr) = 10*-1.0470627546310425;
    *(++ptr) = 10*0.11788631975650787;
    *(++ptr) = 10*4.549334526062012;

    /*
    *ptr = sv_features.etarel;
    *(++ptr) = sv_features.phirel;
    *(++ptr) = sv_features.enratio;
    *(++ptr) = sv_features.costhetasvpv;
    *(++ptr) = sv_features.d3d;
    *(++ptr) = sv_features.d3dsig;
    *(++ptr) = sv_features.dxy;
    *(++ptr) = sv_features.dxysig;
    *(++ptr) = sv_features.normchi2;
    *(++ptr) = sv_features.chi2;
    *(++ptr) = sv_features.mass;
    *(++ptr) = sv_features.ntracks;
    *(++ptr) = sv_features.deltaR;
    *(++ptr) = sv_features.pt;
    *(++ptr) = sv_features.eta;
    *(++ptr) = sv_features.phi;
    *(++ptr) = sv_features.e;*/
    assert(start + n_features_sv_ - 1 == ptr);
  }
 //  std::cout<<" skata 3c "<<max_sv_n<<std::endl;  
 //  for (std::size_t sv_n = max_sv_n; sv_n < 5; sv_n++) {
 //   ptr = &data_[kVertices][offset + sv_n * n_features_sv_];
  //  std::fill(ptr, ptr + n_features_sv_, 0.0f);
 // }
 //  std::cout<<" skata 4"<<std::endl;

  // n_lt candidates
  auto max_lt_n = std::min(features.lt_features.size(), (std::size_t)n_lt_);
  max_lt_n =0;
  for (std::size_t lt_n = 0; lt_n < max_lt_n; lt_n++) {
    const auto& lt_features = features.lt_features.at(lt_n);
    ptr = &data_[kLostTracks][offset + lt_n * n_features_lt_];
    start = ptr;
    *ptr = lt_features.quality;
    *(++ptr) = lt_features.chi2;
    *(++ptr) = lt_features.puppiw;
    *(++ptr) = lt_features.drminsv;
    *(++ptr) = lt_features.btagPf_trackEtaRel;
    *(++ptr) = lt_features.btagPf_trackPtRel;
    *(++ptr) = lt_features.btagPf_trackPPar;
    *(++ptr) = lt_features.btagPf_trackDeltaR;
    *(++ptr) = lt_features.btagPf_trackPParRatio;
    *(++ptr) = lt_features.btagPf_trackSip2dVal;
    *(++ptr) = lt_features.btagPf_trackSip2dSig;
    *(++ptr) = lt_features.btagPf_trackSip3dVal;
    *(++ptr) = lt_features.btagPf_trackSip3dSig;
    *(++ptr) = lt_features.btagPf_trackJetDistVal;
    *(++ptr) = lt_features.pt;
    *(++ptr) = lt_features.eta;
    *(++ptr) = lt_features.phi;
    *(++ptr) = lt_features.e;
    assert(start + n_features_lt_ - 1 == ptr);
  }
   //for (std::size_t lt_n = max_lt_n; lt_n < 50; lt_n++) {
   // ptr = &data_[kLostTracks][offset + lt_n * n_features_lt_];
  //  std::fill(ptr, ptr + n_features_lt_, 0.0f);
 // }

  
  // std::cout<<" skata 5"<<std::endl;
  auto max_edge_n = std::min(features.pair_features.size(), (std::size_t)n_edge_);
  max_edge_n =0;
  for (std::size_t edge_n = 0; edge_n < max_edge_n; edge_n++) {
    const auto& edge_features = features.pair_features.at(edge_n);
    ptr = &data_[kEdges][offset + edge_n * n_features_edge_];
    start = ptr;
    *ptr = edge_features.pca_distpv;
    *(++ptr) = edge_features.distPCA;
    *(++ptr) = edge_features.dotprodTrack;
    *(++ptr) = edge_features.dotprodSeed;
    *(++ptr) = edge_features.pvd_PCAonSeed;
    *(++ptr) = edge_features.pvd_PCAonTrack;
    *(++ptr) = edge_features.dist_PCAjetAxis;
    *(++ptr) = edge_features.dotprod_PCAjetMomenta;
    *(++ptr) = edge_features.deta_PCAjetDirs;
    *(++ptr) = edge_features.dphi_PCAjetDirs;
    assert(start + n_features_edge_ - 1 == ptr);
  }

   // for (std::size_t edge_n = max_edge_n; edge_n < 300; edge_n++) {
  //     ptr = &data_[kEdges][offset + edge_n * n_features_edge_];
  //     std::fill(ptr, ptr + n_features_edge_, 0.0f);
  //  }
   //   std::cout<<" skata 6"<<std::endl;
}

//define this as a plug-in
DEFINE_FWK_MODULE(SoftParticleTransformerAK4ONNXJetTagsProducer);
