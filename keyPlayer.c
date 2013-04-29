
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
int fp1 = 0; // Foot pedal is "true" when it's up - connected to a pullup

// Button States
int redButton = 0;
int greenButton = 0;
int blueButton = 0;

int shakePid = 0;
int shakeMode = 0;

int childCount = 0;
#define MAXCHILDREN 5

int airhornPid = 0;

void playShit(int keyCode) {

  if (childCount >= MAXCHILDREN) return;

  char* fname = NULL;
  int setShakePid = 0;
  int setAirhornPid = 0;

  switch((char)keyCode & 0xFF) {

    case 'a':
      fname = "Samples/chip snare.mp3";
      break;

    case 'b':
      fname = "Samples/chip kick.mp3";
      break;

    case 'c':
      fname = "Samples/chip boom.mp3";
      break;

    case 'd':
      fname = "Samples/bleep.mp3";
      break;

    case 'e':
      fname = fp1 ? "Samples/TR8-808CX 42 CHH 2.mp3" // TODO: Change to e and E
        : "Samples/TR8-808CX 46 OHH 2.mp3";
      break;

    case 'f':
      fp1 = 1;
      //p("DS1 Foot Pedal down");
      //return;
      break;

    case 'F':
      fp1 = 0;
      p("DS1 Foot Pedal up");
      return;

    case 'g':
      fname = "Samples/808 wet snare.mp3";
      break;

    case 'h':
      fname = "Samples/TR8-808CX 48 HTL 1.mp3";
      break;

    case 'i':
      fname = "Samples/TR8-808CX 45 MTL 2.mp3";
      break;

    case 'j':
      fname = "Samples/COWBELL.mp3";
      break;

    case 'k':
      fname = "Samples/TR8-808CX 52 CYH 5.mp3";
      break;

    case 'l':
      fname = "Samples/TR8-808CX 36 BD2 4.mp3";
      break;

    case 'm':
      fname = "Samples/click.mp3";
      break;

    case 'n':
      fname = "Samples/bongo.mp3";
      break;

    case 'o':
      fname = "Samples/explosion.mp3";
      break;

    case 'p':
      fname = "Samples/laser.mp3";
      break;

    case 'q':
      fname = "Samples/clap.mp3";
      break;   

    case 'r':
      fname = "Samples/gong.mp3";
      break;  

    case 's':
      if (airhornPid > 0) {
        kill(airhornPid, SIGKILL);
        airhornPid = 0;
      }
      fname = "Samples/airhorn.mp3";
      setAirhornPid = 1;
      break;

    case 't':  
    case 'T':
      fname = "Samples/rimshot.mp3";
      break;

    case 'u':
      fname = "Samples/robot 1.mp3";
      break;

    case 'w':
      fname = "Samples/robot 2.mp3";
      break;  

    case 'x':
      redButton = 1;
      break;

    case 'X':
      redButton = 0;
      break;

    case 'y':
      greenButton = 1;
      break;

    case 'Y':
      greenButton = 0;
      break;

    case 'z':
      blueButton = 1;
      break;

    case 'Z':
      blueButton = 0;
      break;

      // TODO: Record scratching sound effects

    case '0':
      if (shakeMode != 0 && shakePid > 0) {
        kill(shakePid, SIGKILL);
        shakeMode = 0;
        p("killed track");
        fname = "Samples/pickupneedle.mp3";
        shakePid = 0;
      }
      //return;
      break;

    case '1':
      if (shakeMode != 1) {
        if (shakePid > 0) kill(shakePid, SIGKILL);
        fname = "Samples/shake1.mp3";
        setShakePid = 1;
        shakeMode = 1;
      }
      break;

    case '2':
      if (shakeMode != 2) {
        if (shakePid > 0) kill(shakePid, SIGKILL);
        fname = "Samples/shake2.mp3";
        setShakePid = 1;
        shakeMode = 2;
      }
      break;

  }

  if (fname != NULL)
  {
    printf("%s\r\n", fname);
    fflush(stdout);

    char* const args[10] = { (char*)mpg, "--quiet", (char*)fname, NULL };

    int pid = fork();
    if (pid == 0) {
      execv(mpg, args);
    } else {
      childCount += 1;
    }

    if (setShakePid == 1) {
      shakePid = pid;
    }

    if (setAirhornPid == 1) {
      airhornPid = pid;
    }
  }

}

void child_handler(int sig) {
  printf("Child died\n");
  childCount -= 1;
}

int main() {

  signal(SIGCHLD, &child_handler);

  WINDOW *w = initscr();
  noecho();
  nodelay(w, FALSE);

  while(1) {
    int c = getch();
    playShit(c);
  }

}

