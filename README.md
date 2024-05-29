 ## 게임 설계과정
> 1. 활용한 라이브러리 : SDL,SDL_ttf
> 2. 
***
## 게임 작동 방법
1. [라이브러리 참고 파일]("") 에 들어가 파일을 다운
2. 각 폴더들을 C드라이브에 복사
3. 해당 프로그램을 만든 프로젝트를 클릭 후 프로젝트 속성에 들어간다.
     3-1. ```C/C++ -> 일반 -> 추가 포함 디렉터리```에서 편집을 눌러 각 폴더중 ```include```부분을 선택
     3-2. ```C/C++ -> 일반 -> SDL 검사 ``` 아니요 변경
     3-3. ```링커-> 일반 -> 추가  라이브러리 디렉터리 ``` 에서 편집을 눌러 각 폴더중 ```lib\x64```을 선택
     3-4. ```링커-> 입력 -> 추가 종속성 ``` 에서 ```SDL2.lib, SDL2main.lib, SDL2_ttf.lib ```을 추가
     3-5. ```C:\Windows\System32```에 각 폴더에 있던 ```.dll```파일을 복사
     3-6. ```C:\Users\사용자이름\source\repos\프로젝트 이름 ```에 ```arial.tff```파일 복사
***
## 게임 실행 과정
  - ![프로그램 작동 사진]()
