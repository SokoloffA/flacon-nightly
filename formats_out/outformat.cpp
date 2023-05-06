/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * Flacon - audio File Encoder
 * https://github.com/flacon/flacon
 *
 * Copyright: 2012-2013
 *   Alexander Sokoloff <sokoloff.a@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * END_COMMON_COPYRIGHT_HEADER */

#include "outformat.h"
#include "encoder.h"

#include "wav/out_wav.h"
#include "flac/flacoutformat.h"
#include "aac/out_aac.h"
#include "mp3/out_mp3.h"
#include "ogg/out_ogg.h"
#include "wv/out_wv.h"
#include "opus/out_opus.h"
#include "alac/alacoutformat.h"

#include <QDebug>

/************************************************

 ************************************************/
void initOutFormats(QList<OutFormat *> *formats)
{
    *formats << new OutFormat_Wav();
    *formats << new OutFormat_Flac();
    *formats << new OutFormat_Aac();
    *formats << new OutFormat_Mp3();
    *formats << new OutFormat_Ogg();
    *formats << new OutFormat_Wv();
    *formats << new OutFormat_Opus();
    *formats << new OutFormat_Alac();
}

/************************************************

 ************************************************/
QList<OutFormat *> OutFormat::allFormats()
{
    static QList<OutFormat *> res;
    if (!res.count())
        initOutFormats(&res);

    return res;
}

/************************************************
 *
 ************************************************/
QStringList OutFormat::allFormatsId()
{
    static QStringList res;

    if (res.isEmpty()) {
        for (auto f : allFormats()) {
            res << f->id();
        }
    }

    return res;
}

/************************************************
 *
 ************************************************/
OutFormat *OutFormat::formatForId(const QString &id)
{
    foreach (OutFormat *format, allFormats()) {
        if (format->id() == id)
            return format;
    }

    return nullptr;
}

/************************************************

 ************************************************/
bool OutFormat::check(const Profile &profile, QStringList *errors) const
{
    ExtProgram *prog = encoderProgram(profile);
    if (!prog) {
        return true;
    }

    return prog->check(errors);
}
