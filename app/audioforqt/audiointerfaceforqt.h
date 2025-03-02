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

#ifndef AUDIOINTERFACEFORQT_H
#define AUDIOINTERFACEFORQT_H

#include <QObject>
#include <QAudioDevice>

#include "core/audio/audiointerface.h"
#include "qtpcmdevice.h"

class AudioInterfaceForQt : public QObject, public AudioInterface
{
    Q_OBJECT

public:
    static const int DEFAULT_BUFFER_SIZE_MS;

public:
    AudioInterfaceForQt(QAudioDevice::Mode mode, QObject *parent);

    void reinitialize(int samplingRate, int channelCount, QAudioDevice deviceInfo, int bufferSizeMS);
    virtual void init() override final;

    const QAudioFormat &getFormat() const {return mFormat;}
    const QAudioDevice &getDeviceInfo() const {return mDeviceInfo;}
    int getBufferSizeMS() const;

    virtual const std::string getDeviceName() const override final;
    virtual int getSamplingRate() const override final;
    virtual int getChannelCount() const override final;
    virtual PCMDevice::PcmDataType getDataType() const override final;

    virtual PCMDevice *getDevice() const override final;
    virtual void setDevice(PCMDevice *device) override final;

protected:
    virtual QAudio::Error createDevice(const QAudioFormat &format, const QAudioDevice &info, int bufferSizeMS = -1) = 0;

protected:
    const QAudioDevice::Mode mMode;
    const QString mSettingsPrefix;
    QtPCMDevice mPCMDevice;
    QAudioFormat mFormat;
    QAudioDevice mDeviceInfo;
};

#endif // AUDIOINTERFACEFORQT_H
