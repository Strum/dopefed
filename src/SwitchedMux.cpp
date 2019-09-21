///////////////////////////////////////////////////
//
//   Dopefed Plugin
//   8 to 1 Mux with Manual selector switch
//
//   Strum 2018-19
//   strum@softhome.net
//
///////////////////////////////////////////////////

#include "dopefed.hpp"

struct SwitchedMux : Module
{
	enum ParamIds
  {
    BUTTON_1,
    NUM_PARAMS = BUTTON_1 + 8
	};  
	enum InputIds
  {    
    SIG_INPUT,		  
		NUM_INPUTS = SIG_INPUT + 8
	};
	enum OutputIds
  {
    OUTPUT,    
		NUM_OUTPUTS
	};
  enum LightIds
  {
    BUTTON_LEDS,
    NUM_LIGHTS = BUTTON_LEDS + 8
  };  
    
  dsp::SchmittTrigger button_triggers[8];
  int selected_input = 0;
  bool is_new = true;
  
  SwitchedMux()
  {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
    for (int i = 0; i < 8; ++i)
    {
      configParam(SwitchedMux::BUTTON_1 + i, 0.0f, 1.0f, 0.0f, "");
    }
  };
  
  void process(const ProcessArgs& args) override;   
  
  json_t *dataToJson() override
  {
    json_t *rootJ = json_object();
    
    json_object_set_new(rootJ, "selected", json_integer(selected_input));
     
    return rootJ;
  };
  
  void dataFromJson(json_t *rootJ) override
  {
   json_t *selected_inputJ = json_object_get(rootJ, "selected");
    if(selected_inputJ) selected_input = json_integer_value(selected_inputJ);
  };  
};

//////////
void SwitchedMux::process(const ProcessArgs& args)
{
  if (is_new)
  {
    is_new = false;
    if (selected_input == 0) lights[BUTTON_LEDS].value = 1.0;
    else lights[BUTTON_LEDS + selected_input].value = 1.0;    
  }

  for ( int i = 0 ; i < 8 ; i ++)
  {
    if (button_triggers[i].process(params[BUTTON_1 + i].getValue()))
    {
      selected_input = i;
      for (int j = 0 ; j < 8 ; j ++)
      {        
        lights[BUTTON_LEDS + j ].value = (j==selected_input)?1.0:0.0;        
      }
    }    
  }
  outputs[OUTPUT].setVoltage(inputs[SIG_INPUT + selected_input].getVoltage());  
}

//////////////////////////////////////////////////////////////
struct SwitchedMuxWidget : ModuleWidget
{
  SwitchedMuxWidget(SwitchedMux *module)
  {
    setModule(module);
    setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/SwitchedMux.svg")));
	
    int spacing = 30; 
    int top_space = 15;    
  
    for (int i = 0; i < 8 ; i++)
    { 
      addParam(createParam<LEDButton>(Vec(7,top_space + spacing * i + 54 ), module, SwitchedMux::BUTTON_1 + i)); 
      addInput(createInput<InPort>(Vec(28, top_space + spacing * i + 50), module, SwitchedMux::SIG_INPUT + i));   	 
      addChild(createLight<MediumLight<GreenLight>>(Vec(11, top_space +  spacing * i + 5 + 53), module, SwitchedMux::BUTTON_LEDS + i));
    }  
    addOutput(createOutput<OutPort>(Vec(18, 333), module, SwitchedMux::OUTPUT)); 
  }  
};

Model *modelSwitchedMux = createModel<SwitchedMux, SwitchedMuxWidget>("SwitchedMux");