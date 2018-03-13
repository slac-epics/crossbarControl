#ifndef CROSSBAR_CONTROL_H
#define CROSSBAR_CONTROL_H

#include <asynPortDriver.h>
#include <yamlLoader.h>

#include <crossbarControlYaml.hh>

namespace CrossbarControl {

    class CrossbarControlDriver {
        private:
            CrossbarControlYaml *pCrossbarApi;
        public:
            CrossbarControlDriver(const char * _path_str);
            void Report(void);
            void Control(const char *output_name, const char *source_name);
            CrossbarControlYaml * GetApi(void) { return pCrossbarApi; };
    };
    
    
    class CrossbarControlAsynDriver:asynPortDriver {
        public:
            CrossbarControlAsynDriver(const char * portName, CrossbarControlDriver *pDrv);
            asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value);
            
        protected:
        //
        // parameter section for asynPortDriver
        //
        int firstCrossbarControlParam;
#define FIRST_CROSSBAR_CONTROL_PARAM      firstCrossbarControlParam
        int p_outputConfig[4];      /* asynInt32, rw */
        int lastCrossbarControlParam;
#define LAST_CROSSBAR_CONTROL_PARAM      lastCrossbarControlParam
        
        
    };


};  /* namespace CrossbarControl */

#define NUM_CROSSBAR_CONTROL_DET_PARAMS ((int)(&LAST_CROSSBAR_CONTROL_PARAM - &FIRST_CROSSBAR_CONTROL_PARAM -1))

#define outputConfigString    "output%d"

#endif /* CROSSBAR_CONTROL_H */