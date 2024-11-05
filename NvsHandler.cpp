#include "NvsHandler.hpp"
#include "esp_log.h"
#include <cstring>

#define TAG "NvsHandler"

using namespace DataBridge::Helper;

NvsHandler::NvsHandler(const char *nvs_name)
    : nvs_name(nvs_name), nvs_handle(0)
{
    esp_err_t err = nvs_open(nvs_name, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to open NVS (%s): %s", nvs_name, esp_err_to_name(err));
    }
}

NvsHandler::~NvsHandler()
{
    esp_err_t err = nvs_commit(nvs_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to commit NVS (%s): %s", nvs_name, esp_err_to_name(err));
    }
    nvs_close(nvs_handle);
}

size_t NvsHandler::size(const char *key)
{
    size_t sz;
    esp_err_t err = nvs_get_blob(nvs_handle, key, nullptr, &sz);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to get size of key (%s): %s", key, esp_err_to_name(err));
        return 0;
    }
    return sz;
}

bool NvsHandler::load(const char *key, void *data, size_t data_sz)
{
    esp_err_t err = nvs_get_blob(nvs_handle, key, data, &data_sz);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to load key (%s): %s", key, esp_err_to_name(err));
        return false;
    }
    return true;
}

bool NvsHandler::store(const char *key, const void *data, size_t data_sz)
{
    esp_err_t err = nvs_set_blob(nvs_handle, key, data, data_sz);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to store key (%s): %s", key, esp_err_to_name(err));
        return false;
    }
    return true;
}

void NvsHandler::reset(const char *key)
{
    subs.erase(key);
    esp_err_t err = nvs_erase_key(nvs_handle, key);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to reset key (%s): %s", key, esp_err_to_name(err));
    }
}

void NvsHandler::sub(const char *key, Block *block)
{
    subs[key] = block;
}

void NvsHandler::unsub(const char *key)
{
    subs.erase(key);
}

void NvsHandler::commit(void)
{
    for (auto &sub : subs)
    {
        sub.second->commit(this, sub.first);
    }

    subs.clear();

    esp_err_t err = nvs_commit(nvs_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to commit NVS (%s): %s", nvs_name, esp_err_to_name(err));
    }
}