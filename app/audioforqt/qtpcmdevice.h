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

#ifndef QTPCMDEVICE_H
#define QTPCMDEVICE_H

#include <QIODevice>

#include "core/audio/pcmdevice.h"

class AudioInterfaceForQt;

class QtPCMDevice : public QIODevice
{
    Q_OBJECT
public:
    QtPCMDevice(AudioInterfaceForQt *audioInterface);

    virtual bool open(OpenMode mode) override final;
    virtual qint64 bytesAvailable() const override final;
    virtual void close() override final;

    void setDevice(PCMDevice *device);
    PCMDevice *getDevice() const {return mDevice;}


    virtual bool isSequential() const override {return false;}

signals:
    void deviceChanged(PCMDevice *device);

private:
    virtual qint64 readData(char *data, qint64 maxSize) override final;
    virtual qint64 writeData(const char *data, qint64 maxSize) override final;

private:
    PCMDevice *mDevice = nullptr;
    AudioInterfaceForQt *mAudioInterface;
};

#endif // QTPCMDEVICE_H
