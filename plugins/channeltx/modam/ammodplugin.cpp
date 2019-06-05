///////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2016 Edouard Griffiths, F4EXB                                   //
//                                                                               //
// This program is free software; you can redistribute it and/or modify          //
// it under the terms of the GNU General Public License as published by          //
// the Free Software Foundation as version 3 of the License, or                  //
// (at your option) any later version.                                           //
//                                                                               //
// This program is distributed in the hope that it will be useful,               //
// but WITHOUT ANY WARRANTY; without even the implied warranty of                //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                  //
// GNU General Public License V3 for more details.                               //
//                                                                               //
// You should have received a copy of the GNU General Public License             //
// along with this program. If not, see <http://www.gnu.org/licenses/>.          //
///////////////////////////////////////////////////////////////////////////////////

#include <QtPlugin>
#include "plugin/pluginapi.h"

#ifndef SERVER_MODE
#include "ammodgui.h"
#endif
#include "ammod.h"
#include "ammodplugin.h"

const PluginDescriptor AMModPlugin::m_pluginDescriptor = {
    QString("AM Modulator"),
    QString("4.5.2"),
    QString("(c) Edouard Griffiths, F4EXB"),
    QString("https://github.com/f4exb/sdrangel"),
    true,
    QString("https://github.com/f4exb/sdrangel")
};

AMModPlugin::AMModPlugin(QObject* parent) :
    QObject(parent),
    m_pluginAPI(0)
{
}

const PluginDescriptor& AMModPlugin::getPluginDescriptor() const
{
    return m_pluginDescriptor;
}

void AMModPlugin::initPlugin(PluginAPI* pluginAPI)
{
	m_pluginAPI = pluginAPI;

	// register AM modulator
	m_pluginAPI->registerTxChannel(AMMod::m_channelIdURI, AMMod::m_channelId, this);
}

#ifdef SERVER_MODE
PluginInstanceGUI* AMModPlugin::createTxChannelGUI(
        DeviceUISet *deviceUISet,
        BasebandSampleSource *txChannel)
{
    return 0;
}
#else
PluginInstanceGUI* AMModPlugin::createTxChannelGUI(DeviceUISet *deviceUISet, BasebandSampleSource *txChannel)
{
	return AMModGUI::create(m_pluginAPI, deviceUISet, txChannel);
}
#endif

BasebandSampleSource* AMModPlugin::createTxChannelBS(DeviceAPI *deviceAPI)
{
    return new AMMod(deviceAPI);
}

ChannelAPI* AMModPlugin::createTxChannelCS(DeviceAPI *deviceAPI)
{
    return new AMMod(deviceAPI);
}

