#ifndef CARGUMENT_H
#define CARGUMENT 
#include <string>
#include <vector>

class CArgument {
public:
    CArgument();
    std::vector<int> split_arg(std::string Arg);

private:
    std::string m_filepath;
    std::string m_arg_samp_sock;
    std::string m_arg_queuing_sock;
    std::string m_arg_samp_port;
    std::string m_arg_queuing_port;
};
#endif
