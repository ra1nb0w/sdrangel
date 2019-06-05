///////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2017 Edouard Griffiths, F4EXB                                   //
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
#include "plutosdr/deviceplutosdr.h"

#ifdef SERVER_MODE
#include "plutosdrinput.h"
#else
#include "plutosdrinputgui.h"
#endif
#include "plutosdrinputplugin.h"

class DeviceAPI;

const PluginDescriptor PlutoSDRInputPlugin::m_pluginDescriptor = {
	QString("PlutoSDR Input"),
	QString("4.5.4"),
	QString("(c) Edouard Griffiths, F4EXB"),
	QString("https://github.com/f4exb/sdrangel"),
	true,
	QString("https://github.com/f4exb/sdrangel")
};

const QString PlutoSDRInputPlugin::m_hardwareID = "PlutoSDR";
const QString PlutoSDRInputPlugin::m_deviceTypeID = PLUTOSDR_DEVICE_TYPE_ID;

PlutoSDRInputPlugin::PlutoSDRInputPlugin(QObject* parent) :
	QObject(parent)
{
}

const PluginDescriptor& PlutoSDRInputPlugin::getPluginDescriptor() const
{
	return m_pluginDescriptor;
}

void PlutoSDRInputPlugin::initPlugin(PluginAPI* pluginAPI)
{
	pluginAPI->registerSampleSource(m_deviceTypeID, this);
	DevicePlutoSDR::instance(); // create singleton
}

PluginInterface::SamplingDevices PlutoSDRInputPlugin::enumSampleSources()
{
    DevicePlutoSDR::instance().scan();
    std::vector<std::string> serials;
    DevicePlutoSDR::instance().getSerials(serials);

    std::vector<std::string>::const_iterator it = serials.begin();
    int i;
	SamplingDevices result;

	for (i = 0; it != serials.end(); ++it, ++i)
	{
	    QString serial_str = QString::fromLocal8Bit(it->c_str());
	    QString displayedName(QString("PlutoSDR[%1] %2").arg(i).arg(serial_str));

        result.append(SamplingDevice(displayedName,
                m_hardwareID,
                m_deviceTypeID,
                serial_str,
                i,
                PluginInterface::SamplingDevice::PhysicalDevice,
                PluginInterface::SamplingDevice::StreamSingleRx,
                1,
                0));

        qDebug("PlutoSDRInputPlugin::enumSampleSources: enumerated PlutoSDR device #%d", i);
	}

	return result;
}

#ifdef SERVER_MODE
PluginInstanceGUI* PlutoSDRInputPlugin::createSampleSourcePluginInstanceGUI(
        const QString& sourceId,
        QWidget **widget,
        DeviceUISet *deviceUISet)
{
    return 0;
}
#else
PluginInstanceGUI* PlutoSDRInputPlugin::createSampleSourcePluginInstanceGUI(
        const QString& sourceId,
        QWidget **widget,
        DeviceUISet *deviceUISet)
{
	if(sourceId == m_deviceTypeID)
	{
		PlutoSDRInputGui* gui = new PlutoSDRInputGui(deviceUISet);
		*widget = gui;
		return gui;
	}
	else
	{
		return 0;
	}
}
#endif

DeviceSampleSource *PlutoSDRInputPlugin::createSampleSourcePluginInstanceInput(const QString& sourceId, DeviceAPI *deviceAPI)
{
    if (sourceId == m_deviceTypeID)
    {
        PlutoSDRInput* input = new PlutoSDRInput(deviceAPI);
        return input;
    }
    else
    {
        return 0;
    }
}

