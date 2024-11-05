#pragma once

#include "BaseData.hpp"
#include "Set.hpp"

namespace DataBridge
{
    template <typename T>
    class SetData : public BaseData<T>, public SetObject<T>
    {
    public:
        SetData(SubscribeDelegate<T> *sub_d, StorageDelegate<T> *store_d, SetDelegate<T> *set_d, const char *name) : BaseData<T>(sub_d, store_d, name), set_d(set_d) {}
        SetData(const SetData &) = delete;
        SetData(SetData &&) = default;

        virtual void set(const T &next) override final
        {
            if (set_d->verify(BaseData<T>::value, next))
            {
                BaseData<T>::notify(next);
                set_d->copy(BaseData<T>::value, next);
                BaseData<T>::store();
            }
        }

    protected:
        const SetDelegate<T> *getSetDelegate(void) const
        {
            return set_d;
        }

    private:
        SetDelegate<T> *set_d;
    };
};