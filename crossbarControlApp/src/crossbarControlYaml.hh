#ifndef CROSSBAR_CONTROL_YAML_H
#define CROSSBAR_CONTROL_YAML_H

#include <cpsw_api_builder.h>

#include <stdint.h>
#include <vector>


namespace CrossbarControl {

    class CrossbarControlYaml {
        public:
            CrossbarControlYaml(Path core);
            uint32_t GetOutputConfig0(void);
            uint32_t GetOutputConfig1(void);
            uint32_t GetOutputConfig2(void);
            uint32_t GetOutputConfig3(void);
            
            void     SetOutputConfig0(uint32_t output);
            void     SetOutputConfig1(uint32_t output);
            void     SetOutputConfig2(uint32_t output);
            void     SetOutputConfig3(uint32_t output);
            
            
        protected:
            Path     _path;
            ScalVal  _outputConfig0;
            ScalVal  _outputConfig1;
            ScalVal  _outputConfig2;
            ScalVal  _outputConfig3;
    };

};  /* namespace CrossbarControl */

#endif /* CROSSBAR_CONTROL_YAML_H */