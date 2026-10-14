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
#include <lardataobj/RecoBase/OpFlash.h>
#include <lardataobj/RecoBase/OpHit.h>
#include <lardataobj/RecoBase/PFParticle.h>
#include <lardataobj/RecoBase/Track.h>
#include <dunereco/AnaUtils/DUNEAnaTrackUtils.h>
#include <dunereco/AnaUtils/DUNEAnaPFParticleUtils.h>
#include "detdataformats/trigger/TriggerCandidateData2.hpp"
#include "lardataobj/RecoBase/TrackingTypes.h"

#include <string>
#include <vector>

#pragma GCC diagnostic ignored "-Wunused-variable"

struct edges {
    double xmin, xmax;
    double ymin, ymax;
    double zmin, zmax;
};

inline std::vector<edges> CRT_top_triggers = {
    {200, 320, 310, 340, 80, 214}, //nTCO CRT
    {200, 320, -340, -310, 80, 214} //TCO CRT - top position
};
inline std::vector<edges> CRT_bot_triggers = {
    {140, 320, 310, 340, 80, 214}, //nTCO CRT
    {-226, -106, -340, -310, 80, 214} //TCO CRT - bottom position
};

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

  bool checkInBox(const recob::tracking::Point_t& point, const edges& box) {
    return (point.X() >= box.xmin && point.X() <= box.xmax) &&
           (point.Y() >= box.ymin && point.Y() <= box.ymax) &&
           (point.Z() >= box.zmin && point.Z() <= box.zmax);
  }

  // Selected optional functions.
  void beginJob() override;
  void endJob() override;

  void fill_ophit(art::Event const& evt, std::vector<art::Ptr<dunedaq::trgdataformats2::TriggerCandidateData>> pTrigger, std::string OpHitModuleLabel, std::string fFlashModuleLabel, std::vector<int>& opchannel, std::vector<double>& pe, std::vector<double>& time, std::vector<double>& amplitude){
      std::vector<art::Ptr<recob::OpHit>> opHits;
      art::Handle< std::vector<recob::OpHit> > opListHandle = evt.getHandle< std::vector<recob::OpHit> >(OpHitModuleLabel);

      // art::FindManyP< recob::OpFlash > AssnsFlashHit(opListHandle, evt, fFlashModuleLabel);
      std::cout << OpHitModuleLabel << " OpHit data product size: " << opListHandle->size() << std::endl;

      if (opListHandle.isValid()) {
          art::fill_ptr_vector(opHits, opListHandle);
          std::sort(opHits.begin(), opHits.end(), [](const art::Ptr<recob::OpHit>& a, const art::Ptr<recob::OpHit>& b) {
                  return a->PeakTimeAbs() < b->PeakTimeAbs();
                  });
      }
      else{
          std::cout << "Failed to find OpHit data product with label " << OpHitModuleLabel << std::endl;
          reset();
          return;
      }
      int nhits = opHits.size();

      long long DAQTS = pTrigger[0]->time_candidate;
      double DAQTS_us = (DAQTS&0xffffffffff) * 0.016;
      for (const auto& hit : opHits) {
          if (hit->OpChannel() >= 3000) continue; // Skip hits with OpChannel < 3000
          double timediff = hit->PeakTimeAbs() - DAQTS_us; //flash_time - DAQTS_us;
          if ( timediff > -10 && timediff < 35 ){

              // std::vector< art::Ptr<recob::OpFlash>> matchedflashes = AssnsFlashHit.at(hit.key());
              // if (matchedflashes.empty()){
              //     std::cout << "!!!!!!! No matched flash for hit with OpChannel: " << hit->OpChannel() << " and PeakTimeAbs: " << hit->PeakTimeAbs() << std::endl;
              //     continue;
              // }
              // if (matchedflashes.size() > 1){
              //     std::cout << "!!!!!!! More than one matched flash for hit with OpChannel: " << hit->OpChannel() << " and PeakTimeAbs: " << hit->PeakTimeAbs() << std::endl;
              //     continue;
              // }
              // auto flash = matchedflashes.at(0);
              opchannel.push_back(hit->OpChannel());
              pe.push_back(hit->PE());
              time.push_back(timediff);
              amplitude.push_back(hit->Amplitude());
              // flashid.push_back(flash.key());

          }
          // if ( timediff > 35 ){
          //     // Stop taking hits...
          //     break;
          // }
      }
  }


private:


  TTree *t1 = nullptr;

  Int_t run;
  Int_t event;
  Int_t ntracks;
  Int_t nselected;
  std::vector<Float_t> trkstartx;
  std::vector<Float_t> trkstarty;
  std::vector<Float_t> trkstartz;
  std::vector<Float_t> trkendx;
  std::vector<Float_t> trkendy;
  std::vector<Float_t> trkendz;
  std::vector<Float_t> trklen;
  std::vector<bool> trkIsTrack;
  std::vector<bool> trkIsSelected;
  std::vector<Float_t> trkstartdcosx;
  std::vector<Float_t> trkstartdcosy;
  std::vector<Float_t> trkstartdcosz;

  std::vector<int> opchannel;
  std::vector<double> pe;
  std::vector<double> time;
  std::vector<double> amplitude;
  // std::vector<int> flashid;
  std::vector<int> nopre_opchannel;
  std::vector<double> nopre_pe;
  std::vector<double> nopre_time;
  std::vector<double> nopre_amplitude;



  std::string fTrackModuleLabel = "pandoraTrack";
  std::string fPFParticleModuleLabel = "pandora";

  std::string fFlashModuleLabel = "opflash";
  std::string fOpHitModuleLabel = "ophitpreprocess";
  std::string fOpHitNoPreModuleLabel = "ophit";

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

  run = evt.run();
  event = evt.id().event();

  std::string fTriggerLabel = "triggerrawdecoder:daq";
  std::vector<art::Ptr<dunedaq::trgdataformats2::TriggerCandidateData>> pTrigger;
  auto pTriggerHandle = evt.getHandle< std::vector<dunedaq::trgdataformats2::TriggerCandidateData> >(fTriggerLabel);
  if (!pTriggerHandle.isValid()) {
      std::cout << "Failed to find trigger candidate data product with label " << fTriggerLabel << std::endl;
      reset();
      throw std::runtime_error("Trigger candidate data product not found");
  }
  else{
      art::fill_ptr_vector(pTrigger, pTriggerHandle);
  }
  auto ntriggers = pTrigger.size();
  if (ntriggers != 1){
      std::cout << "Something went wrong... there should be only one trigger candidate data product, but found " << ntriggers << std::endl;
      for (const auto& trigger : pTrigger) {
          std::cout << "Trigger type: " << static_cast<int>(trigger->type) << ", time_candidate: " << trigger->time_candidate << std::endl;
      }
      reset();
      return;
      // throw std::runtime_error("Unexpected number of trigger candidate data products");
  }


  // ------------------------------ Start Tracks ------------------------------

  std::vector<art::Ptr<recob::Track>> pTracks;
  art::Handle< std::vector<recob::Track> > trackListHandle = evt.getHandle< std::vector<recob::Track> >(fTrackModuleLabel);

  if (trackListHandle.isValid()) {
    art::fill_ptr_vector(pTracks, trackListHandle);
  }
  else{
    std::cout << "Failed to find Track data product with label " << fTrackModuleLabel << std::endl;
    reset();
    return;
  }
  ntracks = pTracks.size();
  nselected = 0;

  std::cout << "Tracks found: " << ntracks << std::endl;
  for (const auto& track : pTracks) {

    art::Ptr<recob::PFParticle> pfp = dune_ana::DUNEAnaTrackUtils::GetPFParticle(track, evt, fTrackModuleLabel);
    bool istrack = dune_ana::DUNEAnaPFParticleUtils::IsTrack(pfp, evt, fPFParticleModuleLabel, fTrackModuleLabel);

    // std::cout << "Track start: (" << track->Start().X() << ", " << track->Start().Y() << ", " << track->Start().Z() << ")" << std::endl;

    if (istrack && pfp->PdgCode() != 13 && !pfp->IsPrimary()) {
      std::cout << "Track is not a muon! PdgCode: " << pfp->PdgCode() << " is primary: " << pfp->IsPrimary()  << std::endl;
      throw std::runtime_error("Track is not a muon!");
    }

    bool isbottom = true;

    isbottom = pTrigger[0]->type == dunedaq::trgdataformats2::TriggerCandidateData::Type::kCTBCustomD ? true : false;

    if (pTrigger[0]->type != dunedaq::trgdataformats2::TriggerCandidateData::Type::kCTBCustomD && pTrigger[0]->type != dunedaq::trgdataformats2::TriggerCandidateData::Type::kCTBCustomC){
        std::cout << "Something went wrong... trigger candidate type is not kCTBCustomD or kCTBCustomE, but " << static_cast<int>(pTrigger[0]->type) << std::endl;
        throw std::runtime_error("Unexpected trigger candidate type");
    }

    std::vector<edges> CRT_triggers = isbottom ? CRT_bot_triggers : CRT_top_triggers;

    bool isSelected = false;
    // Check if both start and end points are within any of the CRT trigger boxes
    // If either point is within a box, we consider the track as selected
    auto box1 = CRT_triggers[0];
    auto box2 = CRT_triggers[1];
    if ( (checkInBox(track->Start(), box1) && checkInBox(track->End(), box2)) ||
         (checkInBox(track->Start(), box2) && checkInBox(track->End(), box1))) {
        isSelected = true;
        nselected++;
    }
    if (isSelected){
        trkstartx.push_back(track->Start().X());
        trkstarty.push_back(track->Start().Y());
        trkstartz.push_back(track->Start().Z());
        trkendx.push_back(track->End().X());
        trkendy.push_back(track->End().Y());
        trkendz.push_back(track->End().Z());
        trklen.push_back(track->Length());
        trkIsTrack.push_back(istrack);
        trkIsSelected.push_back(isSelected);
    }
  }

  if (nselected != 1){
      reset();
      return;
  }
  // ------------------------------ Finish Tracks ------------------------------


  fill_ophit(evt, pTrigger, fOpHitModuleLabel, fFlashModuleLabel, opchannel, pe, time, amplitude);
  // fill_ophit(evt, pTrigger, fOpHitNoPreModuleLabel, fFlashModuleLabel, nopre_opchannel, nopre_pe, nopre_time, nopre_amplitude);

  // ------------------------------ Start PDS ------------------------------


  // ------------------------------ Finish PDS ------------------------------
  t1->Fill();
  reset();
}

void dune::XeDopCRT::beginJob()
{
  // Implementation of optional member function here.
  art::ServiceHandle<art::TFileService> tfs;
  t1 = tfs->make<TTree>("t1", "CRT data");
  t1->Branch("run", &run);
  t1->Branch("event", &event);
  t1->Branch("ntracks", &ntracks);
  t1->Branch("nselected", &nselected);
  t1->Branch("trkstartx", &trkstartx);
  t1->Branch("trkstarty", &trkstarty);
  t1->Branch("trkstartz", &trkstartz);
  t1->Branch("trkendx", &trkendx);
  t1->Branch("trkendy", &trkendy);
  t1->Branch("trkendz", &trkendz);
  t1->Branch("trkIsTrack", &trkIsTrack);
  t1->Branch("trklen", &trklen);
  t1->Branch("trkIsSelected", &trkIsSelected);
  // t1->Branch("trkstartdcosx", &trkstartdcosx);
  // t1->Branch("trkstartdcosy", &trkstartdcosy);
  // t1->Branch("trkstartdcosz", &trkstartdcosz);
  t1->Branch("ophit_opchannel", &opchannel);
  t1->Branch("ophit_pe", &pe);
  t1->Branch("ophit_time", &time);
  t1->Branch("ophit_amplitude", &amplitude);
  // t1->Branch("flashid", &flashid);
  // t1->Branch("nopre_opchannel", &nopre_opchannel);
  // t1->Branch("nopre_pe", &nopre_pe);
  // t1->Branch("nopre_time", &nopre_time);
  // t1->Branch("nopre_amplitude", &nopre_amplitude);

}

void dune::XeDopCRT::endJob()
{
  // Implementation of optional member function here.
}

void dune::XeDopCRT::reset()
{
  run = -9999;
  event = -9999;
  ntracks = -9999;
  nselected = -9999;
  trkstartx.clear();
  trkstarty.clear();
  trkstartz.clear();
  trkendx.clear();
  trkendy.clear();
  trkendz.clear();
  trkIsTrack.clear();
  trklen.clear();
  trkIsSelected.clear();
  trkstartdcosx.clear();
  trkstartdcosy.clear();
  trkstartdcosz.clear();
  opchannel.clear();
  pe.clear();
  time.clear();
  amplitude.clear();
  // flashid.clear();
  nopre_opchannel.clear();
  nopre_pe.clear();
  nopre_time.clear();
  nopre_amplitude.clear();
}

DEFINE_ART_MODULE(dune::XeDopCRT)
