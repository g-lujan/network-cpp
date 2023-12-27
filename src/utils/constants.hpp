#if defined(_WIN32)
    #ifndef _WIN32_WINNT
    #define _WIN32_WINNT 0x0600
    #endif
    /* many Windows headers are missing the definitions for IPV6_V6ONLY */
    #if !defined(IPV6_V6ONLY)
    #define IPV6_V6ONLY 27
    #endif
#else
    #define SOCKET int
#endif