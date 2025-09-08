# minishell

## 1️⃣ 프로젝트 소개

Minishell은 Bash 셸의 핵심 기능을 직접 구현하는 시스템 프로그래밍 과제입니다.
사용자가 입력한 명령어를 해석하고, 필요한 경우 자식 프로세스를 생성해 실행하며, 파이프/리다이렉션을 통해 데이터 흐름을 제어합니다. 또한 환경 변수 확장, 시그널 처리, 빌트인 명령어 등을 지원해 실제 셸에 가까운 동작을 구현합니다.
보너스 단계에서는 다중 파이프라인, 논리 연산자(&&, ||), 와일드카드(*)까지 확장해 더 강력한 셸 환경을 제공합니다.

---

## 2️⃣ 과제 목표
	•	리눅스 프로세스 생성/제어(fork, execve, wait 등)에 대한 이해.
	•	표준 입출력 리디렉션 및 파이프(dup2, pipe) 활용 능력 습득.
	•	시그널(SIGINT, SIGQUIT 등)의 처리 및 사용자 경험 최적화.
	•	실제 Bash와 유사한 환경을 직접 구축하며 운영체제와 쉘의 본질 학습.
	•	보너스 구현을 통해 조건부 실행, 파일명 확장(globbing), 파이프 체인 등 고급 기능 경험.

---

## 3️⃣ 구현 사항
* 기본 기능
	- 빌트인 명령어 구현
	- echo, cd, pwd, export, unset, env, exit.
	- 환경 변수 처리
	- $VAR 구문을 통한 변수 확장.
	- export/unset을 통한 환경 변수 동적 변경.
	- 프로세스 실행
	- fork, execve를 통해 외부 명령 실행.
	- waitpid로 자식 프로세스 종료 상태 관리.
	- 리다이렉션
	- 입력 <, 출력 >, 추가 출력 >> 지원.
	- dup2를 이용해 표준 입출력 재지정.
	- 파이프
	- | 연산자를 통해 프로세스 간 데이터 전달 구현.
	- 시그널 처리
	- Ctrl+C (SIGINT), Ctrl+\ (SIGQUIT), Ctrl+D (EOF) 시 Bash와 동일 동작 수행.

* 보너스 기능
	- 다중 파이프라인
	- cmd1 | cmd2 | cmd3 ... 형태의 연속 파이프 실행 지원.
	- 논리 연산자
	- &&, || 구문 구현 → 조건부 명령 실행 가능.
	- 와일드카드(*) 확장
	- 디렉토리 내 파일 이름 패턴 매칭(globbing).

---

## 4️⃣ 배운 점
	•	프로세스 제어: fork, execve, waitpid의 실제 동작과 부모–자식 프로세스 관계를 깊이 이해.
	•	파일 디스크립터 관리: 파이프와 리다이렉션을 구현하며 자원 누수 방지와 에러 처리를 학습.
	•	시그널 처리: SIGINT, SIGQUIT 등 다양한 시그널을 잡아 사용자 친화적인 셸 경험 제공.
	•	셸 동작 원리 이해: 단순 명령 실행이 아닌, 환경 관리·조건부 실행·파이프 연결 등 복잡한 기능이 어떻게 조합되는지 직접 경험.
	•	협업 능력: 팀 단위로 프로젝트를 진행하며 Git을 활용한 버전 관리와 역할 분담을 경험.

---

## 5️⃣ 사용 방법
bash
```
$ ./minishell
minishell$ echo hello world
hello world
minishell$ ls -l | grep .c > out.txt
minishell$ cat out.txt
minishell$ export VAR=42
minishell$ echo $VAR
42
minishell$ ls nonexistent && echo success || echo fail
fail
```

---

## 6️⃣ 동작 영상
📝 Minishell 동작 테스트

#### 1️⃣ 기본 실행
	•	ls
	•	pwd
	•	echo hello world
	•	echo -n hello && echo world

#### 2️⃣ 환경 변수 관련
	•	export TEST=hello 후 echo $TEST
	•	unset TEST 후 echo $TEST (빈 출력 확인)
	•	$? → 직전 명령어 종료 상태 확인 (ls nosuchfile 후 echo $?)
	•	env → 현재 환경변수 출력 확인

#### 3️⃣ 경로/상대경로
	•	cd .. / cd /tmp / cd ~
	•	존재하지 않는 디렉토리: cd /notexist

https://github.com/user-attachments/assets/102279c2-a62b-4c5c-b5c6-8aeef1fb63d0

---

#### 4️⃣ 리다이렉션
	•	출력 리다이렉션: echo hello > out.txt → cat out.txt
	•	이어쓰기: echo world >> out.txt → cat out.txt
	•	입력 리다이렉션: cat < out.txt
	•	잘못된 파일 접근: cat < nofile

#### 5️⃣ 파이프
	•	단일 파이프: ls -l | grep minishell
	•	다중 파이프: cat out.txt | grep hello | wc -l

#### 6️⃣ 조합 (리다이렉션 + 파이프)
	•	cat < out.txt | grep hello > result.txt → cat result.txt

https://github.com/user-attachments/assets/3f2f6c5e-f60b-454f-9cb3-35290dee82ac

---

#### 7️⃣ 따옴표 처리
	•	echo "hello world" (쌍따옴표)
	•	echo 'hello world' (홑따옴표, 변수 미해석)
	•	echo "var=$USER" vs echo 'var=$USER'

#### 8️⃣ 빌트인 명령어
	•	exit (종료 확인)
	•	exit 42 (종료 코드 확인: $?)
	•	cd, export, unset, env, echo

https://github.com/user-attachments/assets/9ef40750-272c-4782-9b4c-05c065af32e3

---

#### 9️⃣ 시그널 처리
	•	minishell 실행 후 Ctrl+C → 새 줄 프롬프트 출력
	•	Ctrl+\ → core dump 없이 프롬프트 유지
	•	Ctrl+D → 정상 종료

#### 🔟 Bonus 기능 (구현하셨다면)
	•	와일드카드: ls *.c
	•	논리 연산자: false && echo no , true || echo no , true && echo yes
	•	그룹화/우선순위: (echo hello && echo world) || echo fail
	•	Here-document:
bash
```
cat << EOF
hello
world
EOF
```
 
https://github.com/user-attachments/assets/b2bb5aa0-b4cb-4e9a-bb31-403abb64a32c

https://github.com/user-attachments/assets/5aaacb99-3fba-41d6-82f2-e779f221b05e

---

## 7️⃣ 기술 스택
	•	언어: C
	•	운영체제/환경: Unix/Linux
	•	시스템 콜: read, write, open, close, access, fork, wait/waitpid/wait3/wait4, kill, exit, getcwd, chdir, stat/lstat/fstat, unlink, execve, dup, dup2
	•	빌드 도구: GNU Make
	•	협업 도구: Git

 ## 8️⃣ Reference
 	•	https://www.die.net/
  	•	https://www.gnu.org/software/bash/manual/html_node/index.html#SEC_Contents
    •	https://tldp.org/LDP/abs/html/index.html
    •	https://pubs.opengroup.org/onlinepubs/9699919799/utilities/contents.html
    •	https://man7.org/linux/man-pages/index.html
