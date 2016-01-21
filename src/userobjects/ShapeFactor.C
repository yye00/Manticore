//  Fluid density base class.
//
#include "ShapeFactor.h"

template<>
InputParameters validParams<ShapeFactor>()
{
  InputParameters params = validParams<GeneralUserObject>();
  params.addClassDescription("User Object that computes the shape factor from the geometry");
  return params;
}

ShapeFactor::ShapeFactor(const InputParameters & parameters) :
    GeneralUserObject(parameters)
{}

void
ShapeFactor::initialize()
{}

void
ShapeFactor::execute()
{}

void
ShapeFactor::finalize()
{}

Real
ShapeFactor::Factor(const Elem * elem) const
{
  // Get the dimension
  unsigned dim = elem->dim();

  // Get the volume
  Real const vol = elem->volume();

  // Get the surface area
  Real surface_area=0;
  for (unsigned int s=0; s<elem->n_sides(); s++)
    surface_area += elem->build_side(s)->volume();

  return 5.0*surface_area/vol/std::pow(vol,1.0/dim);
}
