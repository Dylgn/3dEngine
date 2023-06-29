#pragma once

#include <exception>
#include <string>

namespace json {
    struct Value {
        virtual void *get() = 0;
        virtual Value *copy() = 0;
        virtual ~Value() {};
    };

    class Node {
        Value *v;
        public:
            Node(Value *v = nullptr);
            Node(const Node &n);
            Node(Node &&n);
            ~Node();
            Node &operator=(const json::Node &n);
            Node &operator=(json::Node &&n);
            Node &operator[](std::string key);
            Node &operator[](int i);
            template<typename T> T as() {
                return *static_cast<T*>(v->get());
            }
    };

    struct READ_ERROR: public std::exception {
        std::string msg;
        READ_ERROR(std::string m = "no reason given") {
            msg = std::string("Error when reading json file : ") + m;
        }
        ~READ_ERROR() override {}
        const char *what() const noexcept {
            return msg.c_str();
        }
    };

    Node ReadJSON(std::string file_name);
}