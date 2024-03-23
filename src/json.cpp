#include <map>
#include <memory>
#include <vector>
#include <fstream>
#include <cstring>
#include "json.hpp"

typedef std::map<std::string, std::shared_ptr<json::Node>> Map;
typedef std::pair<std::string, std::shared_ptr<json::Node>> Map_KV;
typedef std::shared_ptr<json::Node> Node_Ptr;
typedef std::vector<Node_Ptr> Node_Ptr_Vec;

namespace {
    template<typename T> struct SubValue : public json::Value {
        T *v;
        SubValue(T v): v{new T{v}} {}
        SubValue(T *v): v{v} {}
        ~SubValue() override {
            delete v;
            v = nullptr;
        }
        Value *copy() override {
            return new SubValue<T>(*v);
        }
        void *get() override { return v; }
    };

    void ThrowIfNotNext(std::ifstream &f, char c) {
        char t;
        f >> t;
        if (t != c) throw json::READ_ERROR{(std::string("Expected: ") + c) + std::string(" as next char but got: ") + t};
    }
    
    Node_Ptr ReadValue(std::ifstream &f);

    std::string ReadString(std::ifstream &f) {
        char c;
        ThrowIfNotNext(f, '\"');
        std::string s = "";
        while (f >> c, c != '\"') {
            if (c == '\\') {
                f >> c;
                switch (c) {
                    case '\"':
                    case '\\':
                        s.push_back(c);
                        break;
                    case 'b':
                        s.push_back('\b');
                        break;
                    case 'f':
                        s.push_back('\f');
                        break;
                    case 'n':
                        s.push_back('\n');
                        break;
                    case 'r':
                        s.push_back('\r');
                        break;
                    case 't':
                        s.push_back('\t');
                        break;
                    default:
                        throw json::READ_ERROR{std::string("Unrecognized Escape Sequence: \\") + c};
                }
            }
            s.push_back(c);
        }
        return s;
    }

    Node_Ptr_Vec ReadArray(std::ifstream &f) {
        char c;
        ThrowIfNotNext(f, '[');

        Node_Ptr_Vec values;
        do {
            values.push_back(ReadValue(f));
        } while (f>> c, c == ',');

        if (c != ']') throw json::READ_ERROR{(std::string("Expected: ") + ']') + std::string(" as next char but got: ") + c};
        return values;
    }

    Node_Ptr ReadObject(std::ifstream &f) {
        char c;
        ThrowIfNotNext(f, '{');

        Map object;
        do {
            std::string s = ReadString(f);
            ThrowIfNotNext(f, ':');
            object.insert(Map_KV(s, ReadValue(f)));
        } while (f >> c, c == ',');

        if (c != '}') throw json::READ_ERROR{(std::string("Expected: ") + ']') + std::string(" as next char but got: ") + c};
        return std::make_shared<json::Node>(new SubValue<Map>{object});
    }

    Node_Ptr ReadValue(std::ifstream &f) {
        char c;
        f >> c;
        f.unget();
        switch (c) {
            case '{': { // Object
                return ReadObject(f);
                break;
            }
            case '\"': { // String
                return std::make_shared<json::Node>(new SubValue<std::string>{ReadString(f)});
                break;
            }
            case '[': { // Array
                return std::make_shared<json::Node>(new SubValue<Node_Ptr_Vec>{ReadArray(f)});
                break;
            }
            case 't': { // True
                char remove[4];
                f.get(remove, 5);
                if (strcmp(remove, "true") != 0) throw json::READ_ERROR{(std::string("Expected: true")) + std::string(" as next 4 characters but got: ") + remove};

                return std::make_shared<json::Node>(new SubValue<bool>{true});
            }
            case 'f': { // False
                char remove2[5];
                f.get(remove2, 6);
                if (strcmp(remove2, "false") != 0) throw json::READ_ERROR{(std::string("Expected: false")) + std::string(" as next 5 characters but got: ") + remove2};

                return std::make_shared<json::Node>(new SubValue<bool>{false});
            }
            case 'n': { // Null
                char remove3[4];
                f.get(remove3, 5);
                if (strcmp(remove3, "null") != 0) throw json::READ_ERROR{(std::string("Expected: null")) + std::string(" as next 4 characters but got: ") + remove3};

                return std::make_shared<json::Node>(new SubValue<void*>{(void*)nullptr});
                break;
            }
            default: {
                if (c == '-' || isdigit(c)) { // Number
                    float num;
                    f >> num;
                    return std::make_shared<json::Node>(new SubValue<float>{num});
                } else throw json::READ_ERROR{std::string("Expected to read value but first character read was ") + c};
            }
        }
    }

    template<typename T> T asNum(void *p) {
        return static_cast<T>(json::as<float>(p));
    }
}

template<> int json::as(void *p) {
    return asNum<int>(p);
}

template<> short json::as(void *p) {
    return asNum<short>(p);
}

template<> long json::as(void *p) {
    return asNum<long>(p);
}

template<> double json::as(void *p) {
    return asNum<double>(p);
}

json::Node::Node(Value *v): v{v} {}

json::Node::Node(const Node &n): v{n.v->copy()} {}

json::Node::Node(Node &&n): v{n.v} {
    n.v = nullptr;
}

json::Node::~Node() { delete v; v = nullptr; }

json::Node &json::Node::operator=(const json::Node &n) {
    v = n.v->copy();
    return *this;
}

json::Node &json::Node::operator=(json::Node &&n) {
    this->v = n.v;
    n.v = nullptr;
    return *this;
}

json::Node &json::Node::operator[](std::string key) {
    return *this->as<Map>()[key];
}

json::Node &json::Node::operator[](int i) {
    return *this->as<Node_Ptr_Vec>()[i];
}

json::Node json::ReadJSON(std::string file_name) {
    std::ifstream f{file_name};
    if (!f.is_open()) throw READ_ERROR{std::string("Could not open file with path: ") + file_name};

    return *ReadObject(f);
}