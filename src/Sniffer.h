#pragma once
#include <functional>
#include <string>

class Sniffer {
public:
    using Callback = std::function<void(const unsigned char* data, int len, long sec, long nsec)>;    
    
    bool open(const std::string& device_name);
    bool setFilter(const std::string& bpf_filter);
    bool start(Callback cb);
    void stop();
    void close();

private:
    // We forward implement to avoid retyping the Pcap headers everywhere.
    // This way we only need Pcap headers in Sniffer.cpp
    // This will stop some nasty dependency errors in the future.
    class Impl;
    Impl* impl_ = nullptr;
};