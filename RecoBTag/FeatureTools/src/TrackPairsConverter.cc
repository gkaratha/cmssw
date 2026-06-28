#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

#include "DataFormats/BTauReco/interface/TrackPairFeatures.h"

#include "RecoBTag/FeatureTools/interface/TrackPairInfoBuilder.h"

#include "DataFormats/PatCandidates/interface/Jet.h"

#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/IPTools/interface/IPTools.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "RecoVertex/VertexTools/interface/VertexDistance3D.h"

#include "RecoBTag/FeatureTools/interface/TrackPairsConverter.h"


namespace btagbtvdeep {

  void trackPairsToFeatures( const reco::TransientTrack& track1,
                               const reco::TransientTrack& track2,
                               const reco::Jet& jet,
                               const reco::Vertex& pv,
                               TrackPairFeatures& tp_features)

  {
    
    GlobalVector jetdirection(jet.px(), jet.py(), jet.pz());

    btagbtvdeep::TrackPairInfoBuilder tp;
    tp.buildTrackPairInfo((&track1),(&track2),pv,jetdirection);

    tp_features.pca_distpv = tp.pca_distpv();
    tp_features.distPCA = tp.pca_distance();
    tp_features.dsigPCA = tp.pca_significance();
    tp_features.x_PCAonSeed = tp.pcaSeed_x();
    tp_features.y_PCAonSeed = tp.pcaSeed_y();
    tp_features.z_PCAonSeed = tp.pcaSeed_z();
    tp_features.xerr_PCAonSeed = tp.pcaSeed_xerr();
    tp_features.yerr_PCAonSeed = tp.pcaSeed_yerr();
    tp_features.zerr_PCAonSeed = tp.pcaSeed_zerr();
    tp_features.x_PCAonTrack = tp.pcaTrack_x();
    tp_features.y_PCAonTrack = tp.pcaTrack_y();
    tp_features.z_PCAonTrack = tp.pcaTrack_z();
    tp_features.xerr_PCAonTrack = tp.pcaTrack_xerr();
    tp_features.yerr_PCAonTrack = tp.pcaTrack_yerr();
    tp_features.zerr_PCAonTrack = tp.pcaTrack_zerr();
    tp_features.dotprodTrack = tp.dotprodTrack();
    tp_features.dotprodSeed = tp.dotprodSeed();
    tp_features.dotprodTrackSeed2D = tp.dotprodTrackSeed2D();
    tp_features.dotprodTrackSeed3D = tp.dotprodTrackSeed3D();
    tp_features.dotprodTrackSeedVectors2D = tp.dotprodTrackSeed2DV();
    tp_features.dotprodTrackSeedVectors3D = tp.dotprodTrackSeed3DV();
    tp_features.pvd_PCAonSeed = tp.pcaSeed_dist();
    tp_features.pvd_PCAonTrack = tp.pcaTrack_dist();
    tp_features.dist_PCAjetAxis = tp.pca_jetAxis_dist();
    tp_features.dotprod_PCAjetMomenta = tp.pca_jetAxis_dotprod();
    tp_features.deta_PCAjetDirs = tp.pca_jetAxis_dEta();
    tp_features.dphi_PCAjetDirs = tp.pca_jetAxis_dPhi();
 
  }

}  // namespace btagbtvdeep
