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
    kChargedCandidates = 0,
    kVertices = 1,
    kLostTracks = 2,
    kEdges=3
  };
  unsigned n_cpf_;
  constexpr static unsigned n_features_cpf_ =  22; 
  unsigned n_lt_;
  constexpr static unsigned n_features_lt_ = 18; 
  unsigned n_sv_;
  constexpr static unsigned n_features_sv_ = 17; 
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

  // initialize output collection
  std::vector<std::unique_ptr<JetTagCollection>> output_tags;
  if (!tag_infos->empty()) {
    auto jet_ref = tag_infos->begin()->jet();
    auto ref2prod = edm::makeRefToBaseProdFrom(jet_ref, iEvent);
    for (std::size_t i = 0; i < flav_names_.size(); i++) {
      output_tags.emplace_back(std::make_unique<JetTagCollection>(ref2prod));
    }
  } else {
    for (std::size_t i = 0; i < flav_names_.size(); i++) {
      output_tags.emplace_back(std::make_unique<JetTagCollection>());
    }
  }

  for (unsigned jet_n = 0; jet_n < tag_infos->size(); ++jet_n) {
    const auto& taginfo = (*tag_infos)[jet_n];
    std::vector<float> outputs(flav_names_.size(), -1.0);

    const auto& jet_ref = tag_infos->at(jet_n).jet();
   // bool Print = fabs(jet_ref->eta())<2;

    if (taginfo.features().is_filled) {
      get_input_sizes(taginfo);
      // run prediction with dynamic batch size per event
      input_shapes_ = { {(int64_t)1, (int64_t)n_cpf_, (int64_t)n_features_cpf_},
                       {(int64_t)1, (int64_t)n_sv_, (int64_t)n_features_sv_},
                       {(int64_t)1, (int64_t)n_lt_, (int64_t)n_features_lt_},
                       {(int64_t)1, (int64_t)n_edge_, (int64_t)n_features_edge_}};
     // std::cout<<" input_shapes_ "<< input_shapes_[1][2]<<std::endl;
     // std::cout<<" Soft: data_.size "<<data_.size()<<" "<<data_[1].size()<<" "<<data_[1][0]<<std::endl;
    //  for(unsigned int ik=0; ik<data_[1].size(); ik++)
   //      std::cout<<" ik "<<ik<<" "<<data_[1][ik]<<std::endl;
      outputs = globalCache()->run(input_names_, data_, input_shapes_, output_names_, 1)[0];
      assert(outputs.size() == flav_names_.size());
    }
    
    for (std::size_t flav_n = 0; flav_n < flav_names_.size(); flav_n++) {
   //   std::cout<<" flav_n "<<flav_n<<" tag "<<flav_names_[flav_n]<<" output "<<outputs[flav_n]<<std::endl;
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
    n_cpf_ = std::clamp((unsigned int)features.c_pf_features.size(), (unsigned int)1, (unsigned int)50);
    n_sv_ = std::clamp((unsigned int)features.sv_features.size(), (unsigned int)1, (unsigned int)5);
    n_lt_ = std::clamp((unsigned int)features.lt_features.size(), (unsigned int)1, (unsigned int)50);
    n_edge_ = std::clamp((unsigned int)features.pair_features.size(), (unsigned int)1, (unsigned int)300);    

  } else {
    // Use fixed sizes for original version
    n_cpf_ = (unsigned int)50;
    n_sv_ = (unsigned int)5;
    n_lt_ = (unsigned int)50;
    n_edge_ = (unsigned int)300;
  }

  input_sizes_ = {
      n_cpf_ * n_features_cpf_,
      n_sv_  * n_features_sv_,
      n_lt_  * n_features_lt_,
      n_edge_ * n_features_edge_,
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
  float* ptr = nullptr;
  const float* start = nullptr;
  unsigned offset = 0;

  // c_pf candidates
  auto max_c_pf_n = std::min(features.c_pf_features.size(), (std::size_t)n_cpf_);
 // std::cout<<" CMSSW ncpf "<<max_c_pf_n<<std::endl;
  for (std::size_t c_pf_n = 0; c_pf_n < max_c_pf_n; c_pf_n++) {
    const auto& c_pf_features = features.c_pf_features.at(c_pf_n);
    ptr = &data_[kChargedCandidates][offset + c_pf_n * n_features_cpf_];
  //  std::cout<<c_pf_n<<"  "<<c_pf_features.pt<<std::endl;
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
   /* std::cout<<" trk "<<c_pf_n<<":  "
             <<" "<<c_pf_features.etarel
             <<" "<<c_pf_features.phirel
             <<" "<<c_pf_features.quality
             <<" "<<c_pf_features.vtx_ass
             <<" "<<c_pf_features.chi2
             <<" "<<c_pf_features.puppiw
             <<" "<<c_pf_features.drminsv
             <<" "<<c_pf_features.ptrel
             <<" "<<c_pf_features.btagPf_trackEtaRel
             <<" "<<c_pf_features.btagPf_trackPtRel
             <<" "<<c_pf_features.btagPf_trackPPar
             <<" "<<c_pf_features.btagPf_trackDeltaR
             <<" "<<c_pf_features.btagPf_trackPParRatio
             <<" "<<c_pf_features.btagPf_trackSip2dVal
             <<" "<<c_pf_features.btagPf_trackSip2dSig
             <<" "<<c_pf_features.btagPf_trackSip3dVal
             <<" "<<c_pf_features.btagPf_trackSip3dSig
             <<" "<<c_pf_features.btagPf_trackJetDistVal
             <<" "<<c_pf_features.pt
             <<" "<<c_pf_features.eta
             <<" "<<c_pf_features.phi
             <<" "<<c_pf_features.e
             <<std::endl;*/
    assert(start + n_features_cpf_ - 1 == ptr);

  }
  // sv candidates
  auto max_sv_n = std::min(features.sv_features.size(), (std::size_t)n_sv_);
 // max_sv_n =0;
 // std::cout<<" CMSSW sv "<<max_sv_n<<std::endl;
  for (std::size_t sv_n = 0; sv_n < max_sv_n; sv_n++) {
    const auto& sv_features = features.sv_features.at(sv_n);
    ptr = &data_[kVertices][offset + sv_n * n_features_sv_];
    start = ptr;
   // std::cout<<sv_n<<"  "<<sv_features.pt<<std::endl;
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
    *(++ptr) = sv_features.e;
  /*  std::cout<<" sv: "<<sv_n
        <<" "<< sv_features.etarel
        <<" "<< sv_features.phirel
        <<" "<< sv_features.enratio
        <<" "<< sv_features.costhetasvpv
        <<" "<< sv_features.d3d
        <<" "<< sv_features.d3dsig
        <<" "<< sv_features.dxy
        <<" "<< sv_features.dxysig
        <<" "<< sv_features.normchi2
        <<" "<< sv_features.chi2
        <<" "<< sv_features.mass
        <<" "<< sv_features.ntracks
        <<" "<< sv_features.deltaR
        <<" "<< sv_features.pt
        <<" "<< sv_features.eta
        <<" "<< sv_features.phi
        <<" "<< sv_features.e
        <<std::endl;*/

    assert(start + n_features_sv_ - 1 == ptr);
  }

  // n_lt candidates
  auto max_lt_n = std::min(features.lt_features.size(), (std::size_t)n_lt_);
//  std::cout<<" CMSSW LT "<<max_lt_n<<std::endl;
//  max_lt_n =0;
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
   // std::cout<<lt_n<<"  "<<lt_features.pt<<std::endl;
                   
    assert(start + n_features_lt_ - 1 == ptr);
  }

  auto max_edge_n = std::min(features.pair_features.size(), (std::size_t)n_edge_);
 // std::cout<<" pairs "<<max_edge_n<<std::endl;
 // max_edge_n =0;
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
   // std::cout<<" pair: "<<edge_n
   //          <<" "<< edge_features.pca_distpv
   //          <<" "<< edge_features.distPCA
   //          <<" "<< edge_features.dotprodTrack
   //          <<" "<< edge_features.dotprodSeed
   //          <<" "<< edge_features.pvd_PCAonSeed
   //          <<" "<< edge_features.pvd_PCAonTrack
   //          <<" "<< edge_features.dist_PCAjetAxis
   //          <<" "<< edge_features.dotprod_PCAjetMomenta
   //          <<" "<< edge_features.deta_PCAjetDirs
   //          <<" "<< edge_features.dphi_PCAjetDirs
   //          <<std::endl;
    
    assert(start + n_features_edge_ - 1 == ptr);
  }

}

//define this as a plug-in
DEFINE_FWK_MODULE(SoftParticleTransformerAK4ONNXJetTagsProducer);
