#include <cpsw_api_builder.h>
#include <cpsw_api_user.h>

#include <crossbarControlYaml.hh>

#include <stdio.h>
#include <string.h>

using namespace CrossbarControl;

CrossbarControlYaml::CrossbarControlYaml(Path core)
{
    _path          = core;
    _outputConfig0 = IScalVal::create(_path->findByName("OutputConfig[0]"));
    _outputConfig1 = IScalVal::create(_path->findByName("OutputConfig[1]"));
    _outputConfig2 = IScalVal::create(_path->findByName("OutputConfig[2]"));
    _outputConfig3 = IScalVal::create(_path->findByName("OutputConfig[3]"));
    
}

uint32_t CrossbarControlYaml::GetOutputConfig0(void)
{
    uint32_t    val;
    IndexRange  rng(0);
    
    _outputConfig0->getVal(&val, 1, &rng);
    
    return val;
}

uint32_t CrossbarControlYaml::GetOutputConfig1(void)
{
    uint32_t    val;
    IndexRange  rng(0);
    
    _outputConfig1->getVal(&val, 1, &rng);
    
    return val;
}

uint32_t CrossbarControlYaml::GetOutputConfig2(void)
{
    uint32_t    val;
    IndexRange  rng(0);
    
    _outputConfig2->getVal(&val, 1, &rng);
    
    return val;
}

uint32_t CrossbarControlYaml::GetOutputConfig3(void)
{
    uint32_t    val;
    IndexRange  rng(0);
    
    _outputConfig3->getVal(&val, 1, &rng);
    
    return val;
}

void CrossbarControlYaml::SetOutputConfig0(uint32_t output)
{
    _outputConfig0->setVal(&output);
}

void CrossbarControlYaml::SetOutputConfig1(uint32_t output)
{
    _outputConfig1->setVal(&output);
}

void CrossbarControlYaml::SetOutputConfig2(uint32_t output)
{
    _outputConfig2->setVal(&output);
}

void CrossbarControlYaml::SetOutputConfig3(uint32_t output)
{
    _outputConfig3->setVal(&output);
}

