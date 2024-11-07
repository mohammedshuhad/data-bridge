#pragma once
#include "Subscribe.hpp"
#include "Storage.hpp"
#include <iostream>

namespace DataBridge
{
    class BaseDataGeneric
    {
    public:
        BaseDataGeneric(const char *name) : name(name) {}
        virtual void print(uint32_t indent_depth = 0) = 0;
        virtual void log_on_sub(bool set = true) = 0;
        virtual void mute_sub() = 0;
        virtual void unmute_sub() = 0;
        virtual void set_default(void) = 0;
        virtual void reset(void) = 0;
        virtual void load_or_reset(void) = 0;

    protected:
        const char *name;
    };

    template <typename T>
    class BaseData : public BaseDataGeneric
    {
    public:
        using sub_id_t = typename SubscribeDelegate<T>::sub_id_t;
        using sub_cb_t = typename SubscribeDelegate<T>::sub_cb_t;

        BaseData(SubscribeDelegate<T> *sub_d, StorageDelegate<T> *store_d, const char *name) : BaseDataGeneric(name), sub_d(sub_d), store_d(store_d)
        {
            if (!store_d->load_or_reset(value))
            {
                store_d->set_default(value);
            }
        }

        BaseData(const BaseData<T> &) = delete;
        BaseData(BaseData<T> &&) = default;

        virtual void print(uint32_t indent_depth = 0) override
        {
            for (int i = 0; i < indent_depth; i++)
                std::cout << "  ";
            if (name)
                std::cout << name << ": ";
            std::cout << value << '\n';
        }

        virtual void log_on_sub(bool set = true) override
        {
            en_logging = set;
        }

        virtual const T &get(void) const final
        {
            return value;
        }

        virtual void set_default(void) override final
        {
            store_d->set_default(value);
            notify(value);
        }

        virtual void reset(void) override final
        {
            store_d->reset(value);
            notify(value);
        }

        virtual void load_or_reset(void) override final
        {
            if (!store_d->load_or_reset(value))
            {
                notify(value);
            }
        }

        virtual sub_id_t sub(sub_cb_t sub_cb, bool immediate = false) final
        {
            if (immediate)
            {
                sub_cb(value);
            }
            return sub_d->sub(sub_cb);
        }

        virtual void unsub(sub_id_t sub_id) final
        {
            sub_d->unsub(sub_id);
        }

        virtual void mute_sub() override final
        {
            muted = true;
        }

        virtual void unmute_sub() override final
        {
            if (muted)
            {
                muted = false;
                notify(value);
            }
        }

    protected:
        void notify(const T &next) const
        {
            if (muted)
                return;
            if (en_logging)
            {
                if (name)
                    std::cout << name << ": ";
                std::cout << value << "->" << next << '\n';
            }
            sub_d->notify(next);
        }

        void store(void)
        {
            store_d->store(*this);
        }
        T value;

    private:
        bool muted = false;
        bool en_logging = false;

        SubscribeDelegate<T> *sub_d;
        StorageDelegate<T> *store_d;
    };
};