#pragma once

#include <exception>
#include <string>

namespace json {
    /** Abstract class used to store json values */
    struct Value {
        virtual void *get() = 0;
        virtual Value *copy() = 0;
        virtual ~Value() {};
    };

    template<typename T> T as(void *p) {
        return *static_cast<T*>(p);
    }
    template<> int as(void *p);
    template<> short as(void *p);
    template<> long as(void *p);
    template<> double as(void *p);

    /** Contains a (possibly nested) json value */
    class Node {
        Value *v;
        public:
            Node(Value *v = nullptr);
            Node(const Node &n);
            Node(Node &&n);
            ~Node();
            Node &operator=(const json::Node &n);
            Node &operator=(json::Node &&n);
            /** Search for key in object*/
            Node &operator[](std::string key);
            /** Search for index in array*/
            Node &operator[](int i);
            /** Cast value stored in node to a given type
             * @throws May throw an error if cast to the wrong type
            */
            template<typename T> T as() {
                return json::as<T>(v->get());
            }
    };

    /** Exception for invalid json files */
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

    /** Load as json file from given file
     * @return json::Node that contains main object of the file
     * @throws json::READ_ERROR if the given file is an invalid json file
     */
    Node ReadJSON(std::string file_name);
}