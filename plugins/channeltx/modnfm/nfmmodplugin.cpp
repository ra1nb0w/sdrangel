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
#include "nfmmodgui.h"
#endif
#include "nfmmod.h"
#include "nfmmodplugin.h"

const PluginDescriptor NFMModPlugin::m_pluginDescriptor = {
    QString("NFM Modulator"),
    QString("4.5.2"),
    QString("(c) Edouard Griffiths, F4EXB"),
    QString("https://github.com/f4exb/sdrangel"),
    true,
    QString("https://github.com/f4exb/sdrangel")
};

NFMModPlugin::NFMModPlugin(QObject* parent) :
    QObject(parent),
    m_pluginAPI(0)
{
}

const PluginDescriptor& NFMModPlugin::getPluginDescriptor() const
{
    return m_pluginDescriptor;
}

void NFMModPlugin::initPlugin(PluginAPI* pluginAPI)
{
	m_pluginAPI = pluginAPI;

	// register AM modulator
	m_pluginAPI->registerTxChannel(NFMMod::m_channelIdURI, NFMMod::m_channelId, this);
}

#ifdef SERVER_MODE
PluginInstanceGUI* NFMModPlugin::createTxChannelGUI(
        DeviceUISet *deviceUISet,
        BasebandSampleSource *txChannel)
{
    return 0;
}
#else
PluginInstanceGUI* NFMModPlugin::createTxChannelGUI(DeviceUISet *deviceUISet, BasebandSampleSource *txChannel)
{
    return NFMModGUI::create(m_pluginAPI, deviceUISet, txChannel);
}
#endif

BasebandSampleSource* NFMModPlugin::createTxChannelBS(DeviceAPI *deviceAPI)
{
    return new NFMMod(deviceAPI);
}

ChannelAPI* NFMModPlugin::createTxChannelCS(DeviceAPI *deviceAPI)
{
    return new NFMMod(deviceAPI);
}


