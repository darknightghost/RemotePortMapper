/*
    Copyright 2017,王思远 <darknightghost.cn@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "./config.h"
#include <map>

using namespace std;

//Element
Config::ConfigElement::ConfigElement(cfg_element_type_t type,
                                     ConfigElement* p_parent)
{

    this->m_type = type;
    this->m_p_parent = p_parent;
    this->m_ref_count = 1;

    if(type == ELEMENT_TYPE_VALUE) {
        this->m_p_value = NULL;

    } else {
        this->m_p_value = new map<std::string, ConfigElement*>();
    }

    return;
}

void Config::ConfigElement::release()
{
    this->m_ref_count--;

    if(this->m_ref_count == 0) {
        //Release element
        if(this->m_type == ELEMENT_TYPE_VALUE) {
            //Free value
            if(this->m_p_value != NULL) {
                delete(std::string*)(this->m_p_value);
            }

        } else {
            //Free all children
            map<std::string, ConfigElement*>* p_child_map =
                (map<std::string, ConfigElement*>*)(this->m_p_value);
            map<std::string, ConfigElement*>::iterator iter;

            for(iter = p_child_map->begin();
                iter != p_child_map->end();
                iter = p_child_map->begin()) {
                ConfigElement* p_element = iter->second;
                p_child_map->erase(iter);
                p_element->release();
            }

            delete p_child_map;
        }

        delete this;
    }

    return;
}

bool Config::ConfigElement::set_value(std::string val)
{
    if(this->m_type != ELEMENT_TYPE_VALUE) {
        return false;
    }

    if(this->m_p_value != NULL) {
        delete(std::string*)(this->m_p_value);
    }

    this->m_p_value = new std::string(val);

    return true;
}

bool Config::ConfigElement::set_value(char* value)
{
    std::string *s = new std::string(value);
    bool ret = this->set_value(*s);
    delete s;
    return ret;
}

/*
std::string* Config::ConfigElement::get_value();
bool Config::ConfigElement::add_child(cfg_element_type_t type, std::string name);
bool Config::ConfigElement::add_child(cfg_element_type_t type, char* name);
*/

bool Config::ConfigElement::remove_child(std::string key)
{
    //Get iterator
    map<std::string, ConfigElement*>* p_child_map =
        (map<std::string, ConfigElement*>*)(this->m_p_value);
    map<std::string, ConfigElement*>::iterator pos = p_child_map->find(key);

    if(pos == p_child_map->end()) {
        return false;
    }

    //Remove key
    ConfigElement* val = pos->second;
    p_child_map->erase(pos);
    val->release();

    return true;
}

bool Config::ConfigElement::remove_child(char* key)
{
    std::string *s = new std::string(key);
    bool ret = this->remove_child(*s);
    delete s;
    return ret;
}
/*
ConfigElement* Config::ConfigElement::get_child(std::string key);
ConfigElement* Config::ConfigElement::get_child(char* key);
std::list<std::string>* Config::ConfigElement::list_children();
Config::ConfigElement::~ConfigElement();
*/
//Config
Config* Config::p_instance = NULL;
