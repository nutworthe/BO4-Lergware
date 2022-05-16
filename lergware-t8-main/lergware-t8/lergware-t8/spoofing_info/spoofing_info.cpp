#include "spoofing_info.h"

namespace spoofing
{
    void spoof_info_response(Msg_InfoResponse* response)
    {
        response->natType = 1;

        for (int i = 0; i < 2; i++)
        {
            if (spoof_hostname)
            {
                for (int i = 0; i < strlen(response->lobby[i].hostName); i++)
                {
                    response->lobby[i].hostName[i] = info_response_hostname.data()[i];
                }
            }
            if (spoof_hostxuid)
                response->lobby[i].hostXuid = std::stoull(info_response_hostxuid);

            if (spoof_netadr)
            {
                response->lobby[i].serializedAdr = { 0 };
                response->lobby[i].isValid = { 0 };
                response->lobby[i].secId = { 0 };
                response->lobby[i].secKey = { 0 };
            }
        }
    }
}