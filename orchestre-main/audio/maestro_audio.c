#include "maestro_audio.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

void initOpenAL() {
    alutInit(0, NULL);
    alGetError();

    // position par défaut pour l'écoute
    alListener3f(AL_POSITION, 0, 0, 0);
    alListener3f(AL_VELOCITY, 0, 0, 0);
}

int playPause(Musician *musicos) {
    int state = -1;
    if (musicos->loadedSource == 1) {

        if (musicos->playing == 1) {
            alSourcePause(musicos->source);
            musicos->playing = 0;
            state = 0;
        } else {
            alSourcePlay(musicos->source);
            musicos->playing = 1;
            state = 1;
        }

    } else {
        perror("Musician source not loaded\n");
    }
    return state;
}

int changePosition(Musician *musicos, float vec[]) {
    int state = 0;
    if (musicos->loadedSource == 1) {
        printf("Updating source position : %f %f %f\n",vec[0], vec[1], vec[2]);
        alSource3f(musicos->source, AL_POSITION, vec[0], vec[1], vec[2]);
    } else {
        perror("Musician source not loaded\n");
        state = -1;
    }
    return state;
}

int changeOrientation(Musician *musicos, float vec[]) {
    int state = 0;
    if (musicos->loadedSource == 1) {
        alSource3f(musicos->source, AL_DIRECTION, vec[0], vec[1], vec[2]);
    } else {
        perror("Musician source not loaded\n");
        state = -1;
    }
    return state;
}

void setSourceParameters(Musician *musicos) {
    alSource3f(musicos->source, AL_POSITION, 0.0f, 0.0f, 0.0f);
    alSource3f(musicos->source, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
    alSourcef(musicos->source, AL_LOOPING, AL_FALSE);
    alSourcei(musicos->source, AL_BUFFER, (ALint) musicos->buffer);

    // Aténuation
    float globalRefDistance = 75.0f;
    float globalMaxDistance = 1250.0f;
    alSourcef(musicos->source, AL_REFERENCE_DISTANCE, globalRefDistance);
    alSourcef(musicos->source, AL_MAX_DISTANCE, globalMaxDistance);

    // Direction
    alSource3f(musicos->source, AL_DIRECTION, 0.0f, 0.0f, 0.0f);
    // Cone
    alSourcef(musicos->source, AL_CONE_INNER_ANGLE, 45.0f);
    alSourcef(musicos->source, AL_CONE_OUTER_ANGLE, 90.0f);
}

void deleteSourceBuffer(Musician * musicos) {
    alDeleteSources(1, &musicos->source);
    alDeleteBuffers(1, &musicos->buffer);
}

void changeSource(Musician *musicos, char *name) {
    if (musicos->loadedSource == 1) {
        // Suppression de l'ancienne source
        deleteSourceBuffer(musicos);
    }

    // Création de la nouvelle
    musicos->buffer = alutCreateBufferFromFile(name);
    musicos->source = 0;

    alGenSources(1, &musicos->source);
    alSourcei(musicos->source, AL_BUFFER, (ALint) musicos->buffer);

    if (musicos->loadedSource == 0) setSourceParameters(musicos);

    musicos->loadedSource = 1;

    if (musicos->playing) alSourcePlay(musicos->source);
}

#pragma clang diagnostic pop