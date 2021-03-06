/*
  stoch_odeint_explicit_euler.hpp : stochastic Euler implimentaion header

  Copyright (C) 2015 Anup Gopalakrishna Pillai, Suhita Nadkarni Lab, IISER, Pune <anupgpillai@gmail.com>
  Copyright (C) 2015 Pranav Kulkarni, Collins Assisi Lab, IISER, Pune <pranavcode@gmail.com>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef STOCH_EULER_INTEGRATOR_HPP_INCLUDED
#define STOCH_EULER_INTEGRATOR_HPP_INCLUDED

#include <iostream>
#include <cmath>
#include <vector>

namespace stochastic {
  
  namespace euler { 
    // Euler-Murayama algorithm
    template<class state_type>  
    class explicit_euler
    {
    private:
    public:
      explicit_euler(){};
      template<class system_type>
      void do_step(system_type &system, state_type &x, state_type &n, const double t, const double dt); 
    };

    template<class state_type> 
    template<class system_type>
    void explicit_euler<state_type>::do_step(system_type &system, state_type &x, state_type &n, const double t, const double dt)
    {
      state_type d (x.size(),0.0);
      system(x,d,n,t);
      auto di = d.begin();
      auto ni = n.begin();  
      for(auto xi = x.begin(); xi < x.end(); xi++) {  
      	// *xi = (*xi) + (dt * (*di)) + ( (*xi) * (*ni));
      	*xi = (*xi) + (dt * (*di)) + ( (1) * (*ni));
      	++di; 
	++ni;
      }
    }
  } // namespace euler

    // An observer that writes the time and state vector into stdout 
  struct null_observer 
  {
    template<class state_type>
    void operator()(const state_type &x, double t) const
    {
      std::cout << t << " " << x << std::endl;
    }
  };

  // integrate function with observer object
  template<class stepper_type, class system_type, class state_type, class observer_type>
  void integrate_const(stepper_type stepper, system_type system, 
		       state_type &x, state_type &n, const double tstart, const double tend,
		       const double dt, observer_type observer)
  {
    for(double t = tstart; t <= tend; t += dt) {
      stepper.do_step(system,x,n,t,dt);
      observer(x,t);
    }
  }

  // integrate function without observer object
  template<class stepper_type, class system_type, class state_type>
  void integrate_const(stepper_type stepper, system_type system,
		       state_type &x, state_type &n, const double tstart, const double tend,
		       const double dt)
  {
    for(double t = tstart; t <= tend; t+=dt){
      stepper.do_step(system,x,n,t,dt);
    }
  }

}   // namespace stochastic

#endif
