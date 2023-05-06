/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * Flacon - audio File Encoder
 * https://github.com/flacon/flacon
 *
 * Copyright: 2012-2015
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

#include "cue.h"
#include "cuedata.h"
#include "types.h"
#include "tags.h"
#include <QFile>
#include <QFileInfo>
#include <QObject>
#include <QDebug>
#include "uchardetect.h"

/************************************************
 *
 ************************************************/
static void splitTitle(TrackTags *track, char separator)
{
    QByteArray b   = track->tagData(TagId::Title);
    int        pos = b.indexOf(separator);
    track->setTag(TagId::Artist, b.left(pos).trimmed());
    track->setTag(TagId::Title, b.right(b.length() - pos - 1).trimmed());
}

/************************************************
 *
 ************************************************/
Cue::Cue(QIODevice *device, const QString &audioFile) noexcept(false)
{
    CueData data(device);
    if (data.isEmpty()) {
        throw CueError(QObject::tr("File contains not a valid CUE data."));
    }

    QFileInfo fileInfo(audioFile);
    mFilePath = EMBEDED_PREFIX + fileInfo.absoluteFilePath();
    mTitle    = QObject::tr("Embedded on %1", "The title for the CUE embedded in the audio file. %1 - is an audio-file name.").arg(fileInfo.fileName());

    read(data);
}

/************************************************
 *
 ************************************************/
Cue::Cue(const QString &fileName) noexcept(false)
{
    CueData data(fileName);
    if (data.isEmpty()) {
        throw CueError(QObject::tr("<b>%1</b> is not a valid CUE file. The CUE sheet has no FILE tag.").arg(fileName));
    }

    QFileInfo fileInfo(fileName);
    mFilePath = fileInfo.absoluteFilePath();
    mTitle    = fileInfo.fileName();

    read(data);
}

/************************************************
 *
 ************************************************/
void Cue::read(const CueData &data)
{

    mDiscCount = 1;
    mDiscNum   = 1;

    QByteArray           albumPerformer = getAlbumPerformer(data);
    const CueData::Tags &global         = data.globalTags();

    QByteArray prevFileTag;

    for (const CueData::Tags &t : data.tracks()) {
        TrackTags track;
        track.setTag(TagId::File, t.value(CueData::FILE_TAG));
        track.setTag(TagId::Album, global.value(CueData::TITLE_TAG));

        QByteArray index00     = t.value("INDEX 00");
        QByteArray index00File = t.value("INDEX 00 FILE");

        QByteArray index01     = t.value("INDEX 01");
        QByteArray index01File = t.value("INDEX 01 FILE");

        if (index00.isEmpty() && !index01.isEmpty()) {
            if (index01File == prevFileTag) {
                index00     = index01;
                index00File = index01File;
            }
            else {
                index00     = "00:00:00";
                index00File = index01File;
            }
        }

        if (index01.isEmpty() && !index00.isEmpty()) {
            index01     = index00;
            index01File = index00File;
        }
        prevFileTag = index01File;

        track.setCueIndex(0, CueIndex(index00, index00File));
        track.setCueIndex(1, CueIndex(index01, index01File));

        track.setTag(TagId::Catalog, global.value("CATALOG"));
        track.setTag(TagId::CDTextfile, global.value("CDTEXTFILE"));
        track.setTag(TagId::Comment, global.value("COMMENT"));

        track.setTag(TagId::Flags, t.value(CueData::FLAGS_TAG));
        track.setTag(TagId::Genre, global.value("GENRE"));
        track.setTag(TagId::ISRC, t.value("ISRC"));
        track.setTag(TagId::Title, t.value("TITLE"));
        track.setTag(TagId::Artist, t.value("PERFORMER", global.value("PERFORMER")));
        track.setTag(TagId::SongWriter, t.value("SONGWRITER", global.value("SONGWRITER")));
        track.setTag(TagId::DiscId, global.value("DISCID"));

        track.setTag(TagId::Date, t.value("DATE", global.value("DATE")));
        track.setTag(TagId::AlbumArtist, albumPerformer);

        track.setTrackNum(TrackNum(t.value(CueData::TRACK_TAG).toUInt()));
        track.setTrackCount(TrackNum(data.tracks().count()));
        track.setTag(TagId::DiscNum, global.value("DISCNUMBER", "1"));
        track.setTag(TagId::DiscCount, global.value("TOTALDISCS", "1"));

        mTracks.append(track);
    }

    splitTitleTag(data);
    setCodecName(data);
    validate();
}

/************************************************
 *
 ************************************************/
void Cue::validate()
{
    bool hasFileTag = false;
    for (const TrackTags &t : mTracks) {
        hasFileTag = hasFileTag || !t.tagData(TagId::File).isEmpty();
    }

    if (!hasFileTag) {
        throw CueError(QObject::tr("<b>%1</b> is not a valid CUE file. The CUE sheet has no FILE tag.").arg(mFilePath));
    }
}

/************************************************
 *
 ************************************************/
bool Cue::isMutiplyAudio() const
{
    if (isEmpty())
        return false;

    QByteArray file = mTracks.last().tagData(TagId::File);
    for (const TrackTags &track : mTracks) {
        if (track.tagData(TagId::File) != file)
            return true;
    }

    return false;
}

/************************************************
 *
 ************************************************/
QByteArray Cue::getAlbumPerformer(const CueData &data)
{
    QByteArray res = data.globalTags().value(CueData::PERFORMER_TAG);

    if (!res.isEmpty())
        return res;

    res = data.tracks().first().value(CueData::PERFORMER_TAG);

    for (const CueData::Tags &t : data.tracks()) {
        if (t.value(CueData::PERFORMER_TAG) != res)
            return "";
    }
    return res;
}

/************************************************
 *
 ************************************************/
void Cue::splitTitleTag(const CueData &data)
{
    bool splitByDash      = true;
    bool splitBySlash     = true;
    bool splitByBackSlash = true;

    for (const CueData::Tags &track : data.tracks()) {
        if (track.contains("PERFORMER")) {
            return;
        }

        QByteArray value = track.value("TITLE");
        splitByDash      = splitByDash && value.contains('-');
        splitBySlash     = splitBySlash && value.contains('/');
        splitByBackSlash = splitByBackSlash && value.contains('\\');
    }

    // clang-format off
    for (TrackTags &track : mTracks) {
        if (splitByBackSlash)  splitTitle(&track, '\\');
        else if (splitBySlash) splitTitle(&track, '/');
        else if (splitByDash)  splitTitle(&track, '-');
    }
    // clang-format off
}

/************************************************
* Auto detect codepage
************************************************/
void Cue::setCodecName(const CueData &data)
{
    QString codecName = data.codecName();

    if (codecName.isEmpty()) {
        UcharDet charDet;
        foreach (const TrackTags &track, mTracks) {
            charDet << track;
        }

        codecName = charDet.textCodecName();
    }

    for (TrackTags &track : mTracks) {
        track.setCodecName(codecName);
    }
}

/************************************************
 *
 ************************************************/
CueError::~CueError()
{
}
