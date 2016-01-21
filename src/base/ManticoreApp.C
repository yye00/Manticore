#include "ManticoreApp.h"
#include "Moose.h"
#include "Factory.h"
#include "AppFactory.h"
#include "MooseSyntax.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"


// Kernels
#include "KlinkenbergPressure.h"
#include "ForchheimerMomentum.h"
#include "ForchheimerMass.h"
#include "CompressibleKlinkenberg.h"
#include "MatrixFractureTransfer.h"

// UserObjects
#include "ShapeFactor.h"

template<>
InputParameters validParams<ManticoreApp>()
{
  InputParameters params = validParams<MooseApp>();

  params.set<bool>("use_legacy_uo_initialization") = false;
  params.set<bool>("use_legacy_uo_aux_computation") = false;
  params.set<bool>("use_legacy_output_syntax") = false;

  return params;
}

ManticoreApp::ManticoreApp(InputParameters parameters) :
    MooseApp(parameters)
{
  Moose::registerObjects(_factory);
  ModulesApp::registerObjects(_factory);
  ManticoreApp::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
  ModulesApp::associateSyntax(_syntax, _action_factory);
  ManticoreApp::associateSyntax(_syntax, _action_factory);
}

ManticoreApp::~ManticoreApp()
{
}

// External entry point for dynamic application loading
extern "C" void ManticoreApp__registerApps() { ManticoreApp::registerApps(); }
void
ManticoreApp::registerApps()
{
  registerApp(ManticoreApp);
}

// External entry point for dynamic object registration
extern "C" void ManticoreApp__registerObjects(Factory & factory) { ManticoreApp::registerObjects(factory); }
void
ManticoreApp::registerObjects(Factory & factory)
{

  // User Objects
  registerUserObject(ShapeFactor);

  // Kernels
  registerKernel(KlinkenbergPressure);
  registerKernel(ForchheimerMomentum);
  registerKernel(ForchheimerMass);
  registerKernel(CompressibleKlinkenberg);
  registerKernel(MatrixFractureTransfer);
}

// External entry point for dynamic syntax association
extern "C" void ManticoreApp__associateSyntax(Syntax & syntax, ActionFactory & action_factory) { ManticoreApp::associateSyntax(syntax, action_factory); }
void
ManticoreApp::associateSyntax(Syntax & /*syntax*/, ActionFactory & /*action_factory*/)
{
}
