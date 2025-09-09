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
* 운영체제 이해 심화
	- fork, execve, dup2, pipe, waitpid 등의 시스템 콜을 사용하면서 프로세스 생성과 자원 분리, 입출력 리다이렉션의 내부 동작을 깊이 이해
	- 부모와 자식 프로세스의 역할을 나누는 과정에서 프로세스 간 메모리 공간이 어떻게 분리되는지 학습
* 시그널과 인터럽트 처리
	- SIGINT, SIGQUIT 같은 시그널을 다루며 사용자 입력(Ctrl-C 등)이 프로세스 동작에 어떻게 개입하는지 체득
	- readline 라이브러리와 시그널 핸들러를 함께 다루면서, 인터랙티브 프로그램에서 안정적으로 사용자 입력을 유지하는 방법을 학습
* 쉘 동작 원리와 파이프라인
	- Bash에서 당연하게 쓰던 파이프(|)가 실제로는 파일 디스크립터 복제와 프로세스 체인으로 연결된다는 것을 직접 구현하며 확인
	- 여러 프로세스를 직렬로 실행할 때, 부모 프로세스가 적절히 close()를 해주지 않으면 자식 프로세스가 EOF를 받지 못해 무한 대기 상태에 빠진다는 점을 경험
* 리다이렉션과 파일 디스크립터 관리
	- 단순 출력/입력 변경뿐만 아니라 >> append, << here_doc 같은 기능을 구현하면서 파일 디스크립터를 원하는 방향으로 자유롭게 조작하는 방법 학습
	- 이를 통해 운영체제 수준에서 표준 입출력 스트림이 단순히 파일 디스크립터의 별칭이라는 사실을 이해
* 환경 변수와 빌트인 구현
	- export, unset, env 같은 명령어를 구현하면서 환경 변수 테이블이 단순한 key=value 구조 배열이라는 사실을 학습
	- $? 구현을 통해 프로세스의 종료 상태 코드가 후속 실행의 흐름에 얼마나 중요한지 실감
* 메모리 관리와 에러 처리
	- 리다이렉션, 파이프, 환경 변수 확장 등에서 동적으로 문자열과 구조체를 할당/해제하면서 메모리 누수와 double free를 막는 코드 작성 습관 강화
	- 단순히 실행만 되는 코드가 아니라, 비정상 입력과 오류 상황에 견고하게 대처할 수 있는 코드를 작성하는 훈련
* 협업 경험
	- 팀원과 역할을 나누어 백엔드 로직과 빌트인 구현을 협업하며, Git 브랜치 전략과 코드 리뷰 문화를 실제 프로젝트에 적용

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

## 6️⃣ 기본 동작 테스트 영상
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
 	•	빌드 도구: GNU Make
  	•	버전관리 도구: Git
 	•	Coding Conventions: Norminette
	•	시스템 콜: read, write, open, close, access, fork, wait/waitpid/wait3/wait4, kill, exit, getcwd, chdir, stat/lstat/fstat, unlink, execve, dup, dup2

---

 ## 8️⃣ Reference
 	•	https://www.die.net/
  	•	https://www.gnu.org/software/bash/manual/html_node/index.html#SEC_Contents
    •	https://tldp.org/LDP/abs/html/index.html
    •	https://pubs.opengroup.org/onlinepubs/9699919799/utilities/contents.html
    •	https://man7.org/linux/man-pages/index.html
