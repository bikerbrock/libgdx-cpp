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

#include "JsonValue.hpp"

#include <cassert>
#include <algorithm>

using namespace gdx_cpp::utils;

JsonValue::JsonValue(int val) : item_type(json_int) {
    item_val = std::tr1::shared_ptr<void>( new int(val) );
}

JsonValue::JsonValue(int* val) : item_val(std::tr1::shared_ptr<void>(val)), item_type(json_int) {

}

JsonValue::JsonValue(float val) : item_type(json_float) {
    item_val = std::tr1::shared_ptr<void>( new int(val) );
}

JsonValue::JsonValue(float* val) : item_val(std::tr1::shared_ptr<void>(val)), item_type(json_float)
{
}

JsonValue::JsonValue(bool* val) : item_type(json_bool) , item_val(std::tr1::shared_ptr<void>( val )) {
}

JsonValue::JsonValue(const std::string& val)
        : item_type(json_string)
{
    item_val = std::tr1::shared_ptr<void>( new std::string(val) );
}

JsonValue::JsonValue(std::string* val)
        : item_type(json_string), item_val(std::tr1::shared_ptr<void>( val ))
{
}

JsonValue::JsonValue(const item_map& val) : item_type(json_json) {
    item_val = std::tr1::shared_ptr<void>( new item_map(val) );
}

JsonValue::JsonValue(const array& val) : item_type(json_list) {
    item_val = std::tr1::shared_ptr<void>( new array(val) );
}

JsonValue::JsonValue(bool val) : item_type(json_bool) , item_val(std::tr1::shared_ptr<void>(new bool(val))) {
}

gdx_cpp::utils::JsonValue& gdx_cpp::utils::JsonValue::at(int idx)
{
    if (this->item_type == json_null) {
        array* new_array = new array;
        this->item_val = std::tr1::shared_ptr<void>(new_array);
        this->item_type = json_list;
    }
    
    assert(item_type == json_list);
    array& thisAsArray = *this;

    if (idx >= thisAsArray.size()) {
        thisAsArray.resize(idx + 1);
    }
    
    if (thisAsArray[idx] == NULL) {
        thisAsArray[idx] = new JsonValue;
    }
    
#ifdef GDX_DEBUGGING
    array& asArray = *this;
    assert( asArray.size() > idx );
    return *(asArray[idx]);
#else
    return *((array&)*this)[idx];
#endif
}

gdx_cpp::utils::JsonValue& gdx_cpp::utils::JsonValue::at(int index) const
{
    assert(item_type == json_list);
    array& thisAsArray = *this;
    assert(thisAsArray.size() > index);

    return *thisAsArray[index];
}

const gdx_cpp::utils::JsonValue& gdx_cpp::utils::JsonValue::operator[](const std::string& name) const
{
    assert(item_type == json_json);
    item_map& thisAsMap = *this;
    
    if (thisAsMap.count(name) == 0) {
        throw std::runtime_error("Missing field named [" + name + "]");
    }
    
    return *(thisAsMap[name]);
}

JsonValue& JsonValue::operator[](const std::string& name)
{
    {
        if (this->item_type == json_null) {
            this->item_val = std::tr1::shared_ptr<void>(new item_map);
            this->item_type = json_json;
        }
    }
    
    assert(item_type == json_json);

    item_map& thisAsMap = *this;
        
    if (thisAsMap.count(name) == 0) {
        thisAsMap[name] = new JsonValue;
    }
    
    return *thisAsMap[name];
}

size_t JsonValue::count() const
{
   if (item_type == json_json) {
       return ((item_map&)*this).size();
   } else if (item_type == json_list) {
       return ((array&)*this).size();
   } else if (item_type == json_null) {
        return 0;
   }
   
   return 1;
}

bool JsonValue::contains(const std::string& name) const
{
    assert(item_type == json_json);
    return ((item_map&) *this).count(name) > 0;
}

JsonValue::~JsonValue() {
    if (item_type == json_list) {
        array& lst = *this;

        array::iterator it = lst.begin();
        array::iterator end = lst.end();

        for (; it != end; ++it) {
            delete *it;
        }
    } else if (item_type == json_json) {
        item_map& map = *this;

        item_map::iterator it = map.begin();
        item_map::iterator end = map.end();

        for (; it != end; ++it) {
            delete it->second;
        }
    }
}

JsonValue::item_map::const_iterator JsonValue::begin() {
    assert(item_type == json_json);
    return ((item_map&)*this).begin();
}

JsonValue::item_map::const_iterator JsonValue::end() {
    assert(item_type == json_json);
    return ((item_map&)*this).end();
}

JsonValue::JsonValue() : item_type(json_null)
{
}

void gdx_cpp::utils::JsonValue::toString(std::ostream& out, bool prettyPrint, int ident) const
{
    std::string identLevel(ident, '\t');
    
    switch (item_type) {
        case json_bool:
            out << ((bool)*this ? "true" : "false");
            break;
        case json_float:
            out << (float) *this;
            break;
        case json_int:
            out << (int) *this;
            break;
        case json_list: {
            JsonValue::array::iterator iit = ((JsonValue::array&)*this).begin();
            JsonValue::array::iterator eend = ((JsonValue::array&)*this).end();
            
            out << "[";
            if (prettyPrint) out << std::endl;
            
            for (; iit != eend;) {
                out << identLevel;
                
                (*iit)->toString(out, prettyPrint, ident + 1);

                if (++iit == eend)
                    break;

                out << ",";
                if (prettyPrint) out << std::endl;
            }
            
            out << "]";
            if (prettyPrint) out << std::endl;
        }
        break;
        case json_json: {
            
            JsonValue::item_map::const_iterator it =  ((item_map&)*this).begin();
            JsonValue::item_map::const_iterator end = ((item_map&)*this).end();
            
            out << "{";
            if (prettyPrint) out << std::endl;
            
            for (; it != end;) {
                out << identLevel;
                
                out << '"' << it->first << "\" : ";
                it->second->toString(out, prettyPrint, ident + 1);
                
                if (++it == end) {
                    if (prettyPrint) out << std::endl;
                    break;
                }
                
                out << ",";
                if (prettyPrint) out << std::endl;
            }
            if (prettyPrint) out << identLevel.substr(0, identLevel.length() - 1);
            out << " }";
        }
        break;
        case json_null:
            out << "null";
            break;
        case json_string:
            out << "\"" << (std::string&) *this << "\"";
            break;
    };
}


void gdx_cpp::utils::JsonValue::toString(std::ostream& out, bool prettyPrint) const
{
    toString(out, prettyPrint, 1);    
}

//sob... c++ sucks A LOT sometimes...
namespace gdx_cpp {
namespace utils {

std::ostream& operator<< (std::ostream& out, gdx_cpp::utils::JsonValue& item) {
    item.toString(out);

    return out;
}

JsonValue& JsonValue::operator+=(const gdx_cpp::utils::JsonValue& other) {
    assert(item_type == json_json && other.item_type == json_json);

    item_map& thisAsMap = *this;
    const item_map& otherAsMap = other;

    item_map::const_iterator it = otherAsMap.begin();
    item_map::const_iterator end = otherAsMap.end();

    for (; it != end; ++it) {
        JsonValue* newValue = new JsonValue;
        *newValue = *it->second;
        thisAsMap[it->first] = newValue;
    }

    return *this;
}
}
}

