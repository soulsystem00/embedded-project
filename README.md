complete implement FreeDrwa Function

add variable 

ShapeMod
ColorMod
DrawMod

화면 및 ui 완성

Line 함수 구현 후 추가하려 했는데
Main.c 구조가 while문이라 뭔가 안되는 듯 함.
작동 됐던 원본 그대로 올려두긴 했습니다.

+ 기존코드에서 수정한거 commit 확인

+read를 while밖에다가 빼니깐 처음에 start좌표 찾는 루프를 빠져나오질 못해서
read함수를 while 문 안에다가 넣었습니다.

+추가로 다음 루프에서 dx dy로 for문을 돌리던데
위에는 선언만 하고 초기화를 안해서 해당부분에서 segmentation fault가 일어납니다.

+dx dy에 대한 연산이 추가 되어야 할거 같습니다.

+해당 수정부분이 어딘지는 commit 확인해 주시고 
위에 서술한 내용도 확인 부탁드립니다.

++ add mainpage color

+++ add Rectangle but, very slow need edit
