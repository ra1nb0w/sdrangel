///////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2018 Edouard Griffiths, F4EXB                                   //
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
#include <libairspyhf/airspyhf.h>

#include "plugin/pluginapi.h"
#include "util/simpleserializer.h"
#include "airspyhfplugin.h"
#ifdef SERVER_MODE
#include "airspyhfinput.h"
#else
#include "airspyhfgui.h"
#endif


const PluginDescriptor AirspyHFPlugin::m_pluginDescriptor = {
	QString("AirspyHF Input"),
	QString("4.5.2"),
	QString("(c) Edouard Griffiths, F4EXB"),
	QString("https://github.com/f4exb/sdrangel"),
	true,
	QString("https://github.com/f4exb/sdrangel")
};

const QString AirspyHFPlugin::m_hardwareID = "AirspyHF";
const QString AirspyHFPlugin::m_deviceTypeID = AIRSPYHF_DEVICE_TYPE_ID;
const int AirspyHFPlugin::m_maxDevices = 32;

AirspyHFPlugin::AirspyHFPlugin(QObject* parent) :
	QObject(parent)
{
}

const PluginDescriptor& AirspyHFPlugin::getPluginDescriptor() const
{
	return m_pluginDescriptor;
}

void AirspyHFPlugin::initPlugin(PluginAPI* pluginAPI)
{
	pluginAPI->registerSampleSource(m_deviceTypeID, this);
}

PluginInterface::SamplingDevices AirspyHFPlugin::enumSampleSources()
{
	SamplingDevices result;
	int nbDevices;
	uint64_t deviceSerials[m_maxDevices];

	nbDevices = airspyhf_list_devices(deviceSerials, m_maxDevices);

    if (nbDevices < 0)
    {
        qCritical("AirspyHFPlugin::enumSampleSources: failed to list Airspy HF devices");
    }

	for (int i = 0; i < nbDevices; i++)
	{
	    if (deviceSerials[i])
	    {
            QString serial_str = QString::number(deviceSerials[i], 16);
            QString displayedName(QString("AirspyHF[%1] %2").arg(i).arg(serial_str));

            result.append(SamplingDevice(displayedName,
                    m_hardwareID,
                    m_deviceTypeID,
                    serial_str,
                    i,
                    PluginInterface::SamplingDevice::PhysicalDevice,
                    PluginInterface::SamplingDevice::StreamSingleRx,
                    1,
                    0));

            qDebug("AirspyHFPlugin::enumSampleSources: enumerated Airspy HF device #%d", i);
	    }
	    else
	    {
            qDebug("AirspyHFPlugin::enumSampleSources: finished to enumerate Airspy HF. %d devices found", i);
	        break; // finished
	    }
	}

	return result;
}

#ifdef SERVER_MODE
PluginInstanceGUI* AirspyHFPlugin::createSampleSourcePluginInstanceGUI(
        const QString& sourceId,
        QWidget **widget,
        DeviceUISet *deviceUISet)
{
    return 0;
}
#else
PluginInstanceGUI* AirspyHFPlugin::createSampleSourcePluginInstanceGUI(
        const QString& sourceId,
        QWidget **widget,
        DeviceUISet *deviceUISet)
{
	if (sourceId == m_deviceTypeID)
	{
	    AirspyHFGui* gui = new AirspyHFGui(deviceUISet);
		*widget = gui;
		return gui;
	}
	else
	{
		return 0;
	}
}
#endif

DeviceSampleSource *AirspyHFPlugin::createSampleSourcePluginInstanceInput(const QString& sourceId, DeviceAPI *deviceAPI)
{
    if (sourceId == m_deviceTypeID)
    {
        AirspyHFInput* input = new AirspyHFInput(deviceAPI);
        return input;
    }
    else
    {
        return 0;
    }
}
