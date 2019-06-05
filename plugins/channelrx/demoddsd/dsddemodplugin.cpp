///////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2016 F4EXB                                                      //
// written by Edouard Griffiths                                                  //
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

#include "dsddemodplugin.h"

#include <QtPlugin>
#include "plugin/pluginapi.h"
#ifndef SERVER_MODE
#include "dsddemodgui.h"
#endif
#include "dsddemod.h"

const PluginDescriptor DSDDemodPlugin::m_pluginDescriptor = {
	QString("DSD Demodulator"),
    QString("4.5.6"),
	QString("(c) Edouard Griffiths, F4EXB"),
	QString("https://github.com/f4exb/sdrangel"),
	true,
	QString("https://github.com/f4exb/sdrangel")
};

DSDDemodPlugin::DSDDemodPlugin(QObject* parent) :
	QObject(parent),
	m_pluginAPI(0)
{
}

const PluginDescriptor& DSDDemodPlugin::getPluginDescriptor() const
{
	return m_pluginDescriptor;
}

void DSDDemodPlugin::initPlugin(PluginAPI* pluginAPI)
{
	m_pluginAPI = pluginAPI;

	// register DSD demodulator
	m_pluginAPI->registerRxChannel(DSDDemod::m_channelIdURI, DSDDemod::m_channelId, this);
}

#ifdef SERVER_MODE
PluginInstanceGUI* DSDDemodPlugin::createRxChannelGUI(
        DeviceUISet *deviceUISet,
        BasebandSampleSink *rxChannel)
{
    return 0;
}
#else
PluginInstanceGUI* DSDDemodPlugin::createRxChannelGUI(DeviceUISet *deviceUISet, BasebandSampleSink *rxChannel)
{
	return DSDDemodGUI::create(m_pluginAPI, deviceUISet, rxChannel);
}
#endif

BasebandSampleSink* DSDDemodPlugin::createRxChannelBS(DeviceAPI *deviceAPI)
{
    return new DSDDemod(deviceAPI);
}

ChannelAPI* DSDDemodPlugin::createRxChannelCS(DeviceAPI *deviceAPI)
{
    return new DSDDemod(deviceAPI);
}

