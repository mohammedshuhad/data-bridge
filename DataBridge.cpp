#include "DataBridge.hpp"

using namespace DataBridge;

static Helper::NvsHandler *s_nvs_handle = nullptr;

Helper::NvsHandler *Factory::GetNvsHandler(void)
{
    if (s_nvs_handle == nullptr)
    {
        s_nvs_handle = new Helper::NvsHandler("storage");
    }
    return s_nvs_handle;
}

const char *Factory::get_name(const char *nvs_key, const char *name)
{
    if (nvs_key && !name)
    {
        return nvs_key;
    }
    return name;
}