#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>
#include <time.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <yamlLoader.h>
#include <crossbarControlYaml.hh>
#include <crossbarControl.h>

#include <asynPortDriver.h>
#include <asynOctetSyncIO.h>

#include <iocsh.h>
#include <drvSup.h>
#include <epicsExport.h>

using namespace CrossbarControl;


typedef struct _timing_ts {
    const char *timing_name;
    int  index;
} timing_ts;


static timing_ts timing_out_string []    = { {"RTM_OUT0", 0},
                                             {"FPGA",     1},
                                             {"BP",       2},
                                             {"RTM_OUT1", 3}, 
                                             { NULL,     -1} };
static timing_ts timing_source_string [] = { {"RTM_IN0 (LCLS1)", 0},
                                             {"FPGA",    1},
                                             {"BP",      2},
                                             {"RTM_IN1 (LCLS2)", 3},
                                             {"RTM_IN0", 0},
                                             {"RTM_IN1", 3},
                                             {"LCLS1",   0},
                                             {"LCLS2",   3},
                                             {NULL,     -1} };


static CrossbarControlDriver *pDrv = NULL;


static const char * driverName = "crossbarControlAsynDriver";


CrossbarControlDriver::CrossbarControlDriver(const char *_path_str)
{
    pCrossbarApi = new CrossbarControlYaml(cpswGetRoot()->findByName(_path_str));
}

void CrossbarControlDriver::Report(void)
{
    printf("Timing Crossbar Status\n");
    printf("OutputConfig[0]: %s <--- %s\n", timing_out_string[0].timing_name, timing_source_string[pCrossbarApi->GetOutputConfig0()].timing_name);
    printf("OutputConfig[1]: %s <-- -%s\n", timing_out_string[1].timing_name, timing_source_string[pCrossbarApi->GetOutputConfig1()].timing_name);
    printf("OutputConfig[2]: %s <--- %s\n", timing_out_string[2].timing_name, timing_source_string[pCrossbarApi->GetOutputConfig2()].timing_name);
    printf("OutputConfig[3]: %s <--- %s\n", timing_out_string[3].timing_name, timing_source_string[pCrossbarApi->GetOutputConfig3()].timing_name);
}

void CrossbarControlDriver::Control(const char *output_name, const char *source_name)
{
    int i, output, source;
    
    if(!pDrv) return;
    
    i = 0;
    while(timing_out_string[i].timing_name) {
        if(!strcmp (timing_out_string[i].timing_name, output_name)) break;
        i ++;
    }
    output = timing_out_string[i].index;
    
    
    i = 0;
    while(timing_source_string[i].timing_name) {
        if(!strcmp (timing_source_string[i].timing_name, source_name)) break;
        i ++;
    }
    source = timing_source_string[i].index;
    
    if(output <0) {
        printf("CrossbarControlDriver could not recognize the output name %s\n", output_name);
    }
    if(source <0) {
        printf("CrossbarControlDriver could not recognize the source name %s\n", source_name);
    }
    
    if(output <0 || source <0 ) return;
    
    switch(output) {
        case 0: pCrossbarApi->SetOutputConfig0((uint32_t) source); break;
        case 1: pCrossbarApi->SetOutputConfig1((uint32_t) source); break;
        case 2: pCrossbarApi->SetOutputConfig2((uint32_t) source); break;
        case 3: pCrossbarApi->SetOutputConfig2((uint32_t) source); break;
    }
    
}



CrossbarControlAsynDriver::CrossbarControlAsynDriver(const char *portName, CrossbarControlDriver *p)
    : asynPortDriver(portName,
                     1, /* number of elements of this driver */
                     NUM_CROSSBAR_CONTROL_DET_PARAMS, /* number of asyn params to be cleared for each device */
                     asynInt32Mask | asynFloat64Mask | asynOctetMask | asynDrvUserMask | asynInt32ArrayMask | asynInt16ArrayMask, /* Interface mask */
                     asynInt32Mask | asynFloat64Mask | asynOctetMask | asynEnumMask | asynInt32ArrayMask | asynInt16ArrayMask, /* Interrupt mask */
                     1,  /* asynFlags, this driver does block and it is not multi-device, so flag is 1 */
                     1,  /* autoconnect */
                     0,  /* default priority */
                     0)  /* default stack size */
{
    char param_name[32];
    
    for(int i =0; i < 4; i++) {
        sprintf(param_name, outputConfigString, i); createParam(param_name, asynParamInt32, &(p_outputConfig[i]));
    }

}


asynStatus CrossbarControlAsynDriver::writeInt32(asynUser *pasynUser, epicsInt32 value)
{
    int function             = pasynUser->reason;
    asynStatus status        = asynSuccess;
    const char *functionName = "writeInt32";
    
    /* set the parameter in the parameter library */
    status = (asynStatus) setIntegerParam(function, value);
    
    switch(function) {
        default:
            break;
    }
    
    if(function == p_outputConfig[0]) {
        pDrv->GetApi()->SetOutputConfig0((uint32_t) value);
    }
    else if(function == p_outputConfig[1]) {
        pDrv->GetApi()->SetOutputConfig1((uint32_t) value);
    }
    else if(function == p_outputConfig[2]) {
        pDrv->GetApi()->SetOutputConfig2((uint32_t) value);
    }
    else if(function == p_outputConfig[3]) {
        pDrv->GetApi()->SetOutputConfig3((uint32_t) value);
    }
    
    
    /* Do callback so higher layer see any changes */
    status = (asynStatus) callParamCallbacks();
    
    if(status)
        epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize,
                      "%s:%s: status=%d, function=%d, value=%d",
                      driverName, functionName, status, function, value);

    else
        asynPrint(pasynUser, ASYN_TRACEIO_DRIVER,
                  "%s:%s: function=%d, value=%d\n",
                  driverName, functionName, function, value);

    
    
    return status;
}




extern "C" {
static int crossbarControlReport(int interest);
static int crossbarControlInitialize(void);
static struct drvet crossbarControlDriver = {
    2,
    (DRVSUPFUN) crossbarControlReport,
    (DRVSUPFUN) crossbarControlInitialize
};


static int crossbarControlReport(int interest)
{
    if(!pDrv) return 0;
    
    pDrv->Report();
    return 0;
}


static int crossbarControlInitialize(void)
{
// nothing todo in this phase, lazy intialization...
    return 0;
}

epicsExportAddress(drvet, crossbarControlDriver);


//
//    ioc shell command for driver initialization
//

static const iocshArg initAsynArg0           = { "asyn port name",          iocshArgString };
static const iocshArg initAsynArg1           = { "path for AmcCarrierCore", iocshArgString };
static const iocshArg * const initAsynArgs[] = { &initAsynArg0,
                                                 &initAsynArg1 };
static const iocshFuncDef initAsynFuncDef    = { "crossbarControlAsynDriverConfigure", 2, initAsynArgs };
static void  initAsynCallFunc(const iocshArgBuf *args)
{
    if(pDrv) {
        printf("The crossbarControlAsynDriver has been initialized already\n");
        return;
    }
    
    new CrossbarControlAsynDriver((const char*) args[0].sval, 
                                  pDrv = new CrossbarControlDriver((const char*) args[1].sval));
} 

static const iocshArg initArg0 = { "path for AmcCarrierCore", iocshArgString };
static const iocshArg * const initArgs[] = { &initArg0 };
static const iocshFuncDef initFuncDef = { "crossbarControlDriverConfigure", 1, initArgs };
static void  initCallFunc(const iocshArgBuf *args)
{
    if(pDrv) {
        printf("The crossbarControlDriver has been initialized already\n");
        return;
    }
    
    pDrv = new CrossbarControlDriver((const char*) args[0].sval);
    
}

static const iocshFuncDef reportFuncDef = {"crossbarControlDriverReport", 0, NULL};
static void reportCallFunc(const iocshArgBuf *args)
{
    if(!pDrv) return;
    
    pDrv->Report();
}

static const iocshArg controlArg0 = { "output: RTM_OUT0 | FPGA | BP | RTM_OUT1", iocshArgString };
static const iocshArg controlArg1 = { "source: RTM_IN0 [LCLS1] | FPGA | BP | RTM_IN0 [LCLS2]",   iocshArgString };
static const iocshArg* const controlArgs [] = { &controlArg0,
                                                &controlArg1 };
static const iocshFuncDef controlFuncDef = {"crossbarControl", 2, controlArgs};
static void  controlCallFunc(const iocshArgBuf *args)
{
    if(!pDrv) return;
    
    pDrv->Control(args[0].sval, args[1].sval);
} 

 


void crossbarControlDriverRegister(void)
{
    iocshRegister(&initAsynFuncDef, initAsynCallFunc);
    iocshRegister(&initFuncDef,    initCallFunc);
    iocshRegister(&reportFuncDef,  reportCallFunc);
    iocshRegister(&controlFuncDef, controlCallFunc);
}

epicsExportRegistrar(crossbarControlDriverRegister);


} /*extern C */