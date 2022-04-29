/*!
 * \file CScalarVariable.cpp
 * \brief Definition of the solution fields.
 * \author F. Palacios, A. Bueno
 * \version 7.3.1 "Blackbird"
 *
 * SU2 Project Website: https://su2code.github.io
 *
 * The SU2 Project is maintained by the SU2 Foundation
 * (http://su2foundation.org)
 *
 * Copyright 2012-2022, SU2 Contributors (cf. AUTHORS.md)
 *
 * SU2 is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * SU2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with SU2. If not, see <http://www.gnu.org/licenses/>.
 */

#include "../../include/variables/CScalarVariable.hpp"

CScalarVariable::CScalarVariable(unsigned long npoint, unsigned long ndim, unsigned long nvar, const CConfig* config)
    : CVariable(npoint, ndim, nvar, config),
      Gradient_Reconstruction(config->GetReconstructionGradientRequired() ? Gradient_Aux : Gradient),
      Smatrix_Reconstruction(config->GetLeastSquaresReconstructionRequired() ? Smatrix_Aux : Smatrix),
      Primitive(config->GetBool_Turb_Model_SST() ? Primitive_Aux : Solution) {
  /*--- Gradient related fields ---*/

  Gradient.resize(nPoint, nVar, nDim, 0.0);

  if (config->GetReconstructionGradientRequired()) {
    Gradient_Aux.resize(nPoint, nVar, nDim, 0.0);
  }

  if (config->GetLeastSquaresRequired()) {
    Rmatrix.resize(nPoint, nDim, nDim, 0.0);
    Smatrix.resize(nPoint,nDim,nDim,0.0);
    if (config->GetLeastSquaresReconstructionRequired())
      Smatrix_Aux.resize(nPoint,nDim,nDim,0.0);
  }

  /*--- Always allocate the slope limiter, and the auxiliary
   variables (check the logic - JST with 2nd order Turb model) ---*/

  Limiter.resize(nPoint, nVar) = su2double(0.0);
  if (config->GetUse_Accurate_Kappa_Jacobians()) {
    LimiterDerivativeDelta.resize(nPoint,nVar) = su2double(0.0);
    LimiterDerivativeGrad.resize(nPoint,nVar) = su2double(0.0);
  }
  Solution_Max.resize(nPoint, nVar) = su2double(0.0);
  Solution_Min.resize(nPoint, nVar) = su2double(0.0);

  Delta_Time.resize(nPoint) = su2double(0.0);

  /* Under-relaxation parameter. */
  UnderRelaxation.resize(nPoint) = su2double(1.0);
  LocalCFL.resize(nPoint) = su2double(0.0);

  /* Non-physical point (first-order) initialization. */
  Non_Physical.resize(nPoint) = false;
  Non_Physical_Counter.resize(nPoint) = 0;

  /*--- Allocate space for the harmonic balance source terms ---*/
  if (config->GetTime_Marching() == TIME_MARCHING::HARMONIC_BALANCE) {
    HB_Source.resize(nPoint, nVar) = su2double(0.0);
  }
}
