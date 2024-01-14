#include "main.h"
int main()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData))
    {
        printf("WSAStartup failed: %d\n", WSAGetLastError());
        return 1;
    }
    DWORD AdaptersSize = 20000;
    PIP_ADAPTER_ADDRESSES Adapters;
    do
    {
        Adapters = (PIP_ADAPTER_ADDRESSES)malloc(AdaptersSize);
        if (!Adapters)
        {
            printf("Couldn't allocate %ld bytes for adapters.\n", AdaptersSize);
            WSACleanup();
            return 1;
        }
        int AdaptersADResult = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, NULL, Adapters, &AdaptersSize);
        if (AdaptersADResult == ERROR_BUFFER_OVERFLOW)
        {
            printf("GetAdaptersAddresses wants %ld bytes for adapters.\n", AdaptersSize);
            free(Adapters);
        }
        else if (AdaptersADResult == ERROR_SUCCESS)
        {
            break;
        }
        else
        {
            printf("Error from GetAdaptersAddresses: %d\n", AdaptersADResult);
            free(Adapters);
            WSACleanup();
        }

    } while (!Adapters);
    PIP_ADAPTER_ADDRESSES Adapter = Adapters;
    while (Adapter)
    {
        printf("\n Adapter name: %S\n", Adapter->FriendlyName);
        PIP_ADAPTER_UNICAST_ADDRESS address = Adapter->FirstUnicastAddress;
        while (address)
        {
            printf("\t%s", address->Address.lpSockaddr->sa_family == AF_INET ? "IPv4" : "IPv6");
            char AdressTextForm[100];
            getnameinfo(address->Address.lpSockaddr, address->Address.iSockaddrLength, AdressTextForm, sizeof(AdressTextForm), 0, 0, NI_NUMERICHOST);
            printf("\t%s\n", AdressTextForm);
            address = address->Next;
        }
        Adapter = Adapter->Next;
    }
    free(Adapters);
    WSACleanup();
    return 0;
}
