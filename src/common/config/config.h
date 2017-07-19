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

#pragma once

#include "../types.h"

#include <pthread.h>

#include <map>
#include <string>
#include <list>

//Config
class Config {
        //Node element
        class ConfigElement {

                //Element type
                typedef enum {
                    ELEMENT_TYPE_NODE,
                    ELEMENT_TYPE_VALUE
                } cfg_element_type_t;
                //Member Variables
            private:
                //Element type
                cfg_element_type_t		m_type;

                //Parent
                ConfigElement*			m_p_parent;

                //Element value,string or map
                void*					m_p_value;

                //Reference count
                u32						m_ref_count;

                //Lock
                pthread_mutex_t*		m_p_lock;

                //Member functions
            public:
                ConfigElement(cfg_element_type_t type, ConfigElement* p_parent,
                              pthread_mutex_t* p_lock);

                //Unreference element
                void	release();

                //Set value
                bool	set_value(std::string value);
                bool	set_value(char* value);

                //Get value
                bool	get_value(std::string& ret);

                //Add child
                bool	add_child(cfg_element_type_t type, std::string key);
                bool	add_child(cfg_element_type_t type, char* key);

                //Remove child
                bool	remove_child(std::string key);
                bool	remove_child(char* key);

                //Get child
                ConfigElement*	get_child(std::string path);
                ConfigElement*	get_child(char* path);

                //List children
                std::list<std::string>*	list_children();

                //Lock
                void	lock();

                //Unlock
                void	unlock();

            private:
                ~ConfigElement();
        };

        //Member Variables
    private:
        //Instance
        static Config*	p_instance;

        //Reference count
        u32				m_ref_count;

    protected:
        //Root element
        ConfigElement*	m_p_root;

        //Member functions
    private:
        Config(char* paths[], u32 num);
        ~Config();

        static	pthread_mutex_t		cfg_lock;

    protected:
        //Called to load file, implement by child classes.
        virtual	bool load_file(char* path);

        //Called to save file, implement by child classes.
        virtual bool save_file(char* path);

        //Called by constructor
        virtual	void	on_load();

        //Called by destructor
        virtual	void	on_release();

    public:
        //Load config
        static	bool	load(char* paths[], u32 num);

        //Get config instance
        static	Config*	get_instance();

        //Unreference instance
        void	release();

        //Lock
        void	lock();

        //Unlock
        void	unlock();

};
