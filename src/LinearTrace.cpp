/* 
* Copyright 2014-2016 Friedemann Zenke
*
* This file is part of Auryn, a simulation package for plastic
* spiking neural networks.
* 
* Auryn is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* Auryn is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with Auryn.  If not, see <http://www.gnu.org/licenses/>.
*
* If you are using Auryn or parts of it for your work please cite:
* Zenke, F. and Gerstner, W., 2014. Limits to high-speed simulations 
* of spiking neural networks using general-purpose computers. 
* Front Neuroinform 8, 76. doi: 10.3389/fninf.2014.00076
*/

#include "LinearTrace.h"

using namespace auryn;

void LinearTrace::init(NeuronID n, AurynFloat timeconstant, AurynTime * clk)
{
	size = n;
	tau = timeconstant;
	tau_auryntime = (AurynTime) (timeconstant/dt);
	zerointerval = 5*tau_auryntime;

	// clock = auryn::sys->get_clock_ptr();
	clock = clk;
	state = new AurynFloat[size];
	timestamp = new AurynTime[size];
	for (NeuronID i = 0 ; i < size ; ++i ) 
	{
		state[i] = 0.;
		timestamp[i] = 0;
	}
}

void LinearTrace::free()
{
	delete [] state;
	delete [] timestamp;
}

LinearTrace::LinearTrace(NeuronID n, AurynFloat timeconstant, AurynTime * clk)
{
	init(n,timeconstant,clk);
}

LinearTrace::~LinearTrace()
{
	free();
}

void LinearTrace::set(NeuronID i , AurynFloat value)
{
	state[i] = value;
}

void LinearTrace::setall(AurynFloat value)
{
	for (NeuronID i = 0 ; i < size ; ++i )
		set(i,value);
}

void LinearTrace::add(NeuronID i, AurynFloat value)
{
	update(i);
	state[i] += value;
}

AurynFloat * LinearTrace::get_state_ptr()
{
	return state;
}

AurynFloat LinearTrace::get_tau()
{
	return tau;
}

inline void LinearTrace::update(NeuronID i)
{
	const int timediff = *clock - timestamp[i];
	if ( timediff == 0 ) return;

	if ( timediff >= zerointerval ) {
		state[i] = 0.0;
	} else { // as a last resort call exp
		state[i] *= std::exp( -(dt*timediff)/tau);
	}

	timestamp[i] = *clock;
}

void LinearTrace::inc(NeuronID i)
{
	update(i);
	state[i] += 1.0;
}

AurynFloat LinearTrace::get(NeuronID i)
{
	update(i);
	return state[i];
}


void LinearTrace::evolve() 
{
}
