///////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2018-2019 Edouard Griffiths, F4EXB                              //
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
#include "remotesourcegui.h"
#endif
#include "remotesource.h"
#include "remotesourceplugin.h"

const PluginDescriptor RemoteSourcePlugin::m_pluginDescriptor = {
    QString("Remote channel source"),
    QString("4.5.2"),
    QString("(c) Edouard Griffiths, F4EXB"),
    QString("https://github.com/f4exb/sdrangel"),
    true,
    QString("https://github.com/f4exb/sdrangel")
};

RemoteSourcePlugin::RemoteSourcePlugin(QObject* parent) :
    QObject(parent),
    m_pluginAPI(0)
{
}

const PluginDescriptor& RemoteSourcePlugin::getPluginDescriptor() const
{
    return m_pluginDescriptor;
}

void RemoteSourcePlugin::initPlugin(PluginAPI* pluginAPI)
{
    m_pluginAPI = pluginAPI;

    // register source
    m_pluginAPI->registerTxChannel(RemoteSource::m_channelIdURI, RemoteSource::m_channelId, this);
}

#ifdef SERVER_MODE
PluginInstanceGUI* RemoteSourcePlugin::createTxChannelGUI(
        DeviceUISet *deviceUISet,
        BasebandSampleSource *txChannel)
{
    return 0;
}
#else
PluginInstanceGUI* RemoteSourcePlugin::createTxChannelGUI(DeviceUISet *deviceUISet, BasebandSampleSource *txChannel)
{
    return RemoteSourceGUI::create(m_pluginAPI, deviceUISet, txChannel);
}
#endif

BasebandSampleSource* RemoteSourcePlugin::createTxChannelBS(DeviceAPI *deviceAPI)
{
    return new RemoteSource(deviceAPI);
}

ChannelAPI* RemoteSourcePlugin::createTxChannelCS(DeviceAPI *deviceAPI)
{
    return new RemoteSource(deviceAPI);
}



