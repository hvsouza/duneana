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
#include <fstream>
#include <iostream>

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
    bool fSaveToFile;
    std::string fFileName;
    std::ofstream fOutFile;

};


dune::ExtractPDSGeo::ExtractPDSGeo(fhicl::ParameterSet const& p)
  : EDAnalyzer{p},
  fSaveToFile(p.get<bool>("SaveToFile")),
  fFileName(p.get<std::string>("FileName", "PDSGeo.txt"))
  // More initializers here.
{
  // Call appropriate consumes<>() for any products to be retrieved by this module.
}

void dune::ExtractPDSGeo::analyze(art::Event const&)
{
  art::ServiceHandle<geo::Geometry const> geo;

  std::ostringstream out;
  std::cout << "Optical Channels positions:  " << geo->Cryostat().NOpDet() << '\n';
  // header
  for (unsigned int ch = 0; ch != geo->Cryostat().NOpDet(); ++ch) {
    auto const c = geo->OpDetGeoFromOpDet(ch).GetCenter();
    out << ch << "  " << c.X() << "  " << c.Y() << "  " << c.Z() << '\n';
  }

  std::cout << out.str();
  if (fSaveToFile) fOutFile << out.str();
}

void dune::ExtractPDSGeo::beginJob()
{
  std::cout << "Extracting PDS geometry information..." << std::endl;
  if (fSaveToFile) {
    std::cout << "Saving to file: " << fFileName << std::endl;
    fOutFile.open(fFileName, std::ios::out | std::ios::trunc);
    if (!fOutFile.is_open()) {
      throw cet::exception("ExtractPDSGeo") << "Could not open output file " << fFileName;
    }
  }
}


void dune::ExtractPDSGeo::endJob()
{
  if (fOutFile.is_open()) fOutFile.close();
}


DEFINE_ART_MODULE(dune::ExtractPDSGeo)
