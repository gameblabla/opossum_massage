#/!bin/sh
ffmpeg -i "start.wav" -f s16be -acodec pcm_s16be -ac 1 -r 11025 start.raw
ffmpeg -i "confirm.wav" -f s16be -acodec pcm_s16be -ac 1 -r 11025 confirm.raw
ffmpeg -i "cancel.wav" -f s16be -acodec pcm_s16be -ac 1 -r 11025 cancel.raw
ffmpeg -i "gameover.wav" -f s16be -acodec pcm_s16be -ac 1 -r 11025 gameover.raw
ffmpeg -i "warn.wav" -f s16be -acodec pcm_s16be -ac 1 -r 11025 warn.raw
ffmpeg -i "bubbleyard.wav" -f s16be -acodec pcm_s16be -ac 1 -r 11025 bubbleyard.raw
ffmpeg -i "unicorn.wav" -ac 1 -r 11025  -f s16be -acodec pcm_s16be unicorn.raw
ffmpeg -i "story.wav" -f s16be -acodec pcm_s16be -ac 1 -r 11025 story.raw
