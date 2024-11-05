#pragma once

namespace DataBridge
{
    template <typename T>
    class SetObject
    {
    public:
        virtual ~SetObject() = default;
        virtual void set(const T &value) = 0;
        void set(const T &&value)
        {
            set(value);
        }
    };

    template <typename T>
    class SetDelegate
    {
        public:
        virtual ~SetDelegate() = default;
        virtual bool verify(const T &value, const T &next) const = 0;
        virtual void copy(T &value, const T &next) const = 0;
    };

    template <typename T>
    class SetAlways : public SetDelegate<T>
    {
        public:
        SetAlways() = default;
        virtual ~SetAlways() = default;
        virtual bool verify(const T &value, const T &next) const override
        {
            return true;
        }
        virtual void copy(T &value, const T &next) const override
        {
            value = next;
        }
    };

    template <typename T>
    class SetDifferent : public SetDelegate<T>
    {
        public:
        SetDifferent() = default;
        ~SetDifferent() = default;
        virtual bool verify(const T &value, const T &next) const override
        {
            return value != next;
        }
        virtual void copy(T &value, const T &next) const override
        {
            value = next;
        }
    };
};
