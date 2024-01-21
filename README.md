# Linux Kernel Install
Operating System Assignment #1

VMWare를 이용하여 Ubuntu 설치
linux-5.15.120 컴파일 및 설치
리눅스 ps 명령어와 유사한 결과 출력할 수 있는 newps 명령어 구현

- **컴파일된 커널 설치 이전 화면** 

![](/src/Aspose.Words.bc1e6474-bd91-4c1e-899e-2603eccb97eb.001.jpeg)

- **과정** 

$ sudo apt install vim build-essential libncurses5 libncurses5-dev bin86 libssl-dev bison flex libelf-dev kernel-package 

$ wget[ https://www.kernel.org/pub/linux/kernel/v5.x/linux-5.15.120.tar.xz ](https://www.kernel.org/pub/linux/kernel/v5.x/linux-5.15.120.tar.xz)

위 두 명령어를 입력하여 linux 5.15.120 커널을 설치한다. 

![](/src/Aspose.Words.bc1e6474-bd91-4c1e-899e-2603eccb97eb.002.jpeg)

$ mkdir /usr/src/linux 

linux  커널을  저장할  디렉토리를  새로  만들어준다. 

$ cp /home/leejin/linux-5.15.120.tar.xz /usr/src/linux 

다운받은  linux  커널  압축  파일을  새로  만들어준  디렉토리로  파일을  복사한다. 

$ cd /usr/src/linux 

현재  경로를  압축파일이  있는  새로  만든  디렉토리로  변경한다. 

$ ls 

현재  해당  디렉토리에  linux-5.15.120.tar.xz  라는  압축파일이  존재함을  확인한다. 

$ tar -xf linux-5.15.120.tar.xz 해당  압축파일을  푼다. 

$ ls 

linux-5.15.120.tar.xz  의  압축을  푼  linux-5.15.120이  있음을  확인한다. 

$ uname -r 

현재  커널  버전  정보를  확인한다 

$ cd linux-5.15.120 

압축을  푼  파일로  경로를  이동한다. 

$ cp /boot/config-5.4.0-162-generic ./.config 

커널  설정  파일이  있는  /boot  디렉토리에  있는  현재  버전의  커널  설정  파일을  현재  디렉 토리에  .config로  파일을  복사한다. 

$ make menuconfig 

아래와  같은  창이  뜬다.  방향키와  엔터를  이용해  Load와  Save에  모두  .config를  입력  후 

Exit  한다. 

![](/src/Aspose.Words.bc1e6474-bd91-4c1e-899e-2603eccb97eb.003.jpeg)

$ sudo apt install dwarves 

$ sudo apt install zstd 

.tmp\_vmlinux.btf: pahole is not available  오류와  zstd  관련  오류를  방지  하기  위해  위  두  명령

어를  입력한다. $ sudo vi .config 

아래와  같은  창이  뜬다.  편집기  모드로  바꿔준  후,  CONFIG\_SYSTEM\_TRUSTED\_KEYS와 CONFIG\_SYSTEM\_REVOCATION\_KEYS에  할당된  값을  빈  문자열로  바꿔준  후, :wq로  저장하고 나온다. 

![](/src/Aspose.Words.bc1e6474-bd91-4c1e-899e-2603eccb97eb.004.jpeg)

![](/src/Aspose.Words.bc1e6474-bd91-4c1e-899e-2603eccb97eb.005.png)

$ grep -c processor /proc/cpuinfo 

커널을  다운받는  데에는  오래  걸리기  때문에  위의  명령어로  Linux  시스템에서  CPU의  코 어  수를  확인한다. 2  로  출력된  값은  CPU의  코어  수가  2개임을  의미한다. 

$ make-kpkg –J 2 –initrd –revision=1.0 kernel\_image 

--J 2는 병렬 컴파일 옵션이다. CPU의 코어 수가 2개인 것을 확인했으니 2개의 병렬 프로

세스를 사용하여 컴파일한다. 

--initred는 마운트 하기 전 필요한 모듈들과 초기 설정을 포함하는 옵션이다. --revision=1.0 생성되는 커널 패키지의 revision 번호를 지정하여 패키지 버전 관리를 유 용하게 해주는 옵션이다. (저는 계속 오류가 나서 해당 revision 번호를 2.0으로 설정하였 습니다.) 

kernel\_image 커널 이미지 패키지를 생성하도록 지시한다. 

컴파일이 완료될 때까지 기다린다. 

컴파일 완료후 

$ cd ../ 

$ ls 

$ dpkg -i linux-image-5.15.120\_2.0\_arm64.deb 

ls를 통해 패캐지 파일명을 확인한 후, dpkg를 통해 해당 패키지 파일을 설치한다. 이를 통해 해당 패키지 파일이 시스템에 설치된다. 

$ reboot 

- **컴파일된 커널 설치 이후 화면** 

![](/src/Aspose.Words.bc1e6474-bd91-4c1e-899e-2603eccb97eb.006.jpeg)

- **newps  명령어  구현한  과정** 

ps는  process  status의  약자로,  현재  터미널에서  실행  중인  프로세스의  목록을  출력한다. 명령어의  결과는  여러  칼럼으로  구성되어  있는데,  이  중  PID는  프로세스  ID를  의미하고, TTY는  프로세스에  연결된  터미널을  의미하고  TIME은  CPU에서  프로세스가  사용한  총  시 간을  의미하고  CMD는  실행된  명령어나  프로그램  이름을  의미한다. 

이와  같은  결과를  출력하기  위해  newps.c  파일에  /proc file system을  활용하여  해당  기능을 구현한다.  newps  실행파일을  실행시켰을  때  ps와  유사한  결과를  출력해야한다. /proc/[pid]/stat  파일에는  여러  정보가  있는데  그  중, comm(실행파일  이름), tty\_nr(프로세스 가  연결된  TTY의  minor  번호), utime(사용자  모드에서  소비된  CPU  시간), stime(커널  모드에 서  소비된  CPU  시간)을  활용한다. comm은  CMD  값에  이용되고, tty\_nr은  TTY  값에, utime, stime은  TIME에  이용된다.  각각의  필드는  2번째, 7번째, 14번째, 15번째있음을  확인했다. 현재  터미널에서  실행  중인  프로세스  중  확실하게  알  수  있는  건  newps이다.  그러므로 tty\_nr값이  newps의  tty\_nr값과  같은  프로세스만  출력한다. newps의  tty\_nr  값을  찾는  함수 가  get\_newps\_tty(pid\_t pid)  이다.  해당  함수에서  newps의  tty\_nr값을  찾으면  해당  값은  전 역변수  newps\_tty에  저장된다. 

그  후  print\_process\_info(pid\_t pid)  함수에서  각각의  pid에  대해  /proc/[pid]/stat  파일을  열어 comm, tty\_nr, utime, stime에  대한  정보를  읽고  저장한다. tty\_nr  이  newps\_tty랑  다르다면  결 과값을  출력하지  않고  해당  함수를  종료한다.  만약,  같다면  함수를  계속  진행시켜  출력값 을  얻어낸다. 

얻어낸  tty\_nr값은  major number와  minor number  두  부분이  포함되어  있는  값이다. ps  명령 어에서  출력되는  TTY  필드의  값은  minor number에  해당된다. major number는  tty\_nr / 8  해 서  얻을  수  있고, minor number는  tty\_nr % 8  해서  얻을  수  있다.  그러므로  pts/  뒤  숫자  자 리에  tty\_nr % 8  값을  넣어준  문자열을  출력한다. 

TIME은  CPU에서  프로세스가  사용한  총  시간이다.  utime과  stime을  합쳐야하는데  이들은 jiffies  라는  단위로  반환된다.  그러므로  sysconf(\_SC\_CLK\_TCK)  함수를  활용한다.  이  함수는 초당  jiffies  수를  반환한다. (utime + stime) / sysconf(\_SC\_CLK\_TCK)  이  연산을  통해  프로세스가 사용한  전체  CPU시간을  초  단위로  변환할  수  있다.  이를  ps  명령어의  출력  결과와  같이 시:분:초  형식으로  변환하기  위해서  hours는  3600으로  나누고, minutes는  시간  부분을  제 외한  나머지를  60으로  나누고, seconds는  시간과  분을  계산한  후  남은  나머지  초를  저장 하여  출력한다. 

CMD는  comm에서  얻을  수  있는데  comm은  (CMD)값이다.  그러므로  괄호  부분을  제외하고 저장받게  포매팅해서  올바른  결괏값을  얻을  수  있다.  이렇게  얻은  결괏값은  printf의  형식 지정자를  활용하여  적절한  너비를  설정하여  ps  명령어  결괏값과  유사하게  출력할  수  있 도록  한다. 

- **newps.c  주석  설명** 

#include <stdio.h>       //  표준  입출력  함수를  사용하기  위한  헤더 #include <stdlib.h>      // atoi()  등의  함수를  사용하기  위한  헤더 #include <dirent.h>      //  디렉터리  관련  함수를  사용하기  위한  헤더 #include <string.h>      //  문자열  처리  함수를  사용하기  위한  헤더 #include <sys/types.h>   //  데이터  타입  정의를  포함하는  헤더 #include <unistd.h>      // sysconf()  등의  함수를  사용하기  위한  헤더 

int newps\_tty; 

//  특정  PID를  가진  프로세스의  comm  필드를  확인하여  "newps"  인지  확인하고,  해당  프 로세스의  tty\_nr  값을  가져오는  함수 

int get\_newps\_tty(pid\_t pid) {      char path[256]; 

`     `char comm[256]; 

`     `int tty\_nr; 

`     `FILE \*file; 

// /proc/[PID]/stat  파일  경로를  생성 sprintf(path, "/proc/%d/stat", pid); 

//  해당  경로의  파일을  읽기  모드로  열기 

file = fopen(path, "r"); 

//  파일의  내용에서  PID, comm, tty\_nr  값을  가져오기  (나머지는  무시) fscanf(file, "%\*d (%255[^)]) %\*c %\*d %\*d %\*d %d", comm, &tty\_nr); 

fclose(file);   //  파일  닫기 

// comm  값이  "newps"  인지  확인 if (strcmp(comm, "newps") == 0) { 

newps\_tty = tty\_nr;   // tty\_nr 값을  전역  변수에  저장 return 1;             // newps를  찾았으므로  1  반환 

`     `} 

`     `return 0;   // newps가  아니므로  0  반환 } 

//  특정  PID를  가진  프로세스의  정보를  출력하는  함수 

void print\_process\_info(pid\_t pid) { 

`     `char path[256]; 

`     `char cmdline[256] = {0}; 

`     `char tty\_name[16]; 

`     `int tty\_nr; 

`     `unsigned long utime = 0, stime = 0;      FILE \*file; 

// /proc/[PID]/stat  파일  경로를  생성 

sprintf(path, "/proc/%d/stat", pid); 

file = fopen(path, "r");   //  해당  경로의  파일을  읽기  모드로  열기 

`     `//  파일이  정상적으로  열렸는지  확인 

`     `if (file) { 

`         `//  파일의  내용에서  필요한  정보만  추출  (나머지는  무시) 

`         `fscanf(file, "%\*d (%255[^)]) %\*c %\*d %\*d %\*d %d %\*d %\*d %\*d %\*d %\*d %\*d %ld %ld", 

cmdline, &tty\_nr, &utime, &stime); 

`         `fclose(file);   //  파일  닫기 

`     `} else { 

`         `return;   //  파일이  열리지  않았다면  함수  종료      } 

//  현재  프로세스의  tty\_nr  값이  newps\_tty  값과  다르면  함수  종료 if (tty\_nr != newps\_tty) return; 

// tty\_nr  값에서  실제  TTY  번호  추출 snprintf(tty\_name, sizeof(tty\_name), "pts/%d", tty\_nr%8); 

// CPU  사용  시간  계산 

unsigned long total\_second = (utime + stime) / sysconf(\_SC\_CLK\_TCK); unsigned long hours = total\_second / 3600; 

unsigned long minutes = (total\_second % 3600) / 60; 

unsigned long seconds = total\_second % 60; 

`     `//  프로세스  정보  출력 

`     `printf("%7d %-8s %02lu:%02lu:%02lu %s\n", pid, tty\_name, hours, minutes, seconds, cmdline); } 

int main() { 

`     `DIR \*dir; 

`     `struct dirent \*entry; 

// /proc  디렉터리  열기 

dir = opendir("/proc"); 

if (dir == NULL) return 1;   //  디렉터리가  열리지  않으면  에러  반환 

`     `// /proc  디렉터리  내의  각  항목을  읽어  "newps"  프로세스를  찾기 

`     `while((entry = readdir(dir)) != NULL) { 

`         `int pid = atoi(entry->d\_name);   //  디렉터리  이름을  숫자로  변환  (PID  값) 

`         `if (pid > 0) { 

`             `if (get\_newps\_tty(pid)) break;   // newps  프로세스를  찾으면  반복  종료 

`         `}      } 

`     `closedir(dir);   // /proc  디렉터리  닫기 

- **ps와  newps  출력  결과  화면** 

![](/src/Aspose.Words.bc1e6474-bd91-4c1e-899e-2603eccb97eb.007.jpeg)
