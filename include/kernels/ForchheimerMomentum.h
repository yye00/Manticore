#ifndef FORCHHEIMER_H
#define FORCHHEIMER_H

#include "Kernel.h"

class ForchheimerPressure;
class ForchheimerVelocity;

template<>
InputParameters validParams<ForchheimerPressure>();

template<>
InputParameters validParams<ForchheimerVelocity>();


class ForchheimerPressure : public Kernel
{
public:
  ForchheimerPressure(const InputParameters & parameters);
  virtual ~ForchheimerPressure();

protected:
  /**
   * Kernels _must_ override computeQpResidual()
   */
  virtual Real computeQpResidual();

  /**
   * This is optional (but recommended!)
   */
  virtual Real computeQpJacobian();

  /// fluid weight (gravity*density) as a vector pointing downwards, eg '0 0 -10000'
  RealVectorValue _fluid_weight;

  /// fluid dynamic viscosity
  Real _fluid_viscosity;

  /// Klinkenberg factor b
  Real _klinkenberg_factor;

  /// Material permeability
  const MaterialProperty<RealTensorValue> &_permeability;
};

class ForchheimerVelocity : public Kernel
{
public:
  ForchheimerVelocity(const InputParameters & parameters);
  virtual ~ForchheimerVelocity();

protected:
  /**
   * Kernels _must_ override computeQpResidual()
   */
  virtual Real computeQpResidual();

  /**
   * This is optional (but recommended!)
   */
  virtual Real computeQpJacobian();

  /// fluid weight (gravity*density) as a vector pointing downwards, eg '0 0 -10000'
  RealVectorValue _fluid_weight;

  /// fluid dynamic viscosity
  Real _fluid_viscosity;

  /// Klinkenberg factor b
  Real _klinkenberg_factor;

  /// Material permeability
  const MaterialProperty<RealTensorValue> &_permeability;
};

#endif /* FORCHHEIMER_H */
