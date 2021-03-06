/**
* Icarus - A multi-platform C++ server
*
* Copyright 2016 Alex "Quackster" Miller
*
* Licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
* (see https://creativecommons.org/licenses/by-nc-sa/4.0/, or LICENSE.txt for a full license
*/
#pragma once
#include "boot/Icarus.h"
#include "communication/outgoing/MessageComposer.h"

class NavigatorMetaDataComposer : public MessageComposer {

public:
    NavigatorMetaDataComposer() { }

    const Response compose() const {

        auto parentTabs = Icarus::getGame()->getNavigatorManager()->getParentTabs();
        Response response = this->createResponse();
        response.writeInt((int)parentTabs.size());

        for (auto tab : parentTabs) {
            response.writeString(tab->getTabName());
            response.writeInt(0);
        } 
        return response;
    }

    const int getHeader() const {
        return Outgoing::NavigatorMetaDataComposer;
    }
};