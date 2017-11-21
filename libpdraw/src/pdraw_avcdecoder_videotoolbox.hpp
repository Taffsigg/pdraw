/**
 * @file pdraw_avcdecoder_videotoolbox.cpp
 * @brief Parrot Drones Awesome Video Viewer Library - Apple VideoToolbox H.264/AVC video decoder
 * @date 04/11/2017
 * @author aurelien.barre@akaaba.net
 *
 * Copyright (c) 2016 Aurelien Barre <aurelien.barre@akaaba.net>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 * 
 *   * Neither the name of the copyright holder nor the names of the
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _PDRAW_AVCDECODER_VIDEOTOOLBOX_HPP_
#define _PDRAW_AVCDECODER_VIDEOTOOLBOX_HPP_

#ifdef USE_VIDEOTOOLBOX

#include <VideoToolbox/VideoToolbox.h>

#include <vector>

#include "pdraw_avcdecoder.hpp"


#define VIDEOTOOLBOX_AVC_DECODER_INPUT_BUFFER_COUNT 5
#define VIDEOTOOLBOX_AVC_DECODER_INPUT_BUFFER_SIZE 1920 * 1080 / 2
#define VIDEOTOOLBOX_AVC_DECODER_OUTPUT_BUFFER_COUNT 5


namespace Pdraw {


class VideoToolboxAvcDecoder : public AvcDecoder {
public:
	VideoToolboxAvcDecoder(
		VideoMedia *media);

	~VideoToolboxAvcDecoder(
		void);

	uint32_t getInputBitstreamFormatCaps(
		void) {
		return AVCDECODER_BITSTREAM_FORMAT_AVCC;
	};

	int configure(
		uint32_t inputBitstreamFormat,
		const uint8_t *pSps,
		unsigned int spsSize,
		const uint8_t *pPps,
		unsigned int ppsSize);

	bool isConfigured(
		void) {
		return mConfigured;
	};

	int getInputBuffer(
		struct vbuf_buffer **buffer,
		bool blocking);

	int queueInputBuffer(
		struct vbuf_buffer *buffer);

	struct vbuf_queue *addOutputQueue(
		void);

	int removeOutputQueue(
		struct vbuf_queue *queue);

	int dequeueOutputBuffer(
		struct vbuf_queue *queue,
		struct vbuf_buffer **buffer,
		bool blocking);

	int releaseOutputBuffer(
		struct vbuf_buffer **buffer);

	int stop(
		void);

	Media *getMedia(
		void) {
		return mMedia;
	};

	VideoMedia *getVideoMedia(
		void) {
		return (VideoMedia*)mMedia;
	};

private:
	bool isOutputQueueValid(
		struct vbuf_queue *queue);

	static void frameOutputCb(
		void *decompressionOutputRefCon,
		void *sourceFrameRefCon,
		OSStatus status,
		VTDecodeInfoFlags infoFlags,
		CVImageBufferRef imageBuffer,
		CMTime presentationTimeStamp,
		CMTime presentationDuration);

	struct vbuf_pool *mInputBufferPool;
	struct vbuf_queue *mInputBufferQueue;
	struct vbuf_pool *mOutputBufferPool;
	std::vector<struct vbuf_queue *> mOutputBufferQueues;
	avc_decoder_color_format_t mOutputColorFormat;
	CMVideoFormatDescriptionRef mFormatDescRef;
	VTDecompressionSessionRef mDecompressRef;
	unsigned int mWidth;
	unsigned int mHeight;
	unsigned int mCropLeft;
	unsigned int mCropRight;
	unsigned int mCropTop;
	unsigned int mCropBottom;
	unsigned int mCroppedWidth;
	unsigned int mCroppedHeight;
	unsigned int mSarWidth;
	unsigned int mSarHeight;
};

}

#endif /* USE_VIDEOTOOLBOX */

#endif /* !_PDRAW_AVCDECODER_VIDEOTOOLBOX_HPP_ */