////////////////////////////////////////////////////////////////////////;
// Class:       PIDASelectionAna
// Plugin Type: analyzer (Unknown Unknown)
// File:        PIDASelectionAna_module.cc
//
// Generated at Tue Aug 13 08:39:57 2024 by Henrique Souza using cetskelgen
// from cetlib version 3.18.02.
////////////////////////////////////////////////////////////////////////

#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Principal/SubRun.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "canvas/Utilities/InputTag.h"
#include "canvas/Persistency/Common/Ptr.h"
#include "canvas/Persistency/Common/FindMany.h"
#include "canvas/Persistency/Common/FindManyP.h"
#include "canvas/Persistency/Common/PtrVector.h"
#include "fhiclcpp/ParameterSet.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

#include "larcore/Geometry/Geometry.h"
#include "larcoreobj/SimpleTypesAndConstants/geo_types.h"
#include "lardata/DetectorInfoServices/DetectorPropertiesService.h"
#include "lardata/DetectorInfoServices/DetectorClocksService.h"
#include "larsim/MCCheater/ParticleInventoryService.h"
#include "nusimdata/SimulationBase/MCTruth.h"
#include "nusimdata/SimulationBase/GTruth.h"
#include "nusimdata/SimulationBase/MCParticle.h"
#include "lardataobj/RecoBase/Track.h"
#include "lardataobj/RecoBase/Shower.h"
#include "lardataobj/RecoBase/Cluster.h"
#include "lardataobj/RecoBase/Hit.h"
#include "lardataobj/RecoBase/Vertex.h"
#include "lardataobj/RecoBase/PFParticle.h"
#include "lardataobj/RecoBase/SpacePoint.h"
#include "lardataobj/AnalysisBase/Calorimetry.h"
#include "lardataobj/AnalysisBase/ParticleID.h"
#include "larsim/Utils/TruthMatchUtils.h"
#include "lardataobj/RecoBase/PFParticleMetadata.h"
#include "lardataobj/Simulation/SimEnergyDeposit.h"

#include "larreco/RecoAlg/TrackMomentumCalculator.h"
#include "larreco/Calorimetry/CalorimetryAlg.h"

#include "dunereco/FDSensOpt/ParticleSelectionAlg/ParticleSelectionAlg.h"
#include "dunereco/FDSensOpt/FDSensOptData/EnergyRecoOutput.h"
#include "dunereco/FDSensOpt/FDSensOptData/AngularRecoOutput.h"

#include <vector>
#include <map>
#include <iterator> // std::begin(), std::end()
#include <string>
#include <stdexcept>
#include <optional>


#include "art_root_io/TFileService.h"
#include "TTree.h"

using std::cout;
using std::endl;
namespace pid {
  class PIDASelectionAna;
}

#pragma GCC diagnostic ignored "-Wunused-variable"
class pid::PIDASelectionAna : public art::EDAnalyzer {
  public:
    explicit PIDASelectionAna(fhicl::ParameterSet const& p);
    // The compiler-generated destructor is fine for non-base
    // classes without bare pointers or other resource use.

    // Plugins should not be copied or assigned.
    PIDASelectionAna(PIDASelectionAna const&) = delete;
    PIDASelectionAna(PIDASelectionAna&&) = delete;
    PIDASelectionAna& operator=(PIDASelectionAna const&) = delete;
    PIDASelectionAna& operator=(PIDASelectionAna&&) = delete;

    // Required functions.
    void analyze(art::Event const& e) override;

    // Selected optional functions.
    void beginJob() override;
    void endJob() override;

  private:

    // Declare member data here.

    std::string fLArG4ModuleLabel      = "largeant";
    std::string fPFParticleModuleLabel = "pandora";
    std::string fTrackModuleLabel      = "pandoraTrack";
    std::string fShowerModuleLabel     = "pandoraShower";
    std::string fHitModuleLabel        = "hitfd";
    std::string fParticleIDModuleLabel = "pandorapid";
    std::string fVertexModuleLabel     = "pandora";
    std::string fSpacePointModuleLabel = "pandora";

    bool fRollUpUnsavedIDs = true;
    const unsigned int kNplanes = 3;


    Float_t fDistanceToWallThreshold = 20;

    art::Handle< std::vector<recob::PFParticle> > pfpListHandle;
    art::Handle< std::vector<recob::Track> > trackListHandle;
    art::Handle< std::vector<recob::Shower> > showerListHandle;
    art::Handle< std::vector<recob::Hit> > hitListHandle;
    art::FindManyP<recob::Hit> *fmht;
    art::FindManyP<recob::PFParticle> *fmpfp;
    art::FindManyP<recob::Track> *fmtrk;
    art::FindManyP<recob::Shower> *fmshw;
    art::FindManyP<anab::ParticleID> *fmpid;
    art::FindManyP<recob::Vertex> *fmvtx;
    art::FindManyP<recob::SpacePoint> *fmsp;


    Float_t correct_hits = 0.f;
    TTree *fTree;

    Int_t run;
    Int_t subrun;
    Int_t event;

    Int_t   nuPDG_truth;     //neutrino PDG code
    Int_t   ccnc_truth;      //0=CC 1=NC
    Int_t   mode_truth;      //0=QE/El, 1=RES, 2=DIS, 3=Coherent production
    Float_t nuWeight_truth;   //neutrino weight from generator
    Float_t enu_truth;        //true neutrino energy
    Float_t enuvis_truth;     //true visible neutrino energy
    Float_t hadvis_truth;     //true visible neutrino energy
    bool    IsContainedTrue; // if event is contained or not

    Float_t vtxx_truth;
    Float_t vtxy_truth;
    Float_t vtxz_truth;

    Float_t nu_dcosx_truth;
    Float_t nu_dcosy_truth;
    Float_t nu_dcosz_truth;

    Float_t recovtxx;
    Float_t recovtxy;
    Float_t recovtxz;


    Float_t lepen;
    Float_t lepmass;
    Int_t leppdg;
    Int_t lepid;

    // Reconstructed neutrino energy
    Float_t Ev_reco_nue;              // Nue reco. energy (GeV)
    Float_t RecoLepEnNue;             // Lepton reco. energy (GeV)
    Float_t RecoHadEnNue;             // Hadrionic reco. energy (GeV)
    Short_t RecoMethodNue;              // Method used, 2 - highest charge + hadro. 3 - All hits

    Float_t Ev_reco_numu;             // Numu reco. energy (GeV)
    Float_t RecoLepEnNumu;            // Lepton reco. energy (GeV)
    Float_t RecoHadEnNumu;            // Hadronic reco. energy (GeV)
    Short_t RecoMethodNumu;             // Method used , 1 - longest track + hadro. 3 - All hits
    Short_t LongestTrackContNumu;       // 1 - longest track is cointaned, 0 not contained, -1 not contained or missing track
    Short_t TrackMomMethodNumu;         // Method use for track energy, 1 - momentum by range, 0 by Multi-coulomb scattering, -1 none.

    Float_t Ev_reco_nc;               // Nu energy by all hits


    Int_t   ntracks;
    std::vector<Short_t> trkId;
    std::vector<Float_t> trkstartx;       // starting x position.
    std::vector<Float_t> trkstarty;       // starting y position.
    std::vector<Float_t> trkstartz;       // starting z position.
    std::vector<Float_t> trkendx;         // ending x position.
    std::vector<Float_t> trkendy;         // ending y position.
    std::vector<Float_t> trkendz;         // ending z position.
    std::vector<Float_t> trkstartdcosx;
    std::vector<Float_t> trkstartdcosy;
    std::vector<Float_t> trkstartdcosz;
    std::vector<Float_t> trkpurity;       // track purity based on hit information
    std::vector<Float_t> trkcompleteness;                //track completeness based on hit information
    std::vector<int> trkg4id;             //true g4 track id for the reconstructed track
    std::vector<int> trkg4pdg;            //true g4 track pdg code
    std::vector<Float_t> trkg4en;            //true g4 track en code
    std::vector<Float_t> trkg4mass;            //true g4 track en code
    std::vector<int> trkg4mother;

    std::vector<int> trknhits; // n hits of track
    std::vector<int> trknhitsmatch; // n hits of track
    std::vector<std::vector<int>> trkg4id_planes;             //true g4 track id for the reconstructed track
    std::vector<std::vector<int>> trkg4pdg_planes;            //true g4 track pdg code
    std::vector<std::vector<Float_t>> trkg4en_planes;            //true g4 track pdg code
    std::vector<std::vector<Float_t>> trkg4mass_planes;            //true g4 track pdg code
    std::vector<std::vector<int>> trkg4mother_planes;
    std::vector<std::vector<Float_t>> trkpurity_planes;       // track purity based on hit information
    std::vector<std::vector<Float_t>> trkcompleteness_planes; //track completeness based on hit information
    std::vector<std::vector<int>> trknhits_planes;            // n hits per plane
    std::vector<std::vector<int>> trknhitsmatch_planes;            // n hits per plane
    std::vector<int> trkbestplane;                            // idx of best plane
    std::vector<Float_t> trklen;             // length.
    std::vector<Float_t> trkmomrange_pr;     // momentum by range assuming proton
    std::vector<Float_t> trkmomrange_mu;     // momentum by range assuming muon
    std::vector<Float_t> trkmomllhd;         // momentum by llhd
    std::vector<Float_t> trkmomchi2;
    std::vector<std::vector<Float_t>> trkcalo_planes;
    std::vector<std::vector<Float_t>> trktotalen_planes;
    std::vector<std::vector<Float_t>> trkpartialen_planes;
    std::vector<bool> trkIsContained;        // if track is contained
    std::vector<bool> trkIsContainedTrue;        // if track is contained
    std::vector<bool> trkPFPIsTrack;         // if PFParticle tells that it is a track
    std::vector<bool> selected_lep;
    std::vector<bool> selected_pr;
    std::vector<bool> selected_pi;

    std::vector<Float_t> trkPFPScoreIsTrack; // the score for being a track


    std::vector<Float_t> trkshwstartx;
    std::vector<Float_t> trkshwstarty;
    std::vector<Float_t> trkshwstartz;
    std::vector<Float_t> trkshwlen;


    std::vector<Float_t> allcalo_planes; // all calo energy of the event
    std::vector<Int_t> nallhits_planes;

    std::vector<std::vector<Int_t>> trkpidndf;      // Particle PID ndf based on valid hits
    std::vector<std::vector<Int_t>> trkpidchi2ndf;
    std::vector<std::vector<Float_t>> trkpidchi2pr; // particle PID chisq for proton
    std::vector<std::vector<Float_t>> trkpidchi2ka; // particle PID chisq for kaon
    std::vector<std::vector<Float_t>> trkpidchi2pi; // particle PID chisq for pion
    std::vector<std::vector<Float_t>> trkpidchi2mu; // particle PID chisq for muon
    std::vector<std::vector<Float_t>> trkpidpida;   // particle PIDA

    enum NuType{
      Numu,
      Nue,
      NC,
    };

    void HitsPurity(detinfo::DetectorClocksData const& clockData,
        std::vector< art::Ptr<recob::Hit> > const& hits,
        std::vector<Int_t>& trackid,
        std::vector<Float_t>& purity,
        std::vector<Float_t>& completeness,
        std::map<Int_t,Int_t> HitsToMCCounts
        );

    template <typename T> std::vector<art::Ptr<T>> GetProductVector(const art::Event &evt, const std::string &label);

    template <typename T, typename U> std::vector<art::Ptr<T>> GetAssocProductVector(const art::Ptr<U> &pProd, const art::Event &evt, const std::string &label, const std::string &assocLabel);

   template <typename T, typename U> art::Ptr<T> GetAssocProduct(const art::Ptr<U> &pProd, const art::Event &evt, const std::string &label, const std::string &assocLabel);

    bool IsContained(const std::vector<art::Ptr<recob::Hit> > &hits, const art::Event &event);

    bool IsPointContained(const double x, const double y, const double z);

    calo::CalorimetryAlg fCalorimetryAlg;                    ///< the calorimetry algorithm
    fhicl::ParameterSet fParticleSelectionAlgParamsNumu;
    fhicl::ParameterSet fParticleSelectionAlgParamsNue;
    fhicl::ParameterSet fParticleSelectionAlgParamsNC;

    void computeEnergies(
        detinfo::DetectorClocksData const & clockData,
        detinfo::DetectorPropertiesData const & detProp,
        const std::vector< std::vector< art::Ptr<recob::Hit> > > hits,
        std::vector< std::vector< Float_t > > &caloEnergies
        );

    void computeEnergies(
        detinfo::DetectorClocksData const & clockData,
        detinfo::DetectorPropertiesData const & detProp,
        const std::vector< std::vector< art::Ptr<recob::Hit> > > hits,
        std::vector< Float_t > &caloEnergies
        );

    double LifetimeCorrection(detinfo::DetectorClocksData const& clockData,
                              detinfo::DetectorPropertiesData const& detProp,
                              const double timeInTicks, const double t0InMicroS);

    double LifetimeCorrectedTotalHitCharge(detinfo::DetectorClocksData const & clockData,
                                           detinfo::DetectorPropertiesData const & detProp,
                                           const std::vector< art::Ptr< recob::Hit > > & hits);

    double CalculateEnergyFromCharge(const double charge, const unsigned short plane);

    bool isEnergyDepositedOutsideActiveVolume(const sim::SimEnergyDeposit &edep);
    std::map<int, std::tuple<double, bool, std::string>> EnergyDepositSummary(const art::Event& evt);
    std::vector<double> fActiveBounds;
    std::vector<double> getActiveBounds();

    void reset();
};


pid::PIDASelectionAna::PIDASelectionAna(fhicl::ParameterSet const& p)
  : EDAnalyzer{p},
  fCalorimetryAlg(p.get<fhicl::ParameterSet>("CalorimetryAlg")),
  fParticleSelectionAlgParamsNumu(p.get<fhicl::ParameterSet>("ParticleSelectionAlgNumu")),
  fParticleSelectionAlgParamsNue(p.get<fhicl::ParameterSet>("ParticleSelectionAlgNue")),
  fParticleSelectionAlgParamsNC(p.get<fhicl::ParameterSet>("ParticleSelectionAlgNC"))
  // More initializers here.
{
  // Call appropriate consumes<>() for any products to be retrieved by this module.
}



void pid::PIDASelectionAna::analyze(art::Event const& evt)
{
  dune::ParticleSelectionAlg
  fParticleSelectionAlgNumu(fParticleSelectionAlgParamsNumu,
      "pandora", "pandoraTrack", "pandoraShower", "hitfd", "pandoraTrack",
      "pandoraShower", "pandora");
  dune::ParticleSelectionAlg
  fParticleSelectionAlgNue(fParticleSelectionAlgParamsNue,
      "pandora", "pandoraTrack", "pandoraShower", "hitfd", "pandoraTrack",
      "pandoraShower", "pandora");
  dune::ParticleSelectionAlg
  fParticleSelectionAlgNC(fParticleSelectionAlgParamsNC,
      "pandora", "pandoraTrack", "pandoraShower", "hitfd", "pandoraTrack",
      "pandoraShower", "pandora");

  std::unique_ptr<dune::ParticleSelectionAlg> psel;

  auto const clockData = art::ServiceHandle<detinfo::DetectorClocksService const>()->DataFor(evt);
  auto const detProp = art::ServiceHandle<detinfo::DetectorPropertiesService const>()->DataFor(evt, clockData);
  art::ServiceHandle<cheat::ParticleInventoryService> pi_serv;
  const sim::ParticleList& plist = pi_serv->ParticleList();
  // Implementation of required member function here.
  run = evt.run();
  subrun = evt.subRun();
  event = evt.id().event();

  bool isMC = !evt.isRealData();

  NuType nutype{NuType::Numu};
  fActiveBounds = getActiveBounds();

  std::vector<art::Ptr<simb::MCTruth> > mclist;
  art::Ptr<simb::MCTruth> truth;
  if (isMC)
  {
    auto gt = evt.getHandle< std::vector<simb::MCTruth> >("generator");
    if (gt)
      art::fill_ptr_vector(mclist, gt);
    truth = mclist[0];
    if (truth->NeutrinoSet())
    {
      enu_truth = truth->GetNeutrino().Nu().E();
      //Compute visible energy (Stolen from Pierre.. FOUND I MISTAKE! )
      double fVisibleEnergy = 0;
      double fVisibleEnergyHad = 0; //Visible energy not coming from the primary lepton in the case of CC interactions
      std::map<int, std::tuple<double, bool, std::string>> edepSummary = EnergyDepositSummary(evt);
      IsContainedTrue = std::all_of(edepSummary.begin(), edepSummary.end(), [](const std::pair<int, std::tuple<double, bool, std::string>> &edep){return std::get<1>(edep.second);});

      for(const std::pair<const int, std::tuple<double, bool, std::string>> &edep : edepSummary){
        fVisibleEnergy += std::get<0>(edep.second);
        //Only add to had ebergy if not a lepton (pdg not in 11, 13, 15)
        const simb::MCParticle* pPart = plist.at(edep.first);
        if(abs(pPart->PdgCode()) != 11 && abs(pPart->PdgCode()) != 13 && abs(pPart->PdgCode()) != 15){
          fVisibleEnergyHad += std::get<0>(edep.second);
        }
      }
      enuvis_truth = static_cast<Float_t>(fVisibleEnergy)/1000.;
      hadvis_truth = static_cast<Float_t>(fVisibleEnergyHad)/1000.;

      vtxx_truth = truth->GetNeutrino().Nu().Vx();
      vtxy_truth = truth->GetNeutrino().Nu().Vy();
      vtxz_truth = truth->GetNeutrino().Nu().Vz();
      nu_dcosx_truth = truth->GetNeutrino().Nu().Px()/truth->GetNeutrino().Nu().P();
      nu_dcosy_truth = truth->GetNeutrino().Nu().Py()/truth->GetNeutrino().Nu().P();
      nu_dcosz_truth = truth->GetNeutrino().Nu().Pz()/truth->GetNeutrino().Nu().P();
      nuPDG_truth = truth->GetNeutrino().Nu().PdgCode();
      ccnc_truth = truth->GetNeutrino().CCNC();
      mode_truth = truth->GetNeutrino().Mode();
      lepen = truth->GetNeutrino().Lepton().E();
      lepmass = truth->GetNeutrino().Lepton().Mass();
      leppdg = truth->GetNeutrino().Lepton().PdgCode();
      lepid = truth->GetNeutrino().Lepton().TrackId();

      if ( gt ){
        auto gt = evt.getHandle< std::vector<simb::GTruth> >("generator");
        auto gtruth = (*gt)[0];
        nuWeight_truth = gtruth.fweight;;
      }

      if( abs(truth->GetNeutrino().Nu().PdgCode()) == 14  && truth->GetNeutrino().CCNC() == 0){
        psel = std::make_unique<dune::ParticleSelectionAlg>(std::move(fParticleSelectionAlgNumu));
        nutype = NuType::Numu;
      }
      else if( abs(truth->GetNeutrino().Nu().PdgCode()) == 12  && truth->GetNeutrino().CCNC() == 0){
        psel = std::make_unique<dune::ParticleSelectionAlg>(std::move(fParticleSelectionAlgNue));
        nutype = NuType::Nue;
      }
      else{
        psel = std::make_unique<dune::ParticleSelectionAlg>(std::move(fParticleSelectionAlgNC));
        nutype = NuType::NC;
      }

    }
    else
    {
      // FIXME
    }
  }

  auto ereconuein = evt.getHandle<dune::EnergyRecoOutput>("energyreconue");
  auto ereconumuin = evt.getHandle<dune::EnergyRecoOutput>("energyreconumu");
  auto ereconcin = evt.getHandle<dune::EnergyRecoOutput>("energyreconc");

  if ( !ereconuein.failedToGet() )
  {
    Ev_reco_nue          = ereconuein->fNuLorentzVector.E();
    RecoLepEnNue         = ereconuein->fLepLorentzVector.E();
    RecoHadEnNue         = ereconuein->fHadLorentzVector.E();
    RecoMethodNue        = ereconuein->recoMethodUsed;
  }
  else{
    std::cerr << "Warning! No product found with label: " << "energyreconue" << std::endl;
  }

  // Get normal energy reco for numu
  if ( !ereconumuin.failedToGet() )
  {
    Ev_reco_numu         = ereconumuin->fNuLorentzVector.E();
    RecoLepEnNumu        = ereconumuin->fLepLorentzVector.E();
    RecoHadEnNumu        = ereconumuin->fHadLorentzVector.E();
    RecoMethodNumu       = ereconumuin->recoMethodUsed;
    LongestTrackContNumu = ereconumuin->longestTrackContained;
    TrackMomMethodNumu   = ereconumuin->trackMomMethod;
  }
  else
    std::cerr << "Warning! No product found with label: " << "energyreconumu" << std::endl;




  if ( !ereconcin.failedToGet() )
    Ev_reco_nc          = ereconcin->fNuLorentzVector.E();
  else
    std::cerr << "Warning! No product found with label: " << "energyreconc" << std::endl;




  std::vector< art::Ptr< recob::PFParticle > > PFPs = GetProductVector<recob::PFParticle>(evt, fPFParticleModuleLabel);
  std::vector< art::Ptr< recob::Track > > tracks = GetProductVector<recob::Track>(evt, fTrackModuleLabel);

  // if (tracks.size() == 0) return;

  // again.. not really tracks here hehe
  ntracks = PFPs.size()-1; //excluding the neutrino
  if (PFPs.size()==0){
    return;
  }

  // * tracks, need this for FindManyP
  pfpListHandle    = evt.getHandle< std::vector<recob::PFParticle> >(fPFParticleModuleLabel);
  trackListHandle  = evt.getHandle< std::vector<recob::Track> >(fTrackModuleLabel);
  showerListHandle = evt.getHandle< std::vector<recob::Shower> >(fShowerModuleLabel);
  hitListHandle    = evt.getHandle< std::vector<recob::Hit> >(fHitModuleLabel);


  fmtrk = new art::FindManyP<recob::Track>(pfpListHandle, evt, fTrackModuleLabel);
  fmshw = new art::FindManyP<recob::Shower>(pfpListHandle, evt, fShowerModuleLabel);
  fmht  = new art::FindManyP<recob::Hit>(trackListHandle, evt, fTrackModuleLabel);
  fmpfp = new art::FindManyP<recob::PFParticle>(trackListHandle, evt, fTrackModuleLabel);
  fmpid = new art::FindManyP<anab::ParticleID>(trackListHandle, evt, fParticleIDModuleLabel);
  fmvtx = new art::FindManyP<recob::Vertex>(pfpListHandle, evt, fPFParticleModuleLabel);
  fmsp  = new art::FindManyP<recob::SpacePoint>(hitListHandle, evt, fSpacePointModuleLabel);



  trkg4id_planes.resize(kNplanes);
  trkg4pdg_planes.resize(kNplanes);
  trkg4en_planes.resize(kNplanes);
  trkg4mass_planes.resize(kNplanes);
  trkg4mother_planes.resize(kNplanes);
  trkpurity_planes.resize(kNplanes);
  trkcompleteness_planes.resize(kNplanes);
  trknhits_planes.resize(kNplanes);
  trknhitsmatch_planes.resize(kNplanes);
  trkcalo_planes.resize(kNplanes);
  trktotalen_planes.resize(kNplanes);
  trkpartialen_planes.resize(kNplanes);
  allcalo_planes.resize(kNplanes);
  nallhits_planes.resize(kNplanes);
  trkpidndf.resize(kNplanes);
  trkpidchi2ndf.resize(kNplanes);
  trkpidchi2pr.resize(kNplanes);
  trkpidchi2ka.resize(kNplanes);
  trkpidchi2pi.resize(kNplanes);
  trkpidchi2mu.resize(kNplanes);
  trkpidpida.resize(kNplanes);


  for (unsigned int ipl = 0; ipl < kNplanes; ++ipl)
  {
    trkpidndf[ipl].assign(ntracks, -1);
    trkpidchi2ndf[ipl].assign(ntracks, -1);
    trkpidchi2pr[ipl].assign(ntracks, -1);
    trkpidchi2ka[ipl].assign(ntracks, -1);
    trkpidchi2pi[ipl].assign(ntracks, -1);
    trkpidchi2mu[ipl].assign(ntracks, -1);
    trkpidpida[ipl].assign(ntracks, -1);
  }

  // Computing hit to MC association before enter the loop in each track
  // This is used for the completeness of tracks
  std::map<int,int> HitsToMCCounts;
  std::vector<std::map<int,int>> HitsToMCCounts_Planes(kNplanes);
  std::vector< art::Ptr<recob::Hit> > allHits = GetProductVector<recob::Hit>(evt, fHitModuleLabel);
  std::vector< std::vector<art::Ptr<recob::Hit>> > hits_plane(kNplanes);
  if(isMC){
    for(size_t i = 0; i < allHits.size(); i++)
    {
      TruthMatchUtils::G4ID hitID(TruthMatchUtils::TrueParticleID(clockData, allHits.at(i), fRollUpUnsavedIDs));
      ++HitsToMCCounts[hitID];
      if (allHits.at(i)->WireID().Plane < kNplanes){
        auto hitplane = allHits.at(i)->WireID().Plane;
        ++HitsToMCCounts_Planes[hitplane][hitID];
        hits_plane[hitplane].push_back(allHits.at(i));
      }
    }
  }
  computeEnergies(clockData, detProp, hits_plane, allcalo_planes);
  for (size_t ipl = 0; ipl < kNplanes; ++ipl){
    nallhits_planes[ipl] = hits_plane[ipl].size();
  }


  trkf::TrackMomentumCalculator trkmrange{0,3000};
  trkf::TrackMomentumCalculator trkmllhd{10, 3000, 10, 3};
  

  if (nutype == NuType::Numu){
    std::optional<art::Ptr<recob::Track>> leptrk = psel->GetLongestTrackPID(evt);
    (void)leptrk;
  }
  else if (nutype == NuType::Nue){
    std::optional<art::Ptr<recob::Shower>> lepshw = psel->GetHighestChargeShowerPID(evt);
    (void)lepshw;
  }
  std::vector<art::Ptr<recob::Track>> protons = psel->GenProtonCandidates(evt);
  std::vector<art::Ptr<recob::Track>> pions = psel->GenPionCandidates(evt);

  std::unordered_set<art::Ptr<recob::Track>::key_type> keys_of_protons;
  for (const art::Ptr<recob::Track> &t : protons) keys_of_protons.insert(t.key());

  std::unordered_set<art::Ptr<recob::Track>::key_type> keys_of_pions;
  for (const art::Ptr<recob::Track> &t : pions) keys_of_pions.insert(t.key());

  for (const auto &pfp: PFPs){
    // auto ipfp = &pfp - &PFPs[0];
    auto ipfp = pfp->Self();
    const int pdg = pfp->PdgCode();
    if ((std::abs(pdg) == 12) || (std::abs(pdg) == 14) || (std::abs(pdg) ==16)){
      // cout << "The neutrinooooo" << endl;
      continue;
    }

    // There isn't a direct association between PFParticles and hits, so we go via clusters
    const std::vector<art::Ptr<recob::Cluster>> theseClusters = GetAssocProductVector<recob::Cluster>(pfp,evt, fPFParticleModuleLabel, fPFParticleModuleLabel);

    std::vector<art::Ptr<recob::Hit>> trkhits;
    for (const art::Ptr<recob::Cluster> pCluster : theseClusters)
    {
      const std::vector<art::Ptr<recob::Hit>> tempHits = GetAssocProductVector<recob::Hit>(pCluster,evt,fPFParticleModuleLabel,fPFParticleModuleLabel);
      trkhits.insert(trkhits.end(),tempHits.begin(),tempHits.end());
    }


    std::vector<std::vector< art::Ptr<recob::Hit> > > trkhits_plane( kNplanes );
    for(size_t ah = 0; ah < trkhits.size(); ++ah){
      if (trkhits[ah]->WireID().Plane < kNplanes){
        trkhits_plane[trkhits[ah]->WireID().Plane].push_back(trkhits[ah]);
      }
    }

    computeEnergies(clockData, detProp, trkhits_plane, trkcalo_planes);

    trknhits.push_back(trkhits.size());
    trkIsContained.push_back(IsContained(trkhits, evt));
    int nhitsU = trkhits_plane[0].size();
    int nhitsV = trkhits_plane[1].size();
    int nhitsW = trkhits_plane[2].size();
    if (nhitsW >= nhitsU && nhitsW >= nhitsV)
      trkbestplane.push_back(2);
    else if( nhitsV >= nhitsU )
      trkbestplane.push_back(1);
    else if( nhitsU > nhitsV && nhitsU > nhitsW)
      trkbestplane.push_back(0);
    else
      trkbestplane.push_back(-1);
    // Computes g4 id corresponding to track using TruthMatchUtils for each plane
    for (size_t ipl = 0; ipl < kNplanes; ++ipl){
      trknhits_planes[ipl].push_back(trkhits_plane[ipl].size());
      HitsPurity(clockData, trkhits_plane[ipl], trkg4id_planes[ipl], trkpurity_planes[ipl], trkcompleteness_planes[ipl], HitsToMCCounts_Planes[ipl]);
      if (trkg4id_planes[ipl].back()>0){
        trknhitsmatch_planes[ipl].push_back(correct_hits);
        const simb::MCParticle *particle = pi_serv->TrackIdToParticle_P(trkg4id_planes[ipl].back());
        trkg4pdg_planes[ipl].push_back(particle->PdgCode());
        trkg4en_planes[ipl].push_back(particle->E());
        trkg4mass_planes[ipl].push_back(particle->Mass());
        trkg4mother_planes[ipl].push_back(particle->Mother());
      }
      else{
        trknhitsmatch_planes[ipl].push_back(-1);
        trkg4pdg_planes[ipl].push_back(-1);
        trkg4en_planes[ipl].push_back(-1);
        trkg4mass_planes[ipl].push_back(-1);
        trkg4mother_planes[ipl].push_back(-1);
      }
    }

    // Computes g4 id corresponding to track using TruthMatchUtils
    HitsPurity(clockData, trkhits, trkg4id, trkpurity, trkcompleteness, HitsToMCCounts);
    if (trkg4id.back()>0){
      trknhitsmatch .push_back(correct_hits);
      const simb::MCParticle *particle = pi_serv->TrackIdToParticle_P(trkg4id.back());
      // cout << trkg4id.back() << " " << particle->TrackId() << " " << endl;
      trkg4pdg.push_back(particle->PdgCode());
      trkg4en.push_back(particle->E());
      trkg4mass.push_back(particle->Mass());
      trkg4mother.push_back(particle->Mother());
    }
    else{
      trknhitsmatch .push_back(-1);
      trkg4pdg.push_back(-1);
      trkg4en.push_back(-1);
      trkg4mass.push_back(-1);
      trkg4mother.push_back(-1);
    }
    int trkmcidx = -1;
    auto mcProd = evt.getHandle< std::vector< simb::MCParticle>>("largeant");
    std::vector< art::Ptr< simb::MCParticle > > mcparticles;
    art::fill_ptr_vector(mcparticles, mcProd);
    art::Ptr< simb::MCParticle > mctrk{};
    if (trkg4id.back()!=-1)
    {
      for (uint imc = 0; imc < mcparticles.size(); imc++){
        if ( mcparticles[imc]->TrackId() == trkg4id.back() ){
          mctrk = mcparticles[imc];
          trkmcidx = imc;
          break;
        }
      }
    }
    bool trueIsContained=true;
    if (trkmcidx==-1) trueIsContained=false;
    else{
      auto const& pos = mctrk->Trajectory();

      int n_points = mctrk->NumberTrajectoryPoints();
      for (int i = 0; i < n_points; i++) {
          if (!(this->IsPointContained(pos.X(i),pos.Y(i),pos.Z(i))))
              trueIsContained = false;
      }
    }
    trkIsContainedTrue.push_back(trueIsContained);



    bool foundlep = false;
    trkId    .push_back(ipfp);
    std::vector<art::Ptr<recob::Track>> tmpTracks = GetAssocProductVector<recob::Track>(pfp, evt, fPFParticleModuleLabel, fTrackModuleLabel);
    if (tmpTracks.empty()){ // no tracks..
      trkstartx.push_back(-9999);
      trkstarty.push_back(-9999);
      trkstartz.push_back(-9999);
      trkendx  .push_back(-9999);
      trkendy  .push_back(-9999);
      trkendz  .push_back(-9999);
      trkstartdcosx.push_back(-9999);
      trkstartdcosy.push_back(-9999);
      trkstartdcosz.push_back(-9999);
      trklen   .push_back(-9999);
      for (size_t ipl = 0; ipl < kNplanes; ++ipl)
      {
        trktotalen_planes[ipl].push_back(-9999);
        trkpartialen_planes[ipl].push_back(-9999);
      }

      trkmomrange_pr.push_back(-9999);
      trkmomrange_mu.push_back(-9999);
      trkmomllhd.push_back(-9999);
      trkmomchi2.push_back(-9999);
      selected_pr.push_back(false); 
      selected_pi.push_back(false); 
    }
    else // so we have tracks
    {
      auto track = tmpTracks.at(0);
      trkstartx.push_back(track->Vertex<TVector3>().X());
      trkstarty.push_back(track->Vertex<TVector3>().Y());
      trkstartz.push_back(track->Vertex<TVector3>().Z());
      trkendx  .push_back(track->End<TVector3>().X());
      trkendy  .push_back(track->End<TVector3>().Y());
      trkendz  .push_back(track->End<TVector3>().Z());
      trkstartdcosx.push_back(track->VertexDirection<TVector3>().X());
      trkstartdcosy.push_back(track->VertexDirection<TVector3>().Y());
      trkstartdcosz.push_back(track->VertexDirection<TVector3>().Z());

      trklen   .push_back(track->Length());

      if (keys_of_protons.count(track.key())){
        selected_pr.push_back(true);
      }
      else{
        selected_pr.push_back(false);
      }

      if (keys_of_pions.count(track.key())){
        selected_pi.push_back(true);
      }
      else{
        selected_pi.push_back(false);
      }

      if (nutype == NuType::Numu){
        auto tlep = psel->GetLepTrack();
        if (tlep.isAvailable())
        {
          if (track.key() == tlep.key())
            foundlep = true;
        }
      }


      std::vector< art::Ptr<anab::Calorimetry> > calos = GetAssocProductVector< anab::Calorimetry >(track, evt, "pandoraTrack", "pandoracalo");

      std::vector<float> totalenergy(kNplanes);
      std::vector<float> particalenergy(kNplanes);
      for (auto calo: calos)
      {
        auto theplane = calo->PlaneID().Plane;
        const std::vector< float > dEdx = calo->dEdx();
        const std::vector< float > ResidualRange = calo->ResidualRange();
        const std::vector< float > TrkPitchVec = calo->TrkPitchVec() ;
        // if (ResidualRange.size() > 0 && ResidualRange[0] > ResidualRange.back()){
        //   throw std::invalid_argument("This shouldn't happen");
        // }
        for (size_t irr = 0; irr < ResidualRange.size(); ++irr){
          if (ResidualRange[irr] <= 10){
            particalenergy[theplane] += dEdx[irr]*TrkPitchVec[irr];
          }
          totalenergy[theplane] += dEdx[irr]*TrkPitchVec[irr];
        }
      }
      for (size_t ipl = 0; ipl < kNplanes; ++ipl)
      {
        trktotalen_planes[ipl].push_back(totalenergy[ipl]);
        trkpartialen_planes[ipl].push_back(particalenergy[ipl]);

      }
      trkmomrange_pr.push_back(trkmrange.GetTrackMomentum(track->Length(),2212));
      trkmomrange_mu.push_back(trkmrange.GetTrackMomentum(track->Length(),13));
      trkmomllhd.push_back(trkmllhd.GetMomentumMultiScatterLLHD(track, true, 100e3, 2, 0, true, 0.757));
      trkmomchi2.push_back(trkmllhd.GetMomentumMultiScatterChi2(track, true, 100e3, 2, 0));

      auto itrack = track.key();

      if(fmpid->isValid()) {
        std::vector< art::Ptr<anab::ParticleID>> pids = fmpid->at(itrack);

        for (size_t ipid = 0; ipid < pids.size(); ++ipid){
          if (!pids[ipid]->PlaneID().isValid) continue;
          int planenum = pids[ipid]->PlaneID().Plane;
          if (planenum<0||planenum>2) continue;

          auto pidScore = pids[ipid]->ParticleIDAlgScores();
          for(auto pScore: pidScore){
            double chi2value = pScore.fValue;

            // PIDA is always the last one and ndf there is -9999
            switch(pScore.fAssumedPdg){
              case 2212:
                trkpidchi2pr[planenum][ipfp] = chi2value;
                break;
              case 321:
                trkpidchi2ka[planenum][ipfp] = chi2value;
                break;
              case 211:
                trkpidchi2pi[planenum][ipfp] = chi2value;
                break;
              case 13:
                trkpidchi2mu[planenum][ipfp] = chi2value;
                trkpidchi2ndf[planenum][ipfp] = pScore.fNdf;
                break;
              case 0:
                trkpidpida[planenum][ipfp] = chi2value;
                trkpidndf[planenum][ipfp] = pScore.fNdf; // This value is the same for each particle type, but different in each plane
                break;
            }
          }
        }
      } // fmpid->isValid()


    } // if there are tracks


    std::vector<art::Ptr<recob::Shower>> tmpShowers = GetAssocProductVector<recob::Shower>(pfp, evt, fPFParticleModuleLabel, fShowerModuleLabel);
    if (tmpShowers.empty()){ // noope..
      trkshwstartx.push_back(-9999);
      trkshwstarty.push_back(-9999);
      trkshwstartz.push_back(-9999);
      trkshwlen   .push_back(-9999);
    }
    else // so we have showers
    {
      auto shower = tmpShowers.at(0);
      trkshwstartx.push_back(shower->ShowerStart().X());
      trkshwstarty.push_back(shower->ShowerStart().Y());
      trkshwstartz.push_back(shower->ShowerStart().Z());
      trkshwlen   .push_back(shower->Length());
      if (nutype == NuType::Nue){
        auto tlep = psel->GetLepShower();
        if (tlep.isAvailable())
        {
          if (shower.key() == tlep.key())
            foundlep = true;
        }
      }
    } // if there are showers

    // Getting PFP
    bool istrack = false;
    Float_t score = -1;
    if (pfp->PdgCode() == 13 && !tmpTracks.empty()) istrack = true;

    art::FindManyP<larpandoraobj::PFParticleMetadata> theMetadataAssns(PFPs, evt, fPFParticleModuleLabel);
    if (theMetadataAssns.isValid()){
      if (theMetadataAssns.at(pfp->Self()).size() > 0)
      {
        const larpandoraobj::PFParticleMetadata metaData = *((theMetadataAssns.at(pfp->Self())).at(0));
        const std::map<std::string, float> &propMap = metaData.GetPropertiesMap();
        if(propMap.count("TrackScore") > 0){
          score = propMap.at("TrackScore");
        }
      }
    }

    trkPFPIsTrack.push_back(istrack);
    trkPFPScoreIsTrack.push_back(score);
    selected_lep.push_back(foundlep);

  }


  art::Ptr< recob::PFParticle > nupfp;
  for (art::Ptr< recob::PFParticle > pfp: PFPs){
    const int pdg = pfp->PdgCode();
    if ((std::abs(pdg) == 12) || (std::abs(pdg) == 14) || (std::abs(pdg) ==16)){
      nupfp = pfp;
    }
  }
  art::Ptr<recob::Vertex> vertex;
  if(fmvtx->isValid()) {
    vertex = fmvtx->at(nupfp.key()).at(0);
  }
  else{
    mf::LogError("LARANA") << " Find many for vetex failed... " << std::endl;
  }

  double xyz[3] = {0.0, 0.0, 0.0} ;
  vertex->XYZ(xyz);
  recovtxx = xyz[0];
  recovtxy = xyz[1];
  recovtxz = xyz[2];

  fTree->Fill();

  reset();
}

void pid::PIDASelectionAna::beginJob()
{
  // Implementation of optional member function here.

  art::ServiceHandle<art::TFileService> tfs;
  fTree = tfs->make<TTree>("t1", "pida analysis tree");

    fTree->Branch("run", &run);
    fTree->Branch("subrun", &subrun);
    fTree->Branch("event", &event);

    fTree->Branch("nuPDG_truth", &nuPDG_truth);
    fTree->Branch("ccnc_truth", &ccnc_truth);
    fTree->Branch("mode_truth", &mode_truth);
    fTree->Branch("nuWeight_truth", &nuWeight_truth);
    fTree->Branch("enu_truth", &enu_truth);
    fTree->Branch("enuvis_truth", &enuvis_truth);
    fTree->Branch("hadvis_truth", &hadvis_truth);
    fTree->Branch("IsContainedTrue", &IsContainedTrue);

    fTree->Branch("vtxx_truth", &vtxx_truth);
    fTree->Branch("vtxy_truth", &vtxy_truth);
    fTree->Branch("vtxz_truth", &vtxz_truth);
    fTree->Branch("nu_dcosx_truth", &nu_dcosx_truth);
    fTree->Branch("nu_dcosy_truth", &nu_dcosy_truth);
    fTree->Branch("nu_dcosz_truth", &nu_dcosz_truth);

    fTree->Branch("recovtxx", &recovtxx);
    fTree->Branch("recovtxy", &recovtxy);
    fTree->Branch("recovtxz", &recovtxz);

    fTree->Branch("lepen", &lepen);
    fTree->Branch("lepmass", &lepmass);
    fTree->Branch("leppdg", &leppdg);
    fTree->Branch("lepid", &lepid);

    fTree->Branch("Ev_reco_nue", &Ev_reco_nue);
    fTree->Branch("RecoLepEnNue", &RecoLepEnNue);
    fTree->Branch("RecoHadEnNue", &RecoHadEnNue);
    fTree->Branch("RecoMethodNue", &RecoMethodNue);

    fTree->Branch("Ev_reco_numu", &Ev_reco_numu);
    fTree->Branch("RecoLepEnNumu", &RecoLepEnNumu);
    fTree->Branch("RecoHadEnNumu", &RecoHadEnNumu);
    fTree->Branch("RecoMethodNumu", &RecoMethodNumu);
    fTree->Branch("LongestTrackContNumu", &LongestTrackContNumu);
    fTree->Branch("TrackMomMethodNumu", &TrackMomMethodNumu);

    // Not actually trk, pfp instead, but that is fine :)
    fTree->Branch("ntracks", &ntracks);

    fTree->Branch("trkId", &trkId);
    fTree->Branch("trkstartx", &trkstartx);
    fTree->Branch("trkstarty", &trkstarty);
    fTree->Branch("trkstartz", &trkstartz);
    // fTree->Branch("trkendx", &trkendx);
    // fTree->Branch("trkendy", &trkendy);
    // fTree->Branch("trkendz", &trkendz);
    fTree->Branch("trkstartdcosx", &trkstartdcosx);
    fTree->Branch("trkstartdcosy", &trkstartdcosy);
    fTree->Branch("trkstartdcosz", &trkstartdcosz);
    fTree->Branch("trkpurity", &trkpurity);
    fTree->Branch("trkcompleteness", &trkcompleteness);
    fTree->Branch("trkg4id", &trkg4id);
    fTree->Branch("trkg4pdg", &trkg4pdg);
    fTree->Branch("trkg4en", &trkg4en);
    fTree->Branch("trkg4mass", &trkg4mass);
    fTree->Branch("trkg4mother", &trkg4mother);

    fTree->Branch("trknhits", &trknhits);
    fTree->Branch("trknhitsmatch", &trknhitsmatch);
    fTree->Branch("trkg4id_planes", &trkg4id_planes);
    fTree->Branch("trkg4pdg_planes", &trkg4pdg_planes);
    fTree->Branch("trkg4en_planes", &trkg4en_planes);
    fTree->Branch("trkg4mass_planes", &trkg4mass_planes);
    fTree->Branch("trkg4mother_planes", &trkg4mother_planes);
    fTree->Branch("trkpurity_planes", &trkpurity_planes);
    fTree->Branch("trkcompleteness_planes", &trkcompleteness_planes);
    fTree->Branch("trknhits_planes", &trknhits_planes);
    fTree->Branch("trknhitsmatch_planes", &trknhitsmatch_planes);
    fTree->Branch("trkbestplane", &trkbestplane);
    fTree->Branch("trklen", &trklen);
    fTree->Branch("trkmomrange_pr", &trkmomrange_pr);
    fTree->Branch("trkmomrange_mu", &trkmomrange_mu);
    fTree->Branch("trkmomllhd", &trkmomllhd);
    fTree->Branch("trkmomchi2", &trkmomchi2);
    fTree->Branch("trkcalo_planes", &trkcalo_planes);
    // fTree->Branch("trktotalen_planes", &trktotalen_planes);
    // fTree->Branch("trkpartialen_planes", &trkpartialen_planes);
    fTree->Branch("trkIsContained", &trkIsContained);
    fTree->Branch("trkIsContainedTrue", &trkIsContainedTrue);
    fTree->Branch("trkPFPIsTrack", &trkPFPIsTrack);
    fTree->Branch("selected_lep", &selected_lep);
    fTree->Branch("selected_pr", &selected_pr);
    fTree->Branch("selected_pi", &selected_pi);
    fTree->Branch("trkPFPScoreIsTrack", &trkPFPScoreIsTrack);
    // fTree->Branch("trkshwstartx", &trkshwstartx);
    // fTree->Branch("trkshwstarty", &trkshwstarty);
    // fTree->Branch("trkshwstartz", &trkshwstartz);
    fTree->Branch("trkshwlen", &trkshwlen);

    fTree->Branch("allcalo_planes", &allcalo_planes);
    fTree->Branch("nallhits_planes", &nallhits_planes);

    // fTree->Branch("trkpidchi2ndf", &trkpidchi2ndf);
    // fTree->Branch("trkpidchi2pr", &trkpidchi2pr);
    // fTree->Branch("trkpidchi2ka", &trkpidchi2ka);
    // fTree->Branch("trkpidchi2pi", &trkpidchi2pi);
    // fTree->Branch("trkpidchi2mu", &trkpidchi2mu);
    fTree->Branch("trkpidndf", &trkpidndf);
    fTree->Branch("trkpidpida", &trkpidpida);

}

void pid::PIDASelectionAna::endJob()
{
  // Implementation of optional member function here.
}

template <typename T> std::vector<art::Ptr<T>> pid::PIDASelectionAna::GetProductVector(const art::Event &evt, const std::string &label){
  auto theseProds = evt.getHandle<std::vector<T>>(label);
  bool success = theseProds.isValid();

  if (!success)
  {
    mf::LogError("LARANA") << " Failed to find product with label " << label << " ... returning empty vector" << std::endl;
    return std::vector<art::Ptr<T>>();
  }

  // We need to convert these to art pointers
  std::vector<art::Ptr<T>> productVector;
  art::fill_ptr_vector(productVector,theseProds);

  return productVector;
}

template <typename T, typename U> std::vector<art::Ptr<T>> pid::PIDASelectionAna::GetAssocProductVector(const art::Ptr<U> &pProd, const art::Event &evt, const std::string &label, const std::string &assocLabel)
{
    auto products = evt.getHandle<std::vector<U>>(label);
    bool success = products.isValid();

    if (!success)
    {
        mf::LogError("LARANA") << " Failed to find product with label " << label << " ... returning empty vector" << std::endl;
        return std::vector<art::Ptr<T>>();
    }

    const art::FindManyP<T> findParticleAssocs(products,evt,assocLabel);

    return findParticleAssocs.at(pProd.key());
}

template <typename T, typename U> art::Ptr<T> pid::PIDASelectionAna::GetAssocProduct(const art::Ptr<U> &pProd, const art::Event &evt, const std::string &label, const std::string &assocLabel)
{
  std::vector<art::Ptr<T>> associatedProducts = pid::PIDASelectionAna::GetAssocProductVector<T>(pProd,evt,label,assocLabel);
  if (associatedProducts.empty())
  {
    mf::LogError("LARANA") << "DUNEAnaUtilsBase::GetAssocProduct --- No associated object found";
  }
  return associatedProducts.at(0);
}
void pid::PIDASelectionAna::reset(){
    run = -9999;
    subrun = -9999;
    event = -9999;

    nuPDG_truth = -9999;
    ccnc_truth = -9999;
    mode_truth = -9999;
    nuWeight_truth = -9999;
    enu_truth = -9999;
    enuvis_truth = -9999;
    hadvis_truth = -9999;
    IsContainedTrue = true;

    vtxx_truth = -9999;
    vtxy_truth = -9999;
    vtxz_truth = -9999;
    nu_dcosx_truth = -9999;
    nu_dcosy_truth = -9999;
    nu_dcosz_truth = -9999;

    recovtxx = -9999;
    recovtxy = -9999;
    recovtxz = -9999;


    lepen = -9999;
    lepmass = -9999;
    leppdg = -9999;
    lepid = -9999;

    // Reconstructed neutrino energy
    Ev_reco_nue = -9999.;
    RecoLepEnNue = -9999.;
    RecoHadEnNue = -9999.;
    RecoMethodNue = -9999;

    Ev_reco_numu = -9999.;
    RecoLepEnNumu = -9999.;
    RecoHadEnNumu = -9999.;
    RecoMethodNumu = -9999;
    LongestTrackContNumu = -9999;
    TrackMomMethodNumu = -9999;

    Ev_reco_nc = -99999.;

    ntracks = -9999;

    trkId.clear();
    trkstartx.clear();
    trkstarty.clear();
    trkstartz.clear();
    trkendx.clear();
    trkendy.clear();
    trkendz.clear();
    trkstartdcosx.clear();
    trkstartdcosy.clear();
    trkstartdcosz.clear();
    trkpurity.clear();
    trkcompleteness.clear();
    trkg4id.clear();
    trkg4pdg.clear();
    trkg4en.clear();
    trkg4mass.clear();
    trkg4mother.clear();

    trknhits.clear();
    trknhitsmatch.clear();
    trkg4id_planes.clear();
    trkg4pdg_planes.clear();
    trkg4en_planes.clear();
    trkg4mass_planes.clear();
    trkg4mother_planes.clear();
    trkpurity_planes.clear();
    trkcompleteness_planes.clear();
    trknhits_planes.clear();
    trknhitsmatch_planes.clear();
    trkbestplane.clear();
    trklen.clear();
    trkmomrange_pr.clear();
    trkmomrange_mu.clear();
    trkmomllhd.clear();
    trkmomchi2.clear();
    trkcalo_planes.clear();
    trktotalen_planes.clear();
    trkpartialen_planes.clear();
    trkIsContained.clear();
    trkIsContainedTrue.clear();
    trkPFPIsTrack.clear();
    selected_lep.clear();
    selected_pr.clear();
    selected_pi.clear();
    trkPFPScoreIsTrack.clear();
    trkshwstartx.clear();
    trkshwstarty.clear();
    trkshwstartz.clear();
    trkshwlen.clear();
    allcalo_planes.clear();
    nallhits_planes.clear();

    trkpidndf.clear();
    trkpidchi2ndf.clear();
    trkpidchi2pr.clear();
    trkpidchi2ka.clear();
    trkpidchi2pi.clear();
    trkpidchi2mu.clear();
    trkpidpida.clear();

}

void pid::PIDASelectionAna::HitsPurity(detinfo::DetectorClocksData const& clockData,
    std::vector< art::Ptr<recob::Hit> > const& hits,
    std::vector<Int_t>& trackid,
    std::vector<Float_t>& purity,
    std::vector<Float_t>& completeness,
    std::map<Int_t,Int_t> HitsToMCCounts
    )
{
  Int_t ktrackid = -1;
  Float_t kpurity = 0;
  Float_t kcompleteness = 0;
  correct_hits = 0.f; // (for complenetess) Count number of hits from the MCParticle with g4ID

  TruthMatchUtils::G4ID g4ID(TruthMatchUtils::TrueParticleIDFromTotalRecoHits(clockData, hits,fRollUpUnsavedIDs));

  if (TruthMatchUtils::Valid(g4ID)){
    ktrackid = g4ID;


    // Compute purity using TruthMatchUtils
    for(size_t i = 0; i < hits.size(); ++i)
    {
      TruthMatchUtils::G4ID hitID(TruthMatchUtils::TrueParticleID(clockData, hits.at(i), fRollUpUnsavedIDs));
      if (hitID == g4ID)
      {
        kpurity+=1.f;
        correct_hits+=1.f;
      }
    }
    if (hits.size() > 0)
      kpurity /= hits.size();

    // Compute completeness using TruthMatchUtils counts
    auto allhitstruth = HitsToMCCounts.find(g4ID)->second;
    kcompleteness = correct_hits/allhitstruth;
  }
  trackid.push_back(ktrackid);
  purity.push_back(kpurity);
  completeness.push_back(kcompleteness);
  return;
}



bool pid::PIDASelectionAna::IsContained(const std::vector<art::Ptr<recob::Hit> > &hits, const art::Event &event)
{
    for (unsigned int iHit = 0; iHit < hits.size(); ++iHit)
    {
        std::vector<art::Ptr<recob::SpacePoint> > spacePoints;

        if(fmsp->isValid()) {
          spacePoints = fmsp->at(hits[iHit].key());
        }
        for (unsigned int iSpacePoint = 0; iSpacePoint < spacePoints.size(); ++iSpacePoint)
        {
            const art::Ptr<recob::SpacePoint> spacePoint(spacePoints[iSpacePoint]);
            if (!(this->IsPointContained(spacePoint->XYZ()[0],spacePoint->XYZ()[1],spacePoint->XYZ()[2])))
                return false;
        }
    }
    return true;
}

bool pid::PIDASelectionAna::IsPointContained(const double x, const double y, const double z)
{
  geo::Point_t const position{x,y,z};
  art::ServiceHandle<geo::Geometry> fGeometry;

  geo::TPCID tpcID(fGeometry->FindTPCAtPosition(position));
  if (!(fGeometry->HasTPC(tpcID)))
    return false;

  double minX(std::numeric_limits<double>::max());
  double maxX(std::numeric_limits<double>::lowest());
  double minY(std::numeric_limits<double>::max());
  double maxY(std::numeric_limits<double>::lowest());
  double minZ(std::numeric_limits<double>::max());
  double maxZ(std::numeric_limits<double>::lowest());
  for (auto const& tpc : fGeometry->Iterate<geo::TPCGeo>())
  {
    minX = std::min(minX,tpc.MinX());
    maxX = std::max(maxX,tpc.MaxX());
    minY = std::min(minY,tpc.MinY());
    maxY = std::max(maxY,tpc.MaxY());
    minZ = std::min(minZ,tpc.MinZ());
    maxZ = std::max(maxZ,tpc.MaxZ());
  }

  minX += fDistanceToWallThreshold;
  maxX -= fDistanceToWallThreshold;
  minY += fDistanceToWallThreshold;
  maxY -= fDistanceToWallThreshold;
  minZ += fDistanceToWallThreshold;
  maxZ -= fDistanceToWallThreshold;

  if (x - minX < -1.*std::numeric_limits<double>::epsilon() ||
      x - maxX > std::numeric_limits<double>::epsilon())
    return false;
  if (y - minY < -1.*std::numeric_limits<double>::epsilon() ||
      y - maxY > std::numeric_limits<double>::epsilon())
    return false;
  if (z - minZ < -1.*std::numeric_limits<double>::epsilon() ||
      z - maxZ > std::numeric_limits<double>::epsilon())
    return false;

  return true;
}



void pid::PIDASelectionAna::computeEnergies(
    detinfo::DetectorClocksData const & clockData,
    detinfo::DetectorPropertiesData const & detProp,
    const std::vector<std::vector< art::Ptr<recob::Hit> > > hits,
    std::vector< std::vector< Float_t > > &caloEnergies
    )
{
  for (size_t ipl = 0; ipl < kNplanes; ++ipl){
    double caloCharge(LifetimeCorrectedTotalHitCharge(clockData, detProp, hits[ipl]));
    double caloEnergy(CalculateEnergyFromCharge(caloCharge, ipl));
    caloEnergies[ipl].push_back(caloEnergy);
  }
  return;
}

void pid::PIDASelectionAna::computeEnergies(
    detinfo::DetectorClocksData const & clockData,
    detinfo::DetectorPropertiesData const & detProp,
    const std::vector<std::vector< art::Ptr<recob::Hit> > > hits,
    std::vector< Float_t > &caloEnergies
    )
{
  for (size_t ipl = 0; ipl < kNplanes; ++ipl){
    double caloCharge(LifetimeCorrectedTotalHitCharge(clockData, detProp, hits[ipl]));
    double caloEnergy(CalculateEnergyFromCharge(caloCharge, ipl));
    caloEnergies[ipl] = caloEnergy;
  }
  return;
}


double pid::PIDASelectionAna::LifetimeCorrection(detinfo::DetectorClocksData const& clockData,
                                           detinfo::DetectorPropertiesData const& detProp,
                                           const double timeInTicks, const double t0InMicroS)
{
    const double tpcSamplingRateInMicroS(clockData.TPCClock().TickPeriod());
    const double tpcTriggerOffsetInTicks(trigger_offset(clockData));

    const double timeCorrectedForT0((timeInTicks - tpcTriggerOffsetInTicks)*tpcSamplingRateInMicroS - t0InMicroS);
    const double tauLifetime(detProp.ElectronLifetime());

    return std::exp(timeCorrectedForT0/tauLifetime);
}

double pid::PIDASelectionAna::LifetimeCorrectedTotalHitCharge(
    detinfo::DetectorClocksData const & clockData,
    detinfo::DetectorPropertiesData const & detProp,
    const std::vector< art::Ptr< recob::Hit > > & hits
    )
{
  double totalHitCharge(0);
  for (unsigned int iHit = 0; iHit < hits.size(); iHit++)
    totalHitCharge += hits[iHit]->Integral() * LifetimeCorrection(clockData, detProp, hits[iHit]->PeakTime(), clockData.TriggerTime());

  return totalHitCharge;
}


double pid::PIDASelectionAna::CalculateEnergyFromCharge(const double charge, const unsigned short plane )
{
  const double fRecombFactor(0.63);
  return fCalorimetryAlg.ElectronsFromADCArea(charge, plane)*1./fRecombFactor/util::kGeVToElectrons;
}
bool pid::PIDASelectionAna::isEnergyDepositedOutsideActiveVolume(const sim::SimEnergyDeposit &edep){
  double start[3] = {edep.StartX(), edep.StartY(), edep.StartZ()};
  double end[3] = {edep.EndX(), edep.EndY(), edep.EndZ()};
  double margin = 5;
  if(start[0] - margin < fActiveBounds[0] || start[0] + margin > fActiveBounds[1] || start[1] - margin < fActiveBounds[2] || start[1] + margin > fActiveBounds[3] || start[2] - margin < fActiveBounds[4] || start[2] + margin > fActiveBounds[5]){
    return true;
  }
  if(end[0] - margin < fActiveBounds[0] || end[0] + margin > fActiveBounds[1] || end[1] - margin < fActiveBounds[2] || end[1] + margin > fActiveBounds[3] || end[2] - margin < fActiveBounds[4] || end[2] + margin > fActiveBounds[5]){
    return true;
  }
  return false;
}

std::vector<double> pid::PIDASelectionAna::getActiveBounds(){
  art::ServiceHandle<geo::Geometry> fGeom;
  double minx = 99999;
  double maxx = -99999;
  double miny = 99999;
  double maxy = -99999;
  double minz = 99999;
  double maxz = -99999;

  std::vector<double> bounds = {minx, maxx, miny, maxy, minz, maxz};

   for (geo::TPCGeo const& TPC: fGeom->Iterate<geo::TPCGeo>()) {
    // get center in world coordinates
    auto const center = TPC.GetCenter();
    double tpcDim[3] = {TPC.HalfWidth(), TPC.HalfHeight(), 0.5*TPC.Length() };

    if( center.X() - tpcDim[0] < bounds[0] ) bounds[0] = center.X() - tpcDim[0];
    if( center.X() + tpcDim[0] > bounds[1] ) bounds[1] = center.X() + tpcDim[0];
    if( center.Y() - tpcDim[1] < bounds[2] ) bounds[2] = center.Y() - tpcDim[1];
    if( center.Y() + tpcDim[1] > bounds[3] ) bounds[3] = center.Y() + tpcDim[1];
    if( center.Z() - tpcDim[2] < bounds[4] ) bounds[4] = center.Z() - tpcDim[2];
    if( center.Z() + tpcDim[2] > bounds[5] ) bounds[5] = center.Z() + tpcDim[2];
  } // for all TPC

  //Tweak the bounds on the y axis as there is an extra on non-instrumented 8cm on each side...
  bounds[2] += 8;
  bounds[3] -= 8;

  return bounds;
}

std::map<int, std::tuple<double, bool, std::string>>  pid::PIDASelectionAna::EnergyDepositSummary(const art::Event& evt){
  std::map<int, std::tuple<double, bool, std::string>>  summary;
  art::ServiceHandle<cheat::ParticleInventoryService> pi_serv;
  const sim::ParticleList& plist = pi_serv->ParticleList();
  std::string fEdepLabel = "largeant:LArG4DetectorServicevolTPCActiveInner";
  auto theseProds = evt.getHandle<std::vector<sim::SimEnergyDeposit>>(fEdepLabel);
  bool success = theseProds.isValid();

  if (!success)
  {
      mf::LogError("LARana") << " Failed to find product with label " << fEdepLabel << " ... returning empty vector" << std::endl;
      return summary;
  }

  // We need to convert these to art pointers
  std::vector<art::Ptr<sim::SimEnergyDeposit>> edeps;
  art::fill_ptr_vector(edeps,theseProds);

  for(const art::Ptr<sim::SimEnergyDeposit> &edep : edeps){
    int g4id = std::abs(edep->TrackID());
    const simb::MCParticle* pPart = plist.at(g4id);

    //Getting the associated primary particle
    while (!plist.IsPrimary(g4id)){
      g4id = pPart->Mother();
      pPart = plist.at(g4id);
    }


    if(summary.count(g4id)){
      // summary.at(g4id) += edep->Energy();
      std::get<0>(summary[g4id]) += edep->Energy();
      std::get<1>(summary[g4id]) &= !isEnergyDepositedOutsideActiveVolume(*edep);
    }
    else{
      summary[g4id] = std::make_tuple(edep->Energy(), !isEnergyDepositedOutsideActiveVolume(*edep), pPart->EndProcess());
    }
  }

  return summary;
}

namespace pid {
  DEFINE_ART_MODULE(PIDASelectionAna)
}
