#pragma once
#include "BaseData.hpp"
#include "SetData.hpp"
#include "Model.hpp"

namespace DataBridge
{
    namespace Factory
    {
        Helper::NvsHandler *GetNvsHandler(void);

        const char *get_name(const char *nvs_key, const char *name);

        template <typename T>
        StorageDelegate<T> *MakeStorageDelegate(const T default_value, const char *nvs_key)
        {
            if (nvs_key == NULL)
            {
                return new StorageNone<T>(default_value);
                // return nullptr;
            }
            else
            {
                return new StorageBasic<T>(default_value, GetNvsHandler(), nvs_key);
            }
        }

        template <typename T>
        SetData<T> MakeSetAlways(const T default_value, const char *nvs_key, const char *name = nullptr)
        {
            return SetData<T>(
                new SubscribeBasic<T>(),
                MakeStorageDelegate(default_value, nvs_key),
                new SetAlways<T>(),
                get_name(nvs_key, name));
        }

        template <typename T>
        SetData<T> MakeSetDifferent(const T default_value, const char *nvs_key, const char *name = nullptr)
        {
            return SetData<T>(
                new SubscribeBasic<T>(),
                MakeStorageDelegate(default_value, nvs_key),
                new SetDifferent<T>(),
                get_name(nvs_key, name));
        }
    };
};
