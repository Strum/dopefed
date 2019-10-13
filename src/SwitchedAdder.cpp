///////////////////////////////////////////////////
//
//   Dopefed Plugin
//   switched precision adder
//
//   Strum 2018-19
//   strum@softhome.net
//
///////////////////////////////////////////////////

#include "dopefed.hpp"

struct SwitchedAdder : Module
{
	enum ParamIds
    {
        CH1_IN1_SWITCH,
        CH1_IN2_SWITCH,
        CH1_IN3_SWITCH,
        CH1_IN4_SWITCH,
        CH1_IN5_SWITCH,
        CH2_IN1_SWITCH,
        CH2_IN2_SWITCH,
        CH2_IN3_SWITCH,
        CH2_IN4_SWITCH,
        CH2_IN5_SWITCH,
		NUM_PARAMS
	};  
	enum InputIds
    {
        INPUT_1,
        INPUT_2,
        INPUT_3,
        INPUT_4,
        INPUT_5,
        INPUT_2_1,
        INPUT_2_2,
        INPUT_2_3,
        INPUT_2_4,
        INPUT_2_5,			  
		NUM_INPUTS
	};
	enum OutputIds
    {
		OUTPUT_1,
        OUTPUT_2,
        OUTPUT_1_INV,
        OUTPUT_2_INV,        
		NUM_OUTPUTS
	};
  
    float sum_out_1 = 0.0;
    float sum_out_2 = 0.0;  
  
	SwitchedAdder()
    {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS);
        configParam(SwitchedAdder::CH1_IN1_SWITCH, 0.0f, 2.0f, 0.0f, "");
        configParam(SwitchedAdder::CH1_IN2_SWITCH, 0.0f, 2.0f, 0.0f, "");
        configParam(SwitchedAdder::CH1_IN3_SWITCH, 0.0f, 2.0f, 0.0f, "");
        configParam(SwitchedAdder::CH1_IN4_SWITCH, 0.0f, 2.0f, 0.0f, "");
        configParam(SwitchedAdder::CH1_IN5_SWITCH, 0.0f, 2.0f, 0.0f, "");
        configParam(SwitchedAdder::CH2_IN1_SWITCH, -1.0f, 1.0f, 0.0f, "");
        configParam(SwitchedAdder::CH2_IN2_SWITCH, -1.0f, 1.0f, 0.0f, "");
        configParam(SwitchedAdder::CH2_IN3_SWITCH, -1.0f, 1.0f, 0.0f, "");
        configParam(SwitchedAdder::CH2_IN4_SWITCH, -1.0f, 1.0f, 0.0f, "");
        configParam(SwitchedAdder::CH2_IN5_SWITCH, -1.0f, 1.0f, 0.0f, "");
    }

	void process(const ProcessArgs& args) override;

    
};

void SwitchedAdder::process(const ProcessArgs& args)
{
  sum_out_1 = 0.0;
  sum_out_2 = 0.0;
  
  for (int i = 0 ; i < 5 ; i++)
  {
    if (params[CH1_IN1_SWITCH + i].getValue() == 0) 
      sum_out_1 -= inputs[INPUT_1 + i].getVoltage();
    else if (params[CH1_IN1_SWITCH + i].getValue() == 2) 
      sum_out_1 += inputs[INPUT_1 + i].getVoltage();
    
    sum_out_2 += inputs[INPUT_2_1 + i].getVoltage() * params[CH2_IN1_SWITCH + i].getValue();
  }
  
  outputs[OUTPUT_1].setVoltage(sum_out_1);
  outputs[OUTPUT_2].setVoltage(sum_out_2);
  outputs[OUTPUT_1_INV].setVoltage(- sum_out_1);
  outputs[OUTPUT_2_INV].setVoltage(- sum_out_2);  
}

////////////////////////////////////////////////////////////////
struct SwitchedAdderWidget : ModuleWidget
{
	SwitchedAdderWidget(SwitchedAdder *module)
    {
        setModule(module);
        setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/SwitchedAdder.svg"))); 
      
        // channel 1
        addInput(createInput<InPort>(Vec(10, 30), module, SwitchedAdder::INPUT_1));
        addInput(createInput<InPort>(Vec(10, 55), module, SwitchedAdder::INPUT_2));
        addInput(createInput<InPort>(Vec(10, 80), module, SwitchedAdder::INPUT_3));
        addInput(createInput<InPort>(Vec(10, 105), module, SwitchedAdder::INPUT_4));
        addInput(createInput<InPort>(Vec(10, 130), module, SwitchedAdder::INPUT_5));

        addParam(createParam<ThreeWaySwitch>(Vec(40, 30), module, SwitchedAdder::CH1_IN1_SWITCH));
        addParam(createParam<ThreeWaySwitch>(Vec(40, 55), module, SwitchedAdder::CH1_IN2_SWITCH));
        addParam(createParam<ThreeWaySwitch>(Vec(40, 80), module, SwitchedAdder::CH1_IN3_SWITCH));
        addParam(createParam<ThreeWaySwitch>(Vec(40, 105), module, SwitchedAdder::CH1_IN4_SWITCH));
        addParam(createParam<ThreeWaySwitch>(Vec(40, 130), module, SwitchedAdder::CH1_IN5_SWITCH)); 

        addOutput(createOutput<OutPort>(Vec(10, 160), module, SwitchedAdder::OUTPUT_1));
        addOutput(createOutput<OutPort>(Vec(33, 160), module, SwitchedAdder::OUTPUT_1_INV)); 
      
        // channel 2
        int offset = 20;
        addInput(createInput<InPort>(Vec(10, 190 + offset), module, SwitchedAdder::INPUT_2_1));
        addInput(createInput<InPort>(Vec(10, 215 + offset), module, SwitchedAdder::INPUT_2_2));
        addInput(createInput<InPort>(Vec(10, 240 + offset), module, SwitchedAdder::INPUT_2_3));
        addInput(createInput<InPort>(Vec(10, 265 + offset), module, SwitchedAdder::INPUT_2_4));
        addInput(createInput<InPort>(Vec(10, 290 + offset), module, SwitchedAdder::INPUT_2_5));	

        addParam(createParam<ThreeWaySwitch>(Vec(40, 190 + offset), module, SwitchedAdder::CH2_IN1_SWITCH));
        addParam(createParam<ThreeWaySwitch>(Vec(40, 215 + offset), module, SwitchedAdder::CH2_IN2_SWITCH));
        addParam(createParam<ThreeWaySwitch>(Vec(40, 240 + offset), module, SwitchedAdder::CH2_IN3_SWITCH));
        addParam(createParam<ThreeWaySwitch>(Vec(40, 265 + offset), module, SwitchedAdder::CH2_IN4_SWITCH));
        addParam(createParam<ThreeWaySwitch>(Vec(40, 290 + offset), module, SwitchedAdder::CH2_IN5_SWITCH));

        addOutput(createOutput<OutPort>(Vec(10, 315 + offset), module, SwitchedAdder::OUTPUT_2));
        addOutput(createOutput<OutPort>(Vec(33, 315 + offset), module, SwitchedAdder::OUTPUT_2_INV));
    }
};

Model *modelSwitchedAdder = createModel<SwitchedAdder, SwitchedAdderWidget>("SwitchedAdder");