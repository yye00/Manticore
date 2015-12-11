#ifndef KLINKENBERGPRESSURE_H
#define KLINKENBERGPRESSURE_H

#include "Kernel.h"

class KlinkenbergPressure;

template<>
InputParameters validParams<KlinkenbergPressure>();

/**
 * Represents K/mu * grad_u * grad_phi
 *
 * We are inheriting from Diffusion instead of from Kernel because
 * the grad_u * grad_phi is already coded in there and all we
 * need to do is specialize that calculation by multiplying by K/mu
 */
class KlinkenbergPressure : public Kernel
{
public:
  KlinkenbergPressure(const InputParameters & parameters);
  virtual ~KlinkenbergPressure();

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

#endif /* KLINKENBERG_H */
