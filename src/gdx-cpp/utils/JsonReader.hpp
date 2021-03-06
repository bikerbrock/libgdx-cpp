
/*
    Copyright 2011 Aevum Software aevum @ aevumlab.com

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

    @author Victor Vicente de Carvalho victor.carvalho@aevumlab.com
    @author Ozires Bortolon de Faria ozires@aevumlab.com
*/

#ifndef GDX_CPP_UTILS_JSONREADER_HPP_
#define GDX_CPP_UTILS_JSONREADER_HPP_

#include "JsonValue.hpp"

#include <string>
#include <list>
#include "gdx-cpp/files/FileHandle.hpp"

namespace gdx_cpp {
namespace utils {

class JsonReader {
public:
    JsonReader();
    
    static JsonValue::ptr parse (const std::string& json);
    static JsonValue::ptr parse (const gdx_cpp::files::FileHandle& file);
    static JsonValue::ptr parse (const char* data, int offset, int length);

protected:
    static void startObject (const std::string& name);
    static void startArray (const std::string& name);

    static void pop ();

    static void string (const std::string& name,  const std::string& value);
    static void number (const std::string& name,  float value);
    static void number (const std::string& name,  int value);
    static void boolean (const std::string& name, bool value);

private:
    static const char _json_actions[61];
    static const short _json_key_offsets[92];
    static const char _json_trans_keys[606];
    static const char _json_single_lengths[92];
    static const char _json_range_lengths[92];
    static const short _json_index_offsets[591];
    static const char _json_trans_targs[591];
    static const char _json_trans_actions[591];
    static const char _json_eof_actions[92];

    static const int json_start;
    static const int json_first_final;
    static const int json_error;

    static const int json_en_object;
    static const int json_en_array;
    static const int json_en_main;
    
    static void set (const std::string& name, gdx_cpp::utils::JsonValue* value);
    
    static std::string unescape (const std::string& value);

    static JsonValue* root;
    static JsonValue* current;

    static std::list< JsonValue* > elements;
};

} // namespace gdx_cpp
} // namespace utils

#endif // GDX_CPP_UTILS_JSONREADER_HPP_
