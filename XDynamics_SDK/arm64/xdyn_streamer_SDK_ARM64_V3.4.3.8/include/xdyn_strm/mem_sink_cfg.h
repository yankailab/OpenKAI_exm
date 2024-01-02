#ifndef MEM_SINK_CFG_H
#define MEM_SINK_CFG_H

#include "xdyn_lib_export.h"

namespace XD {

typedef enum {
    MEM_AGENT_SINK_RAW,
    MEM_AGENT_SINK_DEPTH,
    MEM_AGENT_SINK_POINT_CLOUD,
    MEM_AGENT_SINK_AMP,
    MEM_AGENT_SINK_FLAG,
    MEM_AGENT_SINK_CONFID,
    MEM_AGENT_SINK_GRAY,
    MEM_AGENT_SINK_BG,
    MEM_AGENT_SINK_RGB,
    MEM_AGENT_SINK_USR1,
    MEM_AGENT_SINK_USR2,
    MEM_AGENT_SINK_USR3,
    MEM_AGENT_SINK_USR4,
    MEM_AGENT_SINK_USR5,
    MEM_AGENT_SINK_MAX,
} MEM_SINK_DATA_TYPE;

struct XDYN_LIB_API MemSinkCfg{
    MemSinkCfg();
    void Reset();
    void Full();
    void OR(MemSinkCfg &cfg);

    bool isUsed[MEM_AGENT_SINK_MAX];
};

struct MemSinkInfo{
    unsigned int width;
    unsigned int height;
	unsigned int numPhases;
	unsigned int addInfoLines;
	unsigned int numInputFrames;
	unsigned int binningMode;

    unsigned int rgbW;
    unsigned int rgbH;
    unsigned int stride;
    unsigned int fmt;
};


XDYN_LIB_API unsigned int CalcDataMemSize(MEM_SINK_DATA_TYPE type, MemSinkInfo info);
XDYN_LIB_API unsigned int CalcImgSize(MEM_SINK_DATA_TYPE type, MemSinkInfo info);
XDYN_LIB_API unsigned int CalcTotalMemSize(MemSinkCfg shmCfg, MemSinkInfo info);
}

#endif // MEM_SINK_CFG_H
