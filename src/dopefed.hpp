///////////////////////////////////////////////////
//
//   Dopefed Plugin for VCV Version 1
//
//   Strum 2018-19
//   strum@softhome.net
//
///////////////////////////////////////////////////

#include "rack.hpp"

using namespace rack;

// Forward-declare the Plugin, defined in Template.cpp
extern Plugin *pluginInstance;

// Forward-declare each Model, defined in each module source file
extern Model *modelSwitchedAdder;
extern Model *modelSwitchedMux;
extern Model *modelSwitchMatrix;

//// Gui Components

// Ports

struct InPort : SvgPort
{
	InPort()
	{
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/InPort.svg")));
	}
};

struct OutPort : SvgPort
{
	OutPort()
	{
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/OutPort.svg")));
	}
};

struct GateInPort : SvgPort
{
	GateInPort()
	{
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/GateInPort.svg")));
	}
};

struct GateOutPort : SvgPort
{
	GateOutPort()
	{
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/GateOutPort.svg")));
	}
};

struct ThreeWaySwitch : SvgSwitch
{
	ThreeWaySwitch()
	{
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/Three_0.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/Three_1.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/Three_2.svg")));
	}
};

struct VThreeWaySwitch : SvgSwitch
{
	VThreeWaySwitch()
	{
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/VThree_0.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/VThree_1.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/VThree_2.svg")));
	}
};

// Knobs

struct LrgKnob : RoundKnob
{
	LrgKnob()
	{
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/LrgKnob.svg")));
		box.size = Vec(45,45);
	}
};

struct MedKnob : RoundKnob
{
	MedKnob()
	{
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/MedKnob.svg")));
		box.size = Vec(32,32);
	}
};

struct SmlKnob : RoundKnob
{
	SmlKnob()
	{
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/SmlKnob.svg")));
		box.size = Vec(25,25);
	}
};

/// lights

struct dopefedLight : GrayModuleLightWidget
{
	dopefedLight() { bgColor = nvgRGB(0x40, 0x40, 0x40); }
};
struct RedLED : dopefedLight
{
 	RedLED() { addBaseColor(nvgRGB(0xff, 0x00, 0x00)); }
};

struct BlueLED : dopefedLight
{
 	BlueLED() { addBaseColor(nvgRGB(0x00, 0x47, 0x7e)); }
};

struct OrangeLED : dopefedLight
{
 	OrangeLED() { addBaseColor(nvgRGB(0xff, 0xA5, 0x00)); }
};

template <typename BASE>
 struct TinyLight : BASE
 {
 	TinyLight()	{ this->box.size = Vec(4, 4);}
 };

template <typename BASE>
 struct SmlLight : BASE
 {
 	SmlLight() { this->box.size = Vec(8, 8); }
 };

template <typename BASE>
 struct MedLight : BASE
 {
 	MedLight() { this->box.size = Vec(10, 10); }
 };