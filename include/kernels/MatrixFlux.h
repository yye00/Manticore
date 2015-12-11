#ifndef MATRIXFLUX
#define MATRIXFLUX

#include "Kernel.h"

// Forward Declarations
class MatrixFlux;

template<>
InputParameters validParams<MatrixFlux>();

/**
 * Kernel = grad(permeability*(grad(pressure) - weight))
 * This is mass flow according to the Matrix equation
 */
class MatrixFlux : public Kernel
{
public:

  MatrixFlux(const InputParameters & parameters);


protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

  /// fluid weight (gravity*density) as a vector pointing downwards, eg '0 0 -10000'
  RealVectorValue _fluid_weight;

  /// fluid dynamic viscosity
  Real _fluid_viscosity;

  /// Material permeability
  const MaterialProperty<RealTensorValue> &_permeability;
};

#endif //MATRIXFLUX
