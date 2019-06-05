#include "wfmplugin.h"

#include <QtPlugin>
#include "plugin/pluginapi.h"

#ifndef SERVER_MODE
#include "wfmdemodgui.h"
#endif
#include "wfmdemod.h"

const PluginDescriptor WFMPlugin::m_pluginDescriptor = {
	QString("WFM Demodulator"),
	QString("4.5.2"),
	QString("(c) Edouard Griffiths, F4EXB"),
	QString("https://github.com/f4exb/sdrangel"),
	true,
	QString("https://github.com/f4exb/sdrangel")
};

WFMPlugin::WFMPlugin(QObject* parent) :
	QObject(parent),
	m_pluginAPI(0)
{
}

const PluginDescriptor& WFMPlugin::getPluginDescriptor() const
{
	return m_pluginDescriptor;
}

void WFMPlugin::initPlugin(PluginAPI* pluginAPI)
{
	m_pluginAPI = pluginAPI;

	// register WFM demodulator
	m_pluginAPI->registerRxChannel(WFMDemod::m_channelIdURI, WFMDemod::m_channelId, this);
}

#ifdef SERVER_MODE
PluginInstanceGUI* WFMPlugin::createRxChannelGUI(
        DeviceUISet *deviceUISet,
        BasebandSampleSink *rxChannel)
{
    return 0;
}
#else
PluginInstanceGUI* WFMPlugin::createRxChannelGUI(DeviceUISet *deviceUISet, BasebandSampleSink *rxChannel)
{
	return WFMDemodGUI::create(m_pluginAPI, deviceUISet, rxChannel);
}
#endif

BasebandSampleSink* WFMPlugin::createRxChannelBS(DeviceAPI *deviceAPI)
{
    return new WFMDemod(deviceAPI);
}

ChannelAPI* WFMPlugin::createRxChannelCS(DeviceAPI *deviceAPI)
{
    return new WFMDemod(deviceAPI);
}

