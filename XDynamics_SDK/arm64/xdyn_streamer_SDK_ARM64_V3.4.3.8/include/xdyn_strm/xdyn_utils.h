#ifndef XDYN_UTILS_H
#define XDYN_UTILS_H
#include <vector>
#include <string>
#include "xdyn_streamer.h"
#include "xdyn_lib_export.h"
#include "xdyn_define.h"
namespace XD {

typedef struct XdynProductDesc{
    uint32_t type;
    uint32_t warelfID;
    XDYN_SINK_TYPE_E inter1;
    XDYN_SINK_TYPE_E inter2;
    std::string prtIP;
    XDYN_DEV_TYPE_e devType;
    XdynProductDesc()
    {
        type = XDYN_PRODUCT_TYPE_INVALIB;
        warelfID = 0;
        inter1 = XDYN_SINK_TYPE_NONE;
        inter2 = XDYN_SINK_TYPE_NONE;
    }
}XdynProductDesc_t;

XDYN_LIB_API void ScanProductFromUSB(std::vector<XdynProductDesc_t> &prtList);
XDYN_LIB_API void ScanProductFromNet(std::vector<XdynProductDesc_t> &prtList, std::string ip);
XDYN_LIB_API void XdynScanProduct(std::vector<XdynProductDesc_t> &prtList, std::string ip = "192.168.31.3");

XDYN_LIB_API void ScanProductFromNet(std::vector<XdynProductDesc_t> &prtList, char *ip);
XDYN_LIB_API void XdynScanProduct(std::vector<XdynProductDesc_t> &prtList, char *ip);

XDYN_LIB_API std::string XdynGetProductStr(uint32_t type);

XDYN_LIB_API int XdynDev_SyncTime(XDYN_Streamer *strm);

}

#endif // XDYN_UTILS_H
