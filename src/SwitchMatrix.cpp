///////////////////////////////////////////////////////////////////
//
//  Dopefed Plugin
//  Switch Matrix 
//
//  Strum 2018-19
//  strum@softhome.net
//
///////////////////////////////////////////////////////////////////

#include "dopefed.hpp"


struct SwitchMatrix : Module
{
	enum ParamIds
  {
    MODE_SEL,
    SWITCHES = MODE_SEL + 10,
		NUM_PARAMS = SWITCHES + 100
	};  
	enum InputIds
  {		
		INPUTS,
    NUM_INPUTS = INPUTS + 10
	};
	enum OutputIds
  {
		OUTPUTS,    
		NUM_OUTPUTS = OUTPUTS + 10
	};
  enum LightIds
  {
		SWITCH_LIGHTS,
    NUM_LIGHTS = SWITCH_LIGHTS + 100
	};  

  dsp::SchmittTrigger switch_triggers[10][10];

  bool switch_states[10][10] = 
  {{0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0}};   
  
  float input_values[10] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
  float sums[10] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0}; 
  int selected_in[10] = {0,0,0,0,0,0,0,0,0,0};
  int mode[10] = {0,0,0,0,0,0,0,0,0,0};

	SwitchMatrix()
  {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
    
    for (int i = 0; i < 10 ; i++)
    {  
      configParam(SwitchMatrix::MODE_SEL + i, 0.0f, 2.0f, 0.0f, "");    
      for (int j = 0; j < 10 ; j++)
      {        
        configParam(SwitchMatrix::SWITCHES + i * 10 + j, 0.0f, 1.0f, 0.0f, "");
      }
    }
  }

	void process(const ProcessArgs& args) override;
  
  json_t *dataToJson() override
  {
		json_t *rootJ = json_object();
    
    // button states
		json_t *button_statesJ = json_array();
		for (int i = 0; i < 10; i++)
    {
			for (int j = 0; j < 10; j++)
      {
        json_t *button_stateJ = json_integer((int) switch_states[i][j]);
			  json_array_append_new(button_statesJ, button_stateJ);
      }
		}
		json_object_set_new(rootJ, "buttons", button_statesJ);
    
    return rootJ;
  }
  
  void dataFromJson(json_t *rootJ) override
  {
    // button states
		json_t *button_statesJ = json_object_get(rootJ, "buttons");
		if (button_statesJ)
    {
			for (int i = 0; i < 10; i++)
      {
        for (int j = 0; j < 10; j++)
        {
				  json_t *button_stateJ = json_array_get(button_statesJ, i*10 + j);
				  if (button_stateJ)
					  switch_states[i][j] = !!json_integer_value(button_stateJ);
        }
      }
		}  
  }  
};

//////////////////////////////////////////////////////////////
////// Step function
void SwitchMatrix::process(const ProcessArgs& args)
{
	// zero sums
  for ( int i = 0 ; i < 10 ; i++)
  {
    sums[i] = 0.0;
    if (mode[i] != params[MODE_SEL + i].getValue() )
    {
      mode[i] = params[MODE_SEL + i].getValue();
      if (mode[i] == 2) 
      {
        selected_in[i] = 0;
        for (int j = 1 ; j < 10 ; j++)
        {          
          switch_states[i][j] = false ;
          lights[SWITCH_LIGHTS + i * 10 + j ].value = 0.0f;
        }
        switch_states[i][0] = true ;
        lights[SWITCH_LIGHTS + i * 10 ].value = 1.0f;
      } else
      {
        for (int j = 0 ; j < 10 ; j++)
        {          
          switch_states[i][j] = false ;
          lights[SWITCH_LIGHTS + i * 10 + j ].value = 0.0f;
        }
      }
    }
  }
  
  // deal with buttons
    for (int col = 0 ; col < 10 ; col++)
  {
    for (int row = 0 ; row < 10 ; row++)
    {
      if (params[MODE_SEL + col].getValue() != 2)
      {
          if (switch_triggers[col][row].process(params[SWITCHES+col*10 + row].getValue()))
          {
            switch_states[col][row] = !switch_states[col][row];
          }
          lights[SWITCH_LIGHTS + col * 10 + row].value  = (switch_states[col][row]) ? 1.0 : 0.0;
      }
      else
      {
        if (switch_triggers[col][row].process(params[SWITCHES+col*10 + row].getValue()))
        {
          for (int i = 0 ; i  < 10 ; i++)
          {
           switch_states[col][i] = false ;
           lights[SWITCH_LIGHTS + col * 10 + i ].value = 0.0f;
          }
          switch_states[col][row] = true;
          selected_in[col] = row;
          lights[SWITCH_LIGHTS + col * 10 + row ].value = 1.0f;
        }           
      }
    }       
  }

  // get inputs
  for (int i = 0 ; i < 10 ; i++)
  {
    input_values[i] = inputs[INPUTS + i].getVoltage();
  }
  
  // process  inputs  
  for (int i = 0 ; i < 10 ; i++)
  {   
    // sum mode
    if (params[MODE_SEL + i].getValue() == 0)
    {
      for (int j = 0 ; j < 10 ; j++)
      {
        if (switch_states[i][j]) sums[i] += input_values[j];
      }
    }
    // or mode
    if (params[MODE_SEL + i].getValue() == 1)
    {
      for (int j = 0 ; j < 10 ; j++)
      {
        if (switch_states[i][j] && (inputs[INPUTS + j].getVoltage() > 0.0f)) sums[i] = 10.0f;
      }
    }
    // mux mode
    if (params[MODE_SEL + i].getValue() == 2)
      sums[i] = input_values[selected_in[i]];

    // set output
    outputs[OUTPUTS + i].setVoltage(sums[i]);    
  }  
} // end step loop

///// Gui
struct SwitchMatrixWidget : ModuleWidget
{
  SwitchMatrixWidget(SwitchMatrix *module)
  {
    setModule(module);
  
    setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/SwitchMatrix.svg")));

    int top_row = 75;
    int row_spacing = 25; 
    int column_spacing = 25;

  	for (int i = 0 ; i < 10 ; i++)
    {
      addParam(createParam<VThreeWaySwitch>(Vec(35 + i * column_spacing , 30), module, SwitchMatrix::MODE_SEL + i));
      addInput(createInput<InPort>(Vec(3, i * row_spacing + top_row), module, SwitchMatrix::INPUTS + i));  
      addOutput(createOutput<OutPort>(Vec(33 + i * column_spacing , top_row + 10 * row_spacing), module, SwitchMatrix::OUTPUTS + i));
      for(int j = 0 ; j < 10 ; j++ )
      {
        addParam(createParam<LEDButton>(Vec(35 + column_spacing * i, top_row + row_spacing * j), module, SwitchMatrix::SWITCHES + i * 10 + j ));
        addChild(createLight<MedLight<BlueLED>>(Vec(35 + column_spacing * i + 4, top_row + row_spacing * j + 4), module, SwitchMatrix::SWITCH_LIGHTS  + i * 10 + j ));
      }
  	}
  }  
};

Model *modelSwitchMatrix = createModel<SwitchMatrix, SwitchMatrixWidget>("SwitchMatrix");