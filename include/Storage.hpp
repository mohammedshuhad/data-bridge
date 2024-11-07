#pragma once
#include "NvsHandler.hpp"

namespace DataBridge
{
    template <class T>
    class BaseData;

    template <typename T>
    class StorageDelegate
    {
    public:
        virtual ~StorageDelegate() = default;
        virtual void set_default(T &value) const = 0;
        virtual bool load_or_reset(T &value) const = 0;
        virtual void store(const BaseData<T> &object) = 0;
        virtual void reset(T &value) const = 0;
    };

    template <typename T>
    class StorageBasic : public StorageDelegate<T>, public Helper::NvsHandler::Block
    {
    public:
        StorageBasic(const T default_value, Helper::NvsHandler *nvs_handler, const char *key)
            : default_value(default_value), nvs_handler(nvs_handler), key(key), object(nullptr) {}

        virtual ~StorageBasic()
        {
            nvs_handler->unsub(key);
        }

        virtual void set_default(T &value) const override
        {
            value = default_value;
        }

        virtual bool load_or_reset(T &value) const override
        {
            if (!nvs_handler->load(key, value))
            {
                nvs_handler->reset(key);
            }
            return true;
        }

        virtual void store(const BaseData<T> &object) override final
        {
            this->object = &object;
            nvs_handler->sub(key, this);
        }

        virtual void reset(T &value) const override final
        {
            nvs_handler->reset(key);
            value = default_value;
        }

        virtual void commit(Helper::NvsHandler *handler, const char *key) const override final
        {
            handler->store(key, object->get());
        }

    private:
        const T default_value;
        Helper::NvsHandler *nvs_handler;
        const char *key;
        const BaseData<T> *object;
    };

    template <typename T>
    class StorageNone : public StorageDelegate<T>
    {
    public:
        StorageNone(const T default_value) : 
        default_value(default_value) {}

        virtual ~StorageNone() = default;

        virtual void set_default(T &value) const override final
        {
            value = default_value;
        }

        virtual bool load_or_reset(T &value) const override final
        {
            return false;
        }

        virtual void store(const BaseData<T> &object) override final
        {
            // Do nothing
        }

        virtual void reset(T &value) const override final
        {
            value = default_value;
        }

    private:
        const T default_value;
    };
};