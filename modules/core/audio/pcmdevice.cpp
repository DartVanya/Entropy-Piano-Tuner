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

#include "pcmdevice.h"
#include "audiointerface.h"
#include "core/system/eptexception.h"
#include "core/system/log.h"

void PCMDevice::open(AudioInterface *audioInterface)
{
    EptAssert(audioInterface, "Audio interface is nullptr");

    mAudioInterface = audioInterface;
}

void PCMDevice::close()
{
    mAudioInterface = nullptr;
}

int PCMDevice::getSampleRate() const {
    if (mAudioInterface) {
        return mAudioInterface->getSamplingRate();
    }

    LogE("Requested sampling rate though device is not open.");
    return 0;
}

int PCMDevice::getChannels() const {
    if (mAudioInterface) {
        return mAudioInterface->getChannelCount();
    }

    LogE("Requested channel count though device is not open.");

    return 1;
}

PCMDevice::PcmDataType PCMDevice::getDataType() const
{
    if (mAudioInterface) {
        return mAudioInterface->getDataType();
    }

    return PcmDataType::UNKNOWN_DATA_TYPE;
}

int PCMDevice::getSampleSize() const
{
    switch (getDataType()) {
    case PcmDataType::FLOAT:
        return sizeof(float);
    case PcmDataType::UINT8:
        return sizeof(uint8_t);
    case PcmDataType::INT16:
        return sizeof(int16_t);
    case PcmDataType::INT32:
        return sizeof(int32_t);
    case PcmDataType::UNKNOWN_DATA_TYPE:
        return 0;
    }
}

double PCMDevice::maxSampleValue() const
{
    switch (getDataType()) {
    case PcmDataType::FLOAT:
        return 1;
    case PcmDataType::UINT8:
        return std::numeric_limits<uint8_t>::max();
    case PcmDataType::INT16:
        return std::numeric_limits<int16_t>::max();
    case PcmDataType::INT32:
        return std::numeric_limits<int32_t>::max();
    case PcmDataType::UNKNOWN_DATA_TYPE:
        return 0;
    }
}
