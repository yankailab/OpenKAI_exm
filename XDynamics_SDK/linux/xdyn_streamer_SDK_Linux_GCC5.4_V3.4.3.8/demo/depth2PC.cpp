#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "depth2PC.h"

PCConver::PCConver() : m_isInit(false), m_coe(nullptr)
{
}

PCConver::~PCConver()
{
    if(m_coe)
        free(m_coe);
    m_isInit = false;
}

bool PCConver::IsInit()
{
    return m_isInit;
}

void PCConver::Init(MemSinkInfo info, float fUnitOfDepth, float lens[5])
{
    m_info = info;
    m_fUnitOfDepth = fUnitOfDepth;
    m_fx = lens[0];
    m_fy = lens[1];
    m_cx = lens[2];
    m_cy = lens[3];
    m_zoom = lens[4];

    m_coe = (PCTransCoe_st *)calloc(1, info.width * info.height * sizeof(PCTransCoe_st));
    CalcCoe();
    m_isInit = true;
}

void PCConver::CalcCoe()
{
    if(!m_coe) return;

    // uint32_t imgSize = m_info.width * m_info.height;
    for(uint32_t i = 0; i < m_info.width; i ++){
        for(uint32_t j = 0; j < m_info.height; j ++){
            uint32_t index = j * m_info.width + i;
            m_coe[index].x = m_zoom * (i - m_cx) / m_fy;
            m_coe[index].y = m_zoom * (m_cy - j) / m_fx;
            m_coe[index].z = sqrtf(m_coe[i].x * m_coe[i].x + m_coe[i].y * m_coe[i].y + 1);
        }
    }
}

void PCConver::Depth2PC(uint16_t *depth, PCPoint_t *pcArr, bool hMirro, bool vMirro)
{
    if(!m_isInit) return;

    int imgSize = m_info.width * m_info.height;
    for(int i = 0; i < imgSize; i ++){
        pcArr[i].fZ = depth[i] * m_fUnitOfDepth;
        pcArr[i].fX = (hMirro ? -1 : 1) * m_coe[i].x * pcArr[i].fZ;
        pcArr[i].fY = (vMirro ? -1 : 1) * m_coe[i].y * pcArr[i].fZ;
    }
}