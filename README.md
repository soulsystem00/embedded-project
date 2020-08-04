# 인천대학교 컴퓨터공학부<br>임베디드시스템 기말 팀 프로젝트

인천대학교 컴퓨터공학부  
임베디드시스템 기말 팀 프로젝트입니다.  

총 2가지의 프로젝트 중 `그림판` 프로젝트를 선택, 구현하였습니다.  

## Direction

Odroid의 touch-screen을 활용하여 그림판을 구현  

* User-Interface 구현  
* User-Interface의 버튼을 이용하여 기능을 선택  
* Line, Rectangel, Oval 구현 및 rubber-band 지원  
* Free draw는 입력된 선을 부드럽게 연결  
* select는 그려진 object를 이동  

## 구현 상태 및 상세 설명

### 구현 상태

* `User-Interface` : 구현 완료
* `버튼 선택` : 구현 완료  <br><br>

* `Line` : 구현 완료 및 rubber-band 지원, 버그 존재
* `Rectangle` : 구현 완료 및 rubber-band 지원, 버그 존재
* `Oval` : 구현 완료 및 rubber-band 지원, 버그 존재
* `Free Draw` : 구현 완료 및 rubber-band 지원, 버그 존재  <br><br>

* `select` : Direction대로 구현 실패, 버그 존재  <br><br>

* `색상 선택` : 구현 완료  <br><br>

### 상세 설명

#### 그리기 함수

현재 버그가 존재합니다.  
그리는 영역 밖으로 펜이 벗어나게 되면  
현재 그리고 있던 그림이 지워지게 됩니다.  

#### select

select는 direction대로 구현하지 못하였습니다.  

하지만 윈도우 그림판의 기능을 구현하려고 하였고  
어느정도 구현을 할 수 있었습니다.  

저희가 구현한 방식은 원하는 범위를 선택하여  
선택된 범위의 그림을 옮기는 방식입니다.

## 기타

설명된 것 이외의 기능들은 코드를 참고해 주시기 바랍니다.  

기능의 구현 및 정상적인 실행이 우선이었기 때문에  
효율적으로 코드를 만들지 못하였습니다.  
이에 그림판의 성능 자체가 많이 떨어집니다.  
대표적인 현상으로는 그림이 그려지는 속도가 많이 느립니다.  

그렇기 때문에 이 코드는 참고용으로 사용하시길 바랍니다.  

또한 코드가 깔끔하지 못하고 주석이 거의 없기 때문에  
가독성이 떨어질 수 있습니다.  

## 팀원

[soulsystem00](https://github.com/soulsystem00)  
[Nyppp](https://github.com/Nyppp)  
[lih1691](https://github.com/lih1691)  
[jooy319](https://github.com/jooy319) 
