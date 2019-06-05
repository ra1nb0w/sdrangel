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
#include "plutosdroutput.h"
#else
#include "plutosdroutputgui.h"
#endif
#include "plutosdroutputplugin.h"

const PluginDescriptor PlutoSDROutputPlugin::m_pluginDescriptor = {
	QString("PlutoSDR Output"),
	QString("4.5.4"),
	QString("(c) Edouard Griffiths, F4EXB"),
	QString("https://github.com/f4exb/sdrangel"),
	true,
	QString("https://github.com/f4exb/sdrangel")
};

const QString PlutoSDROutputPlugin::m_hardwareID = "PlutoSDR";
const QString PlutoSDROutputPlugin::m_deviceTypeID = PLUTOSDR_DEVICE_TYPE_ID;

PlutoSDROutputPlugin::PlutoSDROutputPlugin(QObject* parent) :
	QObject(parent)
{
}

const PluginDescriptor& PlutoSDROutputPlugin::getPluginDescriptor() const
{
	return m_pluginDescriptor;
}

void PlutoSDROutputPlugin::initPlugin(PluginAPI* pluginAPI)
{
	pluginAPI->registerSampleSink(m_deviceTypeID, this);
	DevicePlutoSDR::instance(); // create singleton
}

PluginInterface::SamplingDevices PlutoSDROutputPlugin::enumSampleSinks()
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
                PluginInterface::SamplingDevice::StreamSingleTx,
                1,
                0));

        qDebug("PlutoSDROutputPlugin::enumSampleSources: enumerated PlutoSDR device #%d", i);
	}

	return result;
}

#ifdef SERVER_MODE
PluginInstanceGUI* PlutoSDROutputPlugin::createSampleSinkPluginInstanceGUI(
        const QString& sinkId,
        QWidget **widget,
        DeviceUISet *deviceUISet)
{
    return 0;
}
#else
PluginInstanceGUI* PlutoSDROutputPlugin::createSampleSinkPluginInstanceGUI(
        const QString& sinkId,
        QWidget **widget,
        DeviceUISet *deviceUISet)
{
	if(sinkId == m_deviceTypeID)
	{
        PlutoSDROutputGUI* gui = new PlutoSDROutputGUI(deviceUISet);
		*widget = gui;
		return gui;
	}
	else
	{
		return 0;
	}
}
#endif

DeviceSampleSink *PlutoSDROutputPlugin::createSampleSinkPluginInstanceOutput(const QString& sinkId, DeviceAPI *deviceAPI)
{
    if (sinkId == m_deviceTypeID)
    {
        PlutoSDROutput* output = new PlutoSDROutput(deviceAPI);
        return output;
    }
    else
    {
        return 0;
    }
}

