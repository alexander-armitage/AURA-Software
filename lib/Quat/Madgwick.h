#pragma once

#include "Quat.h"

namespace ori {

class madgwick {
 private:
  float _beta{};

  float _jacobian_t[4][6];
  float _cost[6];

  const Quat _ref_mag;
  const Quat _ref_acc;

  void _combine_jacobian(float jacobian_acc[3][4], float jacobian_mag[3][4]);

  void _jacobian(Quat q, Quat d, float jacobian[3][4]);

  void _combine_cost_func(Quat cost_acc, Quat cost_mag);

  [[nodiscard]] const Quat _cost_func(Quat q, Quat d, Quat s);

  [[nodiscard]] const Quat _mult_jacobian_t_cost_func();

 public:
  explicit madgwick(float beta, Vect ref_mag, Vect ref_acc);

  void set_beta(float beta) { _beta = beta; }

  [[nodiscard]] const Quat update(Quat ori, Vect gyro, Vect mag, Vect acc,
                                  float dt);

  [[nodiscard]] const Quat update_acc_gyro(Quat ori, Vect gyro_v, Vect acc_v,
                                           float dt);
};

}  // namespace ori
