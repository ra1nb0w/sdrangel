///////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2015 Edouard Griffiths, F4EXB                                   //
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
#include "util/simpleserializer.h"
#include "fcdproplusplugin.h"

#ifdef SERVER_MODE
#include "fcdproplusinput.h"
#else
#include "fcdproplusgui.h"
#endif
#include "fcdtraits.h"

const PluginDescriptor FCDProPlusPlugin::m_pluginDescriptor = {
	QString(fcd_traits<ProPlus>::pluginDisplayedName),
	QString(fcd_traits<ProPlus>::pluginVersion),
	QString("(c) Edouard Griffiths, F4EXB"),
	QString("https://github.com/f4exb/sdrangel"),
	true,
	QString("https://github.com/f4exb/sdrangel")
};

const QString FCDProPlusPlugin::m_deviceTypeID = FCDPROPLUS_DEVICE_TYPE_ID;

FCDProPlusPlugin::FCDProPlusPlugin(QObject* parent) :
	QObject(parent)
{
}

const PluginDescriptor& FCDProPlusPlugin::getPluginDescriptor() const
{
	return m_pluginDescriptor;
}

void FCDProPlusPlugin::initPlugin(PluginAPI* pluginAPI)
{
	pluginAPI->registerSampleSource(fcd_traits<ProPlus>::interfaceIID, this);
}

PluginInterface::SamplingDevices FCDProPlusPlugin::enumSampleSources()
{
	SamplingDevices result;

	int i = 0;
	struct hid_device_info *device_info = hid_enumerate(fcd_traits<ProPlus>::vendorId, fcd_traits<ProPlus>::productId);

	while (device_info != 0)
	{
		QString serialNumber = QString::fromWCharArray(device_info->serial_number);
		QString displayedName(QString("%1[%2] %3").arg(fcd_traits<ProPlus>::displayedName).arg(i).arg(serialNumber));

		result.append(SamplingDevice(displayedName,
		        fcd_traits<ProPlus>::hardwareID,
				fcd_traits<ProPlus>::interfaceIID,
				serialNumber,
				i,
				PluginInterface::SamplingDevice::PhysicalDevice,
				PluginInterface::SamplingDevice::StreamSingleRx,
				1,
				0));

		device_info = device_info->next;
		i++;
	}

	return result;
}

#ifdef SERVER_MODE
PluginInstanceGUI* FCDProPlusPlugin::createSampleSourcePluginInstanceGUI(
        const QString& sourceId,
        QWidget **widget,
        DeviceUISet *deviceUISet)
{
    return 0;
}
#else
PluginInstanceGUI* FCDProPlusPlugin::createSampleSourcePluginInstanceGUI(
        const QString& sourceId,
        QWidget **widget,
        DeviceUISet *deviceUISet)
{
	if(sourceId == fcd_traits<ProPlus>::interfaceIID)
	{
		FCDProPlusGui* gui = new FCDProPlusGui(deviceUISet);
		*widget = gui;
		return gui;
	}
	else
	{
		return 0;
	}
}
#endif

DeviceSampleSource *FCDProPlusPlugin::createSampleSourcePluginInstanceInput(const QString& sourceId, DeviceAPI *deviceAPI)
{
    if(sourceId == fcd_traits<ProPlus>::interfaceIID)
    {
        FCDProPlusInput* input = new FCDProPlusInput(deviceAPI);
        return input;
    }
    else
    {
        return 0;
    }
}
