#include "Configuration.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////
Configuration::Configuration()
{
    confPath = "./values.conf";

    std::ifstream s;
    std::string line;
    s.open(Configuration::confPath);
    std::map<std::string, std::string> m;        

    if (s.is_open())
    {
        while (std::getline(s, line))
        {
            int num = line.find("=");
            std::string key;
            std::string value;

            if (num != -1)
            {
                key = line.substr(0, num);
                value = line.substr(num + 1, line.length());
                m[key] = value;
            }      
        }
        s.close();
    }

    debug           = atoi(m["debug"].c_str());
    blur_uv_ksize   = atoi(m["blur_uv_ksize"].c_str());
    blur_ir_ksize   = atoi(m["blur_ir_ksize"].c_str());
    frame_percent   = atof(m["frame_percent"].c_str());
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
Configuration::~Configuration()
{
}