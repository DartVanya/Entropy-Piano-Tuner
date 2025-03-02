/*****************************************************************************
 * Copyright 2018 Haye Hinrichsen, Christoph Wick
 *
 * This file is part of Entropy Piano Tuner.
 *
 * Entropy Piano Tuner is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * Entropy Piano Tuner is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Entropy Piano Tuner. If not, see http://www.gnu.org/licenses/.
 *****************************************************************************/

#include "audiointerfaceforqt.h"

#include <QSettings>
#include <QMediaDevices>

#include "core/audio/pcmdevice.h"

const int AudioInterfaceForQt::DEFAULT_BUFFER_SIZE_MS(100);

AudioInterfaceForQt::AudioInterfaceForQt(QAudioDevice::Mode mode, QObject *parent)
    : QObject(parent)
    , mMode(mode)
    , mSettingsPrefix(mode == QAudioDevice::Mode::Input ? "audio/input/" : "audio/output/")
    , mPCMDevice(this)
{
    // these settings are fixed
    mFormat.setSampleFormat(QAudioFormat::Float);

    // default values, these can be changed
    mFormat.setSampleRate(mode == QAudioDevice::Mode::Input ? 44100 : 22050);
    mFormat.setChannelCount(mode == QAudioDevice::Mode::Input ? 1 : 2);
}

void AudioInterfaceForQt::reinitialize(int samplingRate, int channelCount, QAudioDevice deviceInfo, int bufferSizeMS)
{
    exit();

    mDeviceInfo = deviceInfo;
    mFormat.setSampleRate(samplingRate);
    mFormat.setChannelCount(channelCount);

    // only necessary if default settings
    if (!deviceInfo.isFormatSupported(mFormat)) {
        LogW("Raw audio format not supported by backend, falling back to nearest supported");
        mFormat = deviceInfo.preferredFormat();
        // update sampling rate, buffer type has to stay the same!
        if (not deviceInfo.isFormatSupported(mFormat))
        {
            LogW("Fallback failed. Probably there is no device available.");
            return;
        }

        if (mFormat.sampleFormat() != QAudioFormat::Float
            || mFormat.sampleFormat() != QAudioFormat::Int16) {
            LogW("Sample format not supported");
            return;
        }
    }

    QAudio::Error err = createDevice(mFormat, deviceInfo, bufferSizeMS);
    if (err != QAudio::NoError) {
        LogE("Error creating audio device with error %d", err);
        return;
    }

    LogI("Initialized audio using device: %s", getDeviceName().c_str());

    QSettings s;
    s.setValue(mSettingsPrefix + "samplerate", QVariant::fromValue(mFormat.sampleRate()));
    s.setValue(mSettingsPrefix + "channels", QVariant::fromValue(mFormat.channelCount()));
    s.setValue(mSettingsPrefix + "devicename", QVariant::fromValue(mDeviceInfo.description()));
    s.setValue(mSettingsPrefix + "buffersize", QVariant::fromValue(bufferSizeMS));
}

void AudioInterfaceForQt::init()
{
    QSettings s;

    // get device info
    mDeviceInfo = QAudioDevice();
    QString deviceName = s.value(mSettingsPrefix + "devicename").toString();
    if (deviceName.isEmpty()) {
        if (mMode == QAudioDevice::Mode::Input) {
            mDeviceInfo = QMediaDevices::defaultAudioInput();
        } else if (mMode == QAudioDevice::Mode::Output) {
            mDeviceInfo = QMediaDevices::defaultAudioOutput();
        }
    } else {
        QList<QAudioDevice> devices;
        if (mMode == QAudioDevice::Mode::Input) {
            devices = QMediaDevices::audioInputs();
        } else {
            devices = QMediaDevices::audioOutputs();
        }
        for (QAudioDevice d : devices) {
            if (d.description() == deviceName) {
                mDeviceInfo = d;
                break;
            }
        }
    }

    if (mDeviceInfo.isNull()) {
        if (mMode == QAudioDevice::Mode::Input) {
            mDeviceInfo = QMediaDevices::defaultAudioInput();
        } else if (mMode == QAudioDevice::Mode::Output) {
            mDeviceInfo = QMediaDevices::defaultAudioOutput();
        }
    }

    // get format
    mFormat.setSampleRate(s.value(mSettingsPrefix + "samplerate", QVariant::fromValue(mFormat.sampleRate())).toInt());
    mFormat.setChannelCount(s.value(mSettingsPrefix + "channels", QVariant::fromValue(mFormat.channelCount())).toInt());

    // these settings are required
    if (mMode == QAudioDevice::Mode::Input) {
        mFormat.setChannelCount(1);
    }

    // buffer size
    int bufferSize = s.value(mSettingsPrefix + "buffersize", QVariant::fromValue(DEFAULT_BUFFER_SIZE_MS)).toInt();

    // initialize device
    reinitialize(mFormat.sampleRate(), mFormat.channelCount(), mDeviceInfo, bufferSize);
}

const std::string AudioInterfaceForQt::getDeviceName() const
{
    return mDeviceInfo.description().toStdString();
}

int AudioInterfaceForQt::getSamplingRate() const
{
    return mFormat.sampleRate();
}

int AudioInterfaceForQt::getChannelCount() const
{
    return mFormat.channelCount();
}

PCMDevice::PcmDataType AudioInterfaceForQt::getDataType() const
{
    switch (mFormat.sampleFormat()) {
    case QAudioFormat::UInt8:
        return PCMDevice::PcmDataType::UINT8;
    case QAudioFormat::Int16:
        return PCMDevice::PcmDataType::INT16;
    case QAudioFormat::Int32:
        return PCMDevice::PcmDataType::INT32;
    case QAudioFormat::Float:
        return PCMDevice::PcmDataType::FLOAT;
    case QAudioFormat::Unknown:
    case QAudioFormat::NSampleFormats:
        return PCMDevice::UNKNOWN_DATA_TYPE;
    }
}

PCMDevice *AudioInterfaceForQt::getDevice() const
{
    return mPCMDevice.getDevice();
}

void AudioInterfaceForQt::setDevice(PCMDevice *device)
{
    mPCMDevice.setDevice(device);
}

int AudioInterfaceForQt::getBufferSizeMS() const
{
    QSettings s;
    return s.value(mSettingsPrefix + "buffersize", QVariant::fromValue(DEFAULT_BUFFER_SIZE_MS)).toInt();
}
