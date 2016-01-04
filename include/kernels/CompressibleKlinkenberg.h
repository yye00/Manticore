#ifndef COMPRESSIBLEKLINKENBERG_H
#define COMPRESSIBLEKLINKENBERG_H

#include "Kernel.h"

#include "RichardsDensity.h"


class CompressibleKlinkenberg;

template<>
InputParameters validParams<CompressibleKlinkenberg>();

/**
 * Represents K/mu * grad_u * grad_phi
 *
 * We are inheriting from Diffusion instead of from Kernel because
 * the grad_u * grad_phi is already coded in there and all we
 * need to do is specialize that calculation by multiplying by K/mu
 */
class CompressibleKlinkenberg : public Kernel
{
public:
  CompressibleKlinkenberg(const InputParameters & parameters);
  virtual ~CompressibleKlinkenberg();

protected:
  /**
   * Kernels _must_ override computeQpResidual()
   */
  virtual Real computeQpResidual();

  /**
   * This is optional (but recommended!)
   */
  virtual Real computeQpJacobian();

  // We will re-use the Richard's desntiy from the Richard's module
  const RichardsDensity & _density;

  // Gravity vector, for example: (0 0 -9.81 m^2/sec)
  RealVectorValue _gravity_vector;

  // fluid dynamic viscosity
  Real _fluid_viscosity;

  // Klinkenberg factor b
  Real _klinkenberg_factor;

  // Material permeability
  const MaterialProperty<RealTensorValue> &_permeability;
};

#endif /* COMPRESSIBLEKLINKENBERG_H */
