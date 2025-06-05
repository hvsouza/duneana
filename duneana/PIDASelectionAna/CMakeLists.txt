cet_build_plugin(PIDASelectionAna art::EDAnalyzer
  LIBRARIES PRIVATE
  larana::ParticleIdentification
  larana::OpticalDetector
  lardata::AssociationUtil
  lardata::DetectorClocksService
  lardataobj::RecoBase
  art_root_io::TFileService_service
  larreco::RecoAlg
  dunereco::ParticleSelectionAlg
  dunereco::FDSensOpt_FDSensOptData
  art::Framework_Principal
  art::Framework_Services_Registry
  canvas::canvas
  ROOT::Hist
  ROOT::Tree
  nusimdata::SimulationBase
  lardataobj::RecoBase
  larsim::MCCheater_BackTrackerService_service
  larsim::MCCheater_ParticleInventoryService_service
  larsim::Utils
)

install_headers()
install_fhicl()
install_source()

