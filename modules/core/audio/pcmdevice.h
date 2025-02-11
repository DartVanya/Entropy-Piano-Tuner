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

#ifndef PCMDEVICE_H
#define PCMDEVICE_H

#include "prerequisites.h"

#include <stdint.h>

class AudioInterface;

class EPT_EXTERN PCMDevice
{
public:
    typedef float DataType;

    virtual void open(AudioInterface *audioInterface);
    virtual void close();
    virtual int64_t read(char *data, int64_t max_bytes) = 0;
    virtual int64_t write(const char *data, int64_t max_bytes) = 0;

    bool isOpen() const {return mAudioInterface != nullptr;}
    int getSampleRate() const;
    int getChannels() const;
protected:
    AudioInterface *mAudioInterface = nullptr;
};

#endif // PCMDEVICE_H
