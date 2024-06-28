/*
    InstanceManager.h - manage lifecycle of Synth-Engine instances

    Copyright 2024,  Ichthyostega

    Based on existing code from main.cpp
    Copyright 2009-2011, Alan Calvert
    Copyright 2014-2021, Will Godfrey & others

    This file is part of yoshimi, which is free software: you can redistribute
    it and/or modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of the License,
    or (at your option) any later version.

    yoshimi is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  See the GNU General Public License (version 2
    or later) for more details.

    You should have received a copy of the GNU General Public License
    along with yoshimi.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef INSTANCE_MANAGER_H
#define INSTANCE_MANAGER_H

//#include "globals.h"
//#include "Misc/FormatFuncs.h"
#include "Misc/CmdOptions.h"
//#include <iostream>
//#include <iomanip>
//#include <fstream>
#include <functional>
#include <memory>

class SynthEngine;

//using std::cout;
//using std::endl;

class InstanceManager
{

    class Instance;
    class SynthGroom;

        std::unique_ptr<SynthGroom> groom;

        CmdOptions cmdOptions;

         // can not be created nor copied or moved...
        InstanceManager();
    public:
       ~InstanceManager();
        InstanceManager(InstanceManager&&)                 = delete;
        InstanceManager(InstanceManager const&)            = delete;
        InstanceManager& operator=(InstanceManager&&)      = delete;
        InstanceManager& operator=(InstanceManager const&) = delete;

        /** Access: Meyer's Singleton */
        static InstanceManager& get()
        {
            static InstanceManager singleton{};
            return singleton;
        }

        CmdOptions const& getCmdOptions()
        {
            return cmdOptions;
        }

        /** Event handling loop during regular operation */
        void performWhileActive(std::function<void(SynthEngine&)> handleEvents);
};


#endif /*INSTANCE_MANAGER_H*/