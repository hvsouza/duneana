art_make( MODULE_LIBRARIES 
	larcorealg::Geometry
	larcore::Geometry_Geometry_service
	larsim::Simulation 
	nug4::ParticleNavigation 
	lardataobj::Simulation
	duneopdet::OpticalDetector
	art::Framework_Core
	art::Framework_Principal
	art::Framework_Services_Registry
	art_root_io::tfile_support
	ROOT::Core
	art_root_io::TFileService_service
	messagefacility::MF_MessageLogger
	fhiclcpp::fhiclcpp
	cetlib::cetlib cetlib_except::cetlib_except
	ROOT::Geom
	ROOT::XMLIO
	ROOT::Gdml
	ROOT::Core ROOT::Hist ROOT::Tree
	)

install_headers()
install_fhicl()
install_source()
install_scripts()
