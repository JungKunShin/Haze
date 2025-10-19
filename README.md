# HAZE

## 👻 프로젝트 소개
**HAZE**는 **3명**이서 언리얼 C++를 이용하여 개발한 팀프로젝트로 **스팀**에 출시한 공포 스토리 게임입니다.
**리틀나이트메어**를 레퍼런스로 하여 제작된 퍼즐 게임이기도 합니다. 주인공인 생루가 할아버지의 감시를 피해, 어머니와 여동생을 데리고 퍼즐을 해결하고 탈출한다는 이야기입니다.
<br><br>

## 🔗 프로젝트 관련 링크

### 📹 프로젝트 영상 링크<br>

![](https://img.shields.io/badge/YouTube-FF0000?style=for-the-badge&logo=youtube&logoColor=white)&nbsp;&nbsp;
[HAZE 플레이영상](https://youtu.be/poInxu4GCr4 "HAZE 영상")
<br>

### 🎮 프로젝트 스팀 링크<br>
![](https://img.shields.io/badge/Steam-000000?style=for-the-badge&logo=steam&logoColor=white)&nbsp;&nbsp;
[HAZE 스팀](https://store.steampowered.com/app/4041900/Haze/ "HAZE 스팀링크")
<br><br><br>

## 📆 개발기간
+ 2025.07.22 ~ 2025.09.10 (8주) <br><br>
+ 스팀준비기간 2025.09.11 ~ 2025.10.09 (4주) <br>
+ 유저 피드백 소규모 패치 진행중 <br><br>

## 🧰 기술 스택 및 환경
![](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white) ![](	https://img.shields.io/badge/unrealengine-%23313131.svg?style=for-the-badge&logo=unrealengine&logoColor=white)
![](https://img.shields.io/badge/Visual_Studio-5C2D91?style=for-the-badge&logo=visual%20studio&logoColor=white)
- Language : C++
- Game Engine : Unreal Engine 5 (C++ / Blueprint 혼합 개발)
- IDE : Visual Studio 2022
- Version Control : SVN (Team Collaboration)
- Gameplay Systems :
  - Interact System(ActorComponent 기반 상호작용)
  - AI System(NPC / 적 행동 제어)
  - Puzzle System
- Data Handling : DataTable (CSV), SaveGame
- VFX / SFX : Sound Wave, Audio Component
- Tools : Blueprint, Behavior Tree, Material Editor, Sequencer etc
<br><br>

## 🔧 담당 업무
1. 🚪 액자 퍼즐
   - 액터 'BasePictures'와 액터 컴퍼넌트 'PlayerRotationComponent'를 기반으로 구현된 액자퍼즐 시스템
   - 네 개의 퍼즐이 모두 맞으면 창고 문이 열리도록 구현<br><br>
     ![액자퍼즐 플레이1](https://github.com/JungKunShin/Haze/blob/main/Image/PicturePuzzle_Play1.png)<br>
     ![액자퍼즐 플레이2](https://github.com/JungKunShin/Haze/blob/main/Image/PicturePuzzle_Play2.png)<br>
     ![액자퍼즐 플레이3](https://github.com/JungKunShin/Haze/blob/main/Image/PicturePuzzle_Play3.png)<br>
     ![액자퍼즐 플레이4](https://github.com/JungKunShin/Haze/blob/main/Image/PicturePuzzle_Play4.png)<br>
---
2. 🫙 조합 퍼즐
   - 액터 'MixGasStove', 'BaseMedicineElements'와 액터 컴퍼넌트 'PlayerMixComponent'를 기반으로 구현된 조합퍼즐 시스템
   - 저택에 산재해있는 약물들을 모두 모아 힌트대로 조합하면 완성 약품이 나오도록 구현<br><br>
     ![조합퍼즐 플레이1](https://github.com/JungKunShin/Haze/blob/main/Image/MixPuzzle_Play1.png)<br>
     ![조합퍼즐 플레이2](https://github.com/JungKunShin/Haze/blob/main/Image/MixPuzzle_Play2.png)<br>
     ![조합퍼즐 플레이3](https://github.com/JungKunShin/Haze/blob/main/Image/MixPuzzle_Play3.png)<br>
     ![조합퍼즐 플레이4](https://github.com/JungKunShin/Haze/blob/main/Image/MixPuzzle_Play4.png)<br>
---
3. 👴 AI (할아버지 캐릭터)
   - 'BTDecorator','BTService','BTTask'를 통해 AI 움직임 통제
   - 10미터 앞까지 트레이스 Hit를 확인하여, 플레이어 추격
   - Hit된 플레이어가 있고, 거리가 2.5미터 내로 좁혀졌다면, 플레이어 사망 처리
   - 플레이어 추격 5초 경과 후, 현재 시야에 플레이어가 없을 시 추격 중지<br><br>
     ![할아버지 플레이1](https://github.com/JungKunShin/Haze/blob/main/Image/GrandPa_Play1.png)<br>
     ![할아버지 플레이2](https://github.com/JungKunShin/Haze/blob/main/Image/GrandPa_Play2.png)<br>
---
4. 📑 대화창
   - 액터 컴퍼넌트 'QuestSystemComponent'와 액터 'BaseQuest'를 이용해 'ConversationWidget' 갱신
   - 'BaseQuest'로부터 출력해야할 Text를 저장한 뒤, 한글자씩 출력(소리도 한글자씩 출력)
   - 한글자씩 출력 중 Enter(또는 클릭) 입력 시, 전체 Text 출력
   - 전체 Text 출력 후 Enter(또는 클릭) 입력 시, 다음 Text 출력<br><br>
     ![대화창 플레이](https://github.com/JungKunShin/Haze/blob/main/Image/Conversation_Play.png)<br>
---
5. 🔉 사운드 및 볼륨 조절
   - 'SoundManager'를 기반으로 구현된 볼륨 조절 시스템
   - Master, BGM, 효과음, UI 별로 볼륨 조절 가능토록 구현<br><br>
    ![볼륨조절 플레이](https://github.com/JungKunShin/Haze/blob/main/Image/Volume_Play.png)<br>
   
---
6. 🏃 튜토리얼<br>
  - 'BaseFiringPin'을 기반으로 구현된 튜토리얼 시스템
  - 바깥쪽 콜리젼 박스(BoxComp)와 안쪽 콜리젼 박스(InnerBoxComp) 배치
  - 안쪽 콜리젼박스는 상단 조작법 안내 위젯 애니메이션을 재생시켜주는 역할을 담당
  - 바깥쪽 콜리젼박스는 상단 조작법 안내 위젯 애니메이션을 정지시켜주는 역할을 담당
  - 콜리젼박스를 두 개를 둔 이유는 애니메이션이 재생되는 도중 바로 콜리젼 박스를 나갔을 시, 애니메이션이 꺼지는 현상을 방지하기 위함<br><br>
    ![튜토리얼 플레이](https://github.com/JungKunShin/Haze/blob/main/Image/Tutorial_Play.png)<br>
---
7. 🔚 엔딩크레딧
   - 게임종료시 'EndingCreditWidget'을 통해 엔딩크레딧 구현
   - InDeltaTime(이전 틱과 현재 틱 사이 시간)을 통해 프레임 속도에 구애받지 않고, 일정 속도로 스크롤 가능케 구현
   - 스크롤 박스 최하단의 Offset값과 현재 Offset을 비교해, 엔딩크레딧 종료<br><br>
    ![엔딩크레 플레이](https://github.com/JungKunShin/Haze/blob/main/Image/EndingCredit_Play.png)<br>
