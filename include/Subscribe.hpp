#pragma once
#include <functional>

namespace DataBridge
{
    template <typename T>
    class SubscribeDelegate
    {
    public:
        using sub_id_t = size_t;
        using sub_cb_t = std::function<void(const T &)>;
        virtual ~SubscribeDelegate() = default;
        virtual sub_id_t sub(const sub_cb_t &cb) = 0;
        virtual void unsub(sub_id_t id) = 0;
        virtual void notify(const T &value) const = 0;
    };

    template <typename T>
    class SubscribeBasic : public SubscribeDelegate<T>
    {
    public:
        using sub_id_t = SubscribeDelegate<T>::sub_id_t;
        using sub_cb_t = SubscribeDelegate<T>::sub_cb_t;

        SubscribeBasic() : subs() {}
        virtual ~SubscribeBasic() = default;

        virtual sub_id_t sub(const sub_cb_t &cb) override final
        {
            for (size_t i = 0; i < subs.size(); i++)
            {
                if (subs[i] == nullptr)
                {
                    subs[i] = cb;
                    return i;
                }
            }

            size_t id = subs.size();
            subs.push_back(cb);
            return id;
        }

        virtual void unsub(sub_id_t id) override final
        {
            if (id < subs.size())
            {
                subs[id] = nullptr;
            }
        }

        virtual void notify(const T &value) const override final
        {
            for (const auto &sub : subs)
            {
                if (sub != nullptr)
                {
                    sub(value);
                }
            }
        }

    private:
        std::vector<sub_cb_t> subs;
    };
};