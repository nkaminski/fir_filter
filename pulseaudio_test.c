/***
  Portions of this file from PulseAudio

  PulseAudio is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published
  by the Free Software Foundation; either version 2.1 of the License,
  or (at your option) any later version.

  PulseAudio is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  General Public License for more details.
  You should have received a copy of the GNU Lesser General Public License
  along with PulseAudio; if not, see <http://www.gnu.org/licenses/>.
***/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <pulse/simple.h>
#include <pulse/error.h>
#include "fir.h"
#include <assert.h>

#define BUFSIZE 1024


int main(int argc, char*argv[]) {
    /* The sample format to use */
    static const pa_sample_spec ss = {
        .format = PA_SAMPLE_FLOAT32LE,
        .rate = 44100,
        .channels = 1
    };

    pa_simple *sin = NULL, *sout = NULL;
    int ret = 1;
    int i;
    int error;

    /* Create recording/input stream */
    if (!(sin = pa_simple_new(NULL, argv[0], PA_STREAM_RECORD, NULL, "record", &ss, NULL, NULL, &error))) {
        fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
        goto finish;
    }    

    /* Create playback/output stream */
    if (!(sout = pa_simple_new(NULL, argv[0], PA_STREAM_PLAYBACK, NULL, "playback", &ss, NULL, NULL, &error))) {
        fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
        goto finish;
    }
    //FIR filter init
    init_fir(); 
    for (;;) {
        uint8_t buf[BUFSIZE];

        /* Record some data ... */
        if (pa_simple_read(sin, buf, sizeof(buf), &error) < 0) {
            fprintf(stderr, __FILE__": pa_simple_read() failed: %s\n", pa_strerror(error));
            goto finish;
        }
        // ---APPLY FILTER---
        for(i=0; i<BUFSIZE/4; i++){
            *((float *)(buf + (i*4))) = iter_fir(*((float *)(buf + (i*4))));
        }
        /* ... and play it */
        if (pa_simple_write(sout, buf, sizeof(buf), &error) < 0) {
            fprintf(stderr, __FILE__": pa_simple_write() failed: %s\n", pa_strerror(error));
            goto finish;
        }
    }
    ret = 0;
finish:
    if (sin)
        pa_simple_free(sin);
    if (sout)
        pa_simple_free(sout);
   return ret;
}
