#include "LinuxOggInputStream.hpp"
#include <stdexcept>
#include <gdx-cpp/utils/Buffer.hpp>
#include <string>

using namespace gdx_cpp::backends::nix;

void LinuxOggInputStream::open(std::string path)
{
    int result;

    if (!(oggFile = fopen(path.c_str(), "rb")))
        throw std::runtime_error("Could not open Ogg file.");

    if ((result = ov_open(oggFile, &oggStream, NULL, 0)) < 0)
    {
        fclose(oggFile);

        throw std::runtime_error("Could not open Ogg stream. ");
    }

    vorbisInfo = ov_info(&oggStream, -1);
    vorbisComment = ov_comment(&oggStream, -1);

    if (vorbisInfo->channels == 1)
        format = AL_FORMAT_MONO16;
    else
        format = AL_FORMAT_STEREO16;
}

int LinuxOggInputStream::getChannels() {
    return vorbisInfo->channels;
}

int LinuxOggInputStream::getSampleRate() {
    return vorbisInfo->rate;
}


void LinuxOggInputStream::release()
{
    ov_clear(&oggStream);
}



int LinuxOggInputStream::read(char * pcm, int max)
{

    int  size = 0;
    int  section;
    int  result;

    while (size < max)
    {
        result = ov_read(&oggStream, pcm + size, max - size, 0, 2, 1, &section);

        if (result > 0)
            size += result;
        else
            if (result < 0)
                throw std::runtime_error("Error reading file.");
            else
                break;
    }
    return size;
}
