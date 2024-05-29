 ## 게임 설계과정
> 1. 활용한 라이브러리 : SDL,SDL_ttf
> 2. a, d와 방향키를 이용해 플레이를 할수 있는 2인용 게임을 만든다.
> 3. 타이머를 1분과 블럭을 깰 때마다 증가하는 점수를 만들어 승자가 생기게 하였다.
> 4. 블럭을 깰 때 20%의 학률로 공의 속도가 2배가 되게 바꾸었고 공이 바닥에 닿거나 5초가 지나면 원래 속도로 돌아가게 하였다.
> 5. 
***
## 게임 작동 방법
1. [라이브러리 참고 파일](https://github.com/ETKorea/library) 에 들어가 파일을 다운
2. 파일들 압축 해제
3. 압축 해제한 폴더들을 C드라이브에 복사
4. 해당 프로그램을 만든 프로젝트를 클릭 후 프로젝트 속성에 들어간다.
     1. ```C/C++ -> 일반 -> 추가 포함 디렉터리```에서 편집을 눌러 c로 옮긴 각 폴더중 ```include```부분을 선택
     2. ```C/C++ -> 일반 -> SDL 검사 ``` 아니요 변경
     3. ```링커-> 일반 -> 추가  라이브러리 디렉터리 ``` 에서 편집을 눌러 c로 옮긴 각 폴더중 ```lib\x64```을 선택
     4. ```링커-> 입력 -> 추가 종속성 ``` 에서 ```SDL2.lib, SDL2main.lib, SDL2_ttf.lib ```을 추가
     5. ```C:\Windows\System32```에 각 폴더에 있던 ```.dll```파일을 복사
     6. ```C:\Users\사용자이름\source\repos\프로젝트 이름 ```에 ```arial.tff```파일 복사
5. 
***
## 게임 실행 과정
  - ![프로그램 작동 사진]()
