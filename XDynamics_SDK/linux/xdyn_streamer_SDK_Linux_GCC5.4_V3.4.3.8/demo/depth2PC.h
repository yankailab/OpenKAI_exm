#ifndef DEPTH_2_PC_H
#define DEPTH_2_PC_H

#include <stdint.h>
#include "mem_sink_cfg.h"

using namespace XD;

struct PCTransCoe_st
{
    float x;
    float y;
    float z;
};

typedef struct
{
    float fX; //x coordinate
    float fY; //y coordinate
    float fZ; //z coordinate
} PCPoint_t;

class PCConver
{
private:
    bool m_isInit;

    MemSinkInfo m_info;
    float m_fUnitOfDepth;
    float m_fx;
    float m_fy;
    float m_cx;
    float m_cy;
    float m_zoom;
    PCTransCoe_st *m_coe;
public:
    PCConver();
    ~PCConver();

    bool IsInit();
    void Init(MemSinkInfo info, float fUnitOfDepth, float lens[5]);

    void CalcCoe();
    void Depth2PC(uint16_t *depth, PCPoint_t *pcArr, bool hMirro = false, bool vMirro = false);
};

#endif