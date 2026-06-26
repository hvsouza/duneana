////////////////////////////////////////////////////////////////////////
// Class:       XeDopCRT
// Plugin Type: analyzer (Unknown Unknown)
// File:        XeDopCRT_module.cc
//
// Generated at Thu Jun 25 10:23:41 2026 by Henrique Vieira de Souza using cetskelgen
// from cetlib version 3.18.02.
////////////////////////////////////////////////////////////////////////

#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Principal/SubRun.h"
#include "canvas/Utilities/InputTag.h"
#include "fhiclcpp/ParameterSet.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

#include "art_root_io/TFileService.h"
#include <TTree.h>

#include <canvas/Persistency/Common/Ptr.h>
#include <lardataobj/RecoBase/PFParticle.h>
#include <lardataobj/RecoBase/Track.h>
#include <dunereco/AnaUtils/DUNEAnaTrackUtils.h>
#include <dunereco/AnaUtils/DUNEAnaPFParticleUtils.h>

#include <string>

namespace dune {
  class XeDopCRT;
}


class dune::XeDopCRT : public art::EDAnalyzer {
public:
  explicit XeDopCRT(fhicl::ParameterSet const& p);
  // The compiler-generated destructor is fine for non-base
  // classes without bare pointers or other resource use.

  // Plugins should not be copied or assigned.
  XeDopCRT(XeDopCRT const&) = delete;
  XeDopCRT(XeDopCRT&&) = delete;
  XeDopCRT& operator=(XeDopCRT const&) = delete;
  XeDopCRT& operator=(XeDopCRT&&) = delete;

  // Required functions.
  void analyze(art::Event const& evt) override;

  // Selected optional functions.
  void beginJob() override;
  void endJob() override;

private:


  TTree *t1 = nullptr;

  Int_t fEvent;
  Int_t ntracks;
  std::vector<Float_t> trkstartx;
  std::vector<Float_t> trkstarty;
  std::vector<Float_t> trkstartz;
  std::vector<Float_t> trkendx;
  std::vector<Float_t> trkendy;
  std::vector<Float_t> trkendz;
  std::vector<Float_t> trklen;
  std::vector<bool> trkIsTrack;
  std::vector<Float_t> trkstartdcosx;
  std::vector<Float_t> trkstartdcosy;
  std::vector<Float_t> trkstartdcosz;


  std::string fTrackModuleLabel = "pandoraTrack";
  std::string fPFParticleModuleLabel = "pandora";

  void reset();


};


dune::XeDopCRT::XeDopCRT(fhicl::ParameterSet const& p)
  : EDAnalyzer{p}  // ,
  // More initializers here.
{
  // Call appropriate consumes<>() for any products to be retrieved by this module.
}

void dune::XeDopCRT::analyze(art::Event const& evt)
{

  fEvent = evt.id().event();
  
  std::vector<art::Ptr<recob::Track>> pTracks;

  art::Handle< std::vector<recob::Track> > trackListHandle = evt.getHandle< std::vector<recob::Track> >(fTrackModuleLabel);

  if (trackListHandle.isValid()) {
    art::fill_ptr_vector(pTracks, trackListHandle);
  }
  else{
    mf::LogWarning("XeDopCRT") << "Failed to find Track data product with label " << fTrackModuleLabel << std::endl;
    reset();
    return;
  }
  ntracks = pTracks.size();

  std::cout << "Tracks found: " << ntracks << std::endl;
  for (const auto& track : pTracks) {
    trkstartx.push_back(track->Start().X());
    trkstarty.push_back(track->Start().Y());
    trkstartz.push_back(track->Start().Z());
    trkendx.push_back(track->End().X());
    trkendy.push_back(track->End().Y());
    trkendz.push_back(track->End().Z());
    trklen.push_back(track->Length());

    art::Ptr<recob::PFParticle> pfp = dune_ana::DUNEAnaTrackUtils::GetPFParticle(track, evt, fTrackModuleLabel);
    bool istrack = dune_ana::DUNEAnaPFParticleUtils::IsTrack(pfp, evt, fPFParticleModuleLabel, fTrackModuleLabel);
    trkIsTrack.push_back(istrack);

    if (istrack && pfp->PdgCode() != 13){
      mf::LogWarning("XeDopCRT") << "Track is not a muon! PdgCode: " << pfp->PdgCode() << std::endl;
    }

    std::cout << "Track start: (" << track->Start().X() << ", " << track->Start().Y() << ", " << track->Start().Z() << ")" << std::endl;



  }


  t1->Fill();
  reset();
}

void dune::XeDopCRT::beginJob()
{
  // Implementation of optional member function here.
  art::ServiceHandle<art::TFileService> tfs;
  t1 = tfs->make<TTree>("t1", "CRT data");
  t1->Branch("fEvent", &fEvent);
  t1->Branch("ntracks", &ntracks);
  t1->Branch("trkstartx", &trkstartx);
  t1->Branch("trkstarty", &trkstarty);
  t1->Branch("trkstartz", &trkstartz);
  t1->Branch("trkendx", &trkendx);
  t1->Branch("trkendy", &trkendy);
  t1->Branch("trkendz", &trkendz);
  t1->Branch("trkIsTrack", &trkIsTrack);
  t1->Branch("trklen", &trklen);
  // t1->Branch("trkstartdcosx", &trkstartdcosx);
  // t1->Branch("trkstartdcosy", &trkstartdcosy);
  // t1->Branch("trkstartdcosz", &trkstartdcosz);

}

void dune::XeDopCRT::endJob()
{
  // Implementation of optional member function here.
}

void dune::XeDopCRT::reset()
{
  fEvent = -9999;
  ntracks = -9999;
  trkstartx.clear();
  trkstarty.clear();
  trkstartz.clear();
  trkendx.clear();
  trkendy.clear();
  trkendz.clear();
  trkIsTrack.clear();
  trklen.clear();
  trkstartdcosx.clear();
  trkstartdcosy.clear();
  trkstartdcosz.clear();
}

DEFINE_ART_MODULE(dune::XeDopCRT)
