///////////////////////////////////////////////////
//
//   Dopefed Plugin for VCV Version 1
//
//   Strum 2018-19
//   strum@softhome.net
//
///////////////////////////////////////////////////

#include "dopefed.hpp"

Plugin *pluginInstance;

void init(rack::Plugin *p)
{
	pluginInstance = p;
	
	// Add all Models defined throughout the pluginInstance	
	p->addModel(modelSwitchedAdder);
	p->addModel(modelSwitchedMux);
	p->addModel(modelSwitchMatrix);	  

	// Any other pluginInstance initialization may go here.
	// As an alternative, consider lazy-loading assets and lookup tables when your module is created to reduce startup times of Rack.
}
