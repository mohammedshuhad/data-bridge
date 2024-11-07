#pragma once

#include "BaseData.hpp"
#include <vector>

namespace DataBridge
{
    class Model : public BaseDataGeneric
    {
    public:
        Model(const char *name) : BaseDataGeneric(name) {}

        void reset()
        {
            for (auto &data : datas)
            {
                data->reset();
            }
        }

        void set_default()
        {
            for (auto &data : datas)
            {
                data->set_default();
            }
        }

        void load_or_reset()
        {
            for (auto &data : datas)
            {
                data->load_or_reset();
            }
        }

        void print(uint32_t indent_depth = 0)
        {
            for (int i = 0; i < indent_depth; i++)
                std::cout << "  ";
            std::cout << name << '\n';
            for (auto data : datas)
            {
                data->print(indent_depth + 1);
            }
        }

        void log_on_sub(bool set = true)
        {
            for (auto data : datas)
            {
                data->log_on_sub(set);
            }
        }

        void mute_sub()
        {
            for (auto data : datas)
            {
                data->mute_sub();
            }
        }

        void unmute_sub()
        {
            for (auto data : datas)
            {
                data->unmute_sub();
            }
        }

    protected:
        void add_data(BaseDataGeneric &data)
        {
            datas.push_back(&data);
        }
    private:
        std::vector<BaseDataGeneric *> datas;
    };
};