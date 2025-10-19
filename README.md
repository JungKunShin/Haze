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
---
2. 🫙 조합 퍼즐
---
3. 👴 AI (할아버지 캐릭터)
---
4. 📑 대화창
---
5. 🔉 사운드 및 볼륨 조절
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
