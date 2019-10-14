# coding: utf-8

import serial
import json
import argparse
import pygame
from scipy.io import wavfile
import sys
import numpy as np

PEN_EVENT_STAY = 0
PEN_EVENT_PUSH = 1
PEN_EVENT_RELEASE = 2


def speedx(snd_array, factor):
    """ Speeds up / slows down a sound, by some factor. """
    indices = np.round(np.arange(0, len(snd_array), factor))
    indices = indices[indices < len(snd_array)].astype(int)
    return snd_array[indices]


def stretch(snd_array, factor, window_size, h):
    """ Stretches/shortens a sound, by some factor. """
    phase = np.zeros(window_size)
    hanning_window = np.hanning(window_size)
    result = np.zeros(int(len(snd_array) / factor + window_size))

    for i in np.arange(0, len(snd_array) - (window_size + h), h * factor):
        i = int(i)
        # Two potentially overlapping subarrays
        a1 = snd_array[i:i + window_size]
        a2 = snd_array[i + h:i + window_size + h]

        # The spectra of these arrays
        s1 = np.fft.fft(hanning_window * a1)
        s2 = np.fft.fft(hanning_window * a2)

        # Rephase all frequencies
        phase = (phase + np.angle(s2 / s1)) % 2 * np.pi

        a2_rephased = np.fft.ifft(np.abs(s2) * np.exp(1j * phase))
        i2 = int(i / factor)
        result[i2:i2 + window_size] += hanning_window * a2_rephased.real

    # normalize (16bit)
    result = ((2**(16 - 4)) * result / result.max())

    return result.astype('int16')


def pitchshift(snd_array, n, window_size=2**13, h=2**11):
    """ Changes the pitch of a sound by ``n`` semitones. """
    factor = 2**(1.0 * n / 12.0)
    stretched = stretch(snd_array, 1.0 / factor, window_size, h)
    return speedx(stretched[window_size:], factor)


def parse_arguments():
    description = ('Use your computer keyboard as a "piano"')

    parser = argparse.ArgumentParser(description=description)
    parser.add_argument(
        '--wav',
        '-w',
        metavar='FILE',
        type=argparse.FileType('r'),
        default='tudumi.wav',
        # default='bowl.wav',
        help='WAV file (default: bowl.wav)')
    # parser.add_argument('--keyboard',
    #                     '-k',
    #                     metavar='FILE',
    #                     type=argparse.FileType('r'),
    #                     default='typewriter.kb',
    #                     help='keyboard file (default: typewriter.kb)')
    parser.add_argument('--verbose',
                        '-v',
                        action='store_true',
                        help='verbose mode')

    return (parser.parse_args(), parser)


def main():
    (args, parser) = parse_arguments()

    fps, sound = wavfile.read(args.wav.name)
    sys.stdout.write('Transponding sound file... ')
    sys.stdout.flush()
    # tones = range(-25, 25)
    # transposed_sounds = [pitchshift(sound, n) for n in tones]

    pygame.mixer.init(fps, -16, 1, 2048)
    sound_pushed = pygame.sndarray.make_sound(pitchshift(sound, 0))
    sound_released = pygame.sndarray.make_sound(pitchshift(sound, 10))
    print('DONE')
    # screen = pygame.display.set_mode((150, 150))
    # sounds = list(map(pygame.sndarray.make_sound, transposed_sounds))
    # sounds = list(map(pygame.sndarray.make_sound, transposed_sounds))

    ser = serial.Serial('COM3', 57600, timeout=None)
    # ser = serial.Serial('/dev/ttyS3', 9600, timeout=None)
    while True:
        # event = pygame.event.wait()
        # if event.type == pygame.KEYDOWN and event.key == pygame.K_ESCAPE:
        #     pygame.quit()
        #     raise KeyboardInterrupt

        data = None
        line = ser.readline()
        is_playing = {"push": False, "release": False}
        try:
            data = json.loads(line)
            print(data)
        except ValueError:
            continue

        # play music
        # print(data["state"])
        if PEN_EVENT_PUSH == data["state"]:
            sound_pushed.play(fade_ms=50)
            # sound_pushed.play(fade_ms=50)
            sound_released.fadeout(50)
            is_playing["push"] = True
            is_playing["release"] = False
        if PEN_EVENT_RELEASE == data["state"]:
            sound_released.play(fade_ms=150)
            sound_pushed.fadeout(50)
            is_playing["release"] = True
            is_playing["push"] = False

        # print(line)

    ser.close()


if __name__ == "__main__":
    main()
