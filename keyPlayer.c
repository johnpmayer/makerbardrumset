
#include <unistd.h>
#include <ncurses.h>
#include <stdio.h>

#ifdef PI
const char* mpg = "/usr/bin/mpg321";
#else
const char* mpg = "/usr/local/bin/mpg321";
#endif

const char* instruments[90] = {
"808/808clapgate.mp3"
,"808/808claproom 1.mp3"
,"808/808kickgate2.mp3"
,"808/808rimroom.mp3"
,"808/808sngate.mp3"
,"808/808sngatedplate.mp3"
,"808/808snplate.mp3"
,"808/808wetsnr.19.mp3"
,"808/TR8-808CH.mp3"
,"808/TR8-808CX 31 SN3 1.mp3"
,"808/TR8-808CX 31 SN3 3.mp3"
,"808/TR8-808CX 31 SN3 5.mp3"
,"808/TR8-808CX 35 BD1 4.mp3"
,"808/TR8-808CX 36 BD2 4.mp3"
,"808/TR8-808CX 36 BD2 8.mp3"
,"808/TR8-808CX 38 SN1 2.mp3"
,"808/TR8-808CX 38 SN1 6.mp3"
,"808/TR8-808CX 38 SN1 8.mp3"
,"808/TR8-808CX 40 SN2 1.mp3"
,"808/TR8-808CX 40 SN2 3.mp3"
,"808/TR8-808CX 40 SN2 5.mp3"
,"808/TR8-808CX 41 FTL 1.mp3"
,"808/TR8-808CX 41 FTL 6.mp3"
,"808/TR8-808CX 42 CHH 2.mp3"
,"808/TR8-808CX 44 PHH 6.mp3"
,"808/TR8-808CX 45 MTL 2.mp3"
,"808/TR8-808CX 45 MTL 8.mp3"
,"808/TR8-808CX 46 OHH 2.mp3"
,"808/TR8-808CX 46 OHH 8.mp3"
,"808/TR8-808CX 48 HTL 1.mp3"
,"808/TR8-808CX 48 HTL 7.mp3"
,"808/TR8-808CX 49 CYL 7.mp3"
,"808/TR8-808CX 52 CYH 5.mp3"
,"808/TR8-808CX 56 CB1 8.mp3"
,"808/TR8-808CX 62 LCL 4.mp3"
,"808/TR8-808CX 63 LCH 1.mp3"
,"808/TR8-808CX 64 MCL 2.mp3"
,"808/TR8-808CX 65 MCH 1.mp3"
,"808/TR8-808CX 66 HCL 3.mp3"
,"808/TR8-808CX 70 MA1 2.mp3"
,"808/TR8-808CX 75 CLV 2.mp3"
,"808/TR8-808CY2.mp3"
,"808/TR8-808Drms.01.mp3"
,"808/TR8-808Drms.02.mp3"
,"808/TR8-808Drms.04.mp3"
,"808/TR8-808Drms.05.mp3"
,"808/TR8-808Drms.06.mp3"
,"808/TR8-808Drms.07.mp3"
,"808/TR8-808Drms.08.mp3"
,"808/TR8-808Drms.09.mp3"
,"808/TR8-808Drms.10.mp3"
,"808/TR8-808Drms.11.mp3"
,"808/TR8-808Drms.14.mp3"
,"808/TR8-808Drms.16.mp3"
,"808/TR8-808Drms.17.mp3"
,"808/TR8-808Drms.18.mp3"
,"808/TR8-808OH1.mp3"
,"808/TR8-808OH2.mp3"
,"808/TR8-808Rimshot2.mp3"
,"808/TR8-808V-Kick.11.mp3"
,"808/TR8-808V-Kick.15.mp3"
,"808/TR8-808V-Kick.17.mp3"
,"808/TR8-808V-Kick.19.mp3"
,"808/TR8-808V-Kick.21.mp3"
,"808/TR8-808V-Kick.3.mp3"
,"808/TR8-808V-Kick.5.mp3"
,"808/TR8-808V-snr.1.mp3"
,"808/TR8-808V-snr.11.mp3"
,"808/TR8-808V-snr.13.mp3"
,"808/TR8-808V-snr.15.mp3"
,"808/TR8-808V-snr.17.mp3"
,"808/TR8-808V-snr.19.mp3"
,"808/TR8-808V-snr.21.mp3"
,"808/TR8-808V-snr.23.mp3"
,"808/TR8-808V-snr.25.mp3"
,"808/TR8-808V-snr.27.mp3"
,"808/TR8-808V-snr.29.mp3"
,"808/TR8-808V-snr.3.mp3"
,"808/TR8-808V-snr.31.mp3"
,"808/TR8-808V-snr.33.mp3"
,"808/TR8-808V-snr.5.mp3"
,"808/TR8-808V-snr.7.mp3"
,"808/TR8-808V-snr.9.mp3"
,"808/TR8-808hh2b.09.mp3"
,"808/TR8-808hhb.07.mp3"
,"808/TR8-808kick6c.mp3"
,"808/TR8-808ohb.11.mp3"
,"808/TR8-808sngated.mp3"
,"808/TR8-tambo1.mp3"
,"808/neutr808kick.mp3"
};

void playShit(int keyCode) {
  
  int sampleCode = keyCode - 97;

  if (sampleCode > 89 || sampleCode < 0) {
    printf("Complain bad index");
    fflush(stdout);
    return;
  }
  
  const char* fname = instruments[sampleCode];

  printf("%s\r\n", fname);
  fflush(stdout);

  const char* const args[10] = { mpg, "--quiet", "-k", "3", fname, NULL };

  int pid = fork();
  if (pid == 0) {
    execv(mpg, args);
  }

}

int main() {

  WINDOW *w = initscr();
  noecho();
  nodelay(w, FALSE);
  
  while(1) {
    int c = getch();
    printf("%d\r\n", c);
    fflush(stdout);
    playShit(c);
  }

}

