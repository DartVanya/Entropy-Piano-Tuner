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

#ifndef AUDIOINTERFACE_H
#define AUDIOINTERFACE_H

#include "prerequisites.h"

#include <string>

#include "pcmdevice.h"

class EPT_EXTERN AudioInterface
{
public:
    virtual void init() = 0;            ///< Inizialize the audio device.
    virtual void exit() = 0;            ///< Destroy the audio device.

    virtual void start() = 0;           ///< Start/restart the audio device.
    virtual void stop()  = 0;           ///< Stop the audio device.


    void suspend(bool v = true);
    void resume(bool v = true);
    bool isSuspended() const {return mSuspended;}

    virtual const std::string getDeviceName() const = 0;    // get the device name
    virtual int getSamplingRate() const = 0;                // get actual sampling rate
    virtual int getChannelCount() const = 0;                // get number of channels
    virtual PCMDevice::PcmDataType getDataType() const = 0; // get the data type of a sample

    virtual PCMDevice *getDevice() const = 0;
    virtual void setDevice(PCMDevice *device) = 0;

    virtual void setGain(double gain) = 0;
    virtual double getGain() const = 0;

protected:
    virtual void suspendChanged(bool s) = 0;

protected:
    bool mSuspended = false;
};

#endif // AUDIOINTERFACE_H
