초기 화면 완성

추가한 것

src/Screen.c
초기 화면 관련 배열이 들어가 있습니다.
320*240 크기의 1차원 배열인데 크기가 꽤 큽니다.
만일 이 파일을 열면 용량때문에 오류가 날수 있으니 열지 않길 바랍니다.

함수 printScreen
위의 배열을 받아 화면에 출력을 해주는 함수입니다.
1이면 아무것도 출력안하고. (검은 화면)
0이면 하얀색을 출력합니다.

========================================================================

좌표 정보 (좌상단, 우하단)

Draw Area [(80,4)(273,236)]

Line button [(4,34)(75.55)]
Rectangle button [(4,58)(75,79)]
Oval button [(4,82)(75,103)]
Free Draw button [(4,106)(75,127)]
Selete button [(4,130)(75,151)]
Erase button [(4,154)(75,175)]
Clear button [(4,178)(75,199)]

Pen button [(280,154)(314,175)]
Fill button [(280,178)(314,199)]

COLOR BUTTON arrangement

12
34
56
78

color1 button [(277,26)(295,44)]
color2 button [(297,26)(315,44)]

color3 button [(277,47)(295,65)]
color4 button [(297,47)(315,65)]

color5 button [(277,67)(295,85)]
color6 button [(297,67)(315,85)]

color7 button [(277,87)(295,105)]
color8 button [(297,87)(315,105)]

