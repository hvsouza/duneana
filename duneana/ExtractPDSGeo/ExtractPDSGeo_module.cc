////////////////////////////////////////////////////////////////////////
// Class:       ExtractPDSGeo
// Plugin Type: analyzer (Unknown Unknown)
// File:        ExtractPDSGeo_module.cc
//
// Generated at Wed Oct 14 08:09:36 2026 by Henrique Vieira de Souza using cetskelgen
// from cetlib version 3.18.02.
////////////////////////////////////////////////////////////////////////

#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "fhiclcpp/ParameterSet.h"
#include "messagefacility/MessageLogger/MessageLogger.h"
#include "larcore/Geometry/Geometry.h"
#include "larcorealg/Geometry/OpDetGeo.h"

namespace dune {
  class ExtractPDSGeo;
}


class dune::ExtractPDSGeo : public art::EDAnalyzer {
public:
  explicit ExtractPDSGeo(fhicl::ParameterSet const& p);
  // The compiler-generated destructor is fine for non-base
  // classes without bare pointers or other resource use.

  // Plugins should not be copied or assigned.
  ExtractPDSGeo(ExtractPDSGeo const&) = delete;
  ExtractPDSGeo(ExtractPDSGeo&&) = delete;
  ExtractPDSGeo& operator=(ExtractPDSGeo const&) = delete;
  ExtractPDSGeo& operator=(ExtractPDSGeo&&) = delete;

  // Required functions.
  void analyze(art::Event const& e) override;

  // Selected optional functions.
  void beginJob() override;
  void endJob() override;

private:

  // Declare member data here.

};


dune::ExtractPDSGeo::ExtractPDSGeo(fhicl::ParameterSet const& p)
  : EDAnalyzer{p}  // ,
  // More initializers here.
{
  // Call appropriate consumes<>() for any products to be retrieved by this module.
}

void dune::ExtractPDSGeo::analyze(art::Event const& e)
{
    art::ServiceHandle<geo::Geometry const> geo;

    std::cout << "Optical Channels positions:  " << geo->Cryostat().NOpDet() << std::endl;
    for (int ch = 0; ch != int(geo->Cryostat().NOpDet()); ch++) {
        auto const OpDetCenter = geo->OpDetGeoFromOpDet(ch).GetCenter();
        std::cout << ch << "  " << OpDetCenter.X() << "  " << OpDetCenter.Y() << "  "
            << OpDetCenter.Z() << std::endl;
    }
}

void dune::ExtractPDSGeo::beginJob()
{
  // Implementation of optional member function here.
}

void dune::ExtractPDSGeo::endJob()
{
  // Implementation of optional member function here.
}

DEFINE_ART_MODULE(dune::ExtractPDSGeo)
