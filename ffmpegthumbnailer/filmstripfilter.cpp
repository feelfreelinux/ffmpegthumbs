//    Copyright (C) 2010 Dirk Vanden Boer <dirk.vdb@gmail.com>
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#include "filmstripfilter.h"

namespace ffmpegthumbnailer
{

static const int FILMHOLE_WIDTH = 12;
static const int FILMHOLE_HEIGHT = 10;

static const uint8_t filmHole[FILMHOLE_WIDTH * FILMHOLE_HEIGHT * 3] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x23, 0x23, 0x23, 0x7a, 0x7a, 0x7a, 0x83, 0x83, 0x83, 0x8c, 0x8c, 0x8c, 0x90, 0x90, 0x90, 0x8e, 0x8e, 0x8e, 0x52, 0x52, 0x52, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6e, 0x6e, 0x6e, 0x83, 0x83, 0x83, 0x93, 0x93, 0x93, 0xa3, 0xa3, 0xa3, 0xab, 0xab, 0xab, 0xa8, 0xa8, 0xa8, 0x9b, 0x9b, 0x9b, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x72, 0x72, 0x72, 0x8e, 0x8e, 0x8e, 0xa4, 0xa4, 0xa4, 0xbb, 0xbb, 0xbb, 0xc4, 0xc4, 0xc4, 0xc1, 0xc1, 0xc1, 0xaf, 0xaf, 0xaf, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x3e, 0x3e, 0x90, 0x90, 0x90, 0xa6, 0xa6, 0xa6, 0xbe, 0xbe, 0xbe, 0xc8, 0xc8, 0xc8, 0xc4, 0xc4, 0xc4, 0x8e, 0x8e, 0x8e, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x09, 0x09, 0x09, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


void FilmStripFilter::process(VideoFrame& videoFrame)
{
    if (videoFrame.width < FILMHOLE_WIDTH * 2) {
        return;
    }

    int frameIndex = 0;
    int filmHoleIndex = 0;
    int offset = (videoFrame.width * 3) - 3;

    for (int i = 0; i < videoFrame.height; ++i) {
        for (int j = 0; j < FILMHOLE_WIDTH * 3; j += 3) {
            int currentFilmHoleIndex = filmHoleIndex + j;

            videoFrame.frameData[frameIndex + j]     = filmHole[currentFilmHoleIndex];
            videoFrame.frameData[frameIndex + j + 1] = filmHole[currentFilmHoleIndex + 1];
            videoFrame.frameData[frameIndex + j + 2] = filmHole[currentFilmHoleIndex + 2];

            videoFrame.frameData[frameIndex + offset - j]     = filmHole[currentFilmHoleIndex];
            videoFrame.frameData[frameIndex + offset - j + 1] = filmHole[currentFilmHoleIndex + 1];
            videoFrame.frameData[frameIndex + offset - j + 2] = filmHole[currentFilmHoleIndex + 2];
        }
        frameIndex += videoFrame.lineSize;
        filmHoleIndex = (i % FILMHOLE_HEIGHT) * FILMHOLE_WIDTH * 3;
    }
}

}
