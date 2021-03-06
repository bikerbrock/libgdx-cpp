/*
 *  Copyright 2011 Aevum Software aevum @ aevumlab.com
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  @author Victor Vicente de Carvalho victor.carvalho@aevumlab.com
 *  @author Ozires Bortolon de Faria ozires@aevumlab.com
 *  @author aevum team
 */

#ifndef GDX_CPP_UTILS_JSONITEM_HPP
#define GDX_CPP_UTILS_JSONITEM_HPP

#include <tr1/memory>
#include <tr1/unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include <cassert>
#include <stdexcept>
#include "gdx-cpp/utils/Aliases.hpp"

namespace gdx_cpp {

namespace utils {

enum json_item_type {
    json_string,
    json_int,
    json_bool,
    json_list,
    json_json,
    json_float,
    json_null
};

class JsonValue {
public:
    typedef ref_ptr_maker< JsonValue >::type ptr;
    typedef std::tr1::unordered_map< std::string, JsonValue* > item_map;
    typedef std::vector< JsonValue* > array;

    JsonValue(int val) ;
    JsonValue(int* val) ;
    JsonValue(float val) ;
    JsonValue(float* val) ;
    JsonValue(bool val) ;
    JsonValue(bool* val) ;
    JsonValue(const std::string& val) ;
    JsonValue(std::string* val) ;
    JsonValue(const item_map& val) ;
    JsonValue(item_map* val) : item_type(json_json), item_val(std::tr1::shared_ptr<void>( val )) {}
    JsonValue(const array& val) ;
    JsonValue(array* val) : item_type(json_list) , item_val(std::tr1::shared_ptr<void>( val )) {}
    JsonValue(const JsonValue& other) : item_val(other.item_val), item_type(other.item_type)
    {
    }
   
    JsonValue& operator = (const std::string& other) {
        if (item_type == json_null) {
            this->item_val = std::tr1::shared_ptr<void>(new std::string(other));
            this->item_type = json_string;
        } else {
            ((std::string&)*this) = other;
        }
        return *this;
    }

    JsonValue& operator = (const char* other) {
        if (item_type == json_null) {
            this->item_val = std::tr1::shared_ptr<void>(new std::string(other));
            this->item_type = json_string;
        } else {
            ((std::string&)*this) = other;
        }
        return *this;
    }
    
    JsonValue& operator = (int other) {
        if (item_type == json_null) {
            this->item_val = std::tr1::shared_ptr<void>(new int(other));
            this->item_type = json_int;
        } else {
            ((int&)*this) = other;
        }
        return *this;
    }

    JsonValue& operator = (bool other) {
        if (item_type == json_null) {
            this->item_val = std::tr1::shared_ptr<void>(new bool(other));
            this->item_type = json_bool;
        } else {
            ((bool&)*this) = other;
        }
        return *this;
    }

    JsonValue& operator = (float other) {
        if (item_type == json_null) {
            this->item_val = std::tr1::shared_ptr<void>(new float(other));
            this->item_type = json_float;
        } else {
            ((float&)*this) = other;
        }
        return *this;
    }

    JsonValue& operator = (const array& other) {
        if (item_type == json_null) {
            this->item_val = std::tr1::shared_ptr<void>(new array(other));
            this->item_type = json_list;
        } else {
            ((array&)*this) = other;
        }
        return *this;
    }

    JsonValue& operator = (const JsonValue& other) {
        this->item_type = other.item_type;
        this->item_val = other.item_val;

        return *this;
    }

    JsonValue& operator + (const JsonValue& other) {
        return *this += other;
    }

    JsonValue& operator += (const JsonValue& other) ;
    
    
    template <typename T>
    operator T&() const {
        return *(T*)item_val.get();
    }
    
    template <typename T>
    operator T&() {
#ifdef GDX_DEBUGGING
        switch(item_type) {
            case json_int:
                assert(typeid(int) == typeid(T));
                break;
            case json_float:
                assert(typeid(float) == typeid(T));
                break;
            case json_list:
                assert(typeid(array) == typeid(T));
                break;
            case json_json:
                assert(typeid(item_map) == typeid(T));
                break;
            case json_bool:
                assert(typeid(bool) == typeid(T));
                break;
            case json_string:
                assert(typeid(std::string) == typeid(T));
                break;
            case json_null:
                throw std::runtime_error("casting a null json value to another type is forbidden");
            default:
                break;
        }
#endif
        if (this->item_type == json_null) {
            *this = T(0);
        }

        return *(T*)item_val.get();
    }

    template <typename T>
    operator const T&() {
        #ifdef GDX_DEBUGGING
        switch(item_type) {
            case json_int:
                assert(typeid(int) == typeid(T));
                break;
            case json_float:
                assert(typeid(float) == typeid(T));
                break;
            case json_list:
                assert(typeid(array) == typeid(T));
                break;
            case json_json:
                assert(typeid(item_map) == typeid(T));
                break;
            case json_bool:
                assert(typeid(bool) == typeid(T));
                break;
            case json_string:
                assert(typeid(std::string) == typeid(T));
                break;
            case json_null:
                throw std::runtime_error("casting a null json value to another type is forbidden");
            default:
                break;
        }
        #endif
        
        if (this->item_type == json_null) {
            throw std::runtime_error("Trying to deserialize a null json value");
        }
        
        return *(T*)item_val.get();
    }

    const JsonValue& operator[](const std::string& name) const;
    JsonValue& operator[](const std::string& name);
    
    JsonValue& at(int index);
    JsonValue& at(int index) const;
    
    JsonValue() ;

    void toString(std::ostream& out, bool prettyPrint = false) const;

    item_map::const_iterator begin() ;
    item_map::const_iterator end() ;

    size_t count() const;

    bool contains(const std::string& name) const;

    static JsonValue* newNodeAsJson(JsonValue::item_map* preset = NULL) {
        return new JsonValue(preset == NULL ? new JsonValue::item_map : preset);
    }

    static JsonValue* newNodeAsInt(int* val = NULL) {
        return new JsonValue(val == NULL ? new int(0) : val);
    }

    static JsonValue* newNodeAsArray(JsonValue::array* preset = NULL) {
        return new JsonValue(preset == NULL ? new JsonValue::array : preset);
    }

    static JsonValue* newNodeAsString(std::string* preset = NULL) {
        return new JsonValue(preset == NULL ? new std::string : preset);
    }

    static JsonValue* newNodeAsBool(bool* preset = NULL) {
        return new JsonValue(preset == NULL ? new bool : preset);
    }

    static JsonValue* newNodeAsFloat(float* preset = NULL) {
        return new JsonValue(preset == NULL ? new float : preset);
    }

    json_item_type getType() const {
        return (json_item_type) item_type;
    }

    ~JsonValue() ;

    friend std::ostream& operator<< (std::ostream &out, JsonValue& item);

private:

    void toString(std::ostream& out, bool prettyPrint, int ident) const;
    
    friend class JsonReader;
    std::tr1::shared_ptr<void> item_val;
    char item_type;
};

}

}

#endif // GDX_CPP_UTILS_JSONITEM_HPP
