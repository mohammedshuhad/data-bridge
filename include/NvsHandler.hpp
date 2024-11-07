#pragma once
#include "nvs_flash.h"
#include <map>
#include <cstring>

namespace DataBridge
{
    namespace Helper
    {
        class NvsHandler
        {
        public:
            class Block
            {
            public:
                virtual ~Block() = default;
                virtual void commit(NvsHandler *handler, const char *key) const = 0;
            };
            NvsHandler(const char *nvs_name);
            ~NvsHandler();

            size_t size(const char *key);

            bool load(const char *key, void *data, size_t data_sz);
            template <typename T>
            bool load(const char *key, T &value)
            {
                return load(key, &value, sizeof(T));
            }

            bool store(const char *key, const void *data, size_t data_sz);
            template <typename T>
            bool store(const char *key, const T &value)
            {
                return store(key, &value, sizeof(T));
            }

            void reset(const char *key);
            void sub(const char *key, Block *block);
            void unsub(const char *key);
            void commit(void);

        private:
            const char *nvs_name;
            nvs_handle_t nvs_handle;

            class CmpKey
            {
            public:
                bool operator()(const char *a, const char *b) const
                {
                    return std::strcmp(a, b) < 0;
                }
            };

            std::map<const char *, Block *, CmpKey> subs;
        };
    };
};