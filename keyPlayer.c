
#include <unistd.h>
#include <ncurses.h>
#include <signal.h>
#include <stdio.h>

#ifdef PI
const char* mpg = "/usr/bin/mpg321";
#else
const char* mpg = "/usr/local/bin/mpg321";
#endif

void p(char* s) {
  printf("%s\n",s);
  fflush(stdout);
}

// Foot Pedal State
int fp1 = 0;

int shakePid = 0;
int shakeMode = 0;

void playShit(int keyCode) {
 
  char* fname = NULL;
  int setShakePid = 0;

  switch((char)keyCode & 0xFF) {

  case 'a':
    fname = "808/808wetsnr.19.mp3";
    break;

  case 'b':
    fname = "808/TR8-808CX 48 HTL 1.mp3";
    break;

  case 'c':
    fname = "808/TR8-808CX 45 MTL 2.mp3";
    break;

  case 'd':
    fname = "808/dj.mp3";
    break;

  case 'e':
    fname = fp1 ? "808/TR8-808CX 42 CHH 2.mp3"
                : "808/TR8-808CX 46 OHH 2.mp3";
    break;

  case 'f':
    fp1 = 1;
    fname = "808/TR8-808V-Kick.15.mp3";
    //p("DS1 Foot Pedal down");
    //return;
    break;

  case 'F':
    fp1 = 0;
    p("DS1 Foot Pedal up");
    return;

  case '0':
    if (shakeMode != 0 && shakePid > 0) {
      kill(shakePid, SIGKILL);
      shakeMode = 0;
      p("killed track");
      shakePid = 0;
    }
    return;
    
  case '1':
    if (shakeMode != 1) {
      if (shakePid > 0) kill(shakePid, SIGKILL);
      fname = "808/shake1.mp3";
      setShakePid = 1;
      shakeMode = 1;
    }
    break;
    
  case '2':
    if (shakeMode != 2) {
      if (shakePid > 0) kill(shakePid, SIGKILL);
      fname = "808/shake1.mp3";
      setShakePid = 1;
      shakeMode = 2;
    }
    break;

  }

  printf("%s\r\n", fname);
  fflush(stdout);

  char* const args[10] = { (char*)mpg, "--quiet", (char*)fname, NULL };

  int pid = fork();
  if (pid == 0) {
    execv(mpg, args);
  }
  if (setShakePid == 1) {
    shakePid = pid;
  }

}

int main() {

  WINDOW *w = initscr();
  noecho();
  nodelay(w, FALSE);
  
  while(1) {
    int c = getch();
    playShit(c);
  }

}

